#include "board.h"


Board::Board(width_t width, height_t height) : width(width), height(height) {}

void Board::eat_field(width_t x, height_t y) {
    this->eaten_fields.insert({x, y});
}

bool Board::is_field_eaten(width_t x, height_t y) {
    return this->eaten_fields.count({x, y}) > 0;
}

bool Board::is_outside_board(cord_t x, cord_t y) const {
    return x < 0 || y < 0 || x >= this->width || y >= this->height;
}

void Board::reset() {
    this->eaten_fields.clear();
}
