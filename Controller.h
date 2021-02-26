#pragma once

#include <filesystem>
#include "Site.h"

class Controller {
    std::vector<Site> sites;
public:
    explicit Controller();
    int load_file(std::filesystem::path filename);
    void run();
};
