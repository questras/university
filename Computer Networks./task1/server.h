#ifndef CURVE_FEVER_SERVER_H
#define CURVE_FEVER_SERVER_H

#include <map>
#include <set>
#include <string>
#include <poll.h>
#include <vector>

#include "player.h"
#include "constants.h"
#include "worm.h"
#include "board.h"
#include "game_manager.h"
#include "timers_manager.h"
#include "messages_manager.h"
#include "utils.h"

using std::map;
using std::set;
using std::pair;
using std::string;
using std::vector;


class Server {
private:

    // Sockets to use for communication and timers. There is
    // 1 socket for messaging, 1 socket for turn timer one
    // MAX_NUMBER_OF_PLAYERS sockets for timeout timer for each player.
    static const int sockets_num = MAX_NUMBER_OF_PLAYERS + 2;
    struct pollfd poll_sockets[sockets_num];

    port_t port;
    uint16_t one_turn_time_ms;
    map<string, player_id_t> name_to_id;
    map<player_id_t, PlayerInfo> id_to_player_info;
    set<string> lobby;
    set<string> ready;
    set<player_id_t> spectators;
    bool game_started;

    GameManager game_manager;
    TimersManager timers_manager;
    MessagesManager messages_manager;
    RandomGenerator random_generator;

public:

    Server(port_t port, tur_speed_t turning_speed, rnd_speed_t rounds_per_sec,
           width_t board_width, height_t board_height, seed_t seed);

    void init();

    void loop();

    void disconnect(const player_id_t &player_id);

    void connect_player(const player_id_t &player_id, uint64_t session_id,
                        const string &name, uint8_t turn_direction);

    bool time_to_start_game();

    // Return ids of players, that should get messages about game
    // i.e spectators, ready players and players in lobby(waiting).
    vector<player_id_t> get_current_listeners();

    void handle_message(const ClientMessage &message);

    void start_game();

    void next_turn();

    void end_game();
};

#endif //CURVE_FEVER_SERVER_H
