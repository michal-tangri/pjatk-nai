#ifndef _PHENO_GENO_HPP_
#define _PHENO_GENO_HPP_

#include <vector>


class phenotype {
    public:
        double x;
        double y;
        double fitness;
        std::vector<int> chromosome;

    phenotype() {
        this->x = 0;
        this->y = 0;
        this->fitness = 0;
    };

    phenotype(double x, double y) {
        this->x = x;
        this->y = y;
        this->fitness = 0;
    };

    phenotype(double x, double y, double fitness) {
        this->x = x;
        this->y = y;
        this->fitness = fitness;
    };

    void updatePhenotype(std::vector<int> chromosome, double domain_max);
};

std::vector<int> convert_phenotype_to_genotype(phenotype phen, double domain_max);
phenotype convert_genotype_to_phenotype(std::vector<int> genotype, double domain_max);

#endif