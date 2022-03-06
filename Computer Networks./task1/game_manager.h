#ifndef CURVE_FEVER_GAME_MANAGER_H
#define CURVE_FEVER_GAME_MANAGER_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "utils.h"
#include "board.h"
#include "worm.h"
#include "game_event_info.h"

using std::map;
using std::string;
using std::vector;


class GameManager {
private:

    Board board;
    RandomGenerator *random_generator_ptr;

    uint32_t current_game_id;
    tur_speed_t turning_speed;
    map<string, uint8_t> player_name_to_index;
    vector<string> players;
    vector<Worm> worms;
    uint8_t alive_worms;

public:

    bool is_current_game_finished;

    GameManager(width_t width, height_t height, tur_speed_t turning_speed, RandomGenerator *ptr);

    // Initialize new game with new game id and given players and
    // their last chosen directions. Return
    // events that happened during game initialization.
    vector<GameEventInfo> initialize_new_game(uint32_t new_game_id, vector<string> players,
                                              vector<uint8_t> last_directions);

    // Perform one turn of the game and return events that happened during
    // this turn.
    vector<GameEventInfo> play_turn();

    // Update given player's direction modifier, based on which
    // player's worm will change (or not) its direction.
    void update_direction(const string &player_name, uint8_t next_direction);

    width_t get_board_width() const;

    height_t get_board_height() const;

    // Return next random number from current random number generator.
    uint32_t get_rand();
};

#endif //CURVE_FEVER_GAME_MANAGER_H
