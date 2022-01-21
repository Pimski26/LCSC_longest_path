#include <filesystem>
#include "main.h"
#include "longest_path_problem.hpp"
#include "../../include/algorithm.hpp"
#include "../../include/testGraphs.h"
using namespace gal;

int main(){
    RunParameters cfg = read_parameter_file();
    int seed = cfg.random_seed != 0 ? cfg.random_seed : std::mt19937 (std::random_device{}())();

    auto graph = graph_lib::getGraphByType(cfg.graph_type, cfg.graph_nodes, cfg.graph_p, seed);
    if (cfg.graph_override_ones) graph.oneify();

    auto problem = LongestPathProblem(graph, seed);

    auto ga = GeneticAlgorithm<PathChromosome>(
            problem,
            cfg.population_size,
            cfg.mutation_probability,
            cfg.crossover_probability,
            cfg.nr_of_elites);


    // run genetic algorithm until `nr_generations` or convergence criteria
    while (true) {
        ga.nextGeneration();
        std::cout << std::endl << "Generation: " << ga.generation() << std::endl;

        auto popu = ga.population(); // vector of PathChromosome
        auto iter = popu.begin(); // an iterator of this vector
        for (int i = 0; i < 5; i++) {
            std::cout << iter->getValue() << " : " << iter->getChromosomeText() << std::endl;
            iter++;
        }

        std::cout << " * Objective value: " << ga.objectiveValues().back() << std::endl;
        std::cout << " * Optimum solution: " << ga.optimum().getChromosomeText() << std::endl;

        if (ga.hasConverged(cfg.convergence_threshold)
            || ga.generation() >= cfg.nr_generations) {
            std::cout << "Generation " << ga.generation() << " converged." << std::endl;
            break;
        }
    }

    return 0;
};

/**
 * Removes whitespace at the beginning and end of a std::string
 * https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
 *
 * @param s
 * @return
 */
std::string_view trim(std::string_view s) {
    s.remove_prefix(std::min(s.find_first_not_of(" \t\r\v\n"), s.size()));
    s.remove_suffix(std::min(s.size() - s.find_last_not_of(" \t\r\v\n") - 1, s.size()));
    return s;
}

/**
 * Read a parameter file.
 *
 * @param filename
 * @return
 */
RunParameters read_parameter_file(const std::string &filename) {
    if (!std::filesystem::exists(filename)) {
        throw std::invalid_argument(
                "Input file " + filename
                + " does not exist. Note that, in case of relative paths,"
                  " the working directory of the executable must be correct."
                  " In CLion, the working directory can be configured in the"
                  " Run/Debug Configurations.");
    }

    std::ifstream file;
    file.open(filename);
    auto config = RunParameters();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream in_line(line);
        std::string key;
        if (std::getline(in_line, key, '=')) {
            std::string value;
            key = trim(key);
            if (std::getline(in_line, value)) {
                value = trim(value);
                if (key == "graph_type") {
                    config.graph_type = std::stoi(value);
                } else if (key == "graph_nodes") {
                    config.graph_nodes = std::stoi(value);
                } else if (key == "graph_p") {
                    config.graph_p = std::stod(value);
                } else if (key == "random_seed") {
                    config.random_seed = std::stoi(value);
                } else if (key == "graph_override_ones") {
                    config.graph_override_ones = value == "true";
                } else if (key == "nr_generations") {
                    config.nr_generations = std::stoi(value);
                } else if (key == "population_size") {
                    config.population_size = std::stoi(value);
                } else if (key == "chromosome_length") {
                    config.chromosome_length = std::stoi(value);
                } else if (key == "mutation_probability") {
                    config.mutation_probability = std::stod(value);
                } else if (key == "crossover_probability") {
                    config.crossover_probability = std::stod(value);
                } else if (key == "convergence_threshold") {
                    config.convergence_threshold = std::stoi(value);
                } else if (key == "nr_of_elites") {
                    config.nr_of_elites = std::stoi(value);
                } else {
                    throw std::runtime_error("Could not parse unknown parameter with key '" + key + "'.");
                }
            }
        }
    }

    return config;
}