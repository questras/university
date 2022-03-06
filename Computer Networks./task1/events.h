#ifndef CURVE_FEVER_EVENTS_H
#define CURVE_FEVER_EVENTS_H

#include <cstdint>
#include <vector>
#include <string>

#include "constants.h"


class EventData {
public:
    virtual unsigned char *serialize(unsigned char *buf) = 0;
};

class Event {
public:

    uint32_t len;
    uint32_t event_no;
    uint8_t event_type;
    struct EventData *event_data;
    // It is calculated during serialization.
    uint32_t crc32 = 0;

    Event(uint32_t len, uint32_t event_no, uint8_t event_type, EventData *event_data);

    size_t size() const;

    unsigned char *serialize(unsigned char *buf);
};

class NewGameEventData : public EventData {
public:

    uint32_t maxx;
    uint32_t maxy;
    std::vector<std::string> players_names;

    NewGameEventData(uint32_t maxx, uint32_t maxy, const std::vector<std::string> &players_names);

    unsigned char *serialize(unsigned char *buf) override;
};

class PixelEventData : public EventData {
public:

    uint8_t player_number;
    uint32_t x;
    uint32_t y;

    PixelEventData(uint8_t player_number, uint32_t x, uint32_t y);

    unsigned char *serialize(unsigned char *buf) override;
};

class PlayerEliminatedEventData : public EventData {
public:

    uint8_t player_number;

    explicit PlayerEliminatedEventData(uint8_t player_number);

    unsigned char *serialize(unsigned char *buf) override;
};

// Game Over Event doesn't have any data, but the class is created
// to avoid dealing with null pointers in Event class.
class GameOverEventData : public EventData {
public:

    GameOverEventData() = default;

    unsigned char *serialize(unsigned char *buf) override;
};

class EventFactory {
public:
    static Event CreateNewGameEvent(uint32_t event_no, uint32_t maxx, uint32_t maxy,
                                    const std::vector<std::string> &players_names);

    static Event CreatePixelEvent(uint32_t event_no, uint8_t player_number, uint32_t x, uint32_t y);

    static Event CreatePlayerEliminatedEvent(uint32_t event_no, uint8_t player_number);

    static Event CreateGameOverEvent(uint32_t event_no);
};

#endif //CURVE_FEVER_EVENTS_H
