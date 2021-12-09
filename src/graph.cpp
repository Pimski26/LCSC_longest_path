//
// Created by emi on 02/12/2021.
//

#include "../include/graph.h"
#include <stdexcept>
#include <unordered_set>

namespace galplib {
    bool compareIndex(std::vector<unsigned int> I1, std::vector<unsigned int> I2){
        return I1[0] < I2[0];
    }

    Graph::Graph(std::vector<std::vector<unsigned int>> & input, unsigned int nodes) {
        // make sure it's sorted!
        std::sort(input.begin(), input.end(), compareIndex);
        nodeCount = nodes;
        edges = std::vector<std::unordered_map<unsigned int, int>>();
        unsigned int iter = 0;
        for (int index = 0; index <= nodes; index++){
            std::unordered_map<unsigned int, int> edgeMap = std::unordered_map<unsigned int, int>();
            while (iter < input.size() && input[iter][0] == index){
                edgeMap.insert({input[iter][1], input[iter][2]});
                iter++;
            }
            edges.push_back(edgeMap);
        }
    }

    int Graph::getEdge(unsigned int i, unsigned int j) {
        if (i > nodeCount || j >> nodeCount){
            throw std::invalid_argument("node was not in graph!");
        }
        if (i < j){
            if (edges[i].contains(j)) return edges[i][j];
        } else {
            if (edges[j].contains(i)) return edges[j][i];
        }
        return -1;
    }

    std::vector<unsigned int> Graph::computePath(const std::vector<unsigned int> & prefs){
        std::unordered_set<unsigned int> visited = std::unordered_set<unsigned int>(prefs.size());
        unsigned int current = prefs[0];
        std::vector<unsigned int> path{0};
        while (true) {
            visited.insert(current);
            path.push_back(current);
            int w = getEdge(current, prefs[current]);
            if (w >= 0 && !visited.contains(prefs[current])){
                path[0] +=w;
                current = prefs[current];
                continue;
            }
            break;
        }
        return path;
    }

}
