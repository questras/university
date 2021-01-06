/** @file
 * Interfejs modułu odpowiadającego za przeprowadzenie rozgrywki
 * w trybie interaktywnym.
 *
 * @author Michał Wiśniewski 418486
 * @date 14.05.2020
 */

#ifndef GAMMA_INTERACTIVE_MODE_PARSER_H
#define GAMMA_INTERACTIVE_MODE_PARSER_H

#include "gamma.h"

/** @brief Przeprowadza rozgrywkę w trybie interaktywnym.
 *  Wyświetla planszę, a pod planszą wiersz zachęcający gracza
 *  do wykonania ruchu. Funkcja prosi o wykonanie ruchu kolejnych graczy,
 *  przy czym pomija graczy, dla których funkcja @ref gamma_free_fields
 *  zwróciła 0 i funkcja @ref gamma_golden_possible zwróciła @p false.
 *  Ruch wykonuje się, przesuwając kursor na wybrane pole za pomocą
 *  klawiszy ze strzałkami, a następnie wciskając klawisz spacja,
 *  aby wykonać zwykły ruch, lub klawisz G, aby wykonać złoty ruch.
 *  Gracz może zrezygnować z ruchu, wciskając klawisz C.
 *  Funkcja kończy działanie, gdy już żaden gracz nie może wykonać ruchu
 *  lub po jednokrotnym wciśnięciu kombinacji klawiszy Ctrl-D.
 *  Wtedy funkcja wypisuje ostateczną planszę gry wraz z podsumowaniem,
 *  ile pól zajął każdy z graczy.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry.
 */
void parse_interactive_mode(gamma_t *g);

#endif //GAMMA_INTERACTIVE_MODE_PARSER_H
