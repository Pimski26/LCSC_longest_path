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
    class Chromosome {
    public:
        /**
         * Returns the represented value of this BitstringChromosome.
         * This method should always be defined in a derived class.
         */
        virtual T getValue() const = 0;

        /**
         * Returns a string representation of the raw bitstring.
         */
        virtual std::string getText() const = 0;

        /**
         * Perform mutation, i.e. flip each bit with specified probability.
         *
         * @param probability Probability of a single bitflip.
         */
        virtual void mutate(double probability) {
            if (probability < 0.0 || probability > 1.0)
                throw std::invalid_argument("Mutation probability has to be in [0.0, 1.0].");

            for(auto it = bits_.begin(); it != bits_.end(); it++){
                // Generate a random double in [0.0, 1.0]
                double mutation_roll = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX));
                // Determine if we mutate this bit
                if(mutation_roll <= probability){
                    // Flip the bit between 0 and 1
                    *it = changeBit(*it);
                }
            }
        }

        virtual void localSearch() = 0;

        virtual T changeBit(T in) = 0;

        /**
         * Perform crossover with another BitstringChromosome<T>, i.e. split both
         * this and the other chromosome at the same position in their
         * bitstring, and exchange one of their halves.
         * <pre>
         *  chromosome 1: AAAABBB  --> AAAADDD
         *  chromosome 2: CCCCDDD  --> CCCCBBB
         *                    ^pos = 4 (i.e. start at 0)
         * </pre>
         * @param pos bit position where crossover takes place.
         * @param other The other 'parent' in this crossover.
         */
        virtual void crossover(int pos, Chromosome<T> &other) {
            auto bits_it = bits_.begin();
            auto other_it = other.bits_.begin();

            // Move iterators ahead to pos
            for(int i = 0; i < pos; i++){
                other_it++;
                bits_it++;
            }

            // Loop over bits beyond pos
            while(bits_it != bits_.end()){
                // Swap the two bits
                char temp_char = *bits_it;
                *bits_it = *other_it;
                *other_it = temp_char;

                other_it++;
                bits_it++;
            }

        }

        /**
         * Perform crossover with an other BitstringChromosome<T> at a random position.
         * @see BitstringChromosome<T>::crossover(int pos, BitstringChromosome<T>& g)
         */
        virtual void crossover(Chromosome<T> &other) {
            crossover(random_int(bits_.size()), other);
        }

        /**
         * Returns boolean value indicating whether the chromosome is an elite
         */
        virtual bool isElite() {return elite_;}

        /**
         * Sets boolean value indicating whether the chromosome is an elite
         */
        virtual void setElite(bool elite) {elite_ = elite;}

        std::vector<T> bits_;
    protected:
        bool elite_;
    };
}
