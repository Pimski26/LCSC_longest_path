//
// Created by emi on 21/01/2022.
//

#ifndef LCSCGA_TESTGRAPHS_H
#define LCSCGA_TESTGRAPHS_H

#include <vector>
#include "graph.h"

namespace graph_lib {
    std::vector <std::vector<unsigned int>> simpleConnComp(unsigned int param);

    std::vector <std::vector<unsigned int>> exampleGraph(unsigned int param);

    std::vector <std::vector<unsigned int>> ringGraphTricky(unsigned int param);

    std::vector <std::vector<unsigned int>> ringGraphAscending(unsigned int param);

    std::vector <std::vector<unsigned int>> kiteGraph(unsigned int param);

    std::vector <std::vector<unsigned int>> antiLoop(unsigned int nodes);

    /**
     ** Select graph from file initialisation
     ** 1: randomRecursive, 2: randomRejection
     ** 3: ringGraph 2s and 1x1, 4: ringGraph ascending.
     ** 5: Kitegraph, 6: Antiloop (a-b-c-d with ab, cd = inf, and b-c two paths)
     **/
    Graph getGraphByType(int i, int param, double param2, int seed);
}
#endif //LCSCGA_TESTGRAPHS_H
