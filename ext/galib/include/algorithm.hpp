/**
 * @file algorithm.hpp
 *
 * Contains the gal::GeneticAlgorithm class, which should be instantiated in end-user
 * programs.
 *
 * @author Arthur van Dam, dam@math.uu.nl
 * @author Adriaan Graas, a.b.m.graas@uu.nl
 */
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <stdlib.h>
#include "util.hpp"
#include "chromosome.hpp"
#include "problem.hpp"

namespace gal {
    /**
     * GeneticAlgorithm is the center of an optimisation run: it contains the
     * BitstringChromosome members, the Problem at hand, and evolves the population
     * through generations.
     *
     * The GeneticAlgorithm type is a templated type and needs to be instantiated
     * with the type of the chromosome, denoted by C. For a function f: R -> R
     * this type is for instance a BitstringChromosome<double>. But C could also be a
     * derived type, like IntegerChromosome.
     *
     * @see nextGeneration()
     */
    template<typename C>
    class GeneticAlgorithm {
    public:
        /**
         * Constructs a new GeneticAlgorithm, and initializes the population.
         *
         * @param problem The Problem specifying what needs to be optimised.
         * @param population_size The number of chromosomes in this population.
         * @param mutation_probability 
         * @param crossover_probability
         */
        GeneticAlgorithm(const Problem<C> &problem,
                         int population_size,
                         double mutation_probability,
                         double crossover_probability)
                : problem_(problem),
                  population_(),
                  mutation_probability_(mutation_probability),
                  crossover_probability_(crossover_probability) {
            // create initial population
            population_.reserve(population_size);
            objectives_.reserve(population_size);
            for (int i = 0; i < population_size; ++i) {
                population_.push_back(problem_.createChromosome());
                objectives_.push_back(problem_.evaluate(population_[i]));
            }
        }

        /**
         * Evolves the population into a new generation.
         * This is done by mutation, selection and crossover.
         * @see mutate()
         * @see reproduce()
         */
        void nextGeneration() {
            // create new chromosomes, and throw old away
            population_ = reproduce(population_, objectives_);

            // randomly distort chromosomes in-place
            mutate(population_);

            // recompute the objective values of chromosomes in the population
            for (int i = 0; i < population_.size(); i++)
                objectives_[i] = problem_.evaluate(population_[i]);

            // log the best value of the current generation
            double max = *std::max_element(objectives_.begin(), objectives_.end());
            generation_max_objectives_.push_back(max);
        }

        /**
         * Iteration number
         */
        int generation() const {
            return generation_max_objectives_.size();
        }

        /**
         * Returns the current best chromosome in the population.
         */
        C optimum() const {
            auto m = std::max_element(objectives_.begin(), objectives_.end());
            return population_[std::distance(objectives_.begin(), m)];
        }

        /**
         * Objective values getter
         */
        std::vector<double> objectiveValues() const {
            return generation_max_objectives_;
        }

        /**
         * Population getter
         */
        std::vector<C> population() const {
            return population_;
        }

        /**
         * Tests whether the GeneticAlgorithm has converged. Convergence is
         * defined as an unchanged objective value over several generations.
         *
         * @param nr_equal Number of generations with the same objective value.
         */
        bool hasConverged(int nr_equal) const {
            if (generation() < nr_equal)
                return false;

            // all of the `nr_equal` last generations should have an objective
            // value equal to the last objective value.
            return std::all_of(
                    std::next(generation_max_objectives_.end(), -nr_equal),
                    generation_max_objectives_.end(),
                    [&](double v) { return v == generation_max_objectives_.back(); });
        }

        /**
         * Evolves the current population (generation) into the next generation:
         *   1. (roulette wheel) selection
         *   2. crossover on selected chromosomes to produce offspring
         */
        std::vector<C> reproduce(const std::vector<C> &population,
                                 const std::vector<double> &objectives) const {
            auto population_fitness = fitness(objectives);
            double total_fitness = std::reduce(population_fitness.begin(), population_fitness.end());
            std::vector<C> next_generation;
            next_generation.reserve(population.size());

            /*
            throw RequiresImplementationError(std::string(R"(
                Produce offspring by first selecting parents according to
                fitness, and then randomly combining them using crossover.
                Use `crossover_probability_`.

                To copy a BitstringChromosome, you could use the following syntax:
                    auto child = C(parent); // child is of type C
                This calls the so-called copy constructor of the chromosome.
            )"));
            */

            std::vector<C> parents;
            std::vector<C> children;

            while(next_generation.size() < population.size()){
                // Empty vectors of parents and children
                parents = std::vector<C>();
                children = std::vector<C>();

                while(parents.size() < 2 && next_generation.size() < population.size()){
                    // Use roulette method to select survivor
                    float roulette = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/total_fitness));
                    float roulette_test = 0;
                    auto survivor_index = 0;

                    while(roulette_test <= roulette){
                        float add_to_roulette_test = population_fitness[survivor_index];
                        survivor_index++;
                        roulette_test += add_to_roulette_test;
                    }

                    // Add survivor as parent with cross_over_probability_
                    float cross_over_rand = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
                    if(cross_over_rand <= crossover_probability_){
                        parents.push_back(population[survivor_index]);
                    }
                    // Otherwise add directly to next generation
                    else{
                        next_generation.push_back(population[survivor_index]);
                    }
                 }

                while(children.size() < 2 && next_generation.size() < population.size()){
                    // Copy the two parents
                    auto child_a = C(parents[0]);
                    auto child_b = C(parents[1]);
                    int max_pos = 16;
                    // Get random position between 0 and 15;
                    int pos = rand() / (RAND_MAX/max_pos);
                    // Crossover
                    child_a.crossover(pos, child_b);
                    // Add child_a to next generation
                    next_generation.push_back(child_a);
                    children.push_back(child_a);
                }

            }
            /**
             * Pseudo code:
             *
             * while (next_generation not full)
             *     while (nr. of parents < 2 && generation not full)
             *         - select() a survivor, add to next generation.
             *         - Allow the survivor to be a parent with rate `crossover_probability_`.
             *
             *     while (nr. of children < 2 && generation not full)
             *         - Create two copies of parents.
             *         - Crossover the copies to create children, add one to the next generation.
             */
        }

        /**
         * Evaluates the fitness of the current population.
         */
        std::vector<double> fitness(const std::vector<double> &objectives) const {
            if (fitness_b <= fitness_a)
                throw std::logic_error("Fitness values are incorrect.");

            throw RequiresImplementationError(std::string(R"(
                Compute fitness of the members using a vector of objectives
                [f(x_0), f(x_1), ...], see CB section 4.1.1.

                Hint: Use the members `fitness_a` and `fitness_b` in the
                class. Normalize the resulting vector so that the sum of
                fitness values is 1. Mind the special case f+ equals f-.
            )"));
        }

        /**
         * Mutate the bits of each chromosome in the population with probability
         * `mutation_probability_`.
         * @param population
         */
        void mutate(const std::vector<C> &population) const {
            throw RequiresImplementationError(std::string(R"(
                Call `mutate()` on the chromosomes C. Use `mutation_probability_`.
            )"));
        }

        /**
         * Selects a new BitstringChromosome from the current population by roulette
         * wheel selection.
         * @param population_fitness Fitness values for each member.
         * @param total_fitness Sum of `population_fitness`
         * @return The index in `population_fitness_` vector.
         */
        int select(std::vector<double> &population_fitness, double &total_fitness) const {
            throw RequiresImplementationError(std::string(R"(
                Use roulette wheel selection to select a member for
                the next generation (CB section 4.1.2).

                Hint: update `population_fitness` and `total_fitness`
                to have a fair selection in the next draw, and to
                prevent an already selected chromosome to be selected
                again.
            )"));
        }

        double fitness_a = 1.0;
        double fitness_b = 10.0;
    protected:
        const Problem<C> &problem_;                 // problem with obj. func. and BitstringChromosome constructors
        std::vector<C> population_;                 // Population members (chromosomes C)
        std::vector<double> objectives_;            // Population evaluation `f(x_i)`
        std::vector<double> generation_max_objectives_;   // History of best objective values
        double mutation_probability_;               // Probability of mutation for bits
        double crossover_probability_;              // Probability of crossover for chromosomes
    };
}
