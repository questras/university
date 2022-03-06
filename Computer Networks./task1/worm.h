#ifndef CURVE_FEVER_WORM_H
#define CURVE_FEVER_WORM_H

#include <cstdint>

#include "constants.h"


class Worm {
private:

    // Current worm direction, value between 0 and 359 (both ends inclusive).
    dir_t direction;
    pos_t x_pos, y_pos;
    uint8_t direction_modifier;

public:

    bool is_eliminated;

    Worm(pos_t x_pos, pos_t y_pos, dir_t direction);

    // Turn right, left or not at all based on current
    // direction modifier. Direction is changed by given value.
    void turn(uint16_t value);

    // Move the worm by 1 in current direction.
    void move();

    // Mark worm as eliminated.
    void eliminate();

    // Change current direction modifier of the worm.
    void change_direction_modifier(uint8_t modifier);

    cord_t get_x_cord() const;

    cord_t get_y_cord() const;
};

#endif //CURVE_FEVER_WORM_H
