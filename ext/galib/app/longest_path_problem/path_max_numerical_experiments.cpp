#include <filesystem>
#include "main.h"
#include "longest_path_problem.hpp"
#include "../../include/algorithm.hpp"
#include "../../include/testGraphs.h"

using namespace gal;

std::pair<double, double> runGA(int runs, gal::LongestPathProblem::Problem & problem, RunParameters & cfg){
    double avg_generation = 0;
    double avg_objective = 0;
    for (int i = 0; i < runs; i++) {
        auto ga = GeneticAlgorithm<PathChromosome>(
                problem,
                cfg.population_size,
                cfg.mutation_probability,
                cfg.crossover_probability,
                cfg.nr_of_elites);

        // run genetic algorithm until `nr_generations` or convergence criteria
        while (true) {
            ga.nextGeneration();

            if (ga.hasConverged(cfg.convergence_threshold)
                || ga.generation() >= cfg.nr_generations) {
                avg_generation += ga.generation();
                avg_objective += ga.objectiveValues().back();
                break;
            }
        }
    }
    return {avg_generation / runs, avg_objective / runs};
}

void runParamTests(std::string st, gal::LongestPathProblem::Problem & problem, RunParameters & cfg,
                   int number_of_runs_to_avg,
                   std::vector<int> &population_size_array,
                   std::vector<double> &mutation_probability_array,
                   std::vector<double> &cross_over_probability_array,
                   std::vector<int> &nr_of_elites_array,
                   bool selectOpt){
    std::cout<<st << "Varying population_size:" << std::endl;
    double bestObjective = 0;
    double convergeTime = 0;
    unsigned int popSizeOpt = 0;
    unsigned int elitesOpt = 0;
    double mutationProbOpt = -1;
    double crossoverProbOpt = -1;
    auto cfgPop = cfg.population_size;
    for (auto population_size: population_size_array) {
        cfg.population_size = population_size;
        auto ret = runGA(number_of_runs_to_avg, problem, cfg);
        // ret.first == average of generation under the number of runs
        // red.second== average objective value under the number of runs
        if (!selectOpt) {
            std::cout << st << population_size << "," << ret.first << "," << ret.second
                      << std::endl;
        } else {
            if (bestObjective < ret.second){
                bestObjective = ret.second;
                popSizeOpt = population_size;
                convergeTime = ret.first;
            }
        }
    }
    cfg.population_size = cfgPop;
    std::cout << st << popSizeOpt << "," << convergeTime << "," << bestObjective
              << std::endl;
    bestObjective = 0;

    // Run numerical experiment for mutation probability
    std::cout<<st << "Varying mutation_probability:" << std::endl;
    auto cfgMut = cfg.mutation_probability;
    for (auto mutation_probability: mutation_probability_array) {
        cfg.mutation_probability = mutation_probability;
        auto ret = runGA(number_of_runs_to_avg, problem, cfg);
        if (!selectOpt) {
            std::cout << st << mutation_probability << "," << ret.first << "," << ret.second
                                                           << std::endl;
        } else {
            if (bestObjective < ret.second){
                bestObjective = ret.second;
                mutationProbOpt = mutation_probability;
                convergeTime = ret.first;
            }
        }
    }
    cfg.mutation_probability = cfgMut;

    std::cout << st << mutationProbOpt << "," << convergeTime << "," << bestObjective
              << std::endl;

    bestObjective = 0;
    // Run numerical experiment for crossover_probability
    std::cout<<st << "Varying crossover_probability:" << std::endl;
    auto cfgCro = cfg.crossover_probability;
    for (auto crossover_probability: cross_over_probability_array) {
        cfg.crossover_probability = crossover_probability;
        auto ret = runGA(number_of_runs_to_avg, problem, cfg);
        if (!selectOpt) {
            std::cout << st << crossover_probability << "," << ret.first << "," << ret.second
                                                            << std::endl;
        } else {
            if (bestObjective < ret.second){
                bestObjective = ret.second;
                crossoverProbOpt = crossover_probability;
                convergeTime = ret.first;
            }
        }
    }
    std::cout << st << crossoverProbOpt << "," << convergeTime << "," << bestObjective
              << std::endl;

    bestObjective = 0;
    // Run numerical experiment for nr of elites
    std::cout<<st << "Varying nr_of_elites:" << std::endl;
    auto cfgEli = cfg.nr_of_elites;
    for (auto nr_of_elites: nr_of_elites_array) {
        cfg.nr_of_elites = nr_of_elites;
        auto ret = runGA(number_of_runs_to_avg, problem, cfg);
        if (!selectOpt) {
            std::cout << st << nr_of_elites << "," << ret.first << "," << ret.second
                      << std::endl;
        } else {
            if (bestObjective < ret.second){
                bestObjective = ret.second;
                elitesOpt = nr_of_elites;
                convergeTime = ret.first;
            }
        }
    }
    cfg.nr_of_elites = cfgEli;
    std::cout << st << elitesOpt << "," << convergeTime << "," << bestObjective
              << std::endl;
}

void naiveTests(){
    RunParameters cfg = read_parameter_file();
    std::vector<int> graphNodes = {8, 10, 10, 10, 15,   15, 30, 30, 70, 70};
    std::vector<double> graphP = {0.2, 0.2, 0.15, 0.1, 0.1,   0.06, 0.06, 0.03, 0.03, 0.01};
    std::vector<int> graphSeed = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> population_size_array = {100,200,300,400,500,600,700,800,900,1000};
    std::vector<double> mutation_probability_array = {0.001,0.002,0.005,0.01,0.02,0.05,0.1,0.2,0.5,1};
    std::vector<double> cross_over_probability_array = {0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1};
    std::vector<int> nr_of_elites_array = {0,20,40,60,80,100,120,140,160,180,200};

    // Number of runs to average over
    int number_of_runs_to_avg = 2;
    for (int graphType = 0; graphType < 7; graphType++) {
        std::cout << "Graphtype: " << std::to_string(graphType) << std::endl << "{" << std::endl;
        for (int graph_i = 0; graph_i < 10; graph_i++) {
            unsigned int graphnInput = graphNodes[graph_i];
            if (graphType == 2) graphnInput = 9;
            if (graphType == 5) graphnInput = 3*(graphNodes[graph_i] / 3) + 1;
            auto graph = graph_lib::getGraphByType(graphType, graphnInput, graphP[graph_i], graphSeed[graph_i]);
            auto problem = LongestPathProblem(graph, graphSeed[graph_i], cfg.crossover_type, cfg.local_search);

            std::cout<<"\t" << "Graph params: " << graphNodes[graph_i] << ", " << graphP[graph_i] << ", " << graphSeed[graph_i] << std::endl;
            std::cout<<"\t" << "[" <<std::endl;
            std::string st = "\t\t";

            runParamTests(st, problem, cfg, number_of_runs_to_avg,
                          population_size_array,
                          mutation_probability_array,
                          cross_over_probability_array,
                          nr_of_elites_array,
                          true);

            std::cout<<"\t" << ")" << std::endl;
            // End graph params
        }
        std::cout << "}" << std::endl;
        // end graphtype
    }
}

void cfgTests(){
    RunParameters cfg = read_parameter_file();
    std::vector<int> graphNodes = {8, 10, 10, 10, 15,   15, 30, 30, 70, 70};
    std::vector<double> graphP = {0.2, 0.2, 0.15, 0.1, 0.1,   0.06, 0.06, 0.03, 0.03, 0.01};
    std::vector<int> graphSeed = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> population_size_array = {cfg.population_size};
    std::vector<double> mutation_probability_array = {cfg.mutation_probability};
    std::vector<double> cross_over_probability_array = {cfg.crossover_probability};
    std::vector<int> nr_of_elites_array = {cfg.nr_of_elites};
    // Number of runs to average over
    int number_of_runs_to_avg = 2;
    for (int graphType = 0; graphType < 7; graphType++) {
        std::cout << "Graphtype: " << std::to_string(graphType) << std::endl << "{" << std::endl;
        for (int graph_i = 0; graph_i < 10; graph_i++) {
            unsigned int graphnInput = graphNodes[graph_i];
            if (graphType == 2) graphnInput = 9;
            if (graphType == 5) graphnInput = 3*(graphNodes[graph_i] / 3) + 1;
            auto graph = graph_lib::getGraphByType(graphType, graphnInput, graphP[graph_i], graphSeed[graph_i]);
            auto problem = LongestPathProblem(graph, graphSeed[graph_i], cfg.crossover_type, cfg.local_search);

            std::cout<<"\t" << "Graph params: " << graphNodes[graph_i] << ", " << graphP[graph_i] << ", " << graphSeed[graph_i] << std::endl;
            std::cout<<"\t" << "[" <<std::endl;
            std::string st = "\t\t";

            runParamTests(st, problem, cfg, number_of_runs_to_avg,
                          population_size_array,
                          mutation_probability_array,
                          cross_over_probability_array,
                          nr_of_elites_array,
                          true);

            std::cout<<"\t" << ")" << std::endl;
            // End graph params
        }
        std::cout << "}" << std::endl;
        // end graphtype
    }
}

void runTimedAlgTests(int runs, graph_lib::Graph & graph, RunParameters & cfg, int seed){
    std::vector<bool> localSearch = {false, false, false, false, true};
    std::vector<int> crossType = {0, 1, 2, 3, 0};
    for (int i = 0; i < localSearch.size(); i++) {
        cfg.local_search = localSearch[i];
        cfg.crossover_type = crossType[i];
        auto problem = LongestPathProblem(graph, seed, cfg.crossover_type, cfg.local_search);
        std::cout << localSearch[i] << ", " << crossType[i] << ": \t";
        auto start = std::chrono::system_clock::now();

        auto ret = runGA(runs, problem, cfg);

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << ret.first << ", " << ret.second << ", " << elapsed_seconds.count() << std::endl;
    }
}

void graphExTests(){
    std::vector<unsigned int> graphTypes = {2, 3, 4, 5, 6};
    std::vector<int> graphNodes = {9, 25, 25, 25, 25};
    std::vector<int> seeds = {1, 2, 3, 4, 5};
    RunParameters cfg = read_parameter_file();
    int runs_to_avg = 10;
    for (int i = 0; i < graphTypes.size(); i++) {
        auto graph = graph_lib::getGraphByType(graphTypes[i], graphNodes[i], 0, 0);
        std::cout<< "graph : " << i << " [" << std::endl;
        runTimedAlgTests(runs_to_avg, graph, cfg, seeds[i]);
        std::cout<< "]" << std::endl;
    }

}

int main() {
    //naiveTests();
    //cfgTests();
    graphExTests();
    return 0;
}

/**
 *
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
                }else if (key == "crossover_type") {
                    config.crossover_type = std::stoi(value);
                } else if (key == "local_search") {
                    config.local_search = value == "true";
                } else {
                    throw std::runtime_error("Could not parse unknown parameter with key '" + key + "'.");
                }
            }
        }
    }

    return config;
}