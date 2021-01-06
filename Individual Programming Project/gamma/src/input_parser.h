/** @file
 * Interfejs modułu odpowiadającego za wczytywanie i parsowanie
 * danych z wejścia.
 *
 * @author Michał Wiśniewski 418486
 * @date 14.05.2020
 */

#ifndef GAMMA_INPUT_PARSER_H
#define GAMMA_INPUT_PARSER_H

#include "gamma.h"

/**
 * Struktura przechowująca informacje o grze i
 * trybie gry(wsadowy lub interaktywny).
 */
typedef struct gamma_info* GammaInfo;

/** @brief Wczytuje z wejścia informacje o grze.
 * Wczytuje dane z wejścia aktualizując numer aktualnej linii @p line
 * tak długo, aż zostaną wczytane informacje o trybie i parametrach gry lub
 * aż dane się skończą.
 * @param[in,out] line  – numer aktualnej lini wejścia.
 * @return Struktura z informacją o trybie i parametrach gry lub @p NULL,
 * gdy dane na wejściu się skończą, a informacje nie zostaną wczytane.
 */
GammaInfo get_game_info(unsigned long long *line);

/** @brief Wczytuje i wykonuje w grze operacje wczytane z wejścia.
 * Wczytuje dane z wejścia i wykonuje operacje, jeśli są poprawne.
 * Jeśli wynikiem wywołanej operacji jest @p true, wypisana zostanie
 * wartość 1. Jeśli wynikiem wywołanej operacji jest @p false, wypisana
 * zostanie wartość 0. Jeśli linia wczytana z wejścia nie jest poprawną
 * operacją, wypisany zostanie komunikat "ERROR line", gdzie "line" to
 * aktualna wartość parametru @p line.
 * @param[in,out] g    – wskaźnik na strukturę przechowującą stan gry,
 * @param[in,out] line – numer aktualnej lini wejścia.
 */
void parse_batch_mode(gamma_t *g, unsigned long long *line);

/** @brief Sprawdza, czy rozgrywka jest w trybie wsadowym.
 * @param[in] info  – struktura przechowująca informacje o grze i trybie gry.
 * @return @p true jeśli rozgrywka jest w trybie wsadowym, inaczej @p false.
 */
bool is_batch(GammaInfo info);

/** @brief Zwraca gre zapisaną w strukturze z informacją o grze.
 * @param[in] info  – struktura przechowująca informacje o grze.
 * @return Wskaźnik na strukturę przechowującą stan gry.
 */
gamma_t *get_game(GammaInfo info);

#endif //GAMMA_INPUT_PARSER_H
