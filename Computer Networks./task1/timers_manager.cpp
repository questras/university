#include <sys/timerfd.h>
#include <unistd.h>

#include "timers_manager.h"
#include "constants.h"
#include "debug.h"


TimersManager::TimersManager(struct pollfd *poll_sockets_ptr)
        : poll_sockets_ptr(poll_sockets_ptr) {}

bool TimersManager::add_timer(const player_id_t &player_id) {
    int tfd = timerfd_create(CLOCK_REALTIME, 0);
    if (tfd < 0) {
        warning("Timer could not be created.");
        return false;
    }

    timer_id_t timer_id = -1;
    for (int i = first_timer_index; i <= last_timer_index; ++i) {
        if (this->poll_sockets_ptr[i].fd == -1) {
            timer_id = i;
            break;
        }
    }
    if (timer_id < 0) {
        warning("Timer could not be created - too many timers already.");
        return false;
    }

    this->poll_sockets_ptr[timer_id].fd = tfd;
    timer_to_player.insert({timer_id, player_id});
    player_to_timer.insert({player_id, timer_id});

    return true;
}

void TimersManager::refresh_timer(const player_id_t &player_id) {
    struct itimerspec spec = {
            {0,                               0},
            {CLIENT_DISCONNECT_THRESHOLD_SEC, 0}
    };

    timer_id_t timer_id = player_to_timer[player_id];
    timerfd_settime(this->poll_sockets_ptr[timer_id].fd, 0, &spec, nullptr);
}

void TimersManager::remove_timer(const player_id_t &player_id) {
    timer_id_t timer_id = player_to_timer[player_id];

    timer_to_player.erase(timer_id);
    player_to_timer.erase(player_id);
    poll_sockets_ptr[timer_id].fd = -1;
}

vector<player_id_t> TimersManager::get_timeout_players() {
    vector<player_id_t> timeout_players;

    for (int i = first_timer_index; i <= last_timer_index; ++i) {
        if (poll_sockets_ptr[i].fd != -1 && poll_sockets_ptr[i].revents & POLLIN) {
            timeout_players.push_back(timer_to_player[i]);
        }
    }

    return timeout_players;
}

bool TimersManager::start_turn_timer(uint32_t threshold_ms) {
    int tfd = timerfd_create(CLOCK_REALTIME, 0);
    if (tfd < 0) {
        warning("Turn timer could not be created.");
        return false;
    }

    struct itimerspec spec = {
            {0, threshold_ms * 1000000},
            {0, threshold_ms * 1000000}
    };

    poll_sockets_ptr[turn_timer_index].fd = tfd;
    timerfd_settime(poll_sockets_ptr[turn_timer_index].fd, 0, &spec, nullptr);

    return true;
}

void TimersManager::remove_turn_timer() {
    struct itimerspec spec = {
            {0, 0},
            {0, 0}
    };

    timerfd_settime(poll_sockets_ptr[turn_timer_index].fd, 0, &spec, nullptr);
    poll_sockets_ptr[turn_timer_index].fd = -1;
}

bool TimersManager::time_for_turn() {
    bool result = false;
    if (poll_sockets_ptr[turn_timer_index].revents & POLLIN) {
        result = true;
        // Read from turn timer socket so it starts to count time again.
        uint64_t data;
        ssize_t res = read(poll_sockets_ptr[turn_timer_index].fd, &data, sizeof(data));
        if (res < 0) {
            warning("Error when reading from turn timer socket.");
        }
    }
    return result;
}
