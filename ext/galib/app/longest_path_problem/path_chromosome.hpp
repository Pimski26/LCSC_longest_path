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
#include "../../include/chromosome.hpp"

namespace gal {
    /**
     * class IntegerChromosome implements a BitstringChromosome<int> that represents
     * a path. Bitstring is interpreted as a reversed binary
     * number.
     */
    class PathChromosome : public Chromosome<unsigned int>{
    public:
        /**
         * Constructs a new PathChromosome and initialises it randomly.
         * Do not set the length too large (<=32 generally)
         * @param length Length of the bitstring.
         */
        PathChromosome(const graph_lib::Graph & graph, std::mt19937 & gen, int crossover_type) : graph_ref_(graph), gen_(gen) {
            // Generate a random bits_ to set a random path
            bits_ = std::vector<unsigned int>(graph.getNodeCount()+1);
            bits_[0] = (gen() % graph.getNodeCount()) + 1; // 1-indexed nodes.

            for (int i = 1; i <= graph.getNodeCount(); i++) {
                bits_[i] = changeBit(i); // Select a random edge from the available ones.
            }
            elite_ = false;
            crossover_type_ = crossover_type;
        }

        /**
         * Returns path length
         */
        int getPathLength() const {
            return graph_ref_.computePath(bits_)[0];
        }

        virtual unsigned int getValue() const  override{ return getPathLength(); }


        virtual std::string getText() const override {
            std::string str = "(" + std::to_string(bits_[0]) + " | ";
            for (int i = 1; i < bits_.size(); i++){
                if (i != 1) str += ", ";
                str += std::to_string(bits_[i]);
            }
            str += ") - [";
            auto path = graph_ref_.computePath(bits_);
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
        virtual void mutate(double probability) override {
            if (probability < 0.0 || probability > 1.0)
                throw std::invalid_argument("Mutation probability has to be in [0.0, 1.0].");

            for(unsigned int i = 0; i < bits_.size(); i++){
                // Generate a random double in [0.0, 1.0]
                double mutation_roll = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX));
                // Determine if we mutate this bit
                if(mutation_roll <= probability){
                    // Randomly reassign the pref[i]. Reroll randomly untill it is different.
                    auto old_pref = bits_[i];
                    if (graph_ref_.getNodeEdgeSet(i).size() > 1)
                    while (old_pref == bits_[i]) {
                        bits_[i] = changeBit(i);
                    }
                }
            }
        }

        void crossover_random(Chromosome<unsigned int> &chromosome) {
            auto bits_it = bits_.begin();
            auto other_it = chromosome.bits_.begin();

            while(bits_it != bits_.end()){
                // Swap the two bits
                if (random_int(2) == 0) {
                    char temp_char = *bits_it;
                    *bits_it = *other_it;
                    *other_it = temp_char;
                }
                other_it++;
                bits_it++;
            }
        }

        void crossover_optimum(Chromosome<unsigned int> & chromosome){
            // Makes the assumption it is a compatible chromosome.
            std::vector<bool> visited(bits_.size());
            unsigned int current = 0;
            //Starting at the start of the path, take the pref of whichever is the longer path. this means the start
            // Of the path will be the same as you go, each time picking the locally optimal choice. Note that this can
            // Result into very different paths, but will never be worse than the best of the two chromosomes.
            // All the bits that are part of the path will become the same.
            while (!visited[current]){
                if (graph_ref_.computePath(bits_)[0] > graph_ref_.computePath(chromosome.bits_)[0]){
                    chromosome.bits_[current] = bits_[current];
                } else {
                    bits_[current] = chromosome.bits_[current];
                }
                visited[current] = true;
                current = bits_[current];
            }
        }

        // This function appears to work, but makes the GA excessively slow. Maybe interesting to experiment with, but
        // Impractical.
        void crossover_path_pos(Chromosome<unsigned int> & other){
            auto thisPath = graph_ref_.computePath(bits_);
            auto thatPath = graph_ref_.computePath(other.bits_);
            unsigned int pathBound = std::min(thisPath.size(), thatPath.size());
            if (pathBound >= 3) {
                int pos = random_int(pathBound - 1) + 1;
                // The pos'th element of the path is redirected to point to the pos'th element of the path
                if (pos > 2) {
                    bits_[pos - 1] = thatPath[pos];
                    other.bits_[pos - 1] = thisPath[pos];
                } // All subsequent bits follow the other chromosomes path. Regarldess of if this overrides the previous
                // entries. Best case scenario this results in the two completing eachothers paths.
                // Worst case they interfere and only some of the paths are preserved, effectively being semi-randomisation.
                for (; pos < pathBound; pos++) {
                    bits_[thatPath[pos]] = other.bits_[thatPath[pos]];
                    other.bits_[thisPath[pos]] = bits_[thisPath[pos]];
                }
            }
        }

        void crossover(Chromosome<unsigned int> &other) override{
            switch(crossover_type_){
                case 1: Chromosome::crossover(other);
                    return;
                case 2: crossover_optimum(other);
                    return;
                case 3: crossover_path_pos(other);
                    return;
                default: crossover_random(other);
            }
        }

        void localSearch() override{
            unsigned int bestUpgradeIndex = 0;
            unsigned int bestUpgradeParam = 0;
            unsigned int bestLength = 0;
            auto testPrefs_ = std::vector<unsigned int>(bits_);
            // Copy the preferences, then loop over each of the indices in the bits, and change *only that bit* to each
            // possible value, track whichever change has the best result globally, then make only that bit change.
            for (int i = 1; i < bits_.size(); i++){
                testPrefs_[0] = i;
                auto pathL = graph_ref_.computePath(testPrefs_)[0];
                if (pathL > bestLength)
                {
                    bestLength = pathL;
                    bestUpgradeParam = i;
                }
            }
            for (int i = 1; i < bits_.size(); i++){
                //Reset the previous bit that we handled to the original value.
                testPrefs_[i-1] = bits_[i-1];
                for (auto edge : graph_ref_.getNodeEdgeSet(i)){
                    testPrefs_[i] = edge.first;
                    auto pathL = graph_ref_.computePath(testPrefs_)[0];
                    if (pathL > bestLength)
                    {
                        bestUpgradeIndex = i;
                        bestUpgradeParam = edge.first;
                        bestLength = pathL;
                    }
                }
            }
            bits_[bestUpgradeIndex] = bestUpgradeParam;
        }

        unsigned int changeBit(unsigned int in) override {
            return graph_ref_.getRandomEdge(in, gen_);
        }

    protected:
        const graph_lib::Graph & graph_ref_;
        std::mt19937 & gen_;
        int crossover_type_;
        // An array containing the exit node for each node in the graph
        // Its first entry contains the start node
    };
}