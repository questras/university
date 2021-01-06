/** @file
 * Implementacja modułu interactive_mode_parser.h
 *
 * @author Michał Wiśniewski 418486
 * @date 14.05.2020
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "interactive_mode_parser.h"
#include "gamma.h"

#define ARROW 27            ///< Kod ASCII pierwszego znaku strzałki.
#define ARROW_UP 65         ///< Kod ASCII ostatniego znaku strzałki w górę.
#define ARROW_DOWN 66       ///< Kod ASCII ostatniego znaku strzałki w dół.
#define ARROW_RIGHT 67      ///< Kod ASCII ostatniego znaku strzałki w prawo.
#define ARROW_LEFT 68       ///< Kod ASCII ostatniego znaku strzałki w lewo.
#define GOLDEN_BUTTON1 'G'  ///< Przycisk oznaczający złoty ruch.
#define GOLDEN_BUTTON2 'g'  ///< Przycisk oznaczający złoty ruch.
#define SKIP_BUTTON1 'C'    ///< Przycisk oznaczający pominięcie ruchu.
#define SKIP_BUTTON2 'c'    ///< Przycisk oznaczający pominięcie ruchu.
#define MOVE_BUTTON 32      ///< Przycisk oznaczający ruch.
#define CTRL_D 4            ///< Kod ASCII kombinacji CTRL+D.
#define INFORMATION_ROWS 4  ///< Liczba wierszy informacji o graczu.


/** @brief Zwraca liczbę cyfr w liczbie.
 * @param[in] number    – liczba.
 * @return Liczba cyfr w liczbie.
 */
static int get_number_length(uint32_t number) {
    int count = 1;
    while (number >= 10) {
        number /= 10;
        count++;
    }
    return count;
}

/**
 * Czyści terminal.
 */
static inline void clear_terminal() {
    printf("\033[H\033[J");
}

/**
 * Czyści linię, na której aktualnie jest kursor.
 */
static inline void cursor_clear_current_line() {
    printf("\033[2K");
}


/** @brief Przesuwa kursor na pozycję w terminalu.
 * @param[in] x     – współrzędna x terminala,
 * @param[in] y     – współrzędna y terminala.
 */
static inline void cursor_go_to(uint32_t x, uint32_t y) {
    printf("\033[%d;%dH", (y), (x));
}


/** @brief Wypisuje numer gracza.
 * Wypisuje gracza @p player zajmując na terminalu szerokość określoną
 * w @p spacing.
 * @param[in] player    – numer gracza,
 * @param[in] spacing   – szerokość wypisanego napisu.
 */
static inline void print_player(uint32_t player, uint32_t spacing) {
    printf("%*u", spacing, player);
}

/** @brief Wykonuje ruch odpowiadający wciśniętej strzałce.
 * Po wczytaniu kodu @p ARROW przez @ref parse_character
 * sprawdza kolejne dwa znaki i zmienia współrzędne kursora
 * na podstawie wciśniętej strzałki przesuwając kursor w pionie o 1 a w
 * poziomie o wartość podaną w @p spacing, lub nie wykonuje go,
 * jeśli dwa kolejne znaki nie odpowiadają żadnej strzałce.
 * @param[in,out] x         – aktualna współrzędna x kursora,
 * @param[in,out] y         – aktualna współrzędna y kursora,
 * @param[in] width         – szerokość planszy,
 * @param[in] height        – wysokość planszy,
 * @param[in] spacing       – szerokość pojedyńczego pola.
 */
static void parse_move(uint32_t *x, uint32_t *y, uint32_t width,
                       uint32_t height, uint32_t spacing) {
    int c = getchar();
    if (c == '[') {
        c = getchar();
        if (c == ARROW_UP && *y > 1) {
            (*y)--;
        }
        else if (c == ARROW_DOWN && *y < height) {
            (*y)++;
        }
        else if (c == ARROW_RIGHT && *x < width * spacing) {
            (*x) += spacing;
        }
        else if (c == ARROW_LEFT && (*x) > spacing) {
            (*x) -= spacing;
        }
    }
}

/** @brief Wykonuje operację odpowiadającą wciśniętemu klawiszowi.
 * Wykonuje operację na podstawie kodu klawisza @p c wciśniętego przez
 * gracza @p player i zwraca informację o tym, czy operacja została wykonana.
 * @param[in] c              – kod wciśniętego klawisza,
 * @param[in,out] x          – aktualna współrzędna x kursora,
 * @param[in,out] y          – aktualna współrzędna y kursora,
 * @param[in] player         – numer aktualnego gracza,
 * @param[in,out] g          – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] spacing        – szerokość pojedyńczego pola.
 * @return @p true, jeśli udało się wykonać operację, inaczej @p false.
 */
static bool parse_character(int c, uint32_t *x, uint32_t *y,
                            uint32_t player, gamma_t *g, uint32_t spacing) {
    if (c == ARROW) {
        parse_move(x, y, gamma_get_width(g), gamma_get_height(g), spacing);
    }
    else if (c == GOLDEN_BUTTON2 || c == GOLDEN_BUTTON1 || c == MOVE_BUTTON) {
        bool result;
        uint32_t move_x = (*x) / spacing - 1;
        uint32_t move_y = gamma_get_height(g) - *y;

        if (c == GOLDEN_BUTTON2 || c == GOLDEN_BUTTON1) {
            result = gamma_golden_move(g, player, move_x, move_y);
        }
        else {
            result = gamma_move(g, player, move_x, move_y);
        }

        if (result) {
            cursor_go_to(*x - spacing + 1, *y);
            print_player(player, spacing);
            return true;
        }
    }
    else if (c == SKIP_BUTTON1 || c == SKIP_BUTTON2) {
        return true;
    }

    return false;
}

/** @brief Daje numer następnego gracza mogącego wykonać ruch.
 * Daje numer następnego w kolejce gracza, który może wykonać ruch,
 * lub 0, jeśli żaden z graczy nie może wykonać żadnego ruchu.
 * @param[in,out] g         – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player        – numer aktualnego gracza.
 * @return Numer następnego gracza mogącego wykonać ruch lub 0, jeśli
 * żaden z graczy nie może wykonać żadnego ruchu.
 */
static uint32_t next_player(gamma_t *g, uint32_t player) {
    uint32_t number_of_players = gamma_get_number_of_players(g);
    for (uint32_t i = player + 1; i <= number_of_players; i++) {
        if (gamma_free_fields(g, i) > 0 || gamma_golden_possible(g, i)) {
            return i;
        }
    }

    for (uint32_t i = 1; i <= player; i++) {
        if (gamma_free_fields(g, i) > 0 || gamma_golden_possible(g, i)) {
            return i;
        }
    }

    return 0;
}

/** @brief Czyści z terminala informacje o aktualnym graczu.
 * @param[in,out] g     – wskaźnik na strukturę przechowującą stan gry,
 */
static void clear_info(gamma_t *g) {
    uint32_t current_y = gamma_get_height(g) + 1;

    for (int i = 0; i < INFORMATION_ROWS; ++i) {
        cursor_go_to(1, current_y);
        cursor_clear_current_line();
        current_y++;
    }
}

/** @brief Wypisuje pod planszą informacje o aktualnym graczu.
 * Wypisuje pod planszą informacje o aktualnym graczu, czyli numer
 * aktualnego gracza, posiadane przez niego pola, ilość wolnych pól,
 * na które gracz może wykonać zwykły ruch oraz informację, czy możliwy
 * jest do wykonania złoty ruch.
 * @param[in,out] g     – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player    – numer aktualnego gracza.
 */
static void print_info(gamma_t *g, uint32_t player) {
    uint64_t owned = gamma_busy_fields(g, player);
    uint64_t free = gamma_free_fields(g, player);
    bool g_possible = gamma_golden_possible(g, player);

    clear_info(g);
    uint32_t current_y = gamma_get_height(g) + 1;

    cursor_go_to(1, current_y);
    printf("\033[1;36mPLAYER: %u\033[0m", player);

    current_y++;
    cursor_go_to(1, current_y);
    printf("\033[0;36mOwned fields: %lu\033[0m", owned);

    current_y++;
    cursor_go_to(1, current_y);
    printf("\033[0;36m%lu free fields available\033[0m", free);

    current_y++;
    cursor_go_to(1, current_y);
    if (g_possible) {
        printf("\033[0;32mGolden Move available\033[0m");
    }
    else {
        printf("\033[0;31mGolden Move not available\033[0m");
    }
}

/** @brief Wypisuje początkową planszę.
 * Wypisuje początkową planszę gry o szerokości i wysokości podanych
 * w @p width i @p height i o szerokości pojedyńczego pola podanego
 * w @p spacing.
 * @param[in] width     – szerokość planszy,
 * @param[in] height    – wysokość planszy,
 * @param[in] spacing   – szerokość pojedyńczego pola.
 */
static void print_initial_board(uint32_t width, uint32_t height,
                                uint32_t spacing) {
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            printf("%*s", spacing, ".");
        }
        printf("\n");
    }
}

/** @brief Wypisuje końcowe informacje.
 * Wypisuje informacje o każdym graczu i zajętych przez niego polach.
 * @param[in] g   – wskaźnik na strukturę przechowującą stan gry.
 */
static void print_end_game_info(gamma_t *g) {
    printf("\033[1;33mScore Board:\033[0m\n");
    uint32_t number_of_players = gamma_get_number_of_players(g);
    uint64_t winners_fields = 0;

    // Oblicz ilość pól zdobytych przez zwycięzcę gry.
    for (uint32_t i = 1; i <= number_of_players; i++) {
        uint64_t current_player_fields = gamma_busy_fields(g, i);
        if (current_player_fields > winners_fields) {
            winners_fields = current_player_fields;
        }
    }

    // Wypisz informacje o każdym graczu.
    for (uint32_t i = 1; i <= number_of_players; i++) {
        uint64_t current_player_fields = gamma_busy_fields(g, i);
        if (current_player_fields == winners_fields) {
            printf("\033[1;32mPLAYER %u: %lu fields - WINNER\033[0m\n",
                   i, current_player_fields);
        }
        else {
            printf("\033[0;33mPLAYER %u: %lu fields\033[0m\n",
                   i, current_player_fields);
        }
    }
}

/** @brief Wyłącza program jeśli kod oznacza błąd.
 * Wyłącza program z kodem wyjścia 1 jeśli @p status_code jest
 * inny niż kod sukcesu (0).
 * @param[in] status_code  - kod do sprawdzenia.
 */
static void exit_if_command_error(int status_code) {
    if (status_code != 0) {
        clear_terminal();
        printf("\033[0;31m");
        printf("Error: problem with running system commands.\n");
        printf("\033[0m");
        exit(1);
    }
}

/** @brief Wyłącza program jeśli terminal jest za mały.
 * Wyłącza program z kodem wyjścia 1 jeśli plansza @p g jest za duża,
 * aby wyświetlić ją w terminalu.
 * @param[in] g         – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] spacing   – szerokość pojedyńczego pola.
 */
static void exit_if_terminal_too_small(gamma_t *g, uint32_t spacing) {
    uint32_t required_rows = gamma_get_height(g) + INFORMATION_ROWS + 1;
    uint64_t required_cols = (uint64_t)spacing * (uint64_t)gamma_get_width(g);

    struct winsize window_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size);

    if (window_size.ws_row < required_rows ||
        window_size.ws_col < required_cols) {

        printf("\033[0;31m");
        printf("Error: terminal is too small.\n");
        printf("\033[0m");
        exit(1);
    }
}

/** @brief Włącza tryb interaktywny dla terminala.
 * Wywołuje komendy sprawiające, że możliwe jest wciskanie klawiszy
 * nie wypisując znaków i poruszanie kursorem po planszy lub wyłącza program,
 * jeśli wywołanie komend nie zakończyło się sukcesem.
 */
static inline void start_terminal_interactive_mode() {
    int status_code = system("stty -echo");
    exit_if_command_error(status_code);

    status_code = system("stty raw");
    exit_if_command_error(status_code);
}

/** @brief Wyłącza tryb interaktywny dla terminala.
 * Wywołuje komendy przywracające początkowe ustawienia terminala
 * lub wyłącza program, jeśli wywołanie komend nie zakończyło się sukcesem.
 */
static inline void stop_terminal_interactive_mode() {
    int status_code = system("stty echo");
    exit_if_command_error(status_code);

    status_code = system("stty cooked");
    exit_if_command_error(status_code);
}

void parse_interactive_mode(gamma_t *g) {
    uint32_t *x = malloc(sizeof(uint32_t));
    uint32_t *y = malloc(sizeof(uint32_t));
    if (x == NULL || y == NULL) {
        // Nie udało się zaalokować pamięci dla współrzędnej.
        exit(1);
    }

    uint32_t current_player = 1;
    uint32_t number_of_players = gamma_get_number_of_players(g);
    uint32_t spacing = get_number_length(number_of_players);
    if (number_of_players >= 10) {
        spacing++;
    }
    // Sprawdzenie, czy plansza zmieści się w terminalu.
    exit_if_terminal_too_small(g, spacing);

    // Pozycja początkowa kursora w lewym górnym rogu planszy.
    *x = spacing;
    *y = 1;

    // Wyczyszczenie terminala i wypisanie początkowego stanu planszy.
    clear_terminal();
    print_initial_board(gamma_get_width(g), gamma_get_height(g), spacing);
    print_info(g, current_player);
    cursor_go_to(*x, *y);
    start_terminal_interactive_mode();

    int c;
    bool change_player;
    while ((c = getchar()) != CTRL_D) {
        change_player = parse_character(c, x, y, current_player, g, spacing);

        if (change_player) {
            current_player = next_player(g, current_player);
            if (current_player == 0) {
                break;
            }
            print_info(g, current_player);
        }

        cursor_go_to(*x, *y);
    }

    stop_terminal_interactive_mode();

    // Wypisanie informacji końcowych o graczach.
    clear_info(g);
    cursor_go_to(1, gamma_get_height(g) + 1);
    print_end_game_info(g);

    free(x);
    free(y);
}