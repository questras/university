/** @file
 * Implementation of geometry module.
 *
 * @author
 * @author Michał Wiśniewski 418486
 * @author Igor Witkowski 421607
 */

#include "geometry.h"

#include <cassert>
#include <climits>
#include <cstdint>
#include <utility>

Position::Position(int32_t x, int32_t y) : x_value(x), y_value(y) {}

Position::Position(const Vector &v) : Position(v.x(), v.y()) {}

int32_t Position::x() const {
    return this->x_value;
}

int32_t Position::y() const {
    return this->y_value;
}

Position Position::reflection() const {
    return Position(this->y_value, this->x_value);
}

const Position &Position::origin() {
    static Position p(0, 0);
    return p;
}

bool Position::operator==(const Position &other) const {
    return this->x_value == other.x() and this->y_value == other.y();
}

Position &Position::operator+=(const Vector &v) {
    this->x_value += v.x();
    this->y_value += v.y();

    return *this;
}

const Position operator+(const Position &p, const Vector &v) {
    Position result = p;  /// Copy of Position p.
    result += v;

    return result;
}

const Position operator+(const Vector &v, const Position &p) {
    return p + v;
}

Vector::Vector(int32_t x, int32_t y) : pos(x, y) {}

Vector::Vector(const Position &p) : Vector(p.x(), p.y()) {}

int32_t Vector::x() const {
    return this->pos.x();
}

int32_t Vector::y() const {
    return this->pos.y();
}

Vector Vector::reflection() const {
    return Vector(this->y(), this->x());
}

bool Vector::operator==(const Vector &other) const {
    return this->x() == other.x() && this->y() == other.y();
}

Vector &Vector::operator+=(const Vector &other) {
    this->pos += other;

    return *this;
}

const Vector operator+(const Vector &v1, const Vector &v2) {
    Vector result = v1;  /// Copy of Vector v1.
    result += v2;

    return result;
}

Rectangle::Rectangle(int64_t w, int64_t h, Position pos) : lower_left(pos) {
    assert((w > 0 and h > 0 and w <= UINT_MAX and h <= UINT_MAX));

    width_v = uint32_t(w);
    height_v = uint32_t(h);
}

Rectangle::Rectangle(int64_t w, int64_t h) : lower_left(Position::origin()) {
    assert((w > 0 and h > 0 and w <= UINT_MAX and h <= UINT_MAX));

    width_v = uint32_t(w);
    height_v = uint32_t(h);
}

Rectangle::Rectangle(const Rectangle &rect) : lower_left(rect.lower_left),
                                              width_v(rect.width_v),
                                              height_v(rect.height_v) {}

Rectangle &Rectangle::operator=(const Rectangle &rect) {
    if (this == &rect)  ///check for self-assignment
        return *this;

    this->width_v = rect.width_v;
    this->height_v = rect.height_v;
    this->lower_left = rect.lower_left;

    return *this;
}

bool Rectangle::operator==(const Rectangle &rect) const {
    return (this->width_v == rect.width_v and
            this->height_v == rect.height_v and
            this->lower_left == rect.lower_left);
}

Rectangle &Rectangle::operator+=(const Vector &vec) {
    this->lower_left += vec;

    return *this;
}

uint32_t Rectangle::width() const {
    return this->width_v;
}

uint32_t Rectangle::height() const {
    return this->height_v;
}

Position Rectangle::pos() const {
    return this->lower_left;
}

uint64_t Rectangle::area() const {
    return uint64_t(this->width_v) *
           uint64_t(this->height_v);
}

Rectangle Rectangle::reflection() const {
    auto reflected = Rectangle(this->height_v,
                               this->width_v,
                               this->lower_left.reflection());

    return reflected;
}

const Rectangle operator+(const Rectangle &rect, const Vector &vec) {
    Rectangle new_rect(rect);
    new_rect += vec;

    return new_rect;
}

const Rectangle operator+(const Vector &vec, const Rectangle &rect) {
    return rect + vec;
}
Rectangles::Rectangles() {}

Rectangles::Rectangles(std::initializer_list<Rectangle> input) : rects(input) {}

Rectangles::Rectangles(const Rectangles &input) {
    for (auto rect : input.rects)
        rects.push_back(rect);
}

Rectangle &Rectangles::operator[](size_t iter) {
    assert(iter < this->size());
    return rects[iter];
}

const Rectangle &Rectangles::operator[](size_t iter) const {
    assert(iter < this->size());
    return rects[iter];
}

bool Rectangles::operator==(const Rectangles &candidate) const {
    if (candidate.size() != this->size())
        return false;

    for (size_t i = 0; i < candidate.size(); i++) {
        if (!(candidate[i] == rects[i]))
            return false;
    }
    return true;
}

Rectangles &Rectangles::operator+=(const Vector &vec) {
    for (auto &rect : rects)
        rect += vec;

    return *this;
}

Rectangles &Rectangles::operator=(const Rectangles &rect) {
    if (this == &rect)  ///check for self-assignment
        return *this;

    for (size_t i = 0; i < std::min(this->size(), rect.size()); i++)
        (*this)[i] = rect[i];

    for (size_t i = this->size(); i < rect.size(); i++)
        this->rects.push_back(rect[i]);

    while (this->size() > rects.size())
        this->rects.pop_back();

    return *this;
}

size_t Rectangles::size() const {
    return rects.size();
}

const Rectangles operator+(const Rectangles &rects, const Vector &vec) {
    Rectangles new_rects(rects);
    new_rects += vec;

    return new_rects;
}

const Rectangles operator+(const Vector &vec, const Rectangles &rects) {
    return rects + vec;
}
namespace {
    bool possible_horizontal(const Rectangle &rect1, const Rectangle &rect2) {
        if (rect1.width() != rect2.width())
            return false;

        return rect2.pos() == (rect1.pos() + Vector(0, rect1.height()));
    }

    bool possible_vertical(const Rectangle &rect1, const Rectangle &rect2) {
        if (rect1.height() != rect2.height())
            return false;
        return rect2.pos() == (rect1.pos() + Vector(rect1.width(), 0));
    }
}  // namespace

Rectangle merge_horizontally(const Rectangle &rect1, const Rectangle &rect2) {
    assert(possible_horizontal(rect1, rect2));

    return Rectangle(rect1.width(), rect1.height() + rect2.height(), rect1.pos());
}

Rectangle merge_vertically(const Rectangle &rect1, const Rectangle &rect2) {
    assert(possible_vertical(rect1, rect2));

    return Rectangle(rect1.width() + rect2.width(), rect1.height(), rect1.pos());
}

Rectangle merge_all(const Rectangles &rects) {
    assert(rects.size() > 0);

    Rectangle result(rects[0]);

    for (size_t i = 1; i < rects.size(); i++) {
        bool ph = possible_horizontal(result, rects[i]);
        bool pv = possible_vertical(result, rects[i]);

        assert((ph == true or pv == true));

        if (ph)
            result = merge_horizontally(result, rects[i]);
        if (pv)
            result = merge_vertically(result, rects[i]);
    }

    return result;
}