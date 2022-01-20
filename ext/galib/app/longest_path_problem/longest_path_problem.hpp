/**
 * @file max_int_problem.hpp
 *
 * Contains the gal::IntegerMaximizationProblem class, which finds the maximal representable int.
 *
 * @author Arthur van Dam, dam@math.uu.nl
 * @author Adriaan Graas, a.b.m.graas@uu.nl
 */
#include "../../include/problem.hpp"
#include "path_chromosome.hpp"
// TODO: include graph.h

namespace gal {

    class LongestPathProblem : public Problem<PathChromosome> {
    public:
        explicit LongestPathProblem(graph_lib::Graph graph, int seed)
            : graph_(graph)
        {
            gen_ = std::mt19937(seed);
        }

        /**
         * Constructs a new PathChromosome.
         */
        PathChromosome createChromosome() override {
            return PathChromosome(graph_, gen_);
        }

        /**
         * Evaluates the objective function for an input value.
         */
        double evaluate(const PathChromosome chromo) const override {
            return chromo.getValue();

        }

    protected:
        graph_lib::Graph graph_;
        std::mt19937 gen_;
    };
}