#ifndef CURVE_FEVER_MESSAGES_MANAGER_H
#define CURVE_FEVER_MESSAGES_MANAGER_H

#include <vector>
#include <string>
#include <cstdint>
#include <poll.h>

#include "game_event_info.h"
#include "constants.h"
#include "events.h"
#include "messages.h"

using std::vector;
using std::string;


class MessagesManager {
private:

    struct pollfd *poll_sockets_ptr;

    const int message_socket_index = 0;
    uint32_t game_id;
    vector<Event> events_history;

    // Send message over network to player with given player id with data specified
    // in buffer with given size. Return true if message was sent successfully,
    // false otherwise.
    bool send_message(player_id_t player_id, unsigned char *buffer, size_t data_size);

public:

    explicit MessagesManager(struct pollfd *poll_sockets_ptr);

    // Return true if there is new message to read, false otherwise.
    bool new_message();

    // Return next message waiting to be read on socket.
    ClientMessage get_next_message();

    // Send all events from history starting from given starting event number
    // to client specified by player_id.
    void send_past_events(player_id_t player_id, uint32_t starting_event_no);

    // Parse given game events to server events, add them to history and send
    // to all players with ids from players_ids.
    void send_new_events(const vector<player_id_t> &players_ids,
                         const vector<GameEventInfo> &game_events);

    // Send "new game" server event to players with provided ids. "new game"
    // event is based on provided data - players, maxx, maxy. This event is
    // saved to history.
    void send_new_game_event(vector<player_id_t> players_ids, vector<string> players,
                             uint32_t maxx, uint32_t maxy);

    // Reset events history and game id.
    void reset();

    void update_game_id(uint32_t new_game_id);
};

#endif //CURVE_FEVER_MESSAGES_MANAGER_H
