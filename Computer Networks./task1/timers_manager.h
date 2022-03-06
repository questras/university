#ifndef CURVE_FEVER_TIMERS_MANAGER_H
#define CURVE_FEVER_TIMERS_MANAGER_H

#include <vector>
#include <map>
#include <poll.h>

#include "constants.h"

using timer_id_t = int;
using std::vector;
using std::map;


class TimersManager {
private:

    struct pollfd *poll_sockets_ptr;

    const int first_timer_index = 1;
    const int last_timer_index = MAX_NUMBER_OF_PLAYERS;
    const int turn_timer_index = last_timer_index + 1;

    map<player_id_t, timer_id_t> player_to_timer;
    map<timer_id_t, player_id_t> timer_to_player;

public:

    explicit TimersManager(struct pollfd *poll_sockets_ptr);

    // Add timer for given player. If timer is created successfully,
    // return true, false otherwise.
    bool add_timer(const player_id_t &player_id);

    // Refresh timer time for given user.
    void refresh_timer(const player_id_t &player_id);

    void remove_timer(const player_id_t &player_id);

    // Return players ids that faced timeout.
    vector<player_id_t> get_timeout_players();

    // Start interval timer counting time for next game turn.
    bool start_turn_timer(uint32_t threshold_ms);

    void remove_turn_timer();

    // Return true if turn timer expired, so it is time for next turn.
    bool time_for_turn();
};

#endif //CURVE_FEVER_TIMERS_MANAGER_H
