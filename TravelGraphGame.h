#pragma once

#include <boost/graph/adjacency_list.hpp>
#include "Site.h"

class TravelGraphGame {
    using graph_type = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, int>;
    graph_type g;
    const std::vector<Site> &sites;
    std::vector<int> my_path;
    int my_start, my_location, my_destination;
public:
    explicit TravelGraphGame(std::vector<Site> &isites);
    void make_graph();
    void game_loop();
};
