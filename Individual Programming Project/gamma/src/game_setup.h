/** @file
 * Interfejs modułu tworzącego i ustawiającego domyślne wartości
 * dla struktur używanych w grze gamma.
 *
 * @author Michał Wiśniewski 418486
 * @date 14.04.2020
 */

#ifndef GAMMA_GAME_SETUP_H
#define GAMMA_GAME_SETUP_H

#include <stdint.h>

#include "player.h"
#include "field.h"


/** @brief Tworzy strukturę przechowującą graczy.
 * Alokuje pamięć na nową strukturę przechowującą graczy.
 * Dla każdego gracza ustawia domyślne wartości,
 * reprezentujące początkowy stan gracza.
 * @param[in] players   – liczba graczy do stworzenia w strukturze.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Player *set_up_players(uint32_t players);

/** @brief Tworzy strukturę przechowującą planszę gry.
 * Alokuje pamięć na nową strukturę przechowującą planszę.
 * Dla każdego pola na planszy ustawia domyślne wartości,
 * reprezentujące początkowy stan pola.
 * @param[in] width   – szerokość planszy,
 * @param[in] height  – wysokość planszy.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Field **set_up_board(uint32_t width, uint32_t height);

#endif //GAMMA_GAME_SETUP_H
