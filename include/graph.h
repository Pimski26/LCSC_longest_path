//
// Created by emi on 02/12/2021.
//
#ifndef LCSCGA_GRAPH_H
#define LCSCGA_GRAPH_H

#include <vector>
#include <unordered_map>

namespace galplib{
    std::vector<std::vector<unsigned int>> sortInput(std::vector<std::vector<unsigned int>> input);

    class Graph{
    public:
        Graph(std::vector<std::vector<unsigned int>> & input, unsigned int nodes);
        std::vector<unsigned int> computePath(const std::vector<unsigned int> & prefs);
        int getEdge(unsigned int i, unsigned int j);
        std::unordered_map<unsigned int, int> getNodeEdgeSet(unsigned int i);
        unsigned int getNodeCount();
    private:
        std::vector<std::unordered_map<unsigned int, int>> edges;
        unsigned int nodeCount;
    };


}

#endif //LCSCGA_GRAPH_H
