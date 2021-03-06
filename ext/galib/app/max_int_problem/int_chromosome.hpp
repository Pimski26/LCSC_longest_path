/**
 * @file IntegerChromosome.hpp
 *
 * Contains the gal::IntegerChromosome class, which represents a natural number.
 *
 * @author Arthur van Dam, dam@math.uu.nl
 * @author Adriaan Graas, a.b.m.graas@uu.nl
 */

#pragma once

#include <string>
#include <sstream>
#include "../../include/chromosome.hpp"
#include "../../include/util.hpp"
#include "../../include/bitstringChromosome.h"

namespace gal {
    /**
     * class IntegerChromosome implements a BitstringChromosome<int> that represents
     * a natural number. Bitstring is interpreted as a reversed binary
     * number.
     */
    class IntegerChromosome : public BitstringChromosome<int> {
    public:
        /**
         * Constructs a new IntegerChromosome and initialises it randomly.
         * Do not set the length too large (<=32 generally)
         * @param length Length of the bitstring.
         */
        explicit IntegerChromosome(int length)
                : BitstringChromosome<int>(length) {
            if (length > sizeof(int) * 8 - 1) {
                throw std::invalid_argument(
                        "IntegerChromosome: bitstring of " + std::to_string(length) +
                        " length exceeds size of standard int type.");
            }
        }

        int changeBit(int in) override {
            if(in == 0){
                return 1;
            }
            else{
                return 0;
            }
        }
        /**
         * Returns the decimal value that the bitstring represents.
         */
        int getValue() const override {
            unsigned int total = 0;
            for(std::list<char>::const_iterator it = bits_.begin(); it != bits_.end(); it++){
                total *= 2;
                if(*it == 1) {
                    total += 1;
                }
            }
            return total;
        }
    };
}