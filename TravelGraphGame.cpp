#include <iostream>
#include <random>
#include "TravelGraphGame.h"

TravelGraphGame::TravelGraphGame(std::vector<Site> &isites) : sites(isites) {
    for (int i=0; i<sites.size(); i++)
        boost::add_vertex(i, g);
}

void TravelGraphGame::make_graph() {
    std::mt19937 rnd;
    // seed the psuedorandom number generator from a hardware random seed
    std::random_device rdev;
    rnd.seed(rdev());
    std::uniform_int_distribution<> place_gen(0, sites.size());
    std::uniform_int_distribution<> paths_gen(2, 10);  // range of number of base lines we'll draw from each site
    std::normal_distribution<> short_path_gen(0, 20);    // used to generate several paths to close sites
    std::normal_distribution<> long_path_gen(700, 100); // generate a long path to a distant site

    // add connection points from each vertex in the graph (UNESCO site) to others
    for (int i=0; i<sites.size(); i++) {
        std::cout << "Building site " << i << std::endl << std::flush;
        // how many paths going out from each place?
        int paths = paths_gen(rnd);

        // sort other sites by proximity to this one
        std::vector<std::pair<int, long double>> distances;
        distances.reserve(2000);
        for (int j=0; j<sites.size(); j++) {
            if (j != i)
                distances.emplace_back(std::make_pair(j, circle_distance(sites[i], sites[j])));
        }
        std::sort(distances.begin(), distances.end(), [](std::pair<int, long double> a,std::pair<int, long double> b) {
            return a.second < b.second;
        });

        // Now pick "paths" of a random distribution that will fall mostly on closer sites
        int dest;
        for (int j=0; j<paths; j++) {
            dest = std::abs(std::round(short_path_gen(rnd)));
            boost::add_edge(i, distances[dest].first, g);
        }
        // and a farther away path
        dest = long_path_gen(rnd);
        if (dest > 0 && dest < sites.size())
            boost::add_edge(i, distances[dest].first, g);
    }

    // Generate start location and goal
    my_start = place_gen(rnd);
    do {
        my_destination = place_gen(rnd);
    } while (my_destination == my_start);
    my_location = my_start;
}

void TravelGraphGame::game_loop() {
    bool repeated_location{false}, valid_input;
    std::string input;
    int next_dest;
    std::cout << "**** My Destination ****" << std::endl;
    sites[my_destination].print_info();
    do {
        std::cout << "**** My Current Location ****" << std::endl;
        sites[my_location].print_info();
        graph_type::out_edge_iterator eit, eend;
        std::tie(eit, eend) = boost::out_edges(my_location, g);
        std::vector<int> hops;
        // iterate through edges with an edge iterator and build the path menu
        while (eit != eend) {
            auto dest = eit->m_target;
            hops.push_back(dest);
            std::cout << hops.size()-1 << ": ";
            sites[dest].print_short();
            std::cout << std::endl;
            eit++;
        }

        valid_input = false;
        do {
            std::cout << "To where? (0-" << hops.size()-1 << ") or 'd' for destination info.  --> " << std::flush;
            std::cin >> input;
            if (input == "d") {
                std::cout << "**** My Destination ****" << std::endl;
                sites[my_destination].print_info();
                continue;
            }
            try {
                next_dest = std::stoi(input);
                if (next_dest < 0 || next_dest > sites.size()) {
                    std::cout << "Number out of range, try again!" << std::endl;
                } else {
                    valid_input = true;
                }
            } catch (std::invalid_argument &e) {
                std::cout << "Invalid input" << std::endl;
            } catch (std::out_of_range &e) {
                std::cout << "Number VERY out of range. Maybe cut back on the caffeine?" << std::endl;
            }
        } while (!valid_input);

        my_path.push_back(my_location);
        my_location = hops[next_dest];

        if (std::find(my_path.begin(), my_path.end(), my_location) != my_path.end()) {
            std::cout << "Ooops! You've been here before. Game over!" << std::endl;
            repeated_location = true;
        }
    } while (!repeated_location && my_location != my_destination);

    if (my_location == my_destination) {
        std::cout << "You made it to your destination and you WON!!!  Congratulations!" << std::endl;
        std::cout << "You took " << my_path.size() << " turns." << std::endl;
    }
}

