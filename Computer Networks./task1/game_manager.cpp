#include "game_manager.h"
#include "utils.h"
#include "constants.h"

using namespace std;


GameManager::GameManager(width_t width, height_t height, tur_speed_t turning_speed,
                         RandomGenerator *ptr)
        : board(width, height), random_generator_ptr(ptr),
          current_game_id(-1), turning_speed(turning_speed),
          alive_worms(0), is_current_game_finished(true) {}

vector<GameEventInfo> GameManager::initialize_new_game(uint32_t new_game_id,
                                                       vector<string> new_players,
                                                       vector<uint8_t> last_directions) {
    this->current_game_id = new_game_id;
    this->is_current_game_finished = false;
    this->board.reset();
    this->players = new_players;
    this->alive_worms = players.size();

    vector<GameEventInfo> generated_events;
    for (size_t i = 0; i < this->players.size(); i++) {
        player_name_to_index[this->players[i]] = i;

        pos_t worm_x = (get_rand() % get_board_width()) + 0.5;
        pos_t worm_y = (get_rand() % get_board_height()) + 0.5;
        dir_t worm_dir = get_rand() % 360;
        worms.emplace_back(worm_x, worm_y, worm_dir);
        worms[i].change_direction_modifier(last_directions[i]);

        cord_t x_cord = worms[i].get_x_cord(), y_cord = worms[i].get_y_cord();
        if (board.is_field_eaten(x_cord, y_cord)) {
            worms[i].eliminate();
            this->alive_worms--;
            generated_events.emplace_back(EVENT_TYPE_PLAYER_ELIMINATED, i);

            if (this->alive_worms == 1) {
                // Only 1 worm left alive, it is the winner.
                this->is_current_game_finished = true;
                generated_events.emplace_back(EVENT_TYPE_GAME_OVER);
                break;
            }
        }
        else {
            board.eat_field(x_cord, y_cord);
            generated_events.emplace_back(EVENT_TYPE_PIXEL, i, x_cord, y_cord);
        }
    }

    return generated_events;
}

width_t GameManager::get_board_width() const {
    return this->board.width;
}

height_t GameManager::get_board_height() const {
    return this->board.height;
}

vector<GameEventInfo> GameManager::play_turn() {
    vector<GameEventInfo> generated_events;

    for (size_t i = 0; i < this->worms.size(); ++i) {
        if (!(worms[i].is_eliminated)) {
            worms[i].turn(this->turning_speed);
            cord_t old_x = worms[i].get_x_cord(), old_y = worms[i].get_y_cord();
            worms[i].move();
            cord_t new_x = worms[i].get_x_cord(), new_y = worms[i].get_y_cord();

            if (old_x == new_x && old_y == new_y) {
                // Worm didn't move to new pixel.
                continue;
            }

            if (board.is_outside_board(new_x, new_y) || board.is_field_eaten(new_x, new_y)) {
                worms[i].eliminate();
                this->alive_worms--;
                generated_events.emplace_back(EVENT_TYPE_PLAYER_ELIMINATED, i);

                if (this->alive_worms == 1) {
                    // Only 1 worm left alive, it is the winner.
                    this->is_current_game_finished = true;
                    generated_events.emplace_back(EVENT_TYPE_GAME_OVER);
                    break;
                }
            }
            else {
                board.eat_field(new_x, new_y);
                generated_events.emplace_back(EVENT_TYPE_PIXEL, i, new_x, new_y);
            }
        }
    }

    return generated_events;
}

void GameManager::update_direction(const string &player_name, uint8_t next_direction) {
    worms[player_name_to_index[player_name]].change_direction_modifier(next_direction);
}

uint32_t GameManager::get_rand() {
    return this->random_generator_ptr->next();
}
