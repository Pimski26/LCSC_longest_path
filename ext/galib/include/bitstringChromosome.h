//
// Created by emi on 24/01/2022.
//

#ifndef LCSC_PART2_BITSTRINGCHROMOSOME_H
#define LCSC_PART2_BITSTRINGCHROMOSOME_H

/**
 * @file chromosome.hpp
 *
 * Contains the gal::BitstringChromosome class, which serves as the base class for
 * more specific BitstringChromosome types.
 *
 * @author Arthur van Dam, dam@math.uu.nl
 * @author Adriaan Graas, a.b.m.graas@uu.nl
 */
#pragma once

#include <iostream>
#include <string>
#include <list>
#include <ctime>
#include <cstdlib>
#include "util.hpp"
#include "chromosome.hpp"

namespace gal {
    /**
     * Represents a chromosome that models a value of any type T.
     * Examples of type T are: int for one variable, vector<double> for a
     * point in n-dimensional space.
     *
     * One always needs to use a subclass of this class, where all virtual
     * functions are defined.
     */
    template<typename T>
    class BitstringChromosome : public Chromosome<T> {
    public:
        /**
         * Constructs a BitstringChromosome, initialised with a random bitstring.
         *
         * @param length The number of bits in this BitstringChromosome.
         */
        explicit BitstringChromosome(int length)
                : bits_(length) {
            bits_length_ = length;
            for (auto &b: bits_) // initialize `bits_` randomly
                b = getRandomBit();
            Chromosome<T>::elite_ = false;
        }

        /**
         * Returns a string representation of the raw bitstring.
         */
        std::string getText() const override {
            std::string str;
            for(std::list<char>::const_iterator it = bits_.begin(); it != bits_.end(); it++){
                str += std::to_string(*it);
            }
            return str;
        }

    protected:
        std::list<char> bits_;

        int bits_length_;

        static char getRandomBit() {
            return (char) random_int(2);
        }
    };
}

#endif //LCSC_PART2_BITSTRINGCHROMOSOME_H
