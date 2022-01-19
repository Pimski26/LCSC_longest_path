#include <filesystem>
#include "main.hpp"
#include "max_int_problem.hpp"
#include "../../include/algorithm.hpp"

using namespace gal;

int main() {

    RunParameters cfg = read_parameter_file();
    auto problem = IntegerMaximizationProblem(cfg.chromosome_length);

    int population_size_array[20] = {100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000};
    double mutation_probability_array[7] = {0.001,0.003,0.01,0.03,0.1,0.3,1};
    double cross_over_probability_array[20] = {0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1};
    int nr_of_elites_array[11] = {0,20,40,60,80,100,120,140,160,180,200};

    // Number of runs to average over
    int number_of_runs_to_avg = 20;

    // Run numerical experiment for population size
    for(auto population_size : population_size_array) {
        double avg_generation = 0;
        for(int i = 0; i < number_of_runs_to_avg; i++) {
            auto ga = GeneticAlgorithm<IntegerChromosome>(
                    problem,
                    population_size,
                    cfg.mutation_probability,
                    cfg.crossover_probability,
                    cfg.nr_of_elites);

            // run genetic algorithm until `nr_generations` or convergence criteria
            while (true) {
                ga.nextGeneration();

                if (ga.hasConverged(cfg.convergence_threshold)
                    || ga.generation() >= cfg.nr_generations) {
                        avg_generation += ga.generation();
                    break;
                }
            }
        }
        avg_generation = avg_generation/number_of_runs_to_avg;
        std::cout << population_size << "," << avg_generation << std::endl;
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