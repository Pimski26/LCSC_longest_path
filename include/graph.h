//
// Created by emi on 02/12/2021.
//
#ifndef LCSCGA_GRAPH_H
#define LCSCGA_GRAPH_H

#include <vector>
#include <unordered_map>

namespace galplib{
    class Graph{
    public:
        Graph randomGraph(double p, unsigned int size);
        Graph(std::vector<std::vector<unsigned int>> & input, unsigned int nodes);
        int getEdge(unsigned int i, unsigned int j);
    private:
        std::vector<std::unordered_map<unsigned int, int>> edges;
        unsigned int nodeCount;
    };


}

#endif //LCSCGA_GRAPH_H
