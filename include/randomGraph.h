//
// Created by emi on 09/12/2021.
//
#include "graph.h"

#ifndef LCSCGA_RANDOMGRAPH_H
#define LCSCGA_RANDOMGRAPH_H
namespace galplib {
    Graph erdosGraphRecursive(unsigned int nodes, double p, int seed);
    Graph erdosGraphRejection(unsigned int nodes, double p, int seed);
    std::vector<std::vector<unsigned int>> erdosInputGen(unsigned int nodes, double p, int seed);\
    std::vector<std::vector<unsigned int>> findConnectedComponents(std::vector<std::vector<unsigned int>> input, unsigned int nodes) ;

    }
#endif //LCSCGA_RANDOMGRAPH_H
