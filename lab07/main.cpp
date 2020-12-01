#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <fstream>
#include <algorithm>
#include <unordered_map>

#include "functions.hpp"

using namespace std;
random_device r;
default_random_engine e1(r());

auto display_results = [](auto solution, auto func, string name) {
    cout << name << ": [ ";
    for (auto e : solution)
        cout << e << " ";
    cout << "] -> " << func(solution) << endl;
};

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
    for (int i = 0; i < n_elements; i++)
    {
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
        if (iteration < 200)
            results.push_back(func(current_solution));
    }
    return results;
};

void saveDataToFile(vector<double> data)
{
    ofstream output;
    output.open("data.txt");
    if (!output)
        cerr << "Cannot open 'data.txt' file!" << endl;
    for (int i = 0; i < data.size(); i++)
    {
        // output << (i + 1) * 10000 << "   " << data.at(i) << "\n";
        output << (i + 1) << "   " << data.at(i) << "\n";
    }
    output.close();
}

auto createGraph = [](auto func, auto domain, auto restriction, unsigned int n, unsigned int max_iterations) {
    vector<vector<double>> gnuplot_data;
    for (int i = 0; i < 5; i++)
    {
        gnuplot_data.push_back(hill_climbing_r_alg_gnuplot(booth_function, booth_domain, restriction, n, max_iterations));
    }

    const unsigned int results_size = gnuplot_data.at(0).size();

    vector<double> averages_from_data;
    for (int i = 0; i < results_size; i++)
    {
        double sum = 0;
        for (int j = 0; j < gnuplot_data.size(); j++)
        {
            sum += gnuplot_data.at(j).at(i);
        }
        double average = sum / results_size;
        averages_from_data.push_back(average);
    }

    saveDataToFile(averages_from_data);
    system("gnuplot -p ex.gnu");
};

int main(int argc, char **argv)
{

    std::unordered_map<string, string> arguments;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
            arguments[argv[i]] = argv[i + 1];
    }

    int iterations = stoi(arguments["-i"]);
    double restriction = stod(arguments["-r"]);

    if (arguments["-alg"] == "booth")
    {
        auto solution_booth = hill_climbing_r_alg(booth_function, booth_domain, restriction, 2, iterations);
        display_results(solution_booth, booth_function, "Booth");
        createGraph(booth_function, booth_domain, restriction, 2, iterations);
    }
    else if (arguments["-alg"] == "beale")
    {
        auto solution_beale = hill_climbing_r_alg(beale_function, beale_domain, restriction, 2, iterations);
        display_results(solution_beale, beale_function, "Beale");
        createGraph(beale_function, beale_domain, restriction, 2, iterations);
    }
    else if (arguments["-alg"] == "sphere")
    {
        auto solution_sphere = hill_climbing_r_alg(sphere_function, sphere_domain, restriction, 10, iterations);
        display_results(solution_sphere, sphere_function, "Sphere");
        createGraph(sphere_function, sphere_domain, restriction, 10, iterations);
    }
}