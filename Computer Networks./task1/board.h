#ifndef CURVE_FEVER_BOARD_H
#define CURVE_FEVER_BOARD_H

#include <set>

#include "constants.h"


class Board {
private:
    std::set<std::pair<width_t, height_t>> eaten_fields;
public:
    width_t width;
    height_t height;

    Board(width_t width, height_t height);

    // Mark field with given coordinates as eaten.
    void eat_field(width_t x, height_t y);

    bool is_field_eaten(width_t x, height_t y);

    bool is_outside_board(cord_t x, cord_t y) const;

    // Reset board - remove all points that are in eaten_fields set.
    void reset();
};

#endif //CURVE_FEVER_BOARD_H
