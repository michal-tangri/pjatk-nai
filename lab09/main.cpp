#include <vector>
#include <random>
#include <functional>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <math.h>

#include "pheno_geno.hpp"

std::random_device r;
std::default_random_engine e1(r());

phenotype updatePhenotype(std::vector<int> chromosome, double domain_max) {
    return convert_genotype_to_phenotype(chromosome, domain_max);
}

auto printResults = [](std::vector<phenotype> results, auto func, auto fitness, std::string description) {
    std::cout << "============" << description << "============" << std::endl;
    for (auto result : results) {
        std::cout << "[ " << result.x << ", " << result.y << "] -> " << fitness(result, func) << std::endl;

    }
};

auto selection_roulette = [](auto population) {
    decltype(population) selected_phenotypes;
    double total_fitness = accumulate(population.begin(), population.end(), 0.0, [](auto a, auto b) { return a + b.fitness; });

    std::uniform_real_distribution<double> uniform(0.0, total_fitness);
    for (int c = 0; c < population.size(); c++)
    {
        double random_fit = uniform(e1);
        double sum_fit = 0.0;
        for (int i = 0; i < population.size(); i++)
        {
            sum_fit += population[i].fitness;
            if (random_fit < sum_fit)
            {
                selected_phenotypes.push_back(population.at(i));
                break;
            }
        }
    }
    return selected_phenotypes;
};

auto mutation_probabilitic = [](auto population, double p_mutation = 0.1) {
    decltype(population) mutated_population;
    std::uniform_real_distribution<double> mutator(0.0, 1.0);
    for (int i = 0; i < population.size(); i++)
    {
        auto current = population.at(i);
        for (int j = 0; j < current.chromosome.size(); j++)
        {
            if (mutator(e1) < p_mutation)
                current.chromosome[j] = 1 - current.chromosome[j];
        }
        mutated_population.push_back(current);
    }
    return mutated_population;
};

auto crossover_one_point = [](auto population, double domain_max, double p_crossover = 0.9) {
    decltype(population) results;
    std::uniform_real_distribution<double> uniform_cross(0.0, 1.0);
    for (int i = 0; i < (population.size() - 1); i += 2)
    {
        auto current = population.at(i);
        auto next = population.at(i + 1);

        if (uniform_cross(e1) < p_crossover)
        {
            std::uniform_int_distribution<int> uniform(0, current.chromosome.size() - 1);
            auto crossover_p = uniform(e1);
            for (int g = crossover_p; g < current.chromosome.size(); g++) {
                std::swap(current.chromosome[g], next.chromosome[g]);
                current.updatePhenotype(current.chromosome, domain_max);
                next.updatePhenotype(next.chromosome, domain_max);
            }
        }
        results.push_back(current);
        results.push_back(next);
    }
    return results;
};

int main()
{

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
        return 1.0 / (1.0 + abs(func(phen.x, phen.y)));
    };

    auto phen_generator = [](int size = 128) {
        std::uniform_int_distribution<int> uniform_dist(0, 1);

        std::vector<int> result;
        for (int i = 0; i < size; i++)
            result.push_back(uniform_dist(e1));

        return result;
    };

    std::vector<phenotype> population;

    for (int i = 0; i < 10; i++)
    {
        phenotype tmp = convert_genotype_to_phenotype(phen_generator(), 5);
        tmp.fitness = fitness(tmp, himmelblau_function);
        population.push_back(tmp);
    }

    for (auto specimen : population)
    {
        std::cout << "[ " << specimen.x << ", " << specimen.y << "] -> " << himmelblau_function(specimen.x, specimen.y) << std::endl;
    }

    printResults(selection_roulette(population), himmelblau_function, fitness, "Roulette");
    printResults(mutation_probabilitic(population), himmelblau_function, fitness, "Mutation");
    printResults(crossover_one_point(population, 5), himmelblau_function, fitness, "Crossover");
}