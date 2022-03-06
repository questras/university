#ifndef CURVE_FEVER_GAME_EVENT_INFO_H
#define CURVE_FEVER_GAME_EVENT_INFO_H

#include <cstdint>


class GameEventInfo {
public:
    uint8_t event_type, player_number;
    uint32_t x, y;

    // Constructor for pixel event type.
    GameEventInfo(uint8_t event_type, uint8_t player_number, uint32_t x, uint32_t y)
            : event_type(event_type), player_number(player_number), x(x), y(y) {}

    // Constructor for player eliminated event type.
    GameEventInfo(uint8_t event_type, uint8_t player_number)
            : event_type(event_type), player_number(player_number), x(0), y(0) {}

    // Constructor for game over event type.
    explicit GameEventInfo(uint8_t event_type)
            : event_type(event_type), player_number(0), x(0), y(0) {}
};

#endif //CURVE_FEVER_GAME_EVENT_INFO_H
