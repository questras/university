#ifndef CURVE_FEVER_PLAYER_H
#define CURVE_FEVER_PLAYER_H

#include "messages.h"
#include <string>


class PlayerInfo {
public:
    uint64_t session_id;
    std::string name;
    uint8_t last_turn_direction;

    PlayerInfo(std::string name, uint64_t session_id, uint8_t last_turn_direction)
            : session_id(session_id), name(name), last_turn_direction(last_turn_direction) {}
};

#endif //CURVE_FEVER_PLAYER_H
