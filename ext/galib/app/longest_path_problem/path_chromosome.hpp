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
#include "../../include/graph.h"
#include "../../include/util.hpp"

namespace gal {
    /**
     * class IntegerChromosome implements a BitstringChromosome<int> that represents
     * a path. Bitstring is interpreted as a reversed binary
     * number.
     */
    class PathChromosome {
    public:
        /**
         * Constructs a new PathChromosome and initialises it randomly.
         * Do not set the length too large (<=32 generally)
         * @param length Length of the bitstring.
         */
         //MARK -- 2 should be done
        PathChromosome(const graph_lib::Graph & graph, std::mt19937 & gen) : graph_ref_(graph) {
            // Generate a random prefs_ to set a random path
            prefs_ = std::vector<unsigned int>(graph.getNodeCount()+1);
            prefs_[0] = (gen() % graph.getNodeCount()) + 1; // 1-indexed nodes.

            for (int i = 1; i <= graph.getNodeCount(); i++) {
                prefs_[i] = graph.getRandomEdge(i, gen); // Select a random edge from the available ones.
            }
            elite_ = false;
        }

        /**
         * Returns path length
         */
         //MARK -- 1 should be done
        int getPathLength() const {
            return graph_ref_.computePath(prefs_)[0];
        }
        virtual int getValue() const { return getPathLength(); }


        //MARK -- 3 should be done
        virtual std::string getChromosomeText() const {
            std::string str = "(" + std::to_string(prefs_[0]) + " | ";
            for (int i = 1; i < prefs_.size(); i++){
                if (i != 1) str += ", ";
                str += std::to_string(prefs_[i]);
            }
            str += ") - [";
            auto path = graph_ref_.computePath(prefs_);
            str += std::to_string(path[0]) + " | ";
            for (int i = 1; i < path.size(); i++){
                if (i != 1) str += ", ";
                str += std::to_string(path[i]);
            }
            return str + "]";
        }

        /**
         * Perform mutation, i.e. flip each bit with specified probability.
         *
         * @param probability Probability of a single bitflip.
         */
        virtual void mutate(double probability, std::mt19937 & gen) {
            if (probability < 0.0 || probability > 1.0)
                throw std::invalid_argument("Mutation probability has to be in [0.0, 1.0].");

            //MARK -- 5 should be done, TODO make sure graph and gen are passed.
            for(unsigned int i = 0; i < prefs_.size(); i++){
                // Generate a random double in [0.0, 1.0]
                double mutation_roll = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX));
                // Determine if we mutate this bit
                if(mutation_roll <= probability){
                    // Randomly reassign the pref[i]. Reroll randomly untill it is different.
                    auto old_pref = prefs_[i];
                    while (old_pref == prefs_[i] && graph_ref_.getNodeEdgeSet(i).size() > 1) {
                        prefs_[i] = graph_ref_.getRandomEdge(i, gen);
                    }
                }
            }
        }

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
        //MARK -- 4 should be done
        virtual void crossover(int pos, PathChromosome &other) {
            auto bits_it = prefs_.begin();
            auto other_it = other.prefs_.begin();

            // Move iterators ahead to pos
            for(int i = 0; i < pos; i++){
                other_it++;
                bits_it++;
            }

            // Loop over elements beyond pos
            while(bits_it != prefs_.end()){
                // Swap the two elements
                auto temp_elem = *bits_it;
                *bits_it = *other_it;
                *other_it = temp_elem;

                other_it++;
                bits_it++;
            }

        }

        /**
         * Perform crossover with an other BitstringChromosome<T> at a random position.
         * @see BitstringChromosome<T>::crossover(int pos, BitstringChromosome<T>& g)
         */
        virtual void crossover(PathChromosome &other) {
            crossover(random_int(prefs_.size()), other);
        }

        /**
         * Returns boolean value indicating whether the chromosome is an elite
         */
        bool isElite() {return elite_;}

        /**
         * Sets boolean value indicating whether the chromosome is an elite
         */
        void setElite(bool elite) {elite_ = elite;}


    protected:
        bool elite_;
        std::vector<unsigned int> prefs_;
        const graph_lib::Graph & graph_ref_;
        // An array containing the exit node for each node in the graph
        // Its first entry contains the start node
    };
}