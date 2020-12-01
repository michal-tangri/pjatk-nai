#ifndef _TEST_FUNCTIONS_HPP_
#define _TEST_FUNCTIONS_HPP_

#include <cmath>
#include <vector>
#include <functional>

std::vector<double> three_hump_camel_domain{-5, 5};
std::vector<double> booth_domain{-10, 10};
std::vector<double> beale_domain{-4.5, 4.5};
std::vector<double> sphere_domain{-100, 100};

//domain : -5 <= x, y <= 5
//Global minimum f(0, 0) = 0
auto three_hump_camel = [](std::vector<double> values) {
    double x = values[0];
    double y = values[1];
    return (2 * pow(x, 2)) - (1.05 * pow(x, 4)) + (pow(x, 6) / 6) + (x * y) + pow(y, 2);
};

//domain : -10 <= x, y <= 10
//Global minimum f(1, 3) = 0
auto booth_function = [](std::vector<double> values) {
    double x = values[0];
    double y = values[1];
    return pow((x + (2 * y) - 7), 2) + pow(((2 * x) + y - 5), 2);
};

//domain : -4.5 <= x, y <= 4.5
//Global minimum f(3, 0.5) = 0
auto beale_function = [](std::vector<double> values) {
    double x = values[0];
    double y = values[1];
    auto firstPart = (1.5 - x + (x * y));
    auto secondPart = (2.25 - x + (x * pow(y, 2)));
    auto thirdPart = (2.625 - x + (x * pow(y, 3)));
    return pow(firstPart, 2) + pow(secondPart, 2) + pow(thirdPart, 2);
};

//domain : Infnity <= x <= Ininity
//Global minimum f(0,...,0) = 0
auto sphere_function = [](std::vector<double> values) {
    double sum = 0;
    for (auto value : values)
    {
        sum += value * value;
    }
    return sum;
};
#endif