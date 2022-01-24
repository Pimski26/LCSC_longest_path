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

namespace gal {
    /**
     * Maximises an integer (basic GA test)
     * The chromosome with all 1's in its bitstring is the best.
     */
    class IntegerMaximizationProblem : public Problem<IntegerChromosome> {
    public:
        explicit IntegerMaximizationProblem(int chromosome_length)
            : chromosome_length_(chromosome_length)
        {}

        /**
         * Constructs a new BitstringChromosome<int> suitable for representing integers.
         */
        IntegerChromosome createChromosome() const override {
            return IntegerChromosome(chromosome_length_);
        }

        /**
         * Evaluates the objective function for an input value.
         */
        double evaluate(const IntegerChromosome chromo) const override {
            return chromo.getValue();

            /*
            throw RequiresImplementationError(std::string(R"(
                Return an objective value such that the integer
                value represented by the chromosome is maximised.
            )"));
            */
        }
        int getChromosomeLength() const override {return chromosome_length_;}
    protected:
        int chromosome_length_;
    };
}