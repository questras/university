/** @file
 * Główny moduł, zawierający funkcję main.
 *
 * @author Michał Wiśniewski 418486
 * @date 15.04.2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input_parser.h"
#include "interactive_mode_parser.h"


/** @brief Funkcja główna projektu.
 * @return Kod zakończenia programu.
 */
int main() {
    unsigned long long *line = malloc(sizeof(unsigned long long));
    if (line == NULL) {
        // Nie udało się zaalokować pamięci dla numeru linii.
        exit(1);
    }
    (*line) = 0;

    // Wczytaj informacje o trybie gry i o samej grze.
    GammaInfo info = get_game_info(line);

    if (info != NULL) {
        gamma_t *game = get_game(info);

        if (is_batch(info)) {
            printf("OK %llu\n", *line);
            parse_batch_mode(game, line);
        }
        else {
            parse_interactive_mode(game);
        }

        gamma_delete(game);
        free(info);
    }

    free(line);
    return 0;
}