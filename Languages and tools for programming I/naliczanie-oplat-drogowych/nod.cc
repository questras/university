/** @file
 * Implementation of: Naliczanie Opłat Drogowych.
 *
 * @author Szymon Czyżmański 417797
 * @author Michał Wiśniewski 418486
 */

#include <iostream>
#include <string>
#include <regex>
#include <cmath>

using std::string;
using std::stringstream;
using std::regex;
using std::regex_search;
using std::map;
using std::pair;
using std::tuple;
using std::make_tuple;
using std::get;
using std::max;
using std::min;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

/**
 * Pair representing road, storing road category and road number.
 */
using road_t = pair<char, uint16_t>;
/**
 * Tuple representing info about vehicle ride on given road
 * and context (line, line number) where that info occurred.
 */
using info_t = tuple<road_t, uint64_t, string, uint64_t>;
/**
 * Function used to compare two roads.
 */
using road_cmp_f = bool (*)(const road_t &, const road_t &);
/**
 * Map storing roads with distance travelled by all the vehicles on given road.
 */
using road_to_dist_t = map<road_t, uint64_t, road_cmp_f>;
/**
 * Map storing licences with distance travelled by vehicle with given licence
 * on all the roads.
 */
using licence_to_dist_t = map<string, map<char, uint64_t>>;

/** @brief Check if content of string is a number without leading zeros.
 *
 * @param str - string to check.
 * @return true if content of string is a number, false otherwise.
 */
static bool is_number(const string &str) {
    return !str.empty() &&
           str[0] != '0' &&
           all_of(str.begin(), str.end(), ::isdigit);
}

/** @brief Check if string matches correct road format.
 *
 * @param road - string to check.
 * @return true if string matches correct road format, false otherwise.
 */
static bool is_correct_road(const string &road) {
    size_t road_length = road.length();
    return road_length > 1 && road_length < 5 &&
           (road[0] == 'S' || road[0] == 'A') &&
           is_number(road.substr(1));
}

/** @brief Parse string representation of road and get its pair representation.
 *
 * Parse given string and return pair that consists of a char, that
 * represents road category ('A' or 'S') and uint16_t that
 * represents road number.
 * Assumption: @p road is correct string representation of a road.
 *
 * @param road - string representation of a road.
 * @return pair representation of a road.
 */
static road_t parse_road_to_pair(const string &road) {
    char category = road[0];
    uint16_t number;
    stringstream s(road.substr(1));
    s >> number;

    return {category, number};
}

/** @brief Parse string representation of distance and get its
 * uint64_t representation in metres.
 *
 * Parse given string that represents distance in kilometres with
 * one floating point number and return uint64_t representing
 * distance in metres.
 * Assumption: @p distance is correct string representation of distance.
 *
 * @param distance - string representation of distance.
 * @return distance representation in metres.
 */
static uint64_t parse_distance_to_metres(const string &distance) {
    uint64_t kilometres;
    uint64_t hundreds_of_metres;
    stringstream s(distance.substr(0, distance.length() - 2));
    s >> kilometres;
    /* Convert last character of the string to uint64_t. */
    hundreds_of_metres = distance[distance.length() - 1] - '0';

    return kilometres * 1000 + hundreds_of_metres * 100;
}

/** @brief Get licence with its info from correct input line.
 *
 * Parse input line and return pair that consists of licence and
 * tuple with info that consists of: pair representing road,
 * uint64_t representing distance in metres, string representing
 * input line and uint64_t representing number of input line.
 *
 * @param input_line  - input line with correct info.
 * @param line_number - number of input line.
 * @return licence with its info.
 */
static pair<string, info_t> get_licence_with_info(const string &input_line,
                                                  uint64_t line_number) {
    stringstream s(input_line);
    string licence_str, road_str, distance_str;
    s >> licence_str >> road_str >> distance_str;

    road_t road = parse_road_to_pair(road_str);
    uint64_t distance = parse_distance_to_metres(distance_str);

    return {licence_str, make_tuple(road, distance, input_line, line_number)};
}

/** @brief Get query parameter from correct query input line.
 *
 * Parse input line and return string representing query parameter
 * or empty string, if query parameter is not specified.
 *
 * @param input_line - correct query input line.
 * @return string representing query argument.
 */
static string get_query_parameter(const string &input_line) {
    stringstream s(input_line);
    string query_parameter;
    s >> query_parameter;

    if (query_parameter.length() == 1) {
        /* There can be a space between "?" and a possible query parameter. */
        s >> query_parameter;
        if (query_parameter.length() == 1) {
            /* Query parameter is not specified. */
            return "";
        }
        else {
            return query_parameter;
        }
    }
    else {
        /* There is no space between "?" and a parameter,
         * so cut "?" from the string. */
        return query_parameter.substr(1);
    }
}

/** @brief Print incorrect line and its number to standard error stream.
 *
 * @param line_number - number of line where the incorrect line occurred.
 * @param line        - incorrect line.
 */
static void print_error(uint64_t line_number, const string &line) {
    cerr << "Error in line " << line_number << ": " << line << endl;
}

/** @brief Print road name and distance travelled by all the vehicles on that
 * road to standard output stream.
 *
 * @param road         - pair representation of a road.
 * @param road_to_dist - map storing roads with corresponding distances.
 */
static void print_road_with_dist(const road_t &road,
                                 const road_to_dist_t &road_to_dist) {
    if (road_to_dist.count(road) == 1) {
        uint64_t dist = road_to_dist.at(road);

        cout << road.first << road.second << " "
             << dist / 1000 << "," << (dist % 1000) / 100 << endl;
    }
}

/** @brief For every road, print its name and distance travelled by all the
 * vehicles on that road.
 *
 * @param road_to_dist - map storing roads with corresponding distances.
 */
static void print_roads_with_dist(const road_to_dist_t &road_to_dist) {
    for (const auto &entry : road_to_dist) {
        road_t road = entry.first;
        print_road_with_dist(road, road_to_dist);
    }
}

/** @brief Print licence and distance travelled by vehicle with that licence
 * on all the roads.
 *
 * @param licence         - string representing correct licence.
 * @param licence_to_dist - map storing licences with corresponding distances.
 */
static void print_license_with_dist(const string &licence,
                                    const licence_to_dist_t &licence_to_dist) {
    if (licence_to_dist.count(licence) == 1) {
        map<char, uint64_t> road_category_to_dist = licence_to_dist.at(licence);

        cout << licence;

        for (const auto &category_entry : road_category_to_dist) {
            char category = category_entry.first;
            uint64_t dist = category_entry.second;
            cout << " " << category << " " << dist / 1000 << ","
                 << (dist % 1000) / 100;
        }

        cout << endl;
    }
}

/** @brief For every licence, print its name and distance travelled by vehicle
 * with that licence on all the roads.
 *
 * @param road_to_dist - map storing licences with corresponding distances.
 */
static void print_licences_with_dist(const licence_to_dist_t &licence_to_dist) {
    for (const auto &licence_entry : licence_to_dist) {
        string licence = licence_entry.first;
        print_license_with_dist(licence, licence_to_dist);
    }
}

/** @brief Check if previous info about vehicle with licence @p licence
 * is incorrect.
 *
 * Check if map @p licence_to_prev_info stores info about vehicle with licence
 * @p licence. If so, check if road name from @p curr_info is equal to road name
 * from previous info, stored under @p licence key in the
 * map @p licence_to_prev_info.
 *
 * @param licence              - string representing correct licence.
 * @param curr_info            - current info about vehicle with
 *                               licence @p licence.
 * @param licence_to_prev_info - map storing licenses with first info with
 *                               given road.
 * @return @p true if there was previous info about vehicle with given licence
 * and road from that info is not equal to road from current info,
 * @p false otherwise.
 */
static bool is_prev_info_incorrect(const string &licence, info_t &curr_info,
                                   const map<string, info_t> &licence_to_prev_info) {
    if (licence_to_prev_info.count(licence) == 0) {
        return false;
    }
    else {
        road_t curr_road = get<0>(curr_info);
        info_t prev_info = licence_to_prev_info.at(licence);
        road_t prev_road = get<0>(prev_info);

        return curr_road != prev_road;
    }
}

/** @brief Update maps storing data corresponding to roads and licences.
 *
 * Add distance travelled by vehicle with given licence on given
 * road to past distance from the view of that vehicle and from that road.
 *
 * @param licence              - string representing correct licence.
 * @param curr_info            - current info about vehicle with given licence.
 * @param licence_to_prev_info - map storing licenses with first info with
 *                               given road.
 * @param road_to_dist         - map storing roads with distance travelled on
 *                               given road by all the vehicles.
 * @param licence_to_dist      - map storing licences with distance travelled
 *                               by vehicle with corresponding licence on all
 *                               the roads.
 */
static void update_maps(const string &licence, info_t &curr_info,
                        const map<string, info_t> &licence_to_prev_info,
                        road_to_dist_t &road_to_dist,
                        licence_to_dist_t &licence_to_dist) {
    info_t prev_info = licence_to_prev_info.at(licence);
    road_t prev_road = get<0>(prev_info);
    char road_category = prev_road.first;
    uint64_t prev_dist = get<1>(prev_info);
    uint64_t curr_dist = get<1>(curr_info);
    uint64_t dist = max(prev_dist, curr_dist) - min(prev_dist, curr_dist);

    if (road_to_dist.count(prev_road) == 0) {
        road_to_dist[prev_road] = 0;
    }

    if (licence_to_dist.count(licence) == 0) {
        licence_to_dist[licence] = map<char, uint64_t>();
    }

    road_to_dist[prev_road] += dist;
    licence_to_dist[licence][road_category] += dist;
}

/** @brief Check if road @p r1 is smaller than road @p r2, first comparing by
 * numbers of these roads, and then by category.
 *
 * @param r1 - first road.
 * @param r2 - second road.
 * @return @p true if @p r1 < @p r2, @p false otherwise.
 */
static bool road_cmp(const road_t &r1, const road_t &r2) {
    if (r1.second == r2.second) {
        return r1.first < r2.first;
    }
    return r1.second < r2.second;
}

/** @brief Launch program, read data from standard input and interpret it.
 *
 * @return 0 if there is no critical error during program execution,
 * other non-zero value otherwise.
 */
int main() {
    regex info_regex = regex(R"(^\s*[a-zA-Z0-9]{3,11})"
                             R"(\s+[AS][1-9][0-9]{0,2})"
                             R"(\s+(?:0|[1-9][0-9]*),[0-9]\s*$)");
    regex query_regex = regex(
            R"(^\s*\?\s*(?:(?:[a-zA-Z0-9]{3,11})|(?:[AS][1-9][0-9]{0,2})){0,1}\s*$)");

    uint64_t line_number = 0;
    string line;
    std::smatch line_match;

    road_to_dist_t road_to_dist(road_cmp);
    map<string, info_t> licence_to_prev_info;
    licence_to_dist_t licence_to_dist;

    while (getline(cin, line)) {
        line_number++;
        if (regex_search(line, line_match, info_regex)) {
            auto licence_with_info = get_licence_with_info(line, line_number);
            string licence = licence_with_info.first;
            info_t curr_info = licence_with_info.second;

            if (is_prev_info_incorrect(licence, curr_info, licence_to_prev_info)) {
                string incorrect_line = get<2>(licence_to_prev_info.at(licence));
                uint64_t incorrect_line_number = get<3>(
                        licence_to_prev_info.at(licence));
                print_error(incorrect_line_number, incorrect_line);
                licence_to_prev_info.erase(licence);
            }

            if (licence_to_prev_info.count(licence) == 0) {
                licence_to_prev_info[licence] = curr_info;
            }
            else {
                update_maps(licence, curr_info, licence_to_prev_info,
                            road_to_dist, licence_to_dist);
                licence_to_prev_info.erase(licence);
            }
        }
        else if (regex_search(line, line_match, query_regex)) {
            string query_parameter = get_query_parameter(line);

            if (query_parameter.empty()) {
                print_licences_with_dist(licence_to_dist);
                print_roads_with_dist(road_to_dist);
            }
            else {
                print_license_with_dist(query_parameter, licence_to_dist);

                if (is_correct_road(query_parameter)) {
                    road_t road = parse_road_to_pair(query_parameter);
                    print_road_with_dist(road, road_to_dist);
                }
            }
        }
        else if (!line.empty()) {
            print_error(line_number, line);
        }
    }

    return 0;
}