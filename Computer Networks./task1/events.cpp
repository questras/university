#include <cstring>

#include "events.h"
#include "serializers.h"
#include "utils.h"

using std::vector;
using std::string;


Event::Event(uint32_t len, uint32_t event_no, uint8_t event_type,
             EventData *event_data)
        : len(len), event_no(event_no), event_type(event_type),
          event_data(event_data) {}

PixelEventData::PixelEventData(uint8_t player_number, uint32_t x, uint32_t y)
        : player_number(player_number), x(x), y(y) {}

PlayerEliminatedEventData::PlayerEliminatedEventData(uint8_t player_number)
        : player_number(player_number) {}

NewGameEventData::NewGameEventData(uint32_t maxx, uint32_t maxy,
                                   const vector<std::string> &players_names)
        : maxx(maxx), maxy(maxy), players_names(players_names) {}

unsigned char *PixelEventData::serialize(unsigned char *buf) {
    buf = serialize_uint8(buf, this->player_number);
    buf = serialize_uint32(buf, this->x);
    buf = serialize_uint32(buf, this->y);

    return buf;
}

unsigned char *PlayerEliminatedEventData::serialize(unsigned char *buf) {
    buf = serialize_uint8(buf, this->player_number);

    return buf;
}

unsigned char *NewGameEventData::serialize(unsigned char *buf) {
    buf = serialize_uint32(buf, this->maxx);
    buf = serialize_uint32(buf, this->maxy);
    for (const auto &player_name : players_names) {
        buf = serialize_string(buf, player_name);
    }

    return buf;
}

unsigned char *GameOverEventData::serialize(unsigned char *buf) {
    // Game over doesn't have data, so just return the unchanged buf.
    return buf;
}

unsigned char *Event::serialize(unsigned char *buf) {
    unsigned char *beg_ptr = buf;
    buf = serialize_uint32(buf, this->len);
    buf = serialize_uint32(buf, this->event_no);
    buf = serialize_uint8(buf, this->event_type);
    buf = this->event_data->serialize(buf);

    // Calculate crc32 based on buffered data.
    this->crc32 = calculate_crc32(beg_ptr, buf - beg_ptr);
    buf = serialize_uint32(buf, this->crc32);

    return buf;
}

size_t Event::size() const {
    return this->len + sizeof(this->len) + sizeof(this->crc32);
}

Event EventFactory::CreateNewGameEvent(uint32_t event_no, uint32_t maxx, uint32_t maxy,
                                       const std::vector<std::string>& players_names) {
    size_t event_fields_len = 0;
    event_fields_len += sizeof(uint32_t); // size of event_no.
    event_fields_len += sizeof(uint32_t); // size of maxx.
    event_fields_len += sizeof(uint32_t); // size of maxy.
    event_fields_len += sizeof(uint8_t); // size of event_type.

    for (const auto &player_name : players_names) {
        event_fields_len += player_name.size();
    }
    // +1 for each name because each name has '\0' at the end.
    event_fields_len += players_names.size();

    auto *new_game_event_data = new NewGameEventData(maxx, maxy, players_names);

    return Event(event_fields_len, event_no, EVENT_TYPE_NEW_GAME, new_game_event_data);
}

Event EventFactory::CreatePixelEvent(uint32_t event_no, uint8_t player_number,
                                     uint32_t x, uint32_t y) {
    size_t event_fields_len = 0;
    event_fields_len += sizeof(uint32_t); // size of event_no.
    event_fields_len += sizeof(uint32_t); // size of x.
    event_fields_len += sizeof(uint32_t); // size of y.
    event_fields_len += sizeof(uint8_t); // size of event_type.
    event_fields_len += sizeof(uint8_t); // size of player_number.

    auto *pixel_event_data = new PixelEventData(player_number, x, y);

    return Event(event_fields_len, event_no, EVENT_TYPE_PIXEL, pixel_event_data);
}

Event EventFactory::CreatePlayerEliminatedEvent(uint32_t event_no, uint8_t player_number) {
    size_t event_fields_len = 0;
    event_fields_len += sizeof(uint32_t); // size of event_no.
    event_fields_len += sizeof(uint8_t); // size of event_type.
    event_fields_len += sizeof(uint8_t); // size of player_number.

    auto *data = new PlayerEliminatedEventData(player_number);

    return Event(event_fields_len, event_no, EVENT_TYPE_PLAYER_ELIMINATED, data);
}

Event EventFactory::CreateGameOverEvent(uint32_t event_no) {
    size_t event_fields_len = 0;
    event_fields_len += sizeof(uint32_t); // size of event_no.
    event_fields_len += sizeof(uint8_t); // size of event_type.

    auto *data = new GameOverEventData();

    return Event(event_fields_len, event_no, EVENT_TYPE_GAME_OVER, data);
}
