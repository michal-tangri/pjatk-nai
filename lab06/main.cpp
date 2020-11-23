#include <iostream>
#include <string>
#include <random>


using namespace std;
random_device r;
default_random_engine e1(r());

auto generate_values = [](auto randomizer) {
    vector<double> tmp(2);
    tmp[0] = randomizer(e1);
    tmp[1] = randomizer(e1);
    return tmp;
};

auto optimalization_algorithm = [](auto func, auto domain, int max_iterations) {
    uniform_real_distribution<double> uniform_dist(domain[0], domain[1]);
    auto best = generate_values(uniform_dist);
    for (int i = 0; i < max_iterations; i++) {
        auto solution = generate_values(uniform_dist);
        if (func(best) > func(solution)) {
            best = solution;
        }
    }
    return best;
};

int main()
{

    //domain : -5 <= x, y <= 5
    //Global minimum f(0, 0) = 0
    auto three_hump_camel = [](vector<double> values) {
        double x = values[0];
        double y = values[1];
        return (2 * pow(x, 2)) - (1.05 * pow(x, 4)) + (pow(x, 6) / 6) + (x * y) + pow(y, 2);
    };

    //domain : -10 <= x, y <= 10
    //Global minimum f(1, 3) = 0
    auto booth_function = [](vector<double> values) {
        double x = values[0];
        double y = values[1];
        return pow((x + (2 * y) - 7), 2) + pow(((2 * x) + y - 5), 2);
    };

    //domain : -4.5 <= x, y <= 4.5
    //Global minimum f(3, 0.5) = 0
    auto beale_function = [](vector<double> values) {
        double x = values[0];
        double y = values[1];
        auto firstPart = (1.5 - x + (x * y));
        auto secondPart = (2.25 - x + (x * pow(y, 2)));
        auto thirdPart = (2.625 - x + (x * pow(y, 3)));
        return pow(firstPart, 2) + pow(secondPart, 2) + pow(thirdPart, 2);
    };

    //domain : Infnity <= x <= Ininity
    //Global minimum f(0,...,0) = 0
    auto sphere_function = [](vector<double> values) {
        double sum = 0;
        for (auto value : values)
        {
            sum += value * value;
        }
        return sum;
    };

    std::vector<double> camel_domain {-5, 5};
    std::vector<double> booth_domain {-10, 10};
    std::vector<double> beale_domain {-4.5, 4.5};
    std::vector<double> sphere_domain {-100, 100};

    int iterations = 100000;

    auto solution_camel = optimalization_algorithm(three_hump_camel, camel_domain, iterations);
    auto solution_booth = optimalization_algorithm(booth_function, booth_domain, iterations);
    auto solution_beale = optimalization_algorithm(beale_function, beale_domain, iterations);
    auto solution_sphere = optimalization_algorithm(sphere_function, sphere_domain, iterations);

    cout << "Camel: [ ";
    for (auto e : solution_camel)
        cout << e << " ";
    cout << "] -> " << three_hump_camel(solution_camel) << endl;

    cout << "Booth: [ ";
    for (auto e : solution_booth)
        cout << e << " ";
    cout << "] -> " << booth_function(solution_booth) << endl;

    cout << "Beale: [ ";
    for (auto e : solution_beale)
        cout << e << " ";
    cout << "] -> " << beale_function(solution_beale) << endl;

    cout << "Sphere: [ ";
    for (auto e : solution_sphere)
        cout << e << " ";
    cout << "] -> " << sphere_function(solution_sphere) << endl;
}