#ifndef CURVE_FEVER_MESSAGES_H
#define CURVE_FEVER_MESSAGES_H

#include <vector>
#include <cstdint>
#include <string>

#include "constants.h"
#include "events.h"


class ClientMessage {
public:

    player_id_t player_id;
    uint64_t session_id;
    uint8_t turn_direction;
    uint32_t next_expected_event_no;
    std::string player_name;

    // Constructor of object to represent an invalid object.
    ClientMessage();

    static unsigned char *deserialize(unsigned char *buf, size_t buf_size,
                                      const ip_t &host, port_t port,
                                      ClientMessage &data);

    bool is_valid() const;
};

class ServerMessage {
public:

    uint32_t game_id;
    std::vector<Event> events;

    ServerMessage(uint32_t game_id, const std::vector<Event> &events);

    unsigned char *serialize(unsigned char *ptr);
};

#endif //CURVE_FEVER_MESSAGES_H
