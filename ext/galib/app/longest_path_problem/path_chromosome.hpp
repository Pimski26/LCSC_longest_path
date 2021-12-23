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
// TODO: Include graph.h
#include "../../include/util.hpp"

namespace gal {
    /**
     * class IntegerChromosome implements a BitstringChromosome<int> that represents
     * a natural number. Bitstring is interpreted as a reversed binary
     * number.
     */
    class PathChromosome {
    public:
        /**
         * Constructs a new IntegerChromosome and initialises it randomly.
         * Do not set the length too large (<=32 generally)
         * @param length Length of the bitstring.
         */
        PathChromosome(Graph graph) {
            // Generate a random prefs_ to set a random path
            throw(RequiresImplementationError, "Constructor of PathChromosome is not yet implemented!");
            }
        }

        /**
         * Returns path length
         */
        int getPathLength() {
            // Use computePath from graph.h
            throw(RequiresImplementationError, "getPathLength is not yet implemented!")
        }

    protected:
        std::vector<unsigned int> prefs_;       // An array containing the exit node for each node in the graph
                                                // Its first entry contains the start node
    };
}