#include "Site.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numbers>
#include <boost/algorithm/string.hpp>

std::vector<std::string> Site::all_countries;
std::vector<std::string> Site::all_regions;

void Site::print_info() const {
    std::cout << name << std::endl;
    for (int i=0; i<name.size(); i++)
        std::cout << "=";
    std::cout << std::endl << "Lat: " << latitude << " Long:" << longitude << std::endl << description;
    if (countries.size() == 1) {
        std::cout << std::endl << "Country: " << all_countries[countries[0]];
    } else {
        std::cout << std::endl << "Countries: ";
        for (int i=0; i<countries.size(); i++) {
            std::cout << all_countries[countries[i]];
            if (i<countries.size()-1)
                std::cout << ", ";
        }
    }
    std::cout << std::endl << std::endl;
}

void Site::print_short() const {
    std::cout << name;  // might add more
}

std::string next_field(std::string &input, int &pos) {
    // read to next comma, or end quote, return without quotes
    std::string result;
    bool quoted = input[pos] == '"';
    if (quoted)
        pos++;
    int start{pos};
    bool done{false};
    while (!done) {
        if (quoted) {
            if (input[pos] == '"') {
                if (input.size() > pos && input[pos+1] == '"') {
                    pos++;  // pass the second quote
                } else {
                    done = true;
                    break;
                }
            }
        } else {
            if (input[pos] == ',') {
                done = true;
                break;
            }
        }
        result += input[pos];
        pos++;
    }

    int end{pos};
    if (quoted)
        pos++;  // end quote
    pos++;  // comma

    return result;
}

Site Site::from_csv_line(std::string &line) {
    Site site;
    int pos{0};  // position in line

    // Category
    std::string field = next_field(line, pos);
    if (field == "Cultural")
        site.category = Category::Cultural;
    else if (field == "Natural")
        site.category = Category::Natural;
    else  // Mixed
        site.category = static_cast<Category>(Category::Cultural | Category::Natural);

    // Country(ies)
    field = next_field(line, pos);
    std::vector<std::string> countries_vec;
    boost::split(countries_vec, field, [](char c){return c==',';});
    for (auto &country : countries_vec) {
        auto country_iter = std::find(all_countries.begin(), all_countries.end(), country);
        int country_idx;
        if (country_iter == all_countries.end()) {
            all_countries.push_back(country);
            country_idx = all_countries.size()-1;
        } else {
            country_idx = country_iter - all_countries.begin();
        }
        site.countries.push_back(country_idx);
    }

    // Region ... only one
    field = next_field(line, pos);
    auto region_iter = std::find(all_regions.begin(), all_regions.end(), field);
    int region_idx;
    if (region_iter == all_regions.end()) {
        all_regions.push_back(field);
        region_idx = all_regions.size();
    } else {
        region_idx = region_iter - all_regions.begin();
    }
    site.region = region_idx;

    // Unique number
    field = next_field(line, pos);
    site.number = std::stoi(field);

    // ID number
    field = next_field(line, pos);
    site.id = std::stoi(field);

    // rev_biz (???)
    field = next_field(line, pos);
    site.rev_bis = field;

    // Name
    field = next_field(line, pos);
    site.name = field;

    // Description
    field = next_field(line, pos);
    site.description = field;

    // Justification
    field = next_field(line, pos);
    site.justification = field;

    // Date inscribed
    field = next_field(line, pos);
    site.year_inscribed = std::stoi(field);

    // Secondary dates
    field = next_field(line, pos);
    std::vector<std::string> years;
    boost::split(years, field, [](char c){return c==',';});
    for (auto &year : years) {
        if (year.size() < 4)
            continue;
        site.secondary_years.push_back(std::stoi(year));
    }

    // skip "danger" field because we get it from future data
    field = next_field(line, pos);

    // End date
    field = next_field(line, pos);
    if (field.size() < 4)
        site.year_end = std::nullopt;
    else
        site.year_end = std::stoi(field);

    // Danger list
    // "Y yyyy" currently in danger since 4 digit year
    // "P yyyy-yyyy" Danger in past range of years, can be more than one
    field = next_field(line, pos);
    int dpos{pos};
    bool present_danger{false};

    while (dpos < field.size()) {
        if (field[dpos] == 'Y') {
            present_danger = true;
            site.danger_since = std::stoi(field.substr(dpos+2, 4));
            dpos += 7;  // skip past "Y yyyy "
        } else if (field[dpos] == 'P') {
            int start_year = std::stoi(field.substr(dpos+2, 4));
            int end_year = std::stoi(field.substr(dpos+7, 4));
            site.past_danger_years.emplace_back(start_year, end_year);
            dpos += 12;  // above plus another "-YYYY"
        }
    }
    if (!present_danger)
        site.danger_since = std::nullopt;

    // longitude, latitude
    field = next_field(line, pos);
    site.longitude = std::stod(field);
    field = next_field(line, pos);
    site.latitude = std::stod(field);

    // area
    field = next_field(line, pos);
    if (field.empty())
        site.area_hectares = 0;
    else
        site.area_hectares = std::stof(field);

    // Criteria
    // (i)..(x)
    site.criteria = static_cast<Criteria>(0);
    std::vector<std::string> criteria_vec;
    boost::split(criteria_vec, field, [](char c){return c=='(' || c==')';});
    for (auto &criterium : criteria_vec) {
        if (criterium == "i")
            site.criteria = static_cast<Criteria>(site.criteria | static_cast<Criteria>(Site::MasterpieceHumanGenius));
        if (criterium == "ii")
            site.criteria = static_cast<Criteria>(site.criteria | static_cast<Criteria>(Site::InterchangeHumanValues));
        if (criterium == "iii")
            site.criteria = static_cast<Criteria>(site.criteria | static_cast<Criteria>(Site::TestimonyTradition));
        if (criterium == "iv")
            site.criteria = static_cast<Criteria>(site.criteria | static_cast<Criteria>(Site::OutstandingExampleTechnology));
        if (criterium == "v")
            site.criteria = static_cast<Criteria>(site.criteria | static_cast<Criteria>(Site::OutstandingExampleSettlement));
        if (criterium == "vi")
            site.criteria = static_cast<Criteria>(site.criteria | static_cast<Criteria>(Site::AssociatedTraditions));
        if (criterium == "vii")
            site.criteria = static_cast<Criteria>(site.criteria | static_cast<Criteria>(Site::NaturalBeauty));
        if (criterium == "viii")
            site.criteria = static_cast<Criteria>(site.criteria | static_cast<Criteria>(Site::EarthHistory));
        if (criterium == "ix")
            site.criteria = static_cast<Criteria>(site.criteria | static_cast<Criteria>(Site::EcologicalBiological));
        if (criterium == "x")
            site.criteria = static_cast<Criteria>(site.criteria | static_cast<Criteria>(Site::DiversityHabitat));
    }

    // the final fields are duplicates
    return site;
}

// Next two functions brutally stolen and slightly adapted from https://www.geeksforgeeks.org/program-distance-two-points-earth/

// Utility function for converting degrees to radians
long double toRadians(const long double degree)
{
    static long double one_deg {(std::numbers::pi_v<long double>) / 180};
    return (one_deg * degree);
}

long double circle_distance(const Site &a, const Site &b) {
    // Convert the latitudes and longitudes from degree to radians.
    long double lat1 = toRadians(a.latitude);
    long double long1 = toRadians(a.longitude);
    long double lat2 = toRadians(b.latitude);
    long double long2 = toRadians(b.longitude);

    // Haversine Formula
    long double dlong = long2 - long1;
    long double dlat = lat2 - lat1;

    long double ans = std::pow(std::sin(dlat / 2), 2) +
                      std::cos(lat1) * std::cos(lat2) *
                      std::pow(std::sin(dlong / 2), 2);

    ans = 2 * std::asin(std::sqrt(ans));

    // Radius of Earth in Kilometers, R = 6371
    // Use R = 3956 for miles
    long double R = 6371;

    // Calculate the result
    ans = ans * R;

    return ans;
}

