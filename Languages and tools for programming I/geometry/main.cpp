#include "geometry.h"
#include <cassert>
#include <iostream>

// Opis testów
// grupa 1 tworzenie obiektów
// grupa 2 usuniete Position(), Vector() i  Rectangle(), const na origin
// grupa 3 area, +=, +, reflection, move dla Rectangles
// grupa 4 merge_horizontally, merge_vertically
// grupa 5 merge_all
// grupa 6 niekompilujące się operatory

template <typename PosOrVec>
void test_pos_or_vec() {
    PosOrVec p(2, 20);
    PosOrVec const p2(3, -30);
    assert(p.x() == 2 && p.y() == 20);
    assert(p2.x() == 3 && p2.y() == -30);
    p = p2;
    assert(p2 == p);
    assert(p.x() == 3 && p.y() == -30);
    PosOrVec const p3(p);
    assert(p3.x() == 3 && p3.y() == -30);
    assert(p3 == p);
    PosOrVec const p4(std::move(p));
    assert(p4.x() == 3 && p4.y() == -30);
    PosOrVec const p5 = std::move(p4);
    assert(p5.x() == 3 && p5.y() == -30);
}


int main() {
#if TEST_NUM == 101
    // tworzenie obiektów Position
    test_pos_or_vec<Position>();
#endif

#if TEST_NUM == 102
    // tworzenie obiektów Vector
    test_pos_or_vec<Vector>();
#endif

#if TEST_NUM == 103
    // testy prostokątów
    Rectangle const r2(1, 2, Position(3, 4));
    assert(r2.width() == 1);
    assert(r2.height() == 2);
    assert(r2.pos().x() == 3);
    assert(r2.pos().y() == 4);

    Rectangle p(2, 20);
    Rectangle const p2(3, 30);
    assert(p.width() == 2 && p.height() == 20);
    assert(p2.width() == 3 && p2.height() == 30);
    p = p2;
    assert(p.width() == 3 && p.height() == 30);
    Rectangle const p3(p);
    assert(p3.width() == 3 && p3.height() == 30);
    assert(p3 == p);
    Rectangle const p4(std::move(p));
    assert(p4.width() == 3 && p4.height() == 30);
    Rectangle const p5 = std::move(p4);
    assert(p5.width() == 3 && p5.height() == 30);
#endif
    // nie przechodzi
#if TEST_NUM == 104
    // testy Rectangles
    Rectangles rect1({Rectangle(1, 2), Rectangle(3, 4)});
    assert(rect1.size() == static_cast<size_t>(2));
    assert(rect1[0] == Rectangle(1, 2));
    assert(rect1[1] == Rectangle(3, 4));
    Rectangles rect2(rect1);
    assert(rect1 == rect2);
    rect2[0] += Vector(0, 0);

    Rectangles rect3(std::move(rect1));
    std::cout<<rect2.size()<<" "<<rect3.size()<<" "<<rect1.size()<<"\n";
    assert(rect3 == rect2);
    assert(!(rect3 == rect1));

    Rectangles const rect4 = rect2;
//    assert(rect4 == rect2);
    assert(rect2[0] == Rectangle(1, 2));
    assert(rect2[1] == Rectangle(3, 4));
    assert(rect4[0] == Rectangle(1, 2));
    assert(rect4[1] == Rectangle(3, 4));
    assert(rect4.size() == rect2.size());

    Rectangles rect5 = std::move(rect2);
    std::cout<<rect2.size()<<" "<<rect4.size()<<" "<<rect5.size()<<"\n";
    assert(rect5 == rect4);
    assert(!(rect5 == rect2));
#endif


    // W następnych 6 testach są niedodatnie wymiary.
#if TEST_NUM == 105
    Rectangle p(0, 1);
#endif

#if TEST_NUM == 106
    Rectangle p(1, 0);
#endif

#if TEST_NUM == 107
    Rectangle p(0, 1, Position(1, 1));
#endif

#if TEST_NUM == 108
    Rectangle p(1, 0, Position(1, 1));
#endif

#if TEST_NUM == 109
    Rectangle p(-1, 1, Position(1, 1));
#endif

#if TEST_NUM == 110
    Rectangle p(1, -1, Position(1, 1));
#endif




#if TEST_NUM == 201
    // poprawne operacje
    Position p1(1, 2);
    Vector v1(p1);
    Vector v2(2, 3);
    Position p2(v2);
    p2 = Position(v1);
    v1 = Vector(p2);
#endif


#if TEST_NUM == 202
    assert(Position::origin() == Position(0, 0));
#endif

    // Kolejne 7 testów powinno się zakończyć błędem wykonania.
#if TEST_NUM == 203
    Position p1(1, 2);
    Vector v1(p1);
    Vector v2(2, 3);
    Position p2(v2);
    p2 = Position(v1);
    v1 = Vector(p2);
    // niepoprawna operacja: niejawne rzutowanie
    p2 = v1;
#endif

#if TEST_NUM == 204
    Position p1(1, 2);
    Vector v1(p1);
    Vector v2(2, 3);
    Position p2(v2);
    p2 = Position(v1);
    v1 = Vector(p2);
    // niepoprawna operacja: niejawne rzutowanie
    v1 = p2;
#endif


#if TEST_NUM == 205
    [[maybe_unused]] Position p;
#endif

#if TEST_NUM == 206
    [[maybe_unused]] Vector v;
#endif


#if TEST_NUM == 207
    [[maybe_unused]] Rectangle rec;
#endif

#if TEST_NUM == 208
    [[maybe_unused]] Rectangle rec(3);
#endif

#if TEST_NUM == 209
    Position::origin() += Vector(1, 2);
#endif

#if TEST_NUM == 301
    Position p(1, 2);
    assert(p.reflection() == Position(2, 1));

    Position pos1(13, 14);
    Rectangle r(10, 2, pos1);
    assert(r.area() == 20);

    Vector v(1, 2);
    assert(v.reflection() == Vector(2, 1));
    Position pos2(14, 16);
    assert(v + pos1 == pos2);
    assert(pos1 + v == pos2);
    Rectangle r2(10, 2, pos2);
    assert(r + v == r2);
    assert(v + r == r2);
    Rectangles recs({r});
    Rectangles recsd;
    Rectangles recs2({r2});
    assert(recs + v == recs2);
    assert(v + recs == recs2);
    assert(v + v == Vector(2, 4));

    pos1 += v;
    assert(pos1 == pos2);

    r += v;
    assert(r == r2);

    recs += v;
    assert(recs == recs2);

    (recs += v) += v;
#endif
#if TEST_NUM == 302
    Position const p(1, 2);
    assert(p.reflection() == Position(2, 1));

    Position pos1(13, 14);
    Rectangle r(10, 2, pos1);
    assert(r.area() == 20);

    Vector v(1, 2);
    Position pos2(14, 16);
    assert(Vector(v) + Position(pos1) == pos2);
    assert(Position(pos1) + Vector(v) == pos2);
    Rectangle r2(10, 2, Position(pos2));
    assert(Rectangle(r) + Vector(v) == r2);
    assert(Vector(v) + Rectangle(r) == r2);
    Rectangles recs({r});
    Rectangles recs2({r2});
    assert(Rectangles(recs) + Vector(v) == recs2);
    assert(Vector(v) + Rectangles(recs) == recs2);

    Position(pos1) += Vector(v);
    Rectangle(r) += Vector(v);
    Rectangles(recs) += Vector(v);
    (Rectangles(recs) += Vector(v)) += Vector(v);
#endif
    // nie przechodzi
#define TEST_NUM 303

#if TEST_NUM == 303
    Position p(13, 14);
    Rectangle r(10, 2, p);
    Rectangles recs1({r, r});
    auto adr = &(recs1[0]);
    auto recs2 = std::move(recs1) + Vector(1, 1);
    assert(&(recs2[0]) == adr);
    auto recs3 = Vector(1, 1) + std::move(recs2);
    assert(&(recs3[0]) == adr);
#endif
#if TEST_NUM == 401
    Position pos1(12, 14);
    Position pos2(16, 14);
    Rectangle r2(4, 20, pos1);
    Rectangle r3(7, 20, pos2);
    merge_vertically(r2, r3);
#endif
#if TEST_NUM == 402
    Position pos1(12, 14);
    Position pos2(16, 14);
    Rectangle r2(4, 20, pos1);
    Rectangle r3(7, 20, pos2);
    merge_horizontally(r2.reflection(), r3.reflection());
#endif
    // Kolejne 10 testów powinno się zakończyć błędem wykonania.
#if TEST_NUM == 403
    Position pos1(12, 14);
    Position pos2(16, 14);
    Rectangle r2(3, 20, pos1);
    Rectangle r3(7, 20, pos2);
    merge_vertically(r2, r3);
#endif
#if TEST_NUM == 404
    Position pos1(13, 13);
    Position pos2(16, 14);
    Rectangle r2(3, 20, pos1);
    Rectangle r3(7, 20, pos2);
    merge_vertically(r2, r3);
#endif

#if TEST_NUM == 405
    Position pos1(13, 14);
    Position pos2(16, 13);
    Rectangle r2(3, 20, pos1);
    Rectangle r3(7, 20, pos2);
    merge_vertically(r2, r3);
#endif

#if TEST_NUM == 406
    Position pos1(13, 14);
    Position pos2(15, 14);
    Rectangle r2(3, 20, pos1);
    Rectangle r3(7, 20, pos2);
    merge_vertically(r2, r3);
#endif

#if TEST_NUM == 407
    Position pos1(13, 14);
    Position pos2(16, 14);
    Rectangle r2(3, 21, pos1);
    Rectangle r3(7, 20, pos2);
    merge_vertically(r2, r3);
#endif

#if TEST_NUM == 408
    Position pos1(14, 12);
    Position pos2(14, 16);
    Rectangle r2(20, 3, pos1);
    Rectangle r3(20, 7, pos2);
    merge_horizontally(r2, r3);
#endif

#if TEST_NUM == 409
    Position pos1(13, 13);
    Position pos2(14, 16);
    Rectangle r2(20, 3, pos1);
    Rectangle r3(20, 7, pos2);
    merge_vertically(r2, r3);
#endif

#if TEST_NUM == 410
    Position pos1(14, 13);
    Position pos2(13, 16);
    Rectangle r2(20, 3, pos1);
    Rectangle r3(20, 7, pos2);
    merge_vertically(r2, r3);
#endif

#if TEST_NUM == 411
    Position pos1(14, 13);
    Position pos2(14, 15);
    Rectangle r2(20, 3, pos1);
    Rectangle r3(20, 7, pos2);
    merge_vertically(r2, r3);
#endif

#if TEST_NUM == 412
    Position pos1(14, 13);
    Position pos2(14, 16);
    Rectangle r2(21, 3, pos1);
    Rectangle r3(20, 7, pos2);
    merge_vertically(r2, r3);
#endif
#if TEST_NUM == 501
    // jeden element
    Rectangle r{1,2};
    assert(merge_all(Rectangles{r}) == r);
    // Dwa elementy będziemy testować w 504.
#endif
#if TEST_NUM == 502
    // ponad dwa elementy
    bool result = merge_all(Rectangles({Rectangle{3, 20},
                                        Rectangle{7, 20, Position{3, 0}},
                                        Rectangle{10, 20, Position{0, 20}}})) == Rectangle{10, 40};
    assert(result);
#endif
#if TEST_NUM == 503
    // kwadrat
    bool result = merge_all(Rectangles({Rectangle{2, 2},
                                        Rectangle{2, 2, Position{2, 0}}})) == Rectangle{4, 2};
    assert(result);
#endif
    // Kolejne 5 testów powinno zakończyć się błędem wykonania.
#if TEST_NUM == 504
    // kontakt rogiem
    merge_all(Rectangles({
      Rectangle{2, 2},
      Rectangle{2, 2, Position{2, 2}}
    }));
#endif

#if TEST_NUM == 505
    // zero elementów
    Rectangles r;
    merge_all(r);
#endif

#if TEST_NUM == 506
    // zły indeks
    Rectangles r;
    [[maybe_unused]] auto rr = r[0];
#endif

#if TEST_NUM == 507
    // zły indeks
    Rectangles r{ Rectangle{2, 2} };
    [[maybe_unused]] auto rr = r[1];
#endif

#if TEST_NUM == 508
    // zły indeks
    Rectangles r{ Rectangle{2, 2} };
    [[maybe_unused]] auto rr = r[-1];
#endif
#if TEST_NUM == 610
    Position(0, 0) == Vector(0, 0);
#endif
#if TEST_NUM == 611
    Vector(0, 0) == Position(0, 0);
#endif

    return 0;
}

// This macro helps checking that particular expression does not compile.
// Exactly one from EXPR1 and EXPR2 should compile.
#define CHECK_COMP_ERROR(UNIQUE_HELPER_FUN_NAME, EXPR1, EXPR2) \
template <typename Rectangle, typename Rectangles, typename Position, typename Vector> \
auto UNIQUE_HELPER_FUN_NAME([[maybe_unused]] Rectangle & rec, \
                            [[maybe_unused]] Rectangles & recs, \
                            [[maybe_unused]] Position & pos, \
                            [[maybe_unused]] Vector & vec) -> decltype(EXPR1) { \
    return EXPR1; \
} \
\
template <typename Rectangle, typename Rectangles, typename Position, typename Vector> \
auto UNIQUE_HELPER_FUN_NAME([[maybe_unused]] Rectangle & rec, \
                            [[maybe_unused]] Rectangles & recs, \
                            [[maybe_unused]] Position & pos, \
                            [[maybe_unused]] Vector & vec) -> decltype(EXPR2) { \
    return EXPR2; \
} \
\
void UNIQUE_HELPER_FUN_NAME##_test() { \
    Rectangles recs; \
    Rectangle rec(1, 2); \
    Position pos(2, 3); \
    Vector vec(2, 3); \
    UNIQUE_HELPER_FUN_NAME(rec, recs, pos, vec); \
}
#if TEST_NUM == 601
CHECK_COMP_ERROR(F1a, recs + pos, recs + vec)
#endif
#if TEST_NUM == 602
CHECK_COMP_ERROR(F1b, pos + recs, vec + recs)
#endif
#if TEST_NUM == 603
CHECK_COMP_ERROR(F1c, recs += pos, recs += vec)
#endif
#if TEST_NUM == 604
CHECK_COMP_ERROR(F2a, rec + pos, rec + vec)
#endif
#if TEST_NUM == 605
CHECK_COMP_ERROR(F2b, pos + rec, vec + rec)
#endif
#if TEST_NUM == 606
CHECK_COMP_ERROR(F2c, rec += pos, rec += vec)
#endif
#if TEST_NUM == 607
CHECK_COMP_ERROR(F3a, pos + pos, pos + vec)
#endif
#if TEST_NUM == 608
CHECK_COMP_ERROR(F3b, pos + pos, vec + pos)
#endif
#if TEST_NUM == 609
CHECK_COMP_ERROR(F3c, pos += pos, pos += vec)
#endif


//#include "geometry.h"
//#include <type_traits>
//#include <utility>
//#include <functional>
//#include <limits>
//#include <iostream>
//
//#ifdef NDEBUG
//#undef NDEBUG
//#endif // NDEBUG
//
//#include <cassert>
//using namespace std;
//
//int main() {
//    std::cout << "Starting testing procedure." << std::endl;
//
//    // ------------- TEMPLATE TYPE TRAITS CHECKS -------------
//
//    // Default konstruktory
//    assert(!std::is_default_constructible_v<Position>);
//    assert(!std::is_default_constructible_v<Vector>);
//    assert(!std::is_default_constructible_v<Rectangle>);
//    assert(std::is_default_constructible_v<Rectangles>);
//
//    // Copy construction
//    assert(std::is_copy_constructible_v<Position>);
//    assert(std::is_copy_constructible_v<Vector>);
//    assert(std::is_copy_constructible_v<Rectangle>);
//    assert(std::is_copy_constructible_v<Rectangles>);
//
//    // Copy assignment
//    assert(std::is_copy_assignable_v<Position>);
//    assert(std::is_copy_assignable_v<Vector>);
//    assert(std::is_copy_assignable_v<Rectangle>);
//    assert(std::is_copy_assignable_v<Rectangles>);
//
//    // Move construction
//    assert(std::is_move_constructible_v<Rectangles>);
//
//    // Move assignment
//    assert(std::is_move_assignable_v<Rectangles>);
//
//    // Destruction
//    assert(std::is_destructible_v<Position>);
//    assert(std::is_destructible_v<Vector>);
//    assert(std::is_destructible_v<Rectangle>);
//    assert(std::is_destructible_v<Rectangles>);
//
//    // Dzialanie konstuktora Vector(scalar, scalar).
//    Vector vec3{-300, -400};
//    Vector vec4{0, 999};
//
//    // Dzialanie x() i y()
//    Vector vec5(-333, 444);
//    assert(vec5.x() == -333);
//    assert(vec5.y() == 444);
//
//    const Vector vec6(-519, 0);
//    assert(vec6.x() == -519);
//    assert(vec6.y() == 0);
//
//    // Dzialanie reflection
//    Vector vec7(813, -129);
//    assert(vec7.x() == vec7.reflection().reflection().x());
//    assert(vec7.y() == vec7.reflection().reflection().y());
//
//    assert(vec7.reflection().x() == vec7.reflection().x());
//    assert(vec7.reflection().y() == vec7.reflection().y());
//
//    const Vector vec8(1, 3);
//    assert(vec8.x() == vec8.reflection().reflection().x());
//    assert(vec8.y() == vec8.reflection().reflection().y());
//
//    assert(vec8.reflection().x() == vec8.reflection().x());
//    assert(vec8.reflection().y() == vec8.reflection().y());
//
//    // Dzialanie ==
//    Vector vec11(3, 4);
//    Vector vec12(3, 4);
//
//    assert(vec11 == vec12);
//
//    const Vector vec13(3, 4);
//    const Vector vec14(3, 4);
//
//    assert(vec13 == vec14);
//
//    assert(vec11 == vec13);
//    assert(vec13 == vec11);
//
//    Vector vec15(3, 7);
//    Vector vec16(7, 4);
//
//    assert(!(vec11 == vec15));
//    assert(!(vec11 == vec16));
//
//    const Vector vec17{-4, -3};
//
//    // Istnienie Vector::Vector(const Vector &)
//    Vector vec18(vec17);
//
//    // Istnienie Vector::operator=(const Vector &)
//    Vector vec19 = vec18; // Side note: ta linijka to nie operator= tylko wywolanie konstruktora.
//    vec19 = vec17;
//
//    Vector vec20{5, 6};
//    Vector vec21(6, 9);
//
//    // Czy nie dano przypadkiem Vector::Vector(Vector &&) = delete.
//    Vector vec22 = std::move(vec20);
//
//    Vector vec23{-1, -7};
//
//    // Czy nie dano przypadkiem Vector::operator=(Vector &&) = delete.
//    vec23 = std::move(vec21);
//
//    // Istnienie Vector::~Vector().
//    vec23.~Vector();
//
//// POSITION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//    // Dzialanie konstuktora Position(scalar, scalar).
//    Position pos3{-300, -400};
//    Position pos4{0, 999};
//
//    // Dzialanie x() i y()
//    Position pos5(-333, 444);
//    assert(pos5.x() == -333);
//    assert(pos5.y() == 444);
//
//    const Position pos6(-519, 0);
//    assert(pos6.x() == -519);
//    assert(pos6.y() == 0);
//
//    // Dzialanie reflection
//    Position pos7(813, -129);
//    assert(pos7.x() == pos7.reflection().reflection().x());
//    assert(pos7.y() == pos7.reflection().reflection().y());
//
//    assert(pos7.reflection().x() == pos7.reflection().x());
//    assert(pos7.reflection().y() == pos7.reflection().y());
//
//    const Position pos8(1, 3);
//    assert(pos8.x() == pos8.reflection().reflection().x());
//    assert(pos8.y() == pos8.reflection().reflection().y());
//
//    assert(pos8.reflection().x() == pos8.reflection().x());
//    assert(pos8.reflection().y() == pos8.reflection().y());
//
//    // Dzialanie Position::origin
//    Position pos9 = Position::origin();
//    const Position pos10 = Position::origin();
//
//    assert(Position::origin().x() == 0);
//    assert(Position::origin().y() == 0);
//
//    // Dzialanie ==
//    Position pos11(3, 4);
//    Position pos12(3, 4);
//
//    assert(pos11 == pos12);
//
//    const Position pos13(3, 4);
//    const Position pos14(3, 4);
//
//    assert(pos13 == pos14);
//
//    assert(pos11 == pos13);
//    assert(pos13 == pos11);
//
//    Position pos15(3, 7);
//    Position pos16(7, 4);
//
//    assert(!(pos11 == pos15));
//    assert(!(pos11 == pos16));
//
//    assert((Position::origin() == Position(0, 0)));
//
//    const Position pos17{-4, -3};
//
//    // Istnienie Position::Position(const Position &)
//    Position pos18(pos17);
//
//    // Istnienie Position::operator=(const Position &)
//    Position pos19 = pos18; // Side note: ta linijka to nie operator= tylko wywolanie konstruktora.
//    pos19 = pos17;
//
//    Position pos20{5, 6};
//    Position pos21(6, 9);
//
//    // Czy nie dano przypadkiem Position::Position(Position &&) = delete.
//    Position pos22 = std::move(pos20);
//
//    Position pos23{-1, -7};
//
//    // Czy nie dano przypadkiem Position::operator=(Position &&) = delete.
//    pos23 = std::move(pos21);
//
//    // Istnienie Position::~Position().
//    pos23.~Position();
//
//    // Przesuwanie
//
//    Position pos24{100, 300};
//    const Vector vec24(-1, -1);
//
//    pos24 += vec24;
//
//    assert(pos24 == Position(99, 299));
//
//    Vector vec25{15, 37};
//
//    vec25 += vec24;
//
//    assert(vec25 == Vector(14, 36));
//
//    // Przypisywanie
//
//    // Test z tresci
//    Position pos26(1, 2);
//    Vector vec26(pos26);
//    Vector vec27(2, 3);
//    Position pos27(vec27);
//    pos27 = Position(vec26);
//    vec26 = Vector(pos27);
//
//    // Rozne
//
//    Position pos29 = Position(-5, 6);
//    Vector vec29 = Vector(-3, 4);
//
//    // Sprawdza czy nie dano czasem consta do typu zwracanego.
//    pos29.reflection() = {3, 5};
//    vec29.reflection() = {7, 3};
//
//    assert((pos29 == Position{-5, 6}));
//    assert((vec29 == Vector{-3, 4}));
//
//    Vector vec30{51, 63};
//    Vector vec31{-3, -4};
//
//    vec30 += Vector(0, 0);
//    assert(vec30 == vec30);
//
//    auto minScalar = std::numeric_limits<int32_t>::min();
//    auto maxScalar = std::numeric_limits<int32_t>::max();
//
//    Position pos32{minScalar, maxScalar};
//    Vector vec32{minScalar, maxScalar};
//
//    assert(pos32.x() == minScalar);
//    assert(pos32.y() == maxScalar);
//
//    assert(vec32.x() == minScalar);
//    assert(vec32.y() == maxScalar);
//
//    Vector vec33{-215, 341};
//    assert(vec33.reflection() == Vector(341, -215));
//
//    Position pos33{-215, 341};
//    assert(pos33.reflection() == Position(341, -215));
//
//// ------------------- RECTANGLE -------------------
//
//    // UWAGA: Zakladam reprezentacje
//    // scalar width, scalar height, Position pos
//    // Moim zdaniem, tresc wymaga takiej reprezentacji, ze wzgledu na
//    // gettery width(), height(), pos() oraz zwyczajne reusability.
//
//    Rectangle rec7{300, 400};
//    const Rectangle rec8{500, 600, {-2561, -39143}};
//
//    assert(rec7.width() == 300);
//    assert(rec7.height() == 400);
//    assert(rec7.pos() == Position::origin());
//
//    assert(rec8.width() == 500);
//    assert(rec8.height() == 600);
//    assert(rec8.pos() == Position(-2561, -39143));
//
//    assert(rec7.area() == 300 * 400);
//    assert(rec8.area() == 500 * 600);
//
//    assert(rec7.reflection() == Rectangle(400, 300, {0, 0}));
//    assert(rec8.reflection() == Rectangle(600, 500, {-39143, -2561}));
//
//    rec8.reflection() = Rectangle{1, 2};
//
//    const Rectangle rec9{300, 400};
//    Rectangle rec10{500, 600, {-2561, -39143}};
//
//    assert(rec7 == rec7);
//    assert(rec8 == rec8);
//
//    assert(rec7 == rec9);
//    assert(rec8 == rec10);
//
//    assert(!(rec7 == rec10));
//    assert(!(rec8 == rec9));
//
//    assert(!(Rectangle(3, 7, {2, 1}) == Rectangle(3, 7)));
//    assert(!(Rectangle(3, 7, {2, 1}) == Rectangle(3, 7, {2, 0})));
//    assert(!(Rectangle(3, 7, {2, 1}) == Rectangle(3, 7, {0, 1})));
//    assert(!(Rectangle(3, 7, {2, 1}) == Rectangle(7, 7, {2, 1})));
//    assert(!(Rectangle(3, 7, {2, 1}) == Rectangle(3, 3, {2, 1})));
//
//    const Rectangle rec11(3, 7, {3, 3});
//    assert(rec11.reflection() == Rectangle(7, 3, {3, 3}));
//
//    assert(Rectangle(45, 37, {1, 2}).reflection() == Rectangle(37, 45, {2, 1}));
//    assert(Rectangle(45, 37, {-1, -2}).reflection() == Rectangle(37, 45, {-2, -1}));
//    assert(Rectangle(4519, 37431, {-314, -1938}).reflection() == Rectangle(37431, 4519, {-1938, -314}));
//
//    Rectangle rec12{5, 6, {-2, -1}};
//    rec12 += {2, 1};
//    assert(rec12 == Rectangle(5, 6));
//
//    Rectangle rec20{5, 6, {1, -3}};
//    Rectangle rec21{6, 9, {-4, 0}};
//
//    // Czy nie dano przypadkiem Rectangle::Rectangle(Rectangle &&) = delete.
//    Rectangle rec22 = std::move(rec20);
//
//    Rectangle rec23{1, 7};
//
//    // Czy nie dano przypadkiem Rectangle::operator=(Rectangle &&) = delete.
//    rec23 = std::move(rec21);
//
//    // Istnienie Rectangle::~Rectangle().
//    rec23.~Rectangle();
//
//// ------------------- RECTANGLES -------------------
//
//    // Ma istniec konstruktor domyslny
//    Rectangles recs1;
//    const Rectangles recs2{};
//
//    assert(recs1.size() == 0);
//    assert(recs2.size() == 0);
//
//    const Rectangle rec101(3, 5);
//    const Rectangle rec102(26, 31, {-31, -52});
//    const Rectangle rec103(591, 411, {15, 9});
//
//    // Sprawdza czy wykorzystano initializer_list w konstruktorze.
//    Rectangles recs3({rec101, rec102, rec103});
//
//    const Rectangles recs4({rec101, rec102, rec103});
//
//    assert(recs3.size() == 3);
//    assert(recs3.size() == 3);
//
//    assert(recs3 == recs4);
//
//    assert(!(recs1 == recs3));
//
//    recs3[0] = rec102;
//    assert(recs4[0].area() == 3 * 5);
//
//    recs3 += Vector(1, 1);
//
//    // Przenoszenie i kopiowanie
//
//    // Konstruktor kopiujacy.
//    Rectangles recs5 = recs4;
//
//    // operator= kopiujacy.
//    Rectangles recs6{rec103, rec102};
//    recs6 = recs4;
//
//    // Konstruktor przenoszacy
//    Rectangles recs7{rec103, rec102};
//    Rectangles recs8 = std::move(recs7);
//
//    // operator= przenoszacy
//    Rectangles recs9{rec103, rec102};
//    Rectangles recs10{rec101};
//    recs10 = std::move(recs9);
//
//// ------------ VARIOUS ---------------
//
//    // Dodawanie
//    Position p{3, 5};
//    const Position cp{-4, -9};
//
//    Rectangle r{9, 7, {-61, -13}};
//    const Rectangle cr{15, 12, {17, -21}};
//
//    Rectangles rs{Rectangle(8, 1, {-4, 8}),
//                  Rectangle(71, 23, {5, 6}),
//                  Rectangle(9, 15, {43, 12})};
//
//    const Rectangles crs{Rectangle(8, 1, {-4, 8}),
//                         Rectangle(71, 23, {5, 6}),
//                         Rectangle(9, 15, {43, 12})};
//
//    Vector v{-21, 34};
//    const Vector cv{12, 5};
//
//    p + v;
//    cp + v;
//    p + cv;
//    cp + cv;
//
//    v + p;
//    v + cp;
//    cv + p;
//    cv + cp;
//
//    v + v;
//    cv + v;
//    cv + cv;
//
//    r + v;
//    cr + v;
//    r + cv;
//    cr + cv;
//
//    v + r;
//    cv + r;
//    v + cr;
//    cv + cr;
//
//    rs + v;
//    crs + v;
//    rs + cv;
//    crs + cv;
//
//    Rectangles tm1 = rs;
//    Rectangles tm2 = crs;
//    Rectangles tm3 = rs;
//    Rectangles tm4 = crs;
//
//    std::move(tm1) + cv;
//    cv + std::move(tm2);
//    std::move(tm3) + v;
//    v + std::move(tm4);
//
//// ------------- MERGE -------------
//
//    const Rectangle mr1{4, 5, {3, 7}};
//    const Rectangle mr2{4, 1, {3, 12}};
//
//    assert(merge_horizontally(mr1, mr2) == Rectangle(4, 6, {3, 7}));
//
//    const Rectangle mr3{4, 1, {3, 13}};
//    const Rectangle mr4{4, 1, {4, 13}};
//
//    const Rectangle mr5{4, 5};
//    const Rectangle mr6{1, 5, {4, 0}};
//
//    assert(merge_vertically(mr5, mr6) == Rectangle(5, 5, {0, 0}));
//
//    const Rectangle mr7{1, 5, {4, 1}};
//
//    const Rectangle mr8{1, 5, {5, 1}};
//
//    Rectangle ret_all_1 = merge_all({Rectangle(2, 1),
//                                     Rectangle(2, 1, {0, 1}),
//                                     Rectangle(2, 2, {2, 0}),
//                                     Rectangle(4, 2, {0, 2}),
//                                     Rectangle(2, 4, {4, 0}),
//                                     Rectangle(6, 1, {0, 4})});
//
//    const Rectangle mr9{3, 4, {-105, -213}};
//
//    const Rectangle mr10{3, 2, {-105, -209}};
//    assert(merge_horizontally(mr9, mr10) == Rectangle(3, 6, {-105, -213}));
//
//    const Rectangle mr11(2, 4, {-102, -213});
//    assert(merge_vertically(mr9, mr11) == Rectangle(5, 4, {-105, -213}));
//
//    assert(ret_all_1 == Rectangle(6, 5));
//
//    // Czy typ zwracany przez origin() zawiera consta.
//    assert(std::is_const_v<std::remove_reference_t<decltype(Position::origin())>>);
//
//    // Position& Position::operator+=(const Vector&)
//    assert((std::is_same_v<std::invoke_result_t<decltype(&Position::operator+=), Position, const Vector &>, Position &>));
//
//    // Vector& Vector::operator+=(const Vector&)
//    assert((std::is_same_v<std::invoke_result_t<decltype(&Vector::operator+=), Vector, const Vector &>, Vector &>));
//
//    // Rectangle& Rectangle::operator+=(const Vector&)
//    assert((std::is_same_v<std::invoke_result_t<decltype(&Rectangle::operator+=), Rectangle, const Vector &>, Rectangle &>));
//
//    // Rectangles& Rectangles::operator+=(const Vector&)
//    assert((std::is_same_v<std::invoke_result_t<decltype(&Rectangles::operator+=), Rectangles, const Vector &>, Rectangles &>));
//
//    // pos27 = vec26;
//    //  vec26 = pos27;
//    //  Position pos28 = vec27;
//    // Vector vec28 = pos27;
//    // recs4[0] = rec102;
//    //  Rectangle &recRef1 = recs4[0];
//
////     recs3[3];
////      recs4[3];
////      Rectangle rec3{0, 0};
////      Rectangle rec4{3, 0};
////      Rectangle rec5{0, 5};
////      Rectangle rec6{-100, -100};
////      Rectangle rec300(-100, 200);
////      Rectangle rec301(100, -200);
////      merge_horizontally(mr1, mr3);
////      merge_horizontally(mr1, mr4);
////      merge_vertically(mr5, mr7);
////      merge_vertically(mr5, mr8);
//
////     Rectangle ret_all_2 = merge_all({Rectangle(2, 1),
////                                     Rectangle(2, 1, {0, 1}),
////                                     Rectangle(2, 2, {2, 0}),
////                                     Rectangle(4, 2, {0, 2}),
////                                     Rectangle(2, 4, {3, 0}),
////                                     Rectangle(6, 1, {0, 4})});
//
//    std::cout << "All tests passed!" << std::endl;
//}