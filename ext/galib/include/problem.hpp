/**
 * @file problem.hpp
 *
 * Contains the gal::Problem class, which specifies the optimisation problem.
 *
 * @author Arthur van Dam, dam@math.uu.nl
 * @author Adriaan Graas, a.b.m.graas@uu.nl
 */
#pragma once

#include "chromosome.hpp"

namespace gal {
    /**
     * Contains information on the problem that is to be solved with GA.
     * A gal::GeneticAlgorithm requires an instance of a subclass of gal::Problem.
     * Both the objective function and some representation-related functions
     * are relevant.
     *
     * You always need to use a subclass of this class, where all virtual
     * functions below are defined.
     */
    template<typename C>
    class Problem {
    public:
        /**
         * Returns a new BitstringChromosome<T>.
         * Usually this will be an instance of *a subclass of* BitstringChromosome<T>.
         */
        virtual C createChromosome() const = 0;

        /**
         * Evaluates the objective function for the value that a chromosome
         * represents.
         */
        virtual double evaluate(C chromosome) const = 0;
    };
}