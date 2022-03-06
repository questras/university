#include <cmath>

#include "worm.h"
#include "constants.h"


namespace {
    double degrees_to_radians(uint16_t degrees) {
        return (degrees / 180.0) * ((double) M_PI);
    }
}

Worm::Worm(pos_t x_pos, pos_t y_pos, dir_t direction)
        : direction(direction), x_pos(x_pos), y_pos(y_pos),
          direction_modifier(TURN_DIRECTION_STRAIGHT),
          is_eliminated(false) {}

void Worm::turn(uint16_t value) {
    if (this->direction_modifier == TURN_DIRECTION_LEFT) {
        if (value > this->direction) {
            value -= this->direction;
            this->direction = 360;
        }
        this->direction -= value;
    }
    else if (this->direction_modifier == TURN_DIRECTION_RIGHT) {
        this->direction = (this->direction + value) % 360;
    }
}

void Worm::eliminate() {
    this->is_eliminated = true;
}

void Worm::change_direction_modifier(uint8_t modifier) {
    this->direction_modifier = modifier;
}

cord_t Worm::get_x_cord() const {
    if (this->x_pos < 0) {
        return -1;
    }

    return this->x_pos;
}

cord_t Worm::get_y_cord() const {
    if (this->y_pos < 0) {
        return -1;
    }
    return this->y_pos;
}

void Worm::move() {
    double radians = degrees_to_radians(this->direction);
    this->x_pos += std::cos(radians);
    this->y_pos += std::sin(radians);
}
