/** @file
 * Implementacja modułu input_parser.h
 *
 * @author Michał Wiśniewski 418486
 * @date 14.05.2020
 */

#define  _GNU_SOURCE ///< Makro umożliwiające korzystanie z funkcji getline.

#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "input_parser.h"
#include "gamma.h"

#define DELIMITER " \t\v\f\r\n" ///< Makro napisu z możliwymi białymi znakami.


/**
 * Możliwe tryby rozgrywki.
 */
enum game_mode {
    BATCH,          ///< Tryb wsadowy.
    INTERACTIVE,    ///< Tryb interaktywny.
};

/**
 * Struktura przechowująca informacje o grze i trybie rozgrywki.
 */
struct gamma_info {
    /**
     * Tryb gry.
     */
    enum game_mode mode;

    /**
     * Informacje o grze gamma.
     */
    gamma_t *game;
};

bool is_batch(GammaInfo info) {
    return info->mode == BATCH;
}

gamma_t *get_game(GammaInfo info) {
    return info->game;
}

/** @brief Wypisuje informację, że wystąpił błąd.
 * Wypisuje na standardowy strumień błędów "ERROR line", gdzie "line"
 * to aktualna wartośc parametru @p line.
 * @param[in,out] line – numer aktualnej lini wejścia.
 */
static inline void print_error(unsigned long long *line) {
    fprintf(stderr, "ERROR %llu\n", *line);
}

/** @brief Sprawdza, czy napis ma wiodące zero.
 * Sprawdza, czy napis @p string ma wiodące zero i czy jego długość jest
 * większa od 1.
 * @param[in] string    – napis do sprawdzenia.
 * @return @p true jeśli napis ma wiodące zero, inaczej @p false.
 */
static inline bool has_leading_zeros(char *string) {
    return string[0] == '0' && string[1] != '\0';
}

/** @brief Sprawdza, czy napis jest równy "0".
 * @param[in] string    – napis do sprawdzenia.
 * @return @p true jeśli napis jest równy "0", inaczej @p false.
 */
static inline bool is_not_zero(char *string) {
    return strcmp(string, "0") != 0;
}

/** @brief Sprawdza, czy napis jest liczbą z zakresu uint32.
 * @param[in] string    – napis do sprawdzenia.
 * @return @p true jeśli napis jest liczbą z zakresu uint32,
 * inaczej @p false.
 */
static inline bool is_correct_number(char *string) {
    char *end_pointer;
    int64_t number = strtoll(string, &end_pointer, 10);

    return number >= 0 && number <= UINT32_MAX;
}

/** @brief Sprawdza, czy napis zawiera same cyfry.
 * @param[in] string    – napis do sprawdzenia.
 * @return @p true jeśli napis zawiera same cyfry, inaczej @p false.
 */
static inline bool is_numeric(char *string) {
    uint32_t n = strlen(string);

    for (uint32_t i = 0; i < n; i++) {
        unsigned char c = string[i];
        if (c < 48 || c > 57) {
            return false;
        }
    }

    return true;
}

/** @brief Sprawdza, czy napis jest komentarzem lub pustą linią.
 * Sprawdza, czy pierwszym znakiem napisu jest # lub czy napis składa
 * się tylko ze znaku przejścia do nowej linii.
 * @param[in] string  – napis do sprawdzenia.
 * @return @p true jeśli napis jest komentarzem lub pustą linią,
 * inaczej @p false.
 */
static inline bool is_comment_or_empty_line(char *string) {
    return string[0] == '#' || (string[0] == '\n' && string[1] == '\0');
}

/** @brief Sprawdza, czy argument jest poprawny.
 * Sprawdza, czy argument @p arg jest poprawny, czyli
 * nie jest równy @p NULL i jest liczbą z zakresu uint32 i
 * nie ma zer wiodących.
 * @param[in] arg    – argument do sprawdzenia.
 * @return @p true jeśli argument jest poprawny, inaczej @p false.
 */
static inline bool correct_argument(char *arg) {
    return arg != NULL && is_numeric(arg) &&
           !has_leading_zeros(arg) && is_correct_number(arg);
}

/** @brief Sprawdza, czy argumenty trybu gry są poprawne.
 * Sprawdza, czy argumenty trybu gry są poprawne przy
 * pomocy @ref correct_argument i sprawdza, czy argumenty są niezerowe.
 * @param[in] arg1    – argument do sprawdzenia,
 * @param[in] arg2    – argument do sprawdzenia,
 * @param[in] arg3    – argument do sprawdzenia,
 * @param[in] arg4    – argument do sprawdzenia.
 * @return @p true jeśli argumenty są poprawne, inaczej @p false.
 */
static inline bool correct_mode_arguments(char *arg1, char *arg2,
                                          char *arg3, char *arg4) {
    return correct_argument(arg1) && correct_argument(arg2) &&
           correct_argument(arg3) && correct_argument(arg4) &&
           is_not_zero(arg1) && is_not_zero(arg2) &&
           is_not_zero(arg3) && is_not_zero(arg4);
}

/** @brief Sprawdza, czy argumenty ruchu są poprawne.
 * @param[in] arg1     – argument do sprawdzenia,
 * @param[in] arg2     – argument do sprawdzenia,
 * @param[in] arg3     – argument do sprawdzenia.
 * @return @p true jeśli argumenty są poprawne, inaczej @p false.
 */
static inline bool correct_move_arguments(char *arg1, char *arg2, char *arg3) {
    return correct_argument(arg1) && correct_argument(arg2) &&
           correct_argument(arg3);
}

/** @brief Sprawdza, czy tryb jest poprawny.
 * Sprawdza czy tryb jest poprawny, czyli czy nie jest równy @p NULL i jest
 * równy "B" (tryb wsadowy(BATCH)) lub "I" (tryb interaktywny(INTERACTIVE)).
 * @param[in] mode  – tryb gry w postaci napisu.
 * @return @p true jeśli tryb jest poprawny, inaczej @p false.
 */
static inline bool correct_mode(char *mode) {
    return mode != NULL && (strcmp(mode, "B") == 0 || strcmp(mode, "I") == 0);
}

/** @brief Sprawdza, czy napis zawiera poprawną operację.
 * @param[in] string  – napis do sprawdzenia.
 * @return @p true, jeśli napis zawiera poprawną operację, inaczej @p false.
 */
static inline bool correct_operation(char *string) {
    return string[0] == 'm' || string[0] == 'g' ||
           string[0] == 'b' || string[0] == 'f' ||
           string[0] == 'q' || string[0] == 'p';
}

/** @brief Sprawdza, czy operacja jest operacją ruchu.
 * @param[in] operation     – operacja do sprawdzenia.
 * @return @p true, jeśli operacja jest operacją ruchu, inaczej @p false.
 */
static inline bool is_move_operation(char *operation) {
    return strcmp(operation, "m") == 0 || strcmp(operation, "g") == 0;
}

/** @brief Sprawdza, czy operacja jest operacją jedno-argumentową.
 * @param[in] operation     – operacja do sprawdzenia.
 * @return @p true, jeśli operacja jest operacją jedno-argumentową,
 * inaczej @p false.
 */
static inline bool is_one_argument_operation(char *operation) {
    return strcmp(operation, "b") == 0 || strcmp(operation, "f") == 0 ||
           strcmp(operation, "q") == 0;
}

/** @brief Sprawdza, czy operacja jest operacją planszy.
 * @param[in] operation     – operacja do sprawdzenia.
 * @return @p true, jeśli operacja jest operacją planszy, inaczej @p false.
 */
static inline bool is_board_operation(char *operation) {
    return strcmp(operation, "p") == 0;
}

/** @brief Konwertuje napis na tryb gry.
 * Konwertuje napis na tryb wsadowy (napis "B") lub interaktywny (napis "I").
 * Funkcja zakłada, że @p string jest równy "B" lub "I".
 * @param[in] string    – napis.
 * @return @p BATCH jeśli napis jest równy "B", inaczej @p INTERACTIVE.
 */
static enum game_mode string_to_mode(char *string) {
    if (strcmp(string, "B") == 0) {
        return BATCH;
    }
    else {
        return INTERACTIVE;
    }
}

/** @brief Tworzy strukturę gry gamma z argumentów w postaci napisów.
 * Konwertuje argumenty w postaci napisów na liczby i tworzy strukturę
 * gry gamma z podanych argumentów.
 * Funkcja zakłada, że podane napisy są poprawnymi argumentami gry w
 * postaci napisów.
 * @param[in] arg1  – argument gry w postaci napisu,
 * @param[in] arg2  – argument gry w postaci napisu,
 * @param[in] arg3  – argument gry w postaci napisu,
 * @param[in] arg4  – argument gry w postaci napisu.
 * @return Wskaźnik na utworzoną strukturę gry gamma.
 */
static gamma_t *gamma_new_from_input(char *arg1, char *arg2,
                                     char *arg3, char *arg4) {
    char *end_pointer;
    uint32_t width = strtoul(arg1, &end_pointer, 10);
    uint32_t height = strtoul(arg2, &end_pointer, 10);
    uint32_t players = strtoul(arg3, &end_pointer, 10);
    uint32_t areas = strtoul(arg4, &end_pointer, 10);

    return gamma_new(width, height, players, areas);
}

/** @brief Przetwarza linię i konwertuje ją na strukturę informacji o grze.
 * Przetwarza linię, konwertując argumenty w niej zapisane na strukturę
 * informacji o grze jeśli argumenty są poprawne.
 * @param[in] input_line    – linia do przetworzenia.
 * @return Struktura informacji o grze lub @p NULL, jeśli nie udało się
 * przekonwertować linii na strukturę.
 */
static GammaInfo parse_line_to_info(char **input_line) {
    char *mode = strtok(*input_line, DELIMITER);
    char *arg1 = strtok(NULL, DELIMITER);
    char *arg2 = strtok(NULL, DELIMITER);
    char *arg3 = strtok(NULL, DELIMITER);
    char *arg4 = strtok(NULL, DELIMITER);
    // Tryb gry ma 4 argumenty, więc ten argument powinien być NULLem.
    char *arg5 = strtok(NULL, DELIMITER);

    if (correct_mode(mode) && arg5 == NULL &&
        correct_mode_arguments(arg1, arg2, arg3, arg4)) {

        GammaInfo info = malloc(sizeof(struct gamma_info));
        if (info == NULL) {
            // Nie udało się zaalokować pamięci dla informacji o grze.
            exit(1);
        }

        gamma_t *game = gamma_new_from_input(arg1, arg2, arg3, arg4);
        if (game == NULL) {
            // Nie udało się zaalokować pamięci dla gry.
            free(info);
            return NULL;
        }

        info->mode = string_to_mode(mode);
        info->game = game;

        return info;
    }

    return NULL;
}

GammaInfo get_game_info(unsigned long long *line) {
    char *input = NULL;
    size_t size = 0;

    while (getline(&input, &size, stdin) != EOF) {
        (*line)++;
        if (input[0] == 'B' || input[0] == 'I') {
            GammaInfo info = parse_line_to_info(&input);

            if (info == NULL) {
                // Informacje o grze nie zostały wczytane.
                print_error(line);
                continue;
            }

            free(input);
            return info;
        }
        else if (is_comment_or_empty_line(input)) {
            continue;
        }
        print_error(line);
    }

    free(input);
    return NULL;
}

/** @brief Wykonuje operację ruchu na podanych argumentach.
 * Przetwarza argumenty w postaci napisów i wykonuje operacje
 * @ref gamma_move lub @ref gamma_golden_move w zależności od
 * argumentu @p operation i wypisuje 1, jeśli wynikiem wykonanej
 * operacji jest @p true, lub 0, jeśli wynikiem wykonanej operacji
 * jest @p false.
 * Funkcja zakłada, że parametry w postaci napisów są poprawne oraz, że
 * parametr @p operation jest równy "m" lub "g"
 * @param[in,out] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] operation   - operacja ruchu,
 * @param[in] arg1        – argument ruchu w postaci napisu,
 * @param[in] arg2        – argument ruchu w postaci napisu,
 * @param[in] arg3        – argument ruchu w postaci napisu.
 */
static void parse_move_operation(gamma_t *g, char *operation, char *arg1,
                                 char *arg2, char *arg3) {
    char *end_pointer;
    uint32_t player = strtoul(arg1, &end_pointer, 10);
    uint32_t x = strtoul(arg2, &end_pointer, 10);
    uint32_t y = strtoul(arg3, &end_pointer, 10);

    bool result;
    if (strcmp(operation, "m") == 0) {
        result = gamma_move(g, player, x, y);
    }
    else {
        result = gamma_golden_move(g, player, x, y);
    }

    printf("%d\n", result);
}

/** @brief Wykonuje operację jedno-argumentową na podanych argumentach.
 * Przetwarza argumenty w postaci napisów i wykonuje operację
 * @ref gamma_busy_fields, @ref gamma_free_fields lub
 * @ref gamma_golden_possible w zależności od argumentu @p operation i
 * wypisuje wynik operacji.
 * Funkcja zakłada, że parametry w postaci napisów są poprawne oraz, że
 * parametr @p operation jest równy "b" lub "f" lub "q".
 * @param[in,out] g         – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] operation     – operacja jedno-argumentowa,
 * @param[in] arg           – argument w postaci napisu.
 */
static void parse_one_argument_operation(gamma_t *g, char *operation,
                                         char *arg) {
    char *end_pointer;
    uint32_t player = strtoul(arg, &end_pointer, 10);

    uint64_t result;
    if (strcmp(operation, "b") == 0) {
        result = gamma_busy_fields(g, player);
    }
    else if (strcmp(operation, "f") == 0) {
        result = gamma_free_fields(g, player);
    }
    else {
        result = gamma_golden_possible(g, player);
    }

    printf("%lu\n", result);
}

/** @brief Przetwarza linię i konwertuje ją na operację, którą wykonuje.
 * Przetwarza linię, konwertując argumenty w niej zapisane na operację do
 * wykonania w grze @p g i wykonują, jeśli argumenty są poprawne.
 * @param[in] input_line    – linia do przetworzenia,
 * @param[in,out] g         – wskaźnik na strukturę przechowującą stan gry.
 * @return @p true, jeśli operacja została wykonana, inaczej @p false.
 */
static bool parse_line_to_batch_operation(char **input_line, gamma_t *g) {
    char *operation = strtok(*input_line, DELIMITER);
    char *arg1 = strtok(NULL, DELIMITER);
    char *arg2 = strtok(NULL, DELIMITER);
    char *arg3 = strtok(NULL, DELIMITER);
    char *arg4 = strtok(NULL, DELIMITER);

    if (is_move_operation(operation) && arg4 == NULL &&
        correct_move_arguments(arg1, arg2, arg3)) {

        parse_move_operation(g, operation, arg1, arg2, arg3);
        return true;
    }
    else if (is_one_argument_operation(operation) &&
             correct_argument(arg1) && arg2 == NULL) {

        parse_one_argument_operation(g, operation, arg1);
        return true;
    }
    else if (is_board_operation(operation) && arg1 == NULL) {
        char *board = gamma_board(g);
        if (board != NULL) {
            printf("%s", board);
            free(board);
            return true;
        }
    }
    return false;
}

void parse_batch_mode(gamma_t *g, unsigned long long *line) {
    char *input = NULL;
    size_t size = 0;
    while (getline(&input, &size, stdin) != EOF) {
        (*line)++;
        if (correct_operation(input)) {
            bool success = parse_line_to_batch_operation(&input, g);
            if (success) {
                continue;
            }
        }
        else if (is_comment_or_empty_line(input)) {
            continue;
        }

        print_error(line);
    }

    free(input);
}

