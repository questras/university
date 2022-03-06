#include <netinet/in.h>
#include <string>
#include <cstring>

#include "messages.h"
#include "serializers.h"
#include "validators.h"


ClientMessage::ClientMessage() : player_id({"", -1}), session_id(0),
                                 turn_direction(0), next_expected_event_no(0) {}

bool ClientMessage::is_valid() const {
    if (this->player_id.second == -1) {
        // Message was created as invalid.
        return false;
    }

    return validate_player_name(this->player_name) &&
           validate_turn_direction(this->turn_direction);
}

unsigned char *ClientMessage::deserialize(unsigned char *buf, size_t buf_size,
                                          const ip_t &host, port_t port,
                                          ClientMessage &data) {
    uint64_t session_id;
    uint8_t turn_direction;
    uint32_t next_expected_event_no;

    buf = deserialize_uint64(buf, session_id);
    buf = deserialize_uint8(buf, turn_direction);
    buf = deserialize_uint32(buf, next_expected_event_no);

    std::string player_name;
    size_t player_len = buf_size - sizeof(session_id) - sizeof(turn_direction)
                        - sizeof(next_expected_event_no);
    player_name.assign((char *) buf, player_len);

    data.player_id = {host, port};
    data.session_id = session_id;
    data.turn_direction = turn_direction;
    data.next_expected_event_no = next_expected_event_no;
    data.player_name = player_name;

    return buf;
}

ServerMessage::ServerMessage(uint32_t game_id, const std::vector<Event> &events)
        : game_id(game_id), events(events) {}

unsigned char *ServerMessage::serialize(unsigned char *ptr) {
    ptr = serialize_uint32(ptr, this->game_id);
    for (auto event : this->events) {
        ptr = event.serialize(ptr);
    }

    return ptr;
}
