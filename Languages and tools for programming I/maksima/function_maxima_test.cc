#include "function_maxima.h"

// Sprawdź, czy jest header guard.
#if TEST_NUM == 700
#include "function_maxima.h"
#endif

#include <cassert>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <memory>

namespace {

// Wyjątek rzucany, gdy chcemy wartość funkcji w punkcie, którego nie ma.
class InvalidArgEasy : public std::exception {
public:
  const char* what() const noexcept override {
    return "InvalidArg";
  }
};

// Klasa FunctionMaximaEasy jest prostą, ale nieodporną implementacją zadania.
template<typename A, typename V>
class FunctionMaximaEasy {
  // Powierzamy zarządzanie pamięcia sprytnym wskaźnikom.
  using A_ptr = std::shared_ptr<const A>;
  using V_ptr = std::shared_ptr<const V>;
public:
  // Widok pary (argument, wartość), definicja niżej.
  class point_type;
private:
  struct arg_compare;
  struct value_compare;
  using repr_type = std::set<point_type, arg_compare>;
  using mx_type = std::set<point_type, value_compare>;
public:
  using iterator = typename repr_type::iterator;
  using mx_iterator = typename mx_type::iterator;
  using size_type = typename repr_type::size_type;

  // Reprezentacja punktu
  class point_type {
  public:
    const A& arg() const noexcept {
      return *arg_;
    }
    const V& value() const noexcept {
      return *value_;
    }
  private:
    // Pozwalamy głównej klasie dobierać się do wskzników.
    friend class FunctionMaximaEasy;
    // Potrzebujemy tylko konstruktora dwuargumentowego.
    point_type(const A_ptr &a, const V_ptr &v) : arg_(a), value_(v) {}
    // Zarówno argument, jak i wartość trzymamy za pomocą sprytnego wskaźnika.
    A_ptr arg_;
    V_ptr value_;
  };

  size_type size() const noexcept {
    return points.size();
  }
  size_type mx_count() const noexcept {
    return mx_points.size();
  }

  // Funkcje opakowujące iterator
  iterator begin() const noexcept {
    return points.begin();
  }
  iterator end() const noexcept {
    return points.end();
  }

  // Iteratory do maksimów lokalnych
  mx_iterator mx_begin() const noexcept {
    return mx_points.begin();
  }
  mx_iterator mx_end() const noexcept {
    return mx_points.end();
  }

  // Metoda lookup_helper daje sztuczny punkt z samym argumentem,
  // który służy do wyszukiwania.
  iterator find(const A& arg) const {
    return points.find(lookup_helper(arg));
  }

  // Wartość w punkcie lub wyjątek
  const V& value_at(const A& arg) const {
    iterator it = find(arg);
    if (it == end()) {
      throw InvalidArgEasy();
    }
    return it->value();
  }

  // Usuń wartość funkcji w punkcie arg.
  void erase(const A& arg) {
    iterator c = find(arg);
    // Jeśli nie ma równoważnego klucza, nie nie robimy.
    if (c == end()) {
      return;
    }
    points.erase(c);
    rebuild_maxima();
  }

  // Wstaw wartość value funkcji w punkcie arg.
  void set_value(const A& arg, const V& value) {
    iterator c = find(arg);
    A_ptr new_arg = c != end() ? c->arg_ : std::make_shared<const A>(arg);
    V_ptr new_value = std::make_shared<const V>(value);
    point_type p(new_arg, new_value);
    if (c != end()) {
      points.erase(c);
    }
    points.insert(p);
    rebuild_maxima();
  }

  bool operator<(const FunctionMaximaEasy &F) const {
    auto cmp = [](const point_type &a, const point_type &b) -> bool {
      return a.arg() < b.arg() || (!(b.arg() < a.arg()) && a.value() < b.value());
    };
    return std::lexicographical_compare(points.begin(), points.end(),
                                        F.points.begin(), F.points.end(), cmp);
  }
private:
  void rebuild_maxima() {
    mx_points.clear();
    for (auto it = points.begin(); it != points.end(); ++it) {
      if ((it == points.begin() || !(it->value() < prev(it)->value())) &&
          (next(it) == points.end() || !(it->value() < next(it)->value()))) {
        mx_points.insert(*it);
      }
    }
  }

  // Sztuczny punkt slużący do wyszukiwania w reprezentacji.
  // Moglibyśmy tego uniknąć, jeśli byśmy alokowali własną kopię argumentu.
  // Klasa shared_ptr pozwala nam jednak powiedzieć mu, żeby nic nie robił,
  // gdy licznik referencji spadnie do 0 (null_deleter):
  // http://www.boost.org/doc/libs/1_57_0/libs/smart_ptr/sp_techniques.html.
  static point_type lookup_helper(const A& arg) {
    return point_type(A_ptr(&arg, []([[maybe_unused]] const A* ptr){}),
                      V_ptr(nullptr));
  }

  // Komparator wyznaczający kolejność argumentów
  struct arg_compare {
    bool operator()(const point_type &a, const point_type &b) const {
      return a.arg() < b.arg();
    }
  };

  // Komparator do przechodzenia po maksimach we właściwej kolejności
  struct value_compare {
    bool operator()(const point_type &a, const point_type &b) const {
      return b.value() < a.value() ||
             (!(a.value() < b.value()) && a.arg() < b.arg());
    }
  };

  // Lista punktów
  repr_type points;
  // Lista maksimów
  mx_type mx_points;
};

// Sprawdzenie równość na punktach z FunctionMaxima i FunctionMaximaEasy
template<typename A, typename V>
bool point_eq(const typename FunctionMaxima<A, V>::point_type &a,
              const typename FunctionMaximaEasy<A, V>::point_type &b) {
  return !(a.arg() < b.arg()) && !(b.arg() < a.arg()) &&
         !(a.value() < b.value()) && !(b.value() < a.value());
}

// Sprawdzenie równości FunctionMaxima i FunctionMaximaEasy
template<typename A, typename V>
bool operator==(const FunctionMaxima<A, V> &a,
                const FunctionMaximaEasy<A, V> &b) {
  return a.size() == b.size() &&
         std::distance(a.mx_begin(), a.mx_end()) == std::distance(b.mx_begin(), b.mx_end()) &&
         std::equal(a.begin(), a.end(), b.begin(), point_eq<A, V>) &&
         std::equal(a.mx_begin(), a.mx_end(), b.mx_begin(), point_eq<A, V>);
}

#if TEST_NUM == 100
class Secret {
public:
  int get() const {
    return value;
  }
  bool operator<(const Secret &a) const {
    return value < a.value;
  }
  static Secret create(int v) {
    return Secret(v);
  }
private:
  Secret(int v) : value(v) {
  }
  int value;
};

template<typename A, typename V>
struct same {
  bool operator()(const typename FunctionMaxima<A, V>::point_type &p,
                  const std::pair<A, V> &q) {
    return !(p.arg() < q.first) && !(q.first < p.arg()) &&
           !(p.value() < q.second) && !(q.second < p.value());
  }
};

template<typename A, typename V>
bool fun_equal(const FunctionMaxima<A, V> &F,
               const std::initializer_list<std::pair<A, V>> &L) {
  return F.size() == L.size() &&
         std::equal(F.begin(), F.end(), L.begin(), same<A, V>());
}

template<typename A, typename V>
bool fun_mx_equal(const FunctionMaxima<A, V> &F,
                  const std::initializer_list<std::pair<A, V>> &L) {
  return static_cast<typename FunctionMaxima<A, V>::size_type>(std::distance(F.mx_begin(), F.mx_end())) == L.size() &&
         std::equal(F.mx_begin(), F.mx_end(), L.begin(), same<A, V>());
}
#endif

#if TEST_NUM >= 200 && TEST_NUM <= 499

// Wyjątek zgłaszany przez klasę testową
class ExceptionSafetyTester: public std::exception {
public:
  ExceptionSafetyTester() : std::exception() {}
};

// Pomocnicza klasa służąca do systematycznego zgłaszania wyjątków przy
// każdej nadarzającej się okazji
class Thrower {
public:
  Thrower() : enabled(false), throw_value(0), throw_counter(0) {}

  void reset(int val) {
    throw_value = val;
    throw_counter = 0;
  }

  void enable(bool en) {
    enabled = en;
  }

  void execute() {
    if (enabled) {
      if (throw_value == throw_counter++) {
        throw ExceptionSafetyTester();
      }
    } else {
      ++throw_counter;
    }
  }

  int get_current() const {
    return throw_counter;
  }
private:
  bool enabled;
  int throw_value;
  int throw_counter;
};

// Klasa reprezentująca argumenty i wartości funkcji
class SpecInt {
public:
  SpecInt(const SpecInt &s) : value(s.value), copies(s.copies) {
    th_copy.execute();
    ++(*copies);
  }

  bool operator<(const SpecInt &s) const {
    th_less.execute();
    return value < s.value;
  }

  int get() const noexcept {
    return value;
  }

  int count_c() const noexcept {
    return *copies;
  }

  ~SpecInt() noexcept {
    --(*copies);
  }
private:
  // Nie jest explicit i to potem wykorzystujemy.
  SpecInt(int v = 0) : value(v), copies(std::make_shared<int>(1)) {}

  SpecInt& operator=(SpecInt &other) {
    value  = other.value;
    copies = std::make_shared<int>(1);
    return *this;
  }

  bool operator==(const SpecInt &s) const {
    return value == s.value;
  }

  // Osobno obsługujemy wyjątki z konstruktora i operatora porównania.
  static Thrower th_copy;
  static Thrower th_less;
  int value;
  // Zliczamy kopie, żeby potem sprawdzić, czy nie jest ich za dużo.
  std::shared_ptr<int> copies;

  template<typename F1>
  friend void execute_catch(FunctionMaxima<SpecInt, SpecInt>&,
                            FunctionMaximaEasy<SpecInt, SpecInt>&,
                            Thrower&, F1, bool);

  template<typename F1, typename F2>
  friend void apply_op_safety_test(FunctionMaxima<SpecInt, SpecInt>&,
                                   FunctionMaximaEasy<SpecInt, SpecInt>&,
                                   F1, F2, bool, bool);

  friend void bfs_test(int N, bool, bool, bool);
  friend void no_throw_test(void);
  friend void destructor_test(void);
  friend void copy_test(void);
  friend void unnecessary_copies_test(void);
};

Thrower SpecInt::th_copy, SpecInt::th_less;

// Wywołaj exp_op(exp), test_op(test) i sprawdź, czy pozostało to samo.
template<typename A, typename V, typename F1, typename F2>
void apply_op(FunctionMaxima<A, V> &test, FunctionMaximaEasy<A, V> &exp,
              F1 test_op, F2 exp_op) {
  auto cp = exp;
  exp_op(exp);
  try {
    test_op(test);
  }
  catch (...) {
    assert(test == cp);
    throw;
  }
  assert(test == exp);
}

// 1. Policz, ile razy jest możliwość rzucenia wyjątku przez th.
// 2. Spróbuj rzucić wyjątek w każdym możliwym miejscu i sprawdź,
//    czy dotrzymano silnej gwarancji.
template<typename F1>
void execute_catch(FunctionMaxima<SpecInt, SpecInt> &test,
                   FunctionMaximaEasy<SpecInt, SpecInt> &exp,
                   Thrower &th, F1 op, bool no_throw) {
  th.reset(0);
  th.enable(false);
  auto tmp = test;
  op(tmp);
  int cnt = th.get_current();
  for (int i = 0; i < cnt; ++i) {
    tmp = test;
    th.reset(i);
    th.enable(true);
    try {
      op(tmp);
      assert(false);
    }
    catch (ExceptionSafetyTester &e) {
      assert(!no_throw && tmp == exp);
    }
    catch (...) {
      assert(false);
    }
    th.enable(false);
  }
}

// Połączenie apply_op i execute_catch, parametryzowane wyjątkami,
// które chcemy testować.
template<typename F1, typename F2>
void apply_op_safety_test(FunctionMaxima<SpecInt, SpecInt> &test,
                          FunctionMaximaEasy<SpecInt, SpecInt> &exp,
                          F1 test_op, F2 exp_op,
                          bool test_less, bool test_copy) {
  if (test_less) {
    execute_catch(test, exp, SpecInt::th_less, test_op, false);
  }
  if (test_copy) {
    execute_catch(test, exp, SpecInt::th_copy, test_op, false);
  }
  SpecInt::th_copy.reset(0);
  SpecInt::th_copy.enable(false);
  SpecInt::th_less.reset(0);
  SpecInt::th_less.enable(false);
  auto exp_copy = exp;
  apply_op(test, exp, test_op, exp_op);
}

// Test typu BFS: przechodzi po wszystkich funkcjach częściowych [N]->[N]
// i wykonuje wszystkie możliwe operacje, sprawdzając, czy wyszło to samo
// co dla FunctionMaximaEasy.
[[maybe_unused]] void bfs_test(int N, bool test_less, bool test_copy,
                               bool check_accessors) {
  using FE = FunctionMaximaEasy<SpecInt, SpecInt>;
  using F = FunctionMaxima<SpecInt, SpecInt>;
  std::map<FE, F> vis;
  std::queue<FE> que;
  que.push(FE());
  vis[FE()] = F();
  while (!que.empty()) {
    FE v = que.front();
    que.pop();
    const F &cv = vis[v];
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        FE vt = v;
        F tmp = cv;
        apply_op_safety_test(tmp, vt,
                             [&](F &f) -> void {f.set_value(i, j);},
                             [&](FE &f) -> void {f.set_value(i, j);},
                             test_less, test_copy);
        if (!vis.count(vt)) {
          vis[vt] = tmp;
          que.push(vt);
        }
      }
    }
    for (int i = 0; i < N; ++i) {
      FE vt = v;
      F tmp = cv;
      apply_op_safety_test(tmp, vt,
                           [&](F &f) -> void {f.erase(i);},
                           [&](FE &f) -> void {f.erase(i);},
                           test_less, test_copy);
    }
    if (check_accessors) {
      for (int i = 0; i < N; ++i) {
        try {
          auto x = v.value_at(i);
          auto it = v.find(i);
          assert(it != v.end() && !(it->arg() < i) &&
                 !(SpecInt(i) < it->arg()) &&
                 !(it->value() < x) &&
                 !(x < it->value()));
          try {
            auto y = cv.value_at(i);
            auto jt = cv.find(i);
            assert(!(x < y) && !(y < x));
            assert(jt != cv.end() && !(jt->arg() < i) &&
                   !(SpecInt(i) < jt->arg()) &&
                   !(jt->value() < x) &&
                   !(x < jt->value()));
          }
          catch (...) {
            assert(false);
          }
        }
        catch (InvalidArgEasy&) {
          assert(v.find(i) == v.end());
          try {
            auto x = cv.value_at(i);
            assert(false);
          }
          catch (InvalidArg&) {
            assert(cv.find(i) == cv.end());
          }
          catch (...) {
            assert(false);
          }
        }
        catch (...) {
          assert(false);
        }
      }
    }
  }
}

[[maybe_unused]] void no_throw_test(void) {
  using FE = FunctionMaximaEasy<SpecInt, SpecInt>;
  using F = FunctionMaxima<SpecInt, SpecInt>;
  srand(123456);
  SpecInt::th_less.enable(false);
  SpecInt::th_copy.enable(false);
  F f1;
  FE fe;
  for (int i = 1; i <= 100; ++i) {
    int a = rand(), b = rand();
    f1.set_value(a, b);
    fe.set_value(a, b);
  }
  auto op = [&](F &f) -> void {
    // Tutaj testujemy nierzucanie wyjątków przez size i iteratory.
    assert(f.size() == (F::size_type) std::distance(f.begin(), f.end()));
    assert(fe.mx_count() == (FE::size_type) std::distance(f.mx_begin(), f.mx_end()));
  };
  execute_catch(f1, fe, SpecInt::th_less, op, true);
  execute_catch(f1, fe, SpecInt::th_copy, op, true);
}

[[maybe_unused]] void destructor_test(void) {
  SpecInt::th_less.enable(false);
  SpecInt::th_copy.enable(false);
  FunctionMaxima<int, int> f1, f2, f3;
  for (int i = 1; i <= 100; ++i) {
    f1.set_value(i, i);
  }
  try {
    f3 = f1;
    f3 = f2;
    FunctionMaxima<int, int> tmp = f3;
    SpecInt::th_less.reset(0);
    SpecInt::th_less.enable(true);
    SpecInt::th_copy.reset(0);
    SpecInt::th_copy.enable(true);
  }
  catch (...) {
    assert(false);
  }
}

[[maybe_unused]] void copy_test(void) {
  FunctionMaxima<SpecInt, SpecInt> *f = new FunctionMaxima<SpecInt, SpecInt>(); {
    std::unique_ptr<SpecInt> s1(new SpecInt(12345)), s2(new SpecInt(54321));
    f->set_value(*s1, *s2);
  }
  auto p = f->find(SpecInt(12345));
  assert(p != f->end());
  assert(p->arg().get() == 12345);
  assert(p->value().get() == 54321);
  auto pc = *p;
  delete f;
  assert(pc.arg().get() == 12345);
  assert(pc.value().get() == 54321);
}

[[maybe_unused]] void unnecessary_copies_test(void) {
  FunctionMaxima<SpecInt, SpecInt> *f = new FunctionMaxima<SpecInt, SpecInt>();
  int N = 1000;
  for (int i = 1; i <= N; ++i) {
    f->set_value(SpecInt(i), SpecInt(i + N));
  }
  std::vector<FunctionMaxima<SpecInt, SpecInt>> fv = { *f, *f, *f };
  delete f;
  for (const auto &ff : fv) {
    for (const auto &p : ff) {
      assert(p.arg().count_c() == 1);
      assert(p.value().count_c() == 1);
    }
  }
}

[[maybe_unused]] void stress_test_1(void) {
  using T1 = std::pair<int, double>;
  using T2 = std::string;

  FunctionMaxima<T1, T2> f;
  int N = 100000;
  for (int i = 1; i <= N; ++i) {
    f.set_value({i, 0.01 * i}, "adam");
  }
  for (int i = 2; i <= N; i += 2) {
    f.set_value({i, 0.01 * i}, "aaaa");
  }
  for (int i = 2; i <= N; i += 2) {
    f.set_value({i, 0.01 * i}, "elo");
  }
  int cur = 2;
  for (auto it = f.mx_begin(); it != f.mx_end(); ++it) {
    assert(it->arg() == T1(cur, 0.01 * cur));
    assert(it->value() == "elo");
    cur += 2;
  }
  assert(cur == N + 2);
  for (int i = 2; i <= N; i += 2) {
    f.erase({i, 0.01 * i});
    f.erase({i, 0.02 * i});
  }
  for (int i = 1; i <= N; i += 2) {
    f.erase({i, 0.01 * i});
  }
  assert(f.size() == 0);
}

[[maybe_unused]] void stress_test_2(void) {
  srand(12345);
  FunctionMaxima<int, int> f;
  int N = 100000;

  for (int i = 1; i <= 3 * N; ++i) {
    int a = rand() % N, b = rand() % N;
    f.set_value(a, b);
    assert(f.find(a) != f.end() && f.find(a)->value() == b);
  }

  for (auto it = f.mx_begin(); it != f.mx_end(); ++it) {
    auto jt = f.find(it->arg());
    assert(jt == f.begin() || !(jt->value() < std::prev(jt)->value()));
    assert(std::next(jt) == f.end() || !(jt->value() < std::next(jt)->value()));
  }
}

[[maybe_unused]] void stress_test_3(void) {
  srand(99999);
  FunctionMaxima<int, int> f;
  int A = 1000, B = 100;
  for (int j = 0; j < A; ++j) {
    for (int i = 0; i < B; ++i) {
      f.set_value(i * A + j, j);
    }
  }
  std::vector<int> ord(B);
  for (int i = 0; i < B; ++i) {
    ord[i] = i;
  }
  for (int i = A - 1, k = A; i > 0; --i, ++k) {
    std::random_shuffle(ord.begin(), ord.end());
    for (int j = 0; j < B; ++j) {
      f.set_value(ord[j] * A + i, k);
      assert(std::distance(f.mx_begin(), f.mx_end()) == B);
    }
  }
}
#endif

} // anonimowa przestrzeń nazw

#if TEST_NUM >= 600 && TEST_NUM <= 699
long const max_throw_value = 50;
bool throw_zero;
bool throw_bad_alloc;
long throw_value;
long throw_counter;

void ThisCanThrowZero(void) {
  if (throw_zero && throw_value == throw_counter++) {
    throw_zero = false;
    throw_bad_alloc = false;
    throw 0;
  }
}

void ThisCanThrowBadAlloc(void) {
  if (throw_bad_alloc && throw_value == throw_counter++) {
    throw_zero = false;
    throw_bad_alloc = false;
    throw std::bad_alloc();;
  }
}

void* operator new(std::size_t size) {
  ThisCanThrowBadAlloc();
  void *p = malloc(size);
  if (p)
    return p;
  throw "operator new() error";
}

void* operator new[](std::size_t size) {
  ThisCanThrowBadAlloc();
  void *p = malloc(size);
  if (p)
    return p;
  throw "operator new[]() error";
}

void* operator new(std::size_t size, std::align_val_t al) {
  ThisCanThrowBadAlloc();
  void *p = aligned_alloc(static_cast<std::size_t>(al), size);
  if (p)
    return p;
  throw "operator new() error";
}

void* operator new[](std::size_t size, std::align_val_t al) {
  ThisCanThrowBadAlloc();
  void *p = aligned_alloc(static_cast<std::size_t>(al), size);
  if (p)
    return p;
  throw "operator new[]() error";
}

void operator delete(void* p) noexcept {
  free(p);
}

void operator delete(void* p, [[maybe_unused]] std::size_t s) noexcept {
  free(p);
}

void operator delete[](void *p) noexcept {
  free(p);
}

void operator delete[](void *p, [[maybe_unused]] std::size_t s) noexcept {
  free(p);
}

void operator delete(void* p, [[maybe_unused]] std::align_val_t al) noexcept {
  free(p);
}

void operator delete[](void* p, [[maybe_unused]] std::align_val_t al) noexcept {
  free(p);
}

void operator delete(void* p, [[maybe_unused]] std::size_t sz,
                     [[maybe_unused]] std::align_val_t al) noexcept {
  free(p);
}

void operator delete[](void* p, [[maybe_unused]] std::size_t sz,
                       [[maybe_unused]] std::align_val_t al) noexcept {
  free(p);
}

class Tester {
  int p;
public:
  explicit Tester(int v = 0) : p(v) {
  }
  Tester(const Tester& r) : p(r.p) {
    ThisCanThrowZero();
  }
  Tester(Tester&& r) noexcept : p(r.p) {
  }
  Tester& operator=(const Tester& r) {
    ThisCanThrowZero();
    p = r.p;
    return *this;
  }
  Tester& operator=(Tester&& r) noexcept {
    p = r.p;
    return *this;
  }
  ~Tester() noexcept {
  }
  bool operator<(const Tester& r) const {
    ThisCanThrowZero();
    return p < r.p;
  }
};

using F = FunctionMaxima<Tester, Tester>;
using FE = FunctionMaximaEasy<Tester, Tester>;

[[maybe_unused]] void strong_test(void) {
  throw_zero = false;
  throw_bad_alloc = false;

  FE fe0, fe1;
  fe1.set_value(Tester(1), Tester(2));
  fe1.set_value(Tester(2), Tester(1));
  fe1.set_value(Tester(-1), Tester(0));
  fe1.set_value(Tester(-2), Tester(1));

  for (throw_value = 0; throw_value < max_throw_value; ++throw_value) {
    F f0, f1;
    f1.set_value(Tester(1), Tester(2));
    f1.set_value(Tester(2), Tester(1));
    f1.set_value(Tester(-1), Tester(0));
    f1.set_value(Tester(-2), Tester(1));

    try {
      throw_counter = 0;
      throw_zero = true;
      throw_bad_alloc = true;
      f0 = f1; // Testuj operację.
      throw_zero = false;
      throw_bad_alloc = false;
      assert(f0 == fe1);
      assert(f1 == fe1);
    }
    catch (...) {
      assert(f0 == fe0);
      assert(f1 == fe1);
    }
  }

  throw_zero = false;
  throw_bad_alloc = false;

  fe0 = fe1;
  fe1.set_value(Tester(3), Tester(3));

  for (throw_value = 0; throw_value < max_throw_value; ++throw_value) {
    F f0, f1;
    f1.set_value(Tester(1), Tester(2));
    f1.set_value(Tester(2), Tester(1));
    f1.set_value(Tester(-1), Tester(0));
    f1.set_value(Tester(-2), Tester(1));
    f0 = f1;

    try {
      throw_counter = 0;
      throw_zero = true;
      throw_bad_alloc = true;
      f1.set_value(Tester(3), Tester(3)); // Testuj operację.
      throw_zero = false;
      throw_bad_alloc = false;
      assert(f1 == fe1);
      assert(f0 == fe0);
    }
    catch (...) {
      assert(f1 == fe0);
      assert(f0 == fe0);
    }
  }

  throw_zero = false;
  throw_bad_alloc = false;

  fe1 = fe0;
  fe1.set_value(Tester(-1), Tester(3));

  for (throw_value = 0; throw_value < max_throw_value; ++throw_value) {
    F f0, f1;
    f1.set_value(Tester(1), Tester(2));
    f1.set_value(Tester(2), Tester(1));
    f1.set_value(Tester(-1), Tester(0));
    f1.set_value(Tester(-2), Tester(1));
    f0 = f1;

    try {
      throw_counter = 0;
      throw_zero = true;
      throw_bad_alloc = true;
      f1.set_value(Tester(-1), Tester(3)); // Testuj operację.
      throw_zero = false;
      throw_bad_alloc = false;
      assert(f1 == fe1);
      assert(f0 == fe0);
    }
    catch (...) {
      assert(f1 == fe0);
      assert(f0 == fe0);
    }
  }
}

[[maybe_unused]] void exception_transparency_test(void) {
  throw_zero = false;
  throw_bad_alloc = false;

  for (throw_value = 0; throw_value < max_throw_value; ++throw_value) {
    F f;
    f.set_value(Tester(1), Tester(2));
    f.set_value(Tester(2), Tester(1));
    f.set_value(Tester(3), Tester(3));
    f.set_value(Tester(-1), Tester(100));
    f.set_value(Tester(-5), Tester(100));

    try {
      throw_counter = 0;
      throw_zero = true;
      f.value_at(Tester(-1)); // Testuj operację.
      throw_zero = false;
    }
    catch (int e) {
      // ThisCanThrowZero rzuca zero.
      assert(e == 0);
    }
    catch (...) {
      // Każdy inny wyjątek jest tu błędem.
      assert(false);
    }
  }
}
#endif

int main() {
// Sprawdź, czy implementacja przechodzi test przykładowy.
#if TEST_NUM == 100
  FunctionMaxima<int, int> fun;
  fun.set_value(0, 1);
  assert(fun_equal(fun, {{0, 1}}));
  assert(fun_mx_equal(fun, {{0, 1}}));

  fun.set_value(0, 0);
  assert(fun_equal(fun, {{0, 0}}));
  assert(fun_mx_equal(fun, {{0, 0}}));

  fun.set_value(1, 0);
  fun.set_value(2, 0);
  assert(fun_equal(fun, {{0, 0}, {1, 0}, {2, 0}}));
  assert(fun_mx_equal(fun, {{0, 0}, {1, 0}, {2, 0}}));

  fun.set_value(1, 1);
  assert(fun_mx_equal(fun, {{1, 1}}));

  fun.set_value(2, 2);
  assert(fun_mx_equal(fun, {{2, 2}}));
  fun.set_value(0, 2);
  fun.set_value(1, 3);
  assert(fun_mx_equal(fun, {{1, 3}}));

  try {
    std::cout << fun.value_at(4) << std::endl;
    assert(false);
  } catch (InvalidArg &e) {
    std::stringstream ss;
    const std::string error("invalid argument value");
    ss << e.what();
    assert(ss.str().compare(0, error.size(), error) == 0);
  }

  fun.erase(1);
  assert(fun.find(1) == fun.end());
  assert(fun_mx_equal(fun, {{0, 2}, {2, 2}}));

  fun.set_value(-2, 0);
  fun.set_value(-1, -1);
  assert(fun_mx_equal(fun, {{0, 2}, {2, 2}, {-2, 0}}));

  std::vector<FunctionMaxima<Secret, Secret>::point_type> v;
  {
    FunctionMaxima<Secret, Secret> temp;
    temp.set_value(Secret::create(1), Secret::create(10));
    temp.set_value(Secret::create(2), Secret::create(20));
    v.push_back(*temp.begin());
    v.push_back(*temp.mx_begin());
  }
  assert(v[0].arg().get() == 1);
  assert(v[0].value().get() == 10);
  assert(v[1].arg().get() == 2);
  assert(v[1].value().get() == 20);

  // To powinno działać szybko.
  FunctionMaxima<int, int> big;
  using size_type = decltype(big)::size_type;
  const size_type N = 100000;
  for (size_type i = 1; i <= N; ++i) {
    big.set_value(i, i);
  }
  size_type counter = 0;
  for (size_type i = 1; i <= N; ++i) {
    big.set_value(i, big.value_at(i) + 1);
    for (auto it = big.mx_begin(); it != big.mx_end(); ++it) {
      ++counter;
    }
  }
  assert(counter == 2 * N - 1);
  big = fun;
#endif

// Sprawdź, czy zablokowane jest tworzenie punktów.
// Trzy kolejne testy powinny zakończyć się błędem kompilacji.
#if TEST_NUM == 101
  FunctionMaxima<int, int>::point_type p;
  std::cerr << p.arg() << std::endl;
#endif

#if TEST_NUM == 102
  FunctionMaxima<int, int>::point_type p(2, 4);
  std::cerr << p.arg() << std::endl;
#endif

#if TEST_NUM == 103
  FunctionMaxima<int, int>::point_type p(std::make_shared<int>(2), std::make_shared<int>(3));
  std::cerr << p.arg() << std::endl;
#endif

// Test przechodzi po wszystkich możliwych funkcjach (częściowych)
// {1, ..., 5} -> {1, ..., 5}, wykonuje wszystkie możliwe operacje
// i sprawdza ich wyniki.
#if TEST_NUM == 200
  bfs_test(5, false, false, true);
#endif

// Testuj robienie własnych kopii i życie punktów po usunięciu funkcji.
#if TEST_NUM == 201
  copy_test();
#endif

// Uruchom pierwszą grupę testów odporności na wyjątki.
// Sprawdź odporność na wyjątki rzucane przez operator<.
// Znów przechodzimy wszystkie przypadki, lecz teraz [4] -> [4].
#if TEST_NUM == 300
  bfs_test(4, true, false, false);
#endif

// Sprawdź odporność na wyjątki rzucane przez konstruktor kopiujący.
// Znów przechodzimy wszystkie przypadki [4] -> [4].
#if TEST_NUM == 301
  bfs_test(4, false, true, false);
#endif

// Sprawdź, czy metoda size oraz iteratory nie rzucają wyjątków.
#if TEST_NUM == 302
  no_throw_test();
#endif

// Sprawdź, czy destruktor nie rzuca wyjątku.
#if TEST_NUM == 303
  destructor_test();
#endif

// Sprawdź, czy implementacja nie wykonuje zbyt wielu kopii.
#if TEST_NUM == 400
  unnecessary_copies_test();
#endif

// W trzech kolejnych testach sprawdź efektywność implementacji.
#if TEST_NUM == 401
  stress_test_1();
#endif

#if TEST_NUM == 402
  stress_test_2();
#endif

#if TEST_NUM == 403
  stress_test_3();
#endif

// Sprawdź, czy wyjątek dziedziczy publicznie po std::exception.
#if TEST_NUM == 500
  try {
    throw InvalidArg();
  } catch (std::exception&) {
  } catch (...) {
    assert(false);
  }
#endif

// Sprawdź poprawność typów iteratorów i poprawność wstawienia const.
#if TEST_NUM == 501
  using F = FunctionMaxima<int, int>;
  F f1;
  f1.set_value(1, 2);
  const F f2(f1);
  assert(2 == f2.value_at(1));
  F::size_type s = f2.size();
  assert(s == 1);
  const F::iterator it = f2.begin();
  const F::iterator it_mx = f2.mx_begin();
  assert(1 == it->arg());
  assert(2 == (*it).value());
  assert(1 == it_mx->arg());
  assert(2 == (*it_mx).value());
  const F::iterator i = f2.find(2);
  assert(i == f2.end());
#endif

// Trzy kolejne testy nie powinny się kompilować.
#if TEST_NUM == 502
  FunctionMaxima<int, int> f1;
  f1.set_value(1, 1);
  f1.value_at(1) += 1;
#endif

#if TEST_NUM == 503
  FunctionMaxima<int, int> f1;
  f1.set_value(1, 1);
  const FunctionMaxima<int, int>::iterator it = f1.begin();
  it->arg() += 1;
#endif

#if TEST_NUM == 504
  FunctionMaxima<int, int> f1;
  f1.set_value(1, 1);
  const FunctionMaxima<int, int>::iterator it_mx = f1.mx_begin();
  it_mx->value() += 1;
#endif

// Uruchom drugą grupę testów odporności na wyjątki.
#if TEST_NUM == 600
  strong_test();
#endif

// Uruchom test przeźroczystości na wyjątki.
#if TEST_NUM == 601
  exception_transparency_test();
#endif

// Testy sprawdzające, czy przestrzeń nazw nie została zaśmiecona.
// W każdej grupie musi być przynajmniej jeden test, który się kompiluje.
#if TEST_NUM == 800
#endif

// Pozostałe testy z tej grupy nie powinny się kompilować.
#if TEST_NUM == 801
  set<int> A;
  A.insert(5);
#endif

#if TEST_NUM == 802
  map<int, int> A;
  A[1] = 2;
#endif

#if TEST_NUM == 803
  cerr << "cerr" << std::endl;
#endif

#if TEST_NUM == 804
  shared_ptr<int> ptr(new int(5));
  *ptr = 6;
#endif
}
