#include <fstream>
#include <iostream>
#include "Controller.h"
#include "TravelGraphGame.h"

Controller::Controller() {

}

int Controller::load_file(std::filesystem::path filename) {
    std::string line;
    std::ifstream csvfile(filename);
    if (!csvfile.is_open() || !csvfile.good()) {
        std::cout << "Can not open input file" << std::endl;
        return 1;
    }

    // first line is headers, which we'll ignore
    std::getline(csvfile, line);

    while (csvfile.good()) {
        std::getline(csvfile, line);
        if (line.empty())
            break;
        sites.emplace_back(Site::from_csv_line(line));
    }
    return 0;
}

void Controller::run() {
    TravelGraphGame game(sites);
    game.make_graph();
    game.game_loop();
}
