/** @file
 * Implementacja modułu game_setup.h
 *
 * @author Michał Wiśniewski 418486
 * @date 14.04.2020
 */

#include <stdbool.h>
#include <stdlib.h>

#include "game_setup.h"
#include "field.h"
#include "player.h"


/** @brief Tworzy pole z ustawionymi domyślnymi wartościami.
 * Alokuje pamięć dla pojedyńczego pola i ustawia mu wartości domyślne.
 * @return pole z domyślnymi wartościami lub @p NULL jeśli
 * nie udało się zaalokować pamięci.
 */
static Field create_field() {
    Field f = malloc(sizeof(struct field));
    if (f == NULL) {
        return NULL;
    }
    f->owner = NO_PLAYER;
    f->parent = NULL;

    return f;
}

/** @brief Zwalnia pamięć w nie do końca zaalokowanej planszy.
 * Zwalnia pamięć w planszy, w której nie udało się zaalokować
 * wszystkich pól.
 * @param[in] board     – plansza,
 * @param[in] width     – szerokość planszy,
 * @param[in] height    – wysokość planszy,
 * @param[in] last_x    – indeks kolumny, na którym skończyła się alokacja,
 * @param[in] last_y    – indeks wiersza, na którym skończyła się alokacja.
 */
static void free_unpopulated_board(Field **board, uint32_t width,
                                   uint32_t height, uint32_t last_x,
                                   uint32_t last_y) {
    for (uint32_t i = 0; i < width; i++) {
        for (uint32_t j = 0; j < height; j++) {
            if (i == last_x && j == last_y) {
                return;
            }
            free(board[i][j]);
        }
    }
}

/** @brief Ustawia wartości domyślne dla każdego pola w planszy.
 * Ustawia wartości domyślne dla każdego pola w planszy lub zwalnia
 * z pamięci wszystkie pola, jeśli któregoś nie udało się zaalokować.
 * @param[in,out] board       – plansza do ustawienia wartości domyślnych,
 * @param[in] width           – szerokość planszy,
 * @param[in] height          – wysokość planszy.
 * @return @p false, jeśli nie udało się zaalokować pamięci, inaczej @p true.
 */
static bool populate_board(Field **board, uint32_t width, uint32_t height) {
    for (uint32_t i = 0; i < width; i++) {
        for (uint32_t j = 0; j < height; j++) {
            board[i][j] = create_field();

            if (board[i][j] == NULL) {
                // Wyczyść zaalokowaną do tej pory pamięć.
                free_unpopulated_board(board, width, height, i, j);
                return false;
            }
        }
    }

    return true;
}

Field **set_up_board(uint32_t width, uint32_t height) {
    Field **board_array = malloc(width * sizeof(Field *));
    if (board_array == NULL) {
        return NULL;
    }

    for (uint32_t i = 0; i < width; i++) {
        board_array[i] = malloc(height * sizeof(Field));

        if (board_array[i] == NULL) {
            // Wyczyść zaalokowaną do tej pory pamięć.
            for (uint32_t k = 0; k < i; k++) {
                free(board_array[k]);
            }
            free(board_array);
            return NULL;
        }
    }

    if (populate_board(board_array, width, height)) {
        return board_array;
    }
    else {
        // Wyczyść zaalokowaną do tej pory pamięć.
        for (uint32_t i = 0; i < width; i++) {
            free(board_array[i]);
        }
        free(board_array);
        return NULL;
    }
}

/** @brief Tworzy gracza z ustawionymi domyślnymi wartościami.
 * Alokuje pamięć dla pojedyńczego gracza i ustawia mu wartości domyślne.
 * @return gracz z domyślnymi wartościami lub @p NULL jeśli
 * nie udało się zaalokować pamięci.
 */
static Player create_player() {
    Player p = malloc(sizeof(struct player));
    if (p == NULL) {
        return false;
    }

    p->owned_fields = 0;
    p->owned_areas = 0;
    p->golden_move_used = false;

    return p;
}

/** @brief Ustawia wartości domyślne dla każdego gracza w strukturze.
 * Ustawia wartości domyślne dla każdego gracza w strukturze lub
 * zwalnia z pamięci wszystkich graczy, jeśli któregoś nie
 * udało się zaalokować.
 * @param[in,out] players           – wszyscy gracze w grze,
 * @param[in] number_of_players     – ilość graczy,
 * @return @p false, jeśli nie udało się zaalokować pamięci, inaczej @p true.
 */
static bool populate_players(Player *players, uint32_t number_of_players) {
    for (uint32_t i = 0; i < number_of_players; i++) {
        players[i] = create_player();

        if (players[i] == NULL) {
            // Wyczyść zaalokowaną do tej pory pamięć.
            for (uint32_t k = 0; k < i; k++) {
                free(players[k]);
            }
            return false;
        }
    }

    return true;
}

Player *set_up_players(uint32_t players) {
    Player *players_array = malloc(players * sizeof(Player));
    if (players_array == NULL) {
        return NULL;
    }

    if (populate_players(players_array, players)) {
        return players_array;
    }
    else {
        // Wyczyść zaalokowaną do tej pory pamięć.
        free(players_array);
        return NULL;
    }
}