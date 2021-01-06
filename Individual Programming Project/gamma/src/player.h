/** @file
 * Definicja struktury gracza.
 *
 * @author Michał Wiśniewski 418486
 * @date 14.04.2020
 */

#ifndef GAMMA_PLAYER_H
#define GAMMA_PLAYER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Definicja braku gracza.
 */
#define NO_PLAYER 0


/**
 * Struktura przechowująca informacje o pojedyńczym graczu w grze.
 */
struct player {
    /**
     * Ilość posiadanych pól.
     */
    uint64_t owned_fields;

    /**
     * Ilość posiadanych obszarów.
     */
    uint32_t owned_areas;

    /**
     * Informacja, czy złoty ruch został wykonany.
     */
    bool golden_move_used;
};

/**
 * Typ przechowujący wskaźnik na gracza.
 */
typedef struct player* Player;

#endif //GAMMA_PLAYER_H
