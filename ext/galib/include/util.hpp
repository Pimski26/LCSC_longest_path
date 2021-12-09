/**
 * @file util.hpp
 *
 * Set of utitily functions, independent of any GA class.
 *
 * @author Arthur van Dam, dam@math.uu.nl
 * @author Adriaan Graas, a.b.m.graas@uu.nl
 */
#pragma once

#include<sstream>
#include<string>
#include<list>
#include<cmath>
#include<cstdlib>
#include <random>

std::random_device rd; // NOLINT(cert-err58-cpp)
std::default_random_engine engine(rd()); // NOLINT(cert-err58-cpp)

/**
 * Produces a natural number randomly between 0 and nmax-1.
 */
int random_int(int nmax) {
    auto distribution = std::uniform_int_distribution<int>(0, nmax - 1);
    return distribution(engine);
}

/**
 * Produces a real number randomly between 0.0 and 1.0.
 */
double random_real(double a=0.0, double b=1.0) {
    auto distribution = std::uniform_real_distribution<double>(a, b);
    return distribution(engine);
}

class RequiresImplementationError : public std::logic_error {
public:
    explicit RequiresImplementationError(const std::string& message)
        : std::logic_error(message) { };
};