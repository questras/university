/** @file
 * Rozwiązanie zadania maksima.
 *
 * @author Kamil Zwierzchowski 418510
 * @author Michał Wiśniewski 418486
 */

#ifndef MAKSIMA_FUNCTION_MAXIMA_H
#define MAKSIMA_FUNCTION_MAXIMA_H

#include <map>
#include <set>
#include <memory>
#include <exception>


class InvalidArg : public std::exception {
public:
    const char *what() const noexcept override {
        return "invalid argument value";
    }
} InvalidArgException;


template<typename A, typename V>
class FunctionMaxima {
public:
    using size_type = size_t;
    class point_type;

private:
    // Komparator do porównywania point_type z point_type i z typem A.
    struct cmpPoints {
        using is_transparent = std::true_type;

        bool operator()(const point_type &p1, const point_type &p2) const {
            return p1.arg() < p2.arg();
        }

        bool operator()(const point_type &p1, const A &arg) const {
            return p1.arg() < arg;
        }

        bool operator()(const A &arg, const point_type &p2) const {
            return arg < p2.arg();
        }
    };

    // Komparator do zapewnienia, aby Maxima były w kolejności malejących wartości.
    struct cmpMaxima {
        bool operator()(const point_type &p1, const point_type &p2) const {
            if (!(p2.value() < p1.value()) && !(p1.value() < p2.value())) {
                return p1.arg() < p2.arg();
            }

            return p2.value() < p1.value();
        }
    };

    // Aliasy na wskaźniki.
    using arg_ptr_t = std::shared_ptr<A>;
    using value_ptr_t = std::shared_ptr<V>;

    // Aliasy na struktury danych.
    using points_t = std::multiset<point_type, cmpPoints>;
    using maxima_t = std::multiset<point_type, cmpMaxima>;

    // Aliasy na iteratory struktur danych.
    using points_iter = typename points_t::iterator;
    using points_const_iter = typename points_t::const_iterator;
    using maxima_iter = typename maxima_t::iterator;
    using maxima_const_iter = typename maxima_t::const_iterator;

    points_t points;
    maxima_t maxima;

    // Sprawdza czy argumenty punktów wskazywanych przez podane iteratory
    // są równe.
    bool args_equal(points_iter p1, points_iter p2) {
        return !(p1->arg() < p2->arg()) && !(p2->arg() < p1->arg());
    }

    // Sprawdza czy wartości punktów wskazywanych przez podane iteratory
    // są równe.
    bool vals_equal(points_iter p1, points_iter p2) {
        return !(p1->value() < p2->value()) && !(p2->value() < p1->value());
    }

    // Aktualizuje maksima na podstawie punktów obok nowo-dodanego punktu
    // wskazywanego przez new_point_it i iteratora do usuniętego punktu w strukturze
    // z maksimami wskazywanego przez old_max_it.
    void handle_maxima(points_iter new_point_it, maxima_iter old_max_it) {
        bool was_next_old = false, was_prev_old = false;

        points_iter next = safe_next(new_point_it);
        maxima_iter old_next_maxima_it = maxima.end();
        if (next != points.end() && args_equal(next, new_point_it)) {
            // next to "stary", nadpisany punkt.
            was_next_old = true;
            next = safe_next(next);
        }

        points_iter prev = safe_prev(new_point_it);
        maxima_iter old_prev_maxima_it = maxima.end();
        if (prev != new_point_it && args_equal(prev, new_point_it)) {
            // prev to "stary", nadpisany punkt.
            was_prev_old = true;
            prev = safe_prev(prev);

            if (args_equal(prev, new_point_it)) {
                // Sytuacja gdy "stary" punkt był na lewym krańcu funkcji.
                prev = new_point_it;
            }
        }

        if (next != points.end()) {
            old_next_maxima_it = maxima.find(*next);
        }
        if (prev != new_point_it) {
            old_prev_maxima_it = maxima.find(*prev);
        }

        bool it_is_maximum, prev_is_maximum, next_is_maximum;
        it_is_maximum = is_maximum(new_point_it,
                                   (was_prev_old || was_next_old),
                                   was_prev_old);
        prev_is_maximum = prev != new_point_it &&
                          is_maximum(prev, was_prev_old, false);
        next_is_maximum = next != points.end() &&
                          is_maximum(next, was_next_old, true);

        maxima_iter new_pt_maxima_it = maxima.end();
        maxima_iter new_prev_maxima_it = maxima.end();
        if (it_is_maximum) {
            new_pt_maxima_it = maxima.insert(*new_point_it);
        }

        if (prev_is_maximum) {
            try {
                new_prev_maxima_it = maxima.insert(*prev);
            }
            catch (...) {
                if (new_pt_maxima_it != maxima.end()) {
                    maxima.erase(new_pt_maxima_it);
                }
                throw;
            }
        }

        if (next_is_maximum) {
            try {
                maxima.insert(*next);
            }
            catch (...) {
                if (new_prev_maxima_it != maxima.end()) {
                    maxima.erase(new_prev_maxima_it);
                }
                if (new_pt_maxima_it != maxima.end()) {
                    maxima.erase(new_pt_maxima_it);
                }
                throw;
            }
        }

        if (old_max_it != maxima.end()) {
            maxima.erase(old_max_it);
        }
        if (old_next_maxima_it != maxima.end()) {
            maxima.erase(old_next_maxima_it);
        }
        if (old_prev_maxima_it != maxima.end()) {
            maxima.erase(old_prev_maxima_it);
        }
    }

    // Sprawdza czy punkt wskazywany przez it jest maksimum funkcji. Jeśli
    // is_old_present=true to w zależności od is_old_prev pomija w ustalaniu
    // maksimum lewego sąsiada (is_old_prev=true) lub
    // prawego sąsiada(is_old_prev=false).
    bool is_maximum(points_iter it, bool is_old_present, bool is_old_prev) {
        points_iter next = safe_next(it);
        points_iter prev = safe_prev(it);
        if (is_old_present) {
            if (is_old_prev) {
                points_iter old_prev = prev;
                prev = safe_prev(prev);

                if (args_equal(prev, old_prev)) {
                    // Sytuacja gdy punkt, który ignorujemy, jest na lewym końcu.
                    prev = it;
                }
            }
            else {
                next = safe_next(next);
            }
        }

        if (next == points.end() && prev == it) {
            return true;
        }
        else if (next == points.end()) {
            return prev->value() < it->value() || vals_equal(prev, it);
        }
        else if (prev == it) {
            return next->value() < it->value() || vals_equal(next, it);
        }
        else {
            return (prev->value() < it->value() || vals_equal(prev, it)) &&
                   (next->value() < it->value() || vals_equal(next, it));
        }
    }

    // Zwraca iterator wskazujący na poprzedni punkt od it, lub it jeśli
    // it jest pierwszym punktem w funkcji.
    points_iter safe_prev(points_iter it) {
        if (it == points.begin()) {
            return it;
        }
        return std::prev(it);
    }

    // Zwraca iterator wskazujący na następny punkt od it, lub it jeśli
    // it jest ostatnim punktem w funkcji.
    points_iter safe_next(points_iter it) {
        if (it == points.end()) {
            return it;
        }
        return std::next(it);
    }

public:
    FunctionMaxima() = default;

    FunctionMaxima(const FunctionMaxima<A, V> &f)
            : points(f.points), maxima(f.maxima) {}

    class point_type {
    private:
        arg_ptr_t a;
        value_ptr_t v;
    public:
        point_type(arg_ptr_t arg, value_ptr_t val) : a(arg), v(val) {}

        // Zwraca argument funkcji.
        A const &arg() const {
            return *(a);
        }

        // Zwraca wartość funkcji w tym punkcie.
        V const &value() const {
            return *(v);
        }
    };

    FunctionMaxima<A, V> &operator=(const FunctionMaxima<A, V> &other) {
        points_t tmp_points = other.points;
        maxima_t tmp_maxima = other.maxima;
        swap(tmp_points, points);
        swap(maxima, tmp_maxima);

        return *this;
    }

    // Zwraca wartość w punkcie a, rzuca wyjątek InvalidArg, jeśli a nie
    // należy do dziedziny funkcji.
    V const &value_at(A const &a) const {
        points_const_iter it = points.find(a);
        if (it == points.end()) {
            throw InvalidArgException;
        }

        return it->value();
    }

    // Zmienia funkcję tak, żeby zachodziło f(a) = v. Jeśli a nie należy do
    // obecnej dziedziny funkcji, jest do niej dodawany.
    void set_value(A const &a, V const &v) {
        arg_ptr_t a_ptr = std::make_shared<A>(a);
        value_ptr_t v_ptr = std::make_shared<V>(v);

        points_iter old_point_it = points.find(a);
        maxima_iter old_max_it = maxima.end();
        if (old_point_it != points.end()) {
            old_max_it = maxima.find(*old_point_it);
        }

        points_iter new_point_it = points.insert(point_type(a_ptr, v_ptr));

        try {
            handle_maxima(new_point_it, old_max_it);
        }
        catch (...) {
            points.erase(new_point_it);
            throw;
        }

        if (old_point_it != points.end()) {
            points.erase(old_point_it);
        }
    }

    // Usuwa a z dziedziny funkcji. Jeśli a nie należało do dziedziny funkcji,
    // nie dzieje się nic.
    void erase(A const &a) {
        points_iter it = points.find(a);
        if (it != points.end()) {
            maxima_iter old_maxima_it = maxima.find(*it);
            points_iter next = safe_next(it);
            points_iter prev = safe_prev(it);
            maxima_iter prev_maxima_it = maxima.end();
            maxima_iter next_maxima_it = maxima.end();

            if (next != points.end()) {
                next_maxima_it = maxima.find(*next);
            }
            if (prev != it) {
                prev_maxima_it = maxima.find(*prev);
            }

            bool is_next_maximum = next != points.end() &&
                                   is_maximum(next, true, true);
            bool is_prev_maximum = prev != it &&
                                   is_maximum(prev, true, false);

            maxima_iter next_insert_it = maxima.end();
            if (is_next_maximum) {
                next_insert_it = maxima.insert(*next);
            }

            if (is_prev_maximum) {
                try {
                    maxima.insert(*prev);
                }
                catch (...) {
                    if (next_insert_it != maxima.end()) {
                        maxima.erase(next_insert_it);
                    }
                    throw;
                }
            }

            if (prev_maxima_it != maxima.end()) {
                maxima.erase(prev_maxima_it);
            }
            if (next_maxima_it != maxima.end()) {
                maxima.erase(next_maxima_it);
            }
            if (old_maxima_it != maxima.end()) {
                maxima.erase(old_maxima_it);
            }

            points.erase(it);
        }
    }

    // Zwraca rozmiar dziedziny funkcji.
    size_type size() const noexcept {
        return points.size();
    }

    using iterator = points_const_iter;

    // iterator wskazujący na pierwszy punkt.
    iterator begin() const {
        return points.begin();
    }

    // iterator wskazujący za ostatni punkt.
    iterator end() const {
        return points.end();
    }

    // Iterator, który wskazuje na punkt funkcji o argumencie a lub end(),
    // jeśli takiego argumentu nie ma w dziedzinie funkcji.
    iterator find(const A &a) const {
        return points.find(a);
    }

    using mx_iterator = maxima_const_iter;

    // iterator wskazujący na pierwsze lokalne maksimum.
    mx_iterator mx_begin() const {
        return maxima.begin();
    }

    // iterator wskazujący za ostatnie lokalne maksimum.
    mx_iterator mx_end() const {
        return maxima.end();
    }
};

#endif //MAKSIMA_FUNCTION_MAXIMA_H
