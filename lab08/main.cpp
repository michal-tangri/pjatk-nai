#include <vector>
#include <random>
#include <functional>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <math.h>

using big_number = unsigned long long int;

std::random_device r;
std::default_random_engine e1(r());

struct phenotype {
    double x;
    double y;

    phenotype() {
        this->x = 0;
        this->y = 0;
    };

    phenotype(double x, double y) {
        this->x = x;
        this->y = y;
    };
};

std::vector<int> convert_phenotype_to_genotype(phenotype phen, double domain_max) {

    std::vector<int> genotype;
    std::vector<int> binary(128);

    big_number x = ((phen.x / pow(10, 20)) + (0.185 / 2)) * (domain_max * 10.85);
    big_number y = ((phen.y / pow(10, 20)) + (0.185 / 2)) * (domain_max * 10.85);
  
    for(int i = 128 / 2; i > 0; i--) {
        binary.at(i - 1) = x % 2; 
        x /= 2; 
    }

    for(int i = 128; i > 128 / 2; i--) {
        binary.at(i - 1) = y % 2; 
        y /=  2; 
    }

    for(int i = 0; i < 128; i++)
        genotype.push_back(binary.at(i));

    return genotype;
}

phenotype convert_genotype_to_phenotype(std::vector<int> genotype, double domain_max) {    
    unsigned int const half = genotype.size() / 2;

    std::vector<int> phen_array_x(genotype.begin(), genotype.begin() + half);
    std::vector<int> phen_array_y(genotype.begin() + half, genotype.end());

    phenotype phen;
    for (int i = phen_array_x.size() - 1; i >= 0; i--) {
        phen.x += phen_array_x.at(i) * pow(2, i);
    }

    for (int i = phen_array_y.size() - 1; i >= 0; i--) {
        phen.y += phen_array_y.at(i) * pow(2, i);
    }

    /* 
        We divide our values by 10^20 to reduce them to max 1.8 (because the maximum value we can use in C++ is 1.8 * 10^20). 
        Next, we subtract half of the max value to allow use of negative numbers. At last, we multiply them to match their domain.
     */
    phen.x = (phen.x / pow(10, 20) - 0.185 / 2) * (domain_max * 10.85);
    phen.y = (phen.y / pow(10, 20) - 0.185 / 2) * (domain_max * 10.85);

    return phen;
}

auto random_sampling_algorithm = [](auto func, auto fitness, auto phen_generator, double domain_max, int iterations_max) {
    phenotype best_phen;
    phenotype phen = convert_genotype_to_phenotype(phen_generator(), domain_max);

    double current_solution = fitness(phen, func);
    for (int i = 0; i < iterations_max; i++) {
        phen = convert_genotype_to_phenotype(phen_generator(), domain_max);
        double new_solution = fitness(phen, func);   
        if (new_solution > current_solution) {
            best_phen = phen;
            current_solution = new_solution;
        }
    }
    return best_phen;
};

int main() {

    /* 
        Minimums: 
            f(3.0, 2.0)
            f(-2.805118, 3.131312)
            f(-3.779310,-3.283186)
            f(3.584428,-1.848126)
    */
    auto himmelblau_function = [](double x, double y) {
        double firstPart = pow(x, 2) + y - 11;
        double secondPart = x + pow(y, 2) - 7;
        return pow(firstPart, 2) + pow(secondPart, 2);
    };

    auto fitness = [](phenotype phen, auto func) {
        return 1 / abs((func(phen.x, phen.y) * (-1)));
    };

    auto phen_generator = [](int size = 128) {
        
        std::uniform_int_distribution<int> uniform_dist(0,1);

        std::vector<int> result;
        for (int i = 0; i < size; i++)
            result.push_back(uniform_dist(e1));

        return result;
    };

    const unsigned int iterations = 100000;

    phenotype phen;
    for (int i = 0; i < 3; i++) {
        phen = random_sampling_algorithm(himmelblau_function, fitness, phen_generator, 5, iterations);
        std::cout << "Himmelblau: [ " << phen.x << " " << phen.y << " ] --> " << himmelblau_function(phen.x, phen.y) << std::endl;
    }

}