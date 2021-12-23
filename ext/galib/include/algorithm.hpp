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
                         double crossover_probability,
                         int nr_of_elites)
                : problem_(problem),
                  population_(),
                  mutation_probability_(mutation_probability),
                  crossover_probability_(crossover_probability),
                  nr_of_elites_(nr_of_elites) {
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
            population_ = reproduce(population_, objectives_, nr_of_elites_);

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
                                 const std::vector<double> &objectives,
                                 const unsigned int nr_of_elites = 0) const {
            auto population_fitness = fitness(objectives);
            double total_fitness = std::reduce(population_fitness.begin(), population_fitness.end());
            std::vector<C> next_generation;
            next_generation.reserve(population.size());


            // Check there are not more elites than the size of the population
            if(nr_of_elites > population.size()){
                throw std::logic_error("The nr of elites can at most be the size of the population.");
            }

            // Create list of numbers 0,1,...,population.size()-1
            // Sort these population indices by objective values of corresponding chromosomes
            std::vector<int> chromo_indices_by_obj;
            for(int i = 0; i < population.size(); i++){
                chromo_indices_by_obj.push_back(i);
            }
            // TODO: Verify this stuff works
            sort(chromo_indices_by_obj.begin(), chromo_indices_by_obj.end(),
                 [&objectives](int a, int b) {
                     return (objectives[a] > objectives[b]);
                 }
            );

            std::vector<C> sorted_population;
            for(int chromo_index :chromo_indices_by_obj){
                sorted_population.push_back(population[chromo_index]);
            }

            std::vector<C> parents;


            // Keep track of the number of elites we already added to next generation
            unsigned int elite_index = 0;
            // If there are elites, add all of them
            while(elite_index < nr_of_elites && next_generation.size() < population.size()){
                // TODO: Allow elites to be parents
                // TODO: Exempt elites from mutation

                // Set elite property for the chromosome to be true
                sorted_population[elite_index].setElite(true);
                // Add it to next generation
                next_generation.push_back(sorted_population[elite_index]);

                // Add elite as parent with cross_over_probability_
                float cross_over_rand = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
                if(cross_over_rand <= crossover_probability_){
                    parents.push_back(population[elite_index]);
                }

                elite_index++;
            }


            while(next_generation.size() < population.size()){

                while(parents.size() < 2 && next_generation.size() < population.size()){
                    int survivor_index = select(population_fitness, total_fitness);

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

                while(parents.size() >= 2 && next_generation.size() < population.size()){
                    // Copy the first parent from end of vector
                    auto child_a1 = C(parents.back());
                    // Remove the parent we just copied
                    parents.pop_back();
                    // Copy the second parent from end of vector
                    auto child_a2 = C(parents.back());
                    // Remove the parent we just copied
                    parents.pop_back();

                    // Copy both chromosomes for making second child
                    auto child_b1 = C(child_a1);
                    auto child_b2 = C(child_a2);

                    // Get random position between 0 and 15;
                    int max_pos = 16;
                    int pos = rand() / (RAND_MAX/max_pos);
                    // Crossover
                    child_a1.crossover(pos, child_a2);
                    // Add child_a to next generation
                    next_generation.push_back(child_a1);

                    // Check if there is room for second child
                    if(next_generation.size() < population.size()){
                        // Get random position between 0 and 15;
                        int pos = rand() / (RAND_MAX/max_pos);
                        // Crossover
                        child_b1.crossover(pos, child_b2);
                        // Add child_b to next generation
                        next_generation.push_back(child_b1);
                    }
                }

            }
            return next_generation;
        }

        /**
         * Evaluates the fitness of the current population.
         */
        std::vector<double> fitness(const std::vector<double> &objectives) const {
            if (fitness_b <= fitness_a)
                throw std::logic_error("Fitness values are incorrect.");
            // Implementation in accordance with CB section 4.1.1
            // Find minimum and maximum objective function values
            double max = -INFINITY;
            double min = INFINITY;
            for(auto obj_it = objectives.begin(); obj_it!= objectives.end(); obj_it++){
                if(*obj_it > max){
                    max = *obj_it;
                }
                if(*obj_it < min){
                    min = *obj_it;
                }
            }

            // Return vector with only values fitness_b if min and max are equal
            if(min == max){
                std::vector<double> fitness;
                for(auto obj_it = objectives.begin(); obj_it!= objectives.end(); obj_it++){
                    fitness.push_back(fitness_b);
                }
                return fitness;
            }
            else {
                // Calculate CB section 4.1.1 formula 4.6
                std::vector<double> fitness;
                for (auto obj_it = objectives.begin(); obj_it != objectives.end(); obj_it++) {
                    double fitness_val = fitness_a + (fitness_b - fitness_a) * ((*obj_it - min) / (max - min));
                    fitness.push_back(fitness_val);
                }

                return fitness;
            }
        }

        /**
         * Mutate the bits of each chromosome in the population with probability
         * `mutation_probability_`.
         * @param population
         */
        void mutate(std::vector<C> &population) const {
            // Loop over all chromosomes in population
            for(auto chromosome_it = population.begin(); chromosome_it != population.end(); chromosome_it++){
                // If elite, skip mutation, then set elite to false again
                if((*chromosome_it).isElite()) {
                    (*chromosome_it).setElite(false);
                } else {
                    // If not elite, mutate with probability mutation_probability_
                    (*chromosome_it).mutate(mutation_probability_);
                }
            }
        }

        /**
         * Selects a new BitstringChromosome from the current population by roulette
         * wheel selection.
         * @param population_fitness Fitness values for each member.
         * @param total_fitness Sum of `population_fitness`
         * @return The index in `population_fitness_` vector.
         */
        int select(std::vector<double> &population_fitness, double &total_fitness) const {
            // Use roulette method to select survivor
            float roulette = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/total_fitness));
            float roulette_test = 0;
            int survivor_index = 0;

            while(roulette_test <= roulette){
                roulette_test += population_fitness[survivor_index];
                survivor_index++;
            }

            return --survivor_index;
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
        int nr_of_elites_;                          // Nr of elites per generation
    };
}
