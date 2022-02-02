/**
 * @file main.hpp
 *
 * Combines all files and settings for a basic GALib run.
 *
 * @author Arthur van Dam, dam@math.uu.nl
 * @author Adriaan Graas, a.b.m.graas@uu.nl
 */
#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

/**
 * A struct with all necessary parameters to initialize an optimisation
 * Problem and a GeneticAlgorithm.
 * Parameters are generally read from file.
 */
struct RunParameters {
    int graph_type;
    int graph_nodes;
    double graph_p;
    bool graph_override_ones;
    int random_seed;
    int nr_generations;            // Nr of generations
    int population_size;           // Nr of strings per generation
    int chromosome_length;         // Length of each bitstring
    double mutation_probability;   // Probability of mutation for a gene
    double crossover_probability;  // Probability of crossover for a string
    int convergence_threshold;     // Convergence if max fitness is equal for this nr of generations
    int nr_of_elites;              // Nr of elites per generation
    int crossover_type;
    bool local_search;
};

/**
 * Reads parameters from a file.
 */
RunParameters read_parameter_file(const std::string &filename = "../app/longest_path_problem/default.ini");