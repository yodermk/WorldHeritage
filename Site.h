#pragma once
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <folly/FBString.h>

class Site {
    enum Category { Cultural=1, Natural=2 };
    enum Criteria {  // from https://whc.unesco.org/en/criteria/
        MasterpieceHumanGenius       = 1,
        InterchangeHumanValues       = 2,
        TestimonyTradition           = 4,
        OutstandingExampleTechnology = 8,
        OutstandingExampleSettlement = 16,
        AssociatedTraditions         = 32,
        NaturalBeauty                = 64,
        EarthHistory                 = 128,
        EcologicalBiological         = 256,
        DiversityHabitat             = 512
    };

    static std::vector<std::string> all_countries;
    static std::vector<std::string> all_regions;

    Category category;
    int region;
    int number;
    int id;
    int year_inscribed;
    std::optional<int> year_end;
    std::optional<int> danger_since;
    long double latitude;
    long double longitude;
    float area_hectares;
    Criteria criteria;
    std::vector<int> countries;
    std::string rev_bis;
    std::string name;
    std::string description;
    std::string justification;
    std::vector<int> secondary_years;
    std::vector<std::pair<int, int>> past_danger_years;

    Site() {}
public:
    static Site from_csv_line(std::string &line);
    void print_info() const;
    void print_short() const;

    friend long double circle_distance(const Site &a, const Site &b);
};

long double circle_distance(const Site &a, const Site &b);
