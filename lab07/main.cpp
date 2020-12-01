#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <fstream>
#include <algorithm>


using namespace std;
random_device r;
default_random_engine e1(r());

void saveDataToFile(vector<double> data) {
    ofstream output;
    output.open("data.txt");
    if (!output)
        cerr << "Cannot open 'data.txt' file!" << endl;
    for (int i = 0; i < data.size(); i++) {
        output << (i + 1) * 10000 << "   " << data.at(i) << "\n";
    }
    output.close();
}

auto get_random_restricted = [](vector<double> points, auto dx) {
    vector<vector<double>> results;
        for (int i = 0; i < points.size(); i++)
        {
            auto nx = points;
            nx[i] += dx;
            results.push_back(nx);
            nx[i] -= 2.0 * dx;
            results.push_back(nx);
        }
        return results;
};

auto get_random_solution = [](auto domain, auto n_elements) {
    vector<double> randoms;
    uniform_real_distribution<double> uniform_dist(domain[0], domain[1]);
    for (int i = 0; i < n_elements; i++) {
        randoms.push_back(uniform_dist(e1));
    }
    return randoms;
};

auto hill_climbing_r_alg = [](auto func, auto domain, auto dx, unsigned int n_elements, unsigned int max_iterations) {
    auto current_solution = get_random_solution(domain, n_elements);
    for (int iteration = 0; iteration < max_iterations; iteration++)
    {
        auto next_solutions = get_random_restricted(current_solution, dx);
        next_solutions.push_back(current_solution);
        current_solution = *(max_element(next_solutions.begin(), next_solutions.end(), [&func](auto a, auto b) {
            return func(a) > func(b);
        }));
    }
    return current_solution;
};

auto hill_climbing_r_alg_gnuplot = [](auto func, auto domain, auto dx, unsigned int n_elements, unsigned int max_iterations) {
    vector<double> results;
    auto current_solution = get_random_solution(domain, n_elements);
    for (int iteration = 0; iteration < max_iterations; iteration++)
    {
        auto next_solutions = get_random_restricted(current_solution, dx);
        next_solutions.push_back(current_solution);
        current_solution = *(max_element(next_solutions.begin(), next_solutions.end(), [&func](auto a, auto b) {
            return func(a) > func(b);
        }));
        if (iteration % 10000 == 0)
            results.push_back(func(current_solution));
    }
    return results;
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

    // vector<double> camel_domain {-5, 5};
    vector<double> booth_domain {-10, 10};
    vector<double> beale_domain {-4.5, 4.5};
    vector<double> sphere_domain {-100, 100};

    int iterations = 1000000;

    // auto solution_camel = hill_climbing_r_alg(three_hump_camel, camel_domain, 0.1, iterations);
    auto solution_booth = hill_climbing_r_alg(booth_function, booth_domain, 0.1, 2, iterations);
    auto solution_beale = hill_climbing_r_alg(beale_function, beale_domain, 0.1, 2, iterations);
    auto solution_sphere = hill_climbing_r_alg(sphere_function, sphere_domain, 0.1, 6, iterations);

    // cout << "Camel: [ ";
    // for (auto e : solution_camel)
    //     cout << e << " ";
    // cout << "] -> " << three_hump_camel(solution_camel) << endl;

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

    // vector<vector<double>> gnuplot_data;
    // for (int i = 0; i < 5; i++) {
    //     gnuplot_data.push_back(hill_climbing_r_alg_gnuplot(booth_function, booth_domain, 0.1, iterations, 2));
    // }

    // const unsigned int results_size = gnuplot_data.at(0).size();

    // vector<double> averages_from_data;
    // for (int i = 0; i < results_size; i++) {
    //     double sum = 0;
    //     for (int j = 0; j < gnuplot_data.size(); j++) {
    //         sum += gnuplot_data.at(j).at(i);
    //     }
    //     double average = sum / results_size;
    //     averages_from_data.push_back(average);
    // }

    // saveDataToFile(averages_from_data);
    // system("gnuplot -p ex.gnu");
    
}