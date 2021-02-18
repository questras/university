/** @file
 * Implementacja modułu encstrset.
 *
 * @author Ignacy Makowski   418408
 * @author Michał Wiśniewski 418486
 */

#include "encstrset.h"

#include <unordered_map>
#include <unordered_set>
#include <string.h>
#include <iomanip>
#include <climits>
#include <cassert>

using std::hex;
using std::dec;
using std::setw;
using std::uppercase;
using std::setfill;
using std::unordered_map;
using std::unordered_set;
using std::string;
using std::cerr;
using std::endl;
using std::make_pair;
using encstrset_t = unordered_set<string>;
using encstrsetmap_t = unordered_map<unsigned long, encstrset_t>;

#define RUN_IF_DEBUG if(!debug){return;}    /// Makro do przerywania wykonania
                                            /// funkcji, jeśli debug jest
                                            /// ustawione na false.

namespace {

/// Globalna wartość ustalająca, czy program jest w wersji debug, czy nie,
/// w zależności od kompilacji.
#ifdef NDEBUG
    const bool debug = false;
#else
    const bool debug = true;
#endif

    /**
     * Zwraca obiekt typu encstrsetmap_t reprezentujący mapę,
     * gdzie kluczem jest id zbioru, a wartością odpowiedni zbiór.
     * Funkcja tworzy nowy obiekt, jeśli ten obiekt nie został wcześniej
     * stworzony, w przeciwnym wypadku zwraca poprzednio stworzony obiekt.
     *
     * Ponieważ lokalne obiekty typu static są tworzone po pierwszym razie,
     * gdy program do nich dociera, poniższe tworzenie elementu typu
     * encstrsetmap_t zostanie wykonane tylko raz. Za każdym następnym
     * razem funkcja zwróci stworzony wcześniej obiekt.
     *
     * @return globalny obiekt typu encstrsetmap_t.
     */
    encstrsetmap_t &all_sets() {
        static encstrsetmap_t encstrsetmap;
        return encstrsetmap;
    }

    /**
     * Zwraca następny dostępny numer dla zbioru i ustawia wartość
     * zmiennej next_id na następny dostępny numer.
     *
     * @return następny dostępny numer dla zbioru.
     */
    unsigned long get_next_id() {
        /// Id dla następnego stworzonego zbioru, przypisanie na 0 wykona
        /// się tylko raz, bo jest oznaczone jako "static".
        static unsigned long next_id = 0;

        /// Sprawdzenie czy maksymalny numer ID nie został przekroczony.
        assert(next_id < ULONG_MAX);

        return next_id++;
    }

    /**
     * Zwraca napis będący wynikiem szyfrowania wartości
     * value przy użyciu klucza key za pomocą operacji XOR.
     * Jeśli value jest NULLem, to funkcja zwraca NULL.
     * Jeśli key jest pusty lub jest NULLem, zwraca value.
     *
     * @param value - wartość do zaszyfrowania,
     * @param key   - klucz szyfrujący wartość.
     * @return zaszyfrowana wartość.
     */
    string encode(const char *value, const char *key) {
        if (!value) {
            return nullptr;
        }
        else if (!key || strcmp(key, "") == 0) {
            return string(value);
        }
        else {
            size_t val_len = strlen(value);
            size_t key_len = strlen(key);
            string result = string();
            result.reserve(val_len);
            result.resize(val_len);
            for (size_t i = 0; i < val_len; ++i) {
                result[i] = value[i] ^ key[i % key_len];
            }
            return result;
        }
    }

    /**
     * Wypisuje na standardowe wyjście diagnostyczne zapis pojedyńczych
     * liter z podanego napisu w postaci szestnastkowej.
     *
     * Operacja "0xff & static_cast<unsigned int>(s[i])" sprawia, że
     * postać szesnastkowa wypisze się w co najwyżej dwóch znakach.
     *
     * @param s - napis do wypisania.
     */
    void print_as_16(const string &s) {
        size_t n = s.length();
        if (n > 0) {
            for (size_t i = 0; i < n - 1; ++i) {
                cerr << uppercase << setfill('0') << setw(2) << hex
                     << (0xff & static_cast<unsigned int>(s[i]))
                     << " ";
            }
            cerr << uppercase << setfill('0') << setw(2) << hex
                 << (0xff & static_cast<unsigned int>(s[n - 1]))
                 << dec;
        }
    }

    /**
     * Wypisuje na wyjście diagnostyczne informacje o nieprawidłowej
     * wartości value dla podanej funkcji.
     *
     * @param function - funkcja, dla której wartość value jest nieprawidłowa.
     */
    void encstrset_invalid_value_info(const string &function) {
        RUN_IF_DEBUG
        cerr << function << ": invalid value (NULL)" << endl;
    }

    /**
     * Wypisuje na wyjście diagnostyczne informacje o nieistniejącym zbiorze
     * o podanym numerze id dla podanej funkcji.
     *
     * np. encstrset_does_not_exist_info("test", 1) wypisze:
     * encstrset_test: set #1 does not exist
     *
     * @param function - funkcja, dla której zbiór nie istnieje,
     * @param id       - id nieistniejącego zbioru.
     */
    void encstrset_does_not_exist_info(const string &function,
                                       unsigned long id) {
        RUN_IF_DEBUG
        cerr << function << ": set #" << id
             << " does not exist" << endl;
    }

    /**
     * Wypisuje na wyjście diagnostyczne informacje o wywołaniu funkcji
     * przyjmującej jeden argument - id zbioru.
     *
     * np. encstrset_call_with_id_info("test", 1) wypisze:
     * encstrset_test(1)
     *
     * @param function - funkcja, która jest wywoływana,
     * @param id       - argument funkcji: id.
     */
    void encstrset_call_with_id_info(const string &function,
                                     unsigned long id) {
        RUN_IF_DEBUG
        cerr << function << "(" << id << ")" << endl;
    }

    /**
     * Wypisuje na wyjście diagnostyczne informacje o wywołaniu funkcji
     * przyjmującej trzy argumenty: id zbioru, wartość i klucz.
     * Jeśli któryś z argumentów wartość lub klucz jest NULLem, to
     * zostanie wypisany w ich miejsce "NULL".
     *
     * np. encstrset_call_with_three_arguments_info("test", 1, "war", "klu")
     * wypisze:
     * encstrset_test(1, "war", "klu")
     *
     * @param function - funkcja, która jest wywoływana,
     * @param id       - argument funkcji: id,
     * @param value    - argument funkcji: wartość,
     * @param key      - argument funkcji: klucz.
     */
    void encstrset_call_with_three_arguments_info(const string &function,
                                                  unsigned long id,
                                                  const char *value,
                                                  const char *key) {
        RUN_IF_DEBUG
        cerr << function << "(" << id << ", ";
        if (value == nullptr) {
            cerr << "NULL, ";
        }
        else {
            cerr << "\"" << value << "\", ";
        }

        if (key == nullptr) {
            cerr << "NULL";
        }
        else {
            cerr << "\"" << key << "\"";
        }

        cerr << ")" << endl;
    }

    /**
     * Wypisuje na wyjście diagnostyczne informacje o akcji wykonanej
     * w podanej funkcji na zbiorze o podanym id.
     *
     * np. encstrset_set_action_info("test", "tested", 1) wypisze:
     * encstrset_test: set #1 tested
     *
     * @param function - funkcja, która jest wywoływana,
     * @param action   - informacja o akcji wykonywanej na zbiorze,
     * @param id       - id zbioru.
     */
    void encstrset_set_action_info(const string &function,
                                   const string &action,
                                   unsigned long id) {
        RUN_IF_DEBUG
        cerr << function << ": set #" << id << " "
             << action << endl;
    }

    /**
     * Wypisuje na wyjście diagnostyczne informacje o akcji wykonanej
     * w podanej funkcji na zbiorze o podanym id i zaszyfrowanej wartości.
     * Zaszyfrowana wartość jest wypisana w systemie szesnastkowym.
     *
     * np. encstrset_cypher_info("test", "added", 1, "abc") wypisze:
     * encstrset_test: set #1, cypher "61 62 63" added
     *
     * @param function      - funkcja, która jest wywoływana,
     * @param action        - informacja o akcji wykonywanej na zbiorze,
     * @param id            - id zbioru,
     * @param encoded_value - zaszyfrowana wartość.
     */
    void encstrset_cypher_info(const string &function, const string &action,
                               unsigned long id, const string &encoded_value) {
        RUN_IF_DEBUG
        cerr << function << ": set #" << id << ", cypher \"";
        print_as_16(encoded_value);
        cerr << "\" " << action << endl;
    }

    /**
     * Wypisuje na wyjście diagnostyczne informacje o wywołanej
     * funkcji encstrset_new.
     */
    void encstrset_new_call_info() {
        RUN_IF_DEBUG
        cerr << "encstrset_new()" << endl;
    }

    /**
     * Wypisuje na wyjście diagnostyczne informacje o poprawnie
     * wykonanej funkcji encstrset_size na zbiorze o podanym id
     * i wypisuje przekazany rozmiar tego zbioru.
     *
     * np. encstrset_size_success_info(1, 10) wypisze:
     * encstrset_size: set #1 contains 10 element(s)
     *
     * @param id    - id zbioru,
     * @param size  - przekazany rozmiar zbioru.
     */
    void encstrset_size_success_info(unsigned long id, size_t size) {
        RUN_IF_DEBUG
        cerr << "encstrset_size: set #" << id << " contains "
             << size << " element(s)" << endl;
    }

    /**
     * Wypisuje na wyjście diagnostyczne informacje o wywołanej funkcji
     * encstrset_copy_call_info dla zbiorów o podanych numerach id.
     *
     * np. encstrset_copy_call_info(1, 2) wypisze:
     * encstrset_copy(1, 2)
     *
     * @param id1   - id pierwszego zbioru,
     * @param id2   - id drugiego zbioru.
     */
    void encstrset_copy_call_info(unsigned long id1, unsigned long id2) {
        RUN_IF_DEBUG
        cerr << "encstrset_copy(" << id1 << ", " << id2 << ")" << endl;
    }

    /**
     * Wypisuje na wyjście diagnostyczne informacje o prawidłowym
     * wykonaniu funkcji encstrset_copy dla zbiorów o podanych numerach
     * id i dla danej zaszyfrowanej wartości.
     * Zaszyfrowana wartość jest wypisana w systemie szesnastkowym.
     *
     * np. encstrset_copy_success_info(1, 2, "abc") wypisze:
     * encstrset_copy: cypher "61 62 63" copied from set #1 to set #2
     *
     * @param id1           - id pierwszego zbioru,
     * @param id2           - id drugiego zbioru,
     * @param encoded_value - zaszyfrowana wartość.
     */
    void encstrset_copy_success_info(unsigned long id1, unsigned long id2,
                                     const string &encoded_value) {
        RUN_IF_DEBUG
        cerr << "encstrset_copy: cypher \"";
        print_as_16(encoded_value);
        cerr << "\" copied from set #" << id1 << " to set #" << id2 << endl;
    }

    /**
     * Wypisuje na wyjście diagnostyczne informacje o obecności
     * kopiowanej zaszyfrowanej wartości w zbiorze o podanym numerze id.
     * Zaszyfrowana wartość jest wypisana w systemie szesnastkowym.
     *
     * np. encstrset_copy_already_present_info(1, "abc") wypisze:
     * encstrset_copy: copied cypher "61 62 63" was already present in set #1
     *
     * @param id
     * @param encoded_value
     */
    void encstrset_copy_already_present_info(unsigned long id,
                                             const string &encoded_value) {
        RUN_IF_DEBUG
        cerr << "encstrset_copy: copied cypher \"";
        print_as_16(encoded_value);
        cerr << "\" was already present in set #" << id << endl;
    }
}

namespace jnp1 {

    unsigned long encstrset_new() {
        encstrset_new_call_info();

        unsigned long id = get_next_id();
        encstrset_t new_set;

        all_sets().insert(make_pair(id, new_set));
        encstrset_set_action_info(__FUNCTION__, "created", id);

        return id;
    }

    void encstrset_delete(unsigned long id) {
        encstrset_call_with_id_info(__FUNCTION__, id);

        bool result = all_sets().erase(id);
        if (result) {
            encstrset_set_action_info(__FUNCTION__, "deleted", id);
        }
        else {
            encstrset_does_not_exist_info(__FUNCTION__, id);
        }
    }

    size_t encstrset_size(unsigned long id) {
        encstrset_call_with_id_info(__FUNCTION__, id);

        encstrsetmap_t::const_iterator itr = all_sets().find(id);
        if (itr == all_sets().end()) {
            encstrset_does_not_exist_info(__FUNCTION__, id);
            return 0;
        }
        else {
            size_t size = itr->second.size();
            encstrset_size_success_info(id, size);

            return size;
        }
    }

    bool encstrset_insert(unsigned long id, const char *value,
                          const char *key) {
        encstrset_call_with_three_arguments_info(__FUNCTION__, id, value, key);

        if (value == nullptr) {
            encstrset_invalid_value_info(__FUNCTION__);
            return false;
        }
        else {
            encstrsetmap_t::iterator itr = all_sets().find(id);
            if (itr == all_sets().end()) {
                encstrset_does_not_exist_info(__FUNCTION__, id);
                return false;
            }
            else {
                string encoded_value = encode(value, key);
                bool answer = itr->second.insert(encoded_value).second;

                if (answer) {
                    encstrset_cypher_info(__FUNCTION__, "inserted", id,
                                          encoded_value);
                    return true;
                }
                else {
                    encstrset_cypher_info(__FUNCTION__, "was already present",
                                          id, encoded_value);
                    return false;
                }
            }
        }
    }

    bool encstrset_remove(unsigned long id, const char *value,
                          const char *key) {
        encstrset_call_with_three_arguments_info(__FUNCTION__, id, value, key);
        bool result = false;

        if (value == nullptr) {
            encstrset_invalid_value_info(__FUNCTION__);
        }
        else {
            encstrsetmap_t::iterator itr = all_sets().find(id);
            if (itr == all_sets().end()) {
                encstrset_does_not_exist_info(__FUNCTION__, id);
            }
            else {
                string encoded_value = encode(value, key);
                result = itr->second.erase(encoded_value);

                if (result) {
                    encstrset_cypher_info(__FUNCTION__, "removed", id,
                                          encoded_value);
                }
                else {
                    encstrset_cypher_info(__FUNCTION__, "was not present", id,
                                          encoded_value);
                }
            }
        }

        return result;
    }

    bool encstrset_test(unsigned long id, const char *value, const char *key) {
        encstrset_call_with_three_arguments_info(__FUNCTION__, id, value, key);
        bool result = false;

        if (value == nullptr) {
            encstrset_invalid_value_info(__FUNCTION__);
        }
        else {
            encstrsetmap_t::const_iterator itr = all_sets().find(id);
            if (itr == all_sets().end()) {
                encstrset_does_not_exist_info(__FUNCTION__, id);
            }
            else {
                string encoded_value = encode(value, key);
                result = itr->second.count(encoded_value);
                if (result) {
                    encstrset_cypher_info(__FUNCTION__, "is present", id,
                                          encoded_value);
                }
                else {
                    encstrset_cypher_info(__FUNCTION__, "is not present", id,
                                          encoded_value);
                }
            }
        }

        return result;
    }

    void encstrset_clear(unsigned long id) {
        encstrset_call_with_id_info(__FUNCTION__, id);
        encstrsetmap_t::iterator itr = all_sets().find(id);

        if (itr == all_sets().end()) {
            encstrset_does_not_exist_info(__FUNCTION__, id);
        }
        else {
            itr->second.clear();
            encstrset_set_action_info(__FUNCTION__, "cleared", id);
        }
    }

    void encstrset_copy(unsigned long src_id, unsigned long dst_id) {
        encstrset_copy_call_info(src_id, dst_id);
        encstrsetmap_t::iterator source = all_sets().find(src_id);
        encstrsetmap_t::iterator destination = all_sets().find(dst_id);

        if (source == all_sets().end()) {
            encstrset_does_not_exist_info(__FUNCTION__, src_id);
        }
        else if (destination == all_sets().end()) {
            encstrset_does_not_exist_info(__FUNCTION__, dst_id);
        }
        else {
            for (string elem: source->second) {
                bool success = destination->second.insert(elem).second;
                if (success) {
                    encstrset_copy_success_info(src_id, dst_id, elem);
                }
                else {
                    encstrset_copy_already_present_info(dst_id, elem);
                }
            }
        }
    }
}
