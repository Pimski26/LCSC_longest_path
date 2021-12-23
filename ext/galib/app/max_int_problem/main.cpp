#include <filesystem>
#include "main.hpp"
#include "max_int_problem.hpp"
#include "../../include/algorithm.hpp"

using namespace gal;

int main() {
    RunParameters cfg = read_parameter_file();
    auto problem = IntegerMaximizationProblem(cfg.chromosome_length);

    auto ga = GeneticAlgorithm<IntegerChromosome>(
            problem,
            cfg.population_size,
            cfg.mutation_probability,
            cfg.crossover_probability,
            cfg.nr_of_elites);

    // run genetic algorithm until `nr_generations` or convergence criteria
    while (true) {
        ga.nextGeneration();
        std::cout << std::endl << "Generation: " << ga.generation() << std::endl;

        auto popu = ga.population(); // vector of IntegerChromosome
        auto iter = popu.begin(); // an iterator of this vector
        for (int i = 0; i < 5; i++) {
            std::cout << iter->getValue() << " : " << iter->getBitstringText() << std::endl;
            iter++;
        }

        std::cout << " * Objective value: " << ga.objectiveValues().back() << std::endl;
        std::cout << " * Optimum solution: " << ga.optimum().getBitstringText() << std::endl;

        if (ga.hasConverged(cfg.convergence_threshold)
            || ga.generation() >= cfg.nr_generations) {
            std::cout << "Generation " << ga.generation() << " converged." << std::endl;
            break;
        }
    }

    return 0;
}

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
            value = trim(value);
            if (std::getline(in_line, value)) {
                if (key == "nr_generations") {
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