/** @file
 * Definition of geometry module.
 *
 * @author
 * @author Michał Wiśniewski 418486
 * @author Igor Witkowski 421607
 */

#ifndef GEOMETRY_GEOMETRY_H
#define GEOMETRY_GEOMETRY_H

#include <cstdint>
#include <vector>

class Vector;

/**
 * Class representing a Position on a 2D plane with x and y values
 * as coordinates.
 */
class Position {
private:
    int32_t x_value;  /// X coordinate of position.
    int32_t y_value;  /// Y coordinate of position.
public:
<<<<<<< HEAD
    Position() = delete;
    Position(int32_t x, int32_t y);     /// Default constructor.
    explicit Position(const Vector &v); /// Constructor from Vector object.
=======
    Position(int32_t x, int32_t y);      /// Default constructor.
    explicit Position(const Vector &v);  /// Constructor from Vector object.
>>>>>>> mergedv2

    int32_t x() const;  /// Getter to x_value.
    int32_t y() const;  /// Getter to y_value.

    /**
     * Get a new position representing a reflection of the current position
     * across the y=x line i.e position with swapped x and y coordinates.
     *
     * Example:
     * Position(1,2).reflection() == Position(2,1) -> true
     *
     * @return position representing a reflection of the current position
     * across the y=x line.
     */
    Position reflection() const;

    /**
     * Get a static, constant object representing the origin of
     * coordinate system i.e position with x and y coordinates set to 0.
     *
     * @return position representing origin of coordinate system.
     */
    static const Position &origin();

    /**
     * Operator to compare two positions i.e if their x and y coordinates
     * are equal.
     *
     * Example:
     * Position(1,2) == Position(1,2) -> true
     * Position(1,2) == Position(2,1) -> false
     *
     * @param other - position to compare.
     * @return true if two positions are equal, false otherwise.
     */
    bool operator==(const Position &other) const;

    /**
     * Operator to move current position by given vector i.e change
     * current position's x and y coordinates by x and y values of
     * given vector.
     *
     * Example:
     * p = Position(1,2)
     * p += Vector(2, 3)
     * p == Position(3, 5) -> true
     *
     * @param v - vector by which to move the current position.
     * @return reference to the moved position.
     */
    Position &operator+=(const Vector &v);
};

/**
 * Class representing a Vector on a 2D plane with x and y coordinate.
 */
class Vector {
private:
    Position pos;  /// Position holding information about vector's coordinates.
public:
<<<<<<< HEAD
    Vector() = delete;
    Vector(int32_t x, int32_t y);       /// Default constructor.
    explicit Vector(const Position &p); /// Constructor from Position object.
=======
    Vector(int32_t x, int32_t y);        /// Default constructor.
    explicit Vector(const Position &p);  /// Constructor from Position object.
>>>>>>> mergedv2

    int32_t x() const;  /// Getter to x coordinate of the vector.
    int32_t y() const;  /// Getter to y coordinate of the vector.

    /**
     * Get a new vector representing a reflection of the current vector
     * across the y=x line i.e vector with swapped x and y coordinates.
     *
     * Example:
     * Vector(1,2).reflection() == Vector(2,1) -> true
     *
     * @return vector representing a reflection of the current vector
     * across the y=x line.
     */
    Vector reflection() const;

    /**
     * Operator to compare two vectors i.e if their x and y coordinates
     * are equal.
     *
     * Example:
     * Vector(1,2) == Vector(1,2) -> true
     * Vector(1,2) == Vector(2,1) -> false
     *
     * @param other - vector to compare.
     * @return true if two vectors are equal, false otherwise.
     */
    bool operator==(const Vector &other) const;

    /**
     * Operator to change the coordinates of current vector by coordinates
     * of given vector i.e change current vector's x and y coordinates
     * by x and y coordinates of the given vector.
     *
     * Example:
     * v = Vector(1,2)
     * v += Vector(2, 3)
     * v == Vector(3, 5) -> true
     *
     * @param v - vector by which to change the current vector.
     * @return reference to the changed vector.
     */
    Vector &operator+=(const Vector &other);
};

/**
 * Class representing a rectangle on a 2D plane with sides
 * parallel to the axes and whole coordinates of vertices.
 *
 */
class Rectangle {
private:
    Position lower_left;
    uint32_t width_v;
    uint32_t height_v;

public:
    Rectangle() = delete;  ///disabling the default constructor
    Rectangle(int64_t w, int64_t h, Position pos);
    Rectangle(int64_t w, int64_t h);   //constructor with lower left vertex in the origin
    Rectangle(const Rectangle &rect);  //copy constructor
    Rectangle(Rectangle &&input) = default;

    /**
     * @brief Operator to assign given rectangle's value to the current rectangle.
     *
     * @param rect - rectangle to assign.
     * @return Rectangle& - reference to changed rectangel.
     */
    Rectangle &operator=(const Rectangle &rect);

    /**
     * @brief Operator to determine if a given rectangle is equal to current rectangle
     * (both dimensions and position on the plane).
     *
     * @param rect - Rectangle to compare to current rectangle.
     * @return true - returned if the rectangles are the same,
     * @return false - returned if the rectangles are different.
     */
    bool operator==(const Rectangle &rect) const;

    /**
     * @brief Operator to move the current rectangle by a given vector.
     *
     * @param vec - vector by which to move.
     * @return Rectangle& reference to current rectangle after moving.
     */
    Rectangle &operator+=(const Vector &vec);

    uint32_t width() const;
    uint32_t height() const;
    Position pos() const;
    uint64_t area() const;
    Rectangle reflection() const;
};

class Rectangles {
private:
    std::vector<Rectangle> rects;

public:
    Rectangles();  ///Constructor of an empty list.
    Rectangles(std::initializer_list<Rectangle> input);
    Rectangles(const Rectangles &input);  ///Copy constructor.
    Rectangles(Rectangles &&input) = default;

    /**
     * @brief Operator to access a rectangle under given index in
     * the current list.
     *
     * @param iter - index of the desired rectangle.
     * @return Rectangle& - reference to the rectangle.
     */
    Rectangle &operator[](size_t iter);

    /**
     * @brief Operator[] meant for read-only lists.
     *
     * @param iter - index of the desired rectangle
     * @return const Rectangle& - reference to the rectangle.
     */
    const Rectangle &operator[](size_t iter) const;

    /**
     * @brief Operator to assign a given list to the current list.
     *
     * @param rect - reference to the list to assign.
     * @return Rectangles& - reference to current list after assignment/
     */
    Rectangles &operator=(const Rectangles &rect);

    /**
     * @brief Operator to determine if given list is equal to the current list.
     *
     * @param candidate - reference to a list to compare to the current list.
     * @return true - returned if the lists are equal,
     * @return false - returned if the lists are different.
     */
    bool operator==(const Rectangles &candidate) const;

    /**
     * @brief Operator to move all rectangles in the current list by a given vector.
     *
     * @param vec - vector by which to move
     * @return Rectangles& - reference to current list after moving.
     */
    Rectangles &operator+=(const Vector &vec);

    size_t size() const;
};

/**
 * Operator to return a sum of a position and a vector i.e
 * the given position moved by the given vector.
 *
 * Example:
 * p = Position(1,2) + Vector(2,3)
 * p == Position(3,5) -> true
 *
 * @param p - position to move,
 * @param v - vector by which to move the given position.
 * @return a position representing the sum of the given position and vector.
 */
const Position operator+(const Position &p, const Vector &v);

/**
 * A sum of vector and position which is analogous sum of position and vector.
 */
const Position operator+(const Vector &v, const Position &p);

/**
 * Operator to return a sum of two vectors i.e a vector with
 * x and y coordinates as a sum of given vectors' x and y coordinates.
 *
 * Example:
 * v = Vector(1,2) + Vector(2,3)
 * v == Vector(3,5) -> true
 *
 * @param v1 - vector to sum,
 * @param v2 - vector to sum.
 * @return a vector representing the sum of two vectors.
 */
const Vector operator+(const Vector &v1, const Vector &v2);

/**
 * @brief Operator to move a rectangle by a set vector.
 *
 * @param rect - rectangle to move,
 * @param vec - vector by which to move.
 * @return const Rectangle - moved rectangle.
 */
const Rectangle operator+(const Rectangle &rect, const Vector &vec);

/**
 * @brief Operator to move a rectangle by a set vector.
 *
 * @param vec - vector by which to move,
 * @param rect - rectangle to move.
 * @return const Rectangle - moved rectangle.
 */
const Rectangle operator+(const Vector &vec, const Rectangle &rect);

/**
 * @brief Operator to move all rectangles in a given Rectangles object by
 * a set vector.
 *
 * @param rects object containing rectangles to move,
 * @param vec - vector by which to move.
 * @return const Rectangles object containing moved rectangles.
 */
const Rectangles operator+(const Rectangles &rects, const Vector &vec);

/**
 * @brief Operator to move all rectangles in a given Rectangles object by
 * a set vector.
 *
 * @param vec - vector by which to move,
 * @param rects object containing rectangles to move.
 * @return const Rectangles object containing moved rectangles.
 */
const Rectangles operator+(const Vector &vec, const Rectangles &rects);

/**
 * @brief Function that, if possible, merges rectangle \p rect1 's right edge
 * with \p rect2 's left edge, creating a new rectangle. If the rectangles are
 * incompatible, an assertion fails.
 *
 * @param rect1 - left rectangle,
 * @param rect2 - right rectangle.
 * @return Rectangle - merged rectangle.
 */
Rectangle merge_horizontally(const Rectangle &rect1, const Rectangle &rect2);

/**
 * @brief Function that, if possible, merges rectangle \p rect1 's upper edge
 * with \p rect2 's lower edge, creating a new rectangle. If the rectangles are
 * incompatible, an assertion fails.
 *
 * @param rect1 - lower rectangle,
 * @param rect2 - upper rectangle.
 * @return Rectangle - merged rectangle.
 */
Rectangle merge_vertically(const Rectangle &rect1, const Rectangle &rect2);

/**
 * @brief Function that, if possible, merges all rectangles in \p rects in order,
 * either vertically or horizontaly, depending on which is possible.
 * If any of the merges fail, an assertion fails.
 *
 * @param rect1 - lower rectangle,
 * @param rect2 - upper rectangle.
 * @return Rectangle - merged rectangle.
 */
Rectangle merge_all(const Rectangles &rects);
#endif  //GEOMETRY_GEOMETRY_H