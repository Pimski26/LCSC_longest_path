/**
 * @file max_int_problem.hpp
 *
 * Contains the gal::IntegerMaximizationProblem class, which finds the maximal representable int.
 *
 * @author Arthur van Dam, dam@math.uu.nl
 * @author Adriaan Graas, a.b.m.graas@uu.nl
 */
#include "../../include/problem.hpp"
#include "int_chromosome.hpp"
// TODO: include graph.h

namespace gal {

    class LongestPathProblem : public Problem<PathChromosome> {
    public:
        explicit LongestPathProblem(Graph graph)
            : graph_(graph)
        {}

        /**
         * Constructs a new PathChromosome.
         */
        PathChromosome createChromosome() const override {
            return PathChromosome(graph_);
        }

        /**
         * Evaluates the objective function for an input value.
         */
        double evaluate(const PathChromosome chromo) const override {
            return chromo.getValue();

            
        }

    protected:
        Graph graph_;
    };
}