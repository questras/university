/** @file
 * Implementacja modułu gamma.h
 *
 * @author Michał Wiśniewski 418486
 * @date 14.04.2020
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gamma.h"
#include "game_setup.h"
#include "field.h"
#include "player.h"


/**
 * Struktura przechowująca informacje o grze gamma.
 */
struct gamma {
    /**
     * Szerokość planszy gry.
     * */
    uint32_t width;
    /**
     * Wysokość planszy gry.
     * */
    uint32_t height;
    /**
     * Plansza gry.
     * */
    Field **board;

    /**
     * Liczba graczy w grze.
     * */
    uint32_t number_of_players;
    /**
     * Struktura przechowująca graczy.
     * */
    Player *players;

    /**
     * Maksymalna ilość obszarów do zajęcia przez gracza.
     * */
    uint32_t max_areas;
    /**
     * Liczba wolnych pól na planszy.
     * */
    uint64_t free_fields;
};

uint32_t gamma_get_width(gamma_t *g) {
    return g->width;
}

uint32_t gamma_get_height(gamma_t *g) {
    return g->height;
}

uint32_t gamma_get_number_of_players(gamma_t *g) {
    return g->number_of_players;
}

/** @brief Zwraca gracza o podanym indeksie.
 * Zwraca strukturę przechowującą informacje o graczu z indeksem @p player.
 * @param[in] g        – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player   – indeks gracza.
 * @return struktura przechowująca gracza o indeksie @p player.
 */
static inline Player get_player(gamma_t *g, uint32_t player) {
    return g->players[player - 1];
}

/** @brief Sprawdza czy podany gracz jest poprawny.
 * Sprawdza czy podany indeks gracza jest poprawny,
 * czyli różny od 0 i niewiększy od liczby graczy ze struktury @ref gamma.
 * @param[in] g        – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player   – indeks gracza.
 * @return wartość @p true, jeśli gracz jest poprawny, inaczej @p false.
 */
static inline bool correct_player(gamma_t *g, uint32_t player) {
    return player <= g->number_of_players && player != 0;
}

/** @brief Sprawdza czy podane współrzędne są poprawne.
 * Sprawdza, czy współrzędna @p x jest mniejsza od szerokości planszy
 * określonej w @ref gamma i współrzędna @p y jest mniejsza od wysokości
 * określonej w @ref gamma.
 * @param[in] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x   – współrzędna pola,
 * @param[in] y   – współrzędna pola,
 * @return wartość @p true, jeśli współrzędne są poprawne, inaczej @p false.
 */
static inline bool correct_coordinates(gamma_t *g, uint32_t x, uint32_t y) {
    return x < g->width && y < g->height;
}

/** @brief Sprawdza czy gracza posiada maksymalną ilość obszarów.
 * Sprawdza czy podany gracz posiada maksymalną ilość obszarów
 * określoną w strukturze @ref gamma.
 * @param[in] g        – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player   – indeks gracza.
 * @return wartość @p true, jeśli gracz posiada maksymalną ilość obszarów,
 * inaczej @p false.
 */
static inline bool has_maximum_areas(gamma_t *g, uint32_t player) {
    return get_player(g, player)->owned_areas >= g->max_areas;
}

/** @brief Sprawdza czy pole jest w posiadaniu gracza.
 * Sprawdza, czy podane współrzędne @p x i @p y są poprawne, a jeśli tak to
 * sprawdza, czy gracz @p player posiada pole o podanych współrzędnych.
 * @param[in] g        – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player   – indeks gracza.
 * @param[in] x        – współrzędna pola,
 * @param[in] y        – współrzędna pola,
 * @return wartość @p true, jeśli współrzędne są poprawne i gracz
 * posiada dane pole, inaczej @p false.
 */
static inline bool is_owned(gamma_t *g, uint32_t player,
                            uint32_t x, uint32_t y) {
    return correct_coordinates(g, x, y) && g->board[x][y]->owner == player;
}

/** @brief Sprawdza, czy gracz posiada któreś z pól wokół danego pola.
 * Sprawdza, czy gracz @p player posiada któreś z pól w sąsiedztwie
 * pola w punkcie (@p x, @p y).
 * @param[in] g        – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player   – indeks gracza.
 * @param[in] x        – współrzędna pola,
 * @param[in] y        – współrzędna pola,
 * @return wartość @p true, jeśli gracz posiada któres z pól w sąsiedztwie
 * danego pola, inaczej @p false.
 */
static inline bool own_neighbours(gamma_t *g, uint32_t player,
                                  uint32_t x, uint32_t y) {
    return is_owned(g, player, x - 1, y) || is_owned(g, player, x + 1, y) ||
           is_owned(g, player, x, y - 1) || is_owned(g, player, x, y + 1);
}

/** @brief Zwalnia pamięć struktury z graczami.
 * Zwalnia pamięć zaalokowaną dla każdego gracza i pamięć zaalokowaną
 * dla tablicy graczy.
 * @param[in] players           – wskaźnik na strukturę przechowującą graczy,
 * @param[in] number_of_players – liczba graczy.
 */
static void free_players(Player *players, uint32_t number_of_players) {
    for (uint32_t i = 0; i < number_of_players; i++) {
        free(players[i]);
    }

    free(players);
}

/** @brief Zwalnia pamięć planszy.
 * Zwalnia pamięć zaalokowaną dla każdego pola i pamięć zaalokowaną
 * dla tablicy pól w planszy.
 * @param[in] board     – wskaźnik na strukturę przechowującą planszę,
 * @param[in] width     – szerokość planszy,
 * @param[in] height    – wysokość planszy.
 */
static void free_board(Field **board, uint32_t width, uint32_t height) {
    for (uint32_t i = 0; i < width; i++) {
        for (uint32_t j = 0; j < height; j++) {
            free(board[i][j]);
        }
        free(board[i]);
    }

    free(board);
}

gamma_t *gamma_new(uint32_t width, uint32_t height,
                   uint32_t players, uint32_t areas) {

    if (width == 0 || height == 0 || players == 0 || areas == 0) {
        return NULL;
    }

    Field **board_array = set_up_board(width, height);
    if (board_array == NULL) {
        return NULL;
    }

    Player *players_array = set_up_players(players);
    if (players_array == NULL) {
        free_board(board_array, width, height);
        return NULL;
    }

    gamma_t *g = malloc(sizeof(gamma_t));
    if (g == NULL) {
        free_board(board_array, width, height);
        free_players(players_array, players);
        return NULL;
    }

    g->width = width;
    g->height = height;
    g->board = board_array;
    g->number_of_players = players;
    g->players = players_array;
    g->free_fields = width * height;
    g->max_areas = areas;

    return g;
}

void gamma_delete(gamma_t *g) {
    if (g == NULL) {
        return;
    }

    free_board(g->board, g->width, g->height);
    free_players(g->players, g->number_of_players);
    free(g);
}

/** @brief Łączy obszary dwóch pól gracza.
 * Łączy obszar pola w punkcie (@p x1, @p y1) z obszarem
 * pola w punkcie (@p x2, @p y2), jeśli należy ono do gracza @p player.
 * @param[in,out] g    – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player   – indeks gracza,
 * @param[in] x1       – współrzędna pierwszego pola,
 * @param[in] y1       – współrzędna pierwszego pola,
 * @param[in] x2       – współrzędna drugiego pola,
 * @param[in] y2       – współrzędna drugiego pola,
 * @return wartość @p true, obszary zostały połączone, a @p false,
 * jeśli obszary nie zostały połączone lub gracz nie posiada drugiego pola.
 */
static bool merge_fields(gamma_t *g, uint32_t player,
                         uint32_t x1, uint32_t y1,
                         uint32_t x2, uint32_t y2) {

    if (is_owned(g, player, x2, y2)) {
        return union_fields(g->board[x1][y1], g->board[x2][y2]);
    }

    return false;
}

/** @brief Łączy obszar pola z obszarami pól sąsiednich posiadanych przez gracza.
 * Łączy obszar pola w punkcie (@p x, @p y) z obszarami pól sąsiednich,
 * jeśli gracz je posiada.
 * @param[in,out] g    – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player   – indeks gracza,
 * @param[in] x        – współrzędna pola,
 * @param[in] y        – współrzędna pola,
 */
static void merge_fields_around_point(gamma_t *g, uint32_t player,
                                      uint32_t x, uint32_t y) {
    int merged_areas = 0;

    merged_areas += merge_fields(g, player, x, y, x - 1, y);
    merged_areas += merge_fields(g, player, x, y, x, y - 1);
    merged_areas += merge_fields(g, player, x, y, x + 1, y);
    merged_areas += merge_fields(g, player, x, y, x, y + 1);

    // Odejmujemy 1 od połączonych obszarów, bo połączenie pola (x, y)
    // z pierwszym sąsiadem dodaje 1, a wtedy nie łączą się
    // żadne różne obszary, bo pole (x, y) jeszcze do
    // żadnego nie należy.
    get_player(g, player)->owned_areas -= (merged_areas - 1);
}

/** @brief Aktualizuje parametry planszy po wykonaniu ruchu.
 * Aktualizuje parametry planszy po wykonaniu ruchu, czyli dodaje
 * 1 do posiadanych pól przez gracza @p player, odejmuje 1 od wolnych
 * pól na planszy w grze @p g i ustawia gracza @p player na właściciela
 * pola w punkcie (@p x, @p y).
 * @param[in,out] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player      – indeks gracza,
 * @param[in] x           – współrzędna pola,
 * @param[in] y           – współrzędna pola,
 */
static void update_game_after_move(gamma_t *g, uint32_t player,
                                   uint32_t x, uint32_t y) {
    get_player(g, player)->owned_fields += 1;
    g->free_fields--;
    g->board[x][y]->owner = player;
}

/** @brief Sprawdza, czy ruch jest możliwy ze względu na parametry.
 * Sprawdza czy ruch jest możliwy, czyli czy indeks gracza @p player jest
 * poprawny, czy współrzędne @p x i @p y są poprawne i czy pole w
 * punkcie (@p x, @p y) nie jest przez nikogo posiadane.
 * @param[in] g        – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player   – indeks gracza,
 * @param[in] x        – współrzędna pola,
 * @param[in] y        – współrzędna pola,
 * @return wartość @p true, jeśli ruch jest możliwy, inaczej @p false,
 */
static inline bool move_possible(gamma_t *g, uint32_t player,
                                 uint32_t x, uint32_t y) {
    return correct_player(g, player) &&
           correct_coordinates(g, x, y) &&
           is_owned(g, NO_PLAYER, x, y);
}

bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g == NULL || !move_possible(g, player, x, y)) {
        return false;
    }

    g->board[x][y]->parent = g->board[x][y];

    if (!own_neighbours(g, player, x, y)) {
        if (has_maximum_areas(g, player)) {
            g->board[x][y]->parent = NULL;

            return false;
        }
        else {
            get_player(g, player)->owned_areas += 1;
        }
    }
    else {
        merge_fields_around_point(g, player, x, y);
    }

    update_game_after_move(g, player, x, y);

    return true;
}

/** @brief Sprawdza, czy złoty ruch jest możliwy ze względu na parametry.
 * Sprawdza czy złoty ruch jest możliwy, czyli czy indeks gracza @p player jest
 * poprawny, czy współrzędne @p x i @p y są poprawne, czy pole w
 * punkcie (@p x, @p y) jest w posiadaniu innego gracza i czy
 * gracz @p player nie użył już złotego ruchu.
 * @param[in] g        – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player   – indeks gracza,
 * @param[in] x        – współrzędna pola,
 * @param[in] y        – współrzędna pola,
 * @return wartość @p true, jeśli złoty ruch jest możliwy, inaczej @p false,
 */
static inline bool golden_move_possible(gamma_t *g, uint32_t player,
                                        uint32_t x, uint32_t y) {
    return correct_player(g, player) &&
           correct_coordinates(g, x, y) &&
           g->board[x][y]->owner != NO_PLAYER &&
           g->board[x][y]->owner != player &&
           !(get_player(g, player)->golden_move_used);
}

/** @brief Ustawia ponownie pola gracza bez pola o podanych współrzędnych.
 * @param[in,out] g    – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player   – indeks gracza,
 * @param[in] x        – współrzędna pola,
 * @param[in] y        – współrzędna pola,
 */
static void setup_fields_without_point(gamma_t *g, uint32_t player,
                                       uint32_t x, uint32_t y) {
    get_player(g, player)->owned_areas = 0;

    // Zresetuj pole w punkcie (x, y).
    g->board[x][y]->owner = 0;
    get_player(g, player)->owned_fields--;
    g->free_fields++;

    int merged_areas;
    // Połącz każde pole z posiadanym polem tylko po lewej i na górze od pola,
    // aby nie łączyć wiele razy tych samych pól.
    for (uint32_t i = 0; i < g->width; i++) {
        for (uint32_t j = 0; j < g->height; j++) {
            if (g->board[i][j]->owner == player) {

                g->board[i][j]->parent = g->board[i][j];

                merged_areas = 0;
                merged_areas += merge_fields(g, player, i, j, i - 1, j);
                merged_areas += merge_fields(g, player, i, j, i, j - 1);

                if (merged_areas == 0) {
                    // Nie ma żadnego sąsiada posiadanego przez gracza.
                    get_player(g, player)->owned_areas++;
                }
                else {
                    // Odejmujemy 1 od połączonych obszarów, bo
                    // połączenie pola (i, j) z pierwszym sąsiadem dodaje 1,
                    // a wtedy nie łączą się żadne różne obszary, bo
                    // pole (i, j) jeszcze do żadnego nie należy.
                    get_player(g, player)->owned_areas -= (merged_areas - 1);
                }
            }
        }
    }
}

bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if ((g == NULL) || (!golden_move_possible(g, player, x, y))) {
        return false;
    }

    // Sprawdź czy gracz robiący złoty ruch może go zrobić.
    if (!own_neighbours(g, player, x, y) &&
        has_maximum_areas(g, player)) {
        return false;
    }

    uint32_t moved_player = g->board[x][y]->owner;
    setup_fields_without_point(g, moved_player, x, y);

    // Sprawdź czy obszary zgadzają sie dla poruszonego gracza.
    if (get_player(g, moved_player)->owned_areas > g->max_areas) {
        // Przywróć grę do stanu przed złotym ruchem.
        gamma_move(g, moved_player, x, y);
        return false;
    }
    else {
        gamma_move(g, player, x, y);
        get_player(g, player)->golden_move_used = true;
        return true;
    }
}

bool gamma_golden_possible(gamma_t *g, uint32_t player) {
    if (g == NULL || !correct_player(g, player) ||
        get_player(g, player)->golden_move_used) {

        return false;
    }

    // Sprawdź czy są zajęte pola przez innych graczy.
    uint64_t busy_fields = (uint64_t) g->width * (uint64_t) g->height;
    busy_fields -= g->free_fields + get_player(g, player)->owned_fields;
    if (busy_fields == 0) {
        return false;
    }

    bool player_has_maximum_areas = has_maximum_areas(g, player);

    // Sprawdź czy gracz może wykonać złoty ruch na którymkolwiek polu.
    for (uint32_t x = 0; x < g->width; x++) {
        for (uint32_t y = 0; y < g->height; y++) {
            if (g->board[x][y]->owner == player ||
                g->board[x][y]->owner == NO_PLAYER) {

                continue;
            }
            else {
                // Sprawdź czy gracz może zrobić złoty ruch na danym polu.
                if (!own_neighbours(g, player, x, y) &&
                    player_has_maximum_areas) {
                    continue;
                }

                // Sprawdź czy złoty ruch jest możliwy względem
                // właściciela danego pola.
                uint32_t field_owner = g->board[x][y]->owner;
                setup_fields_without_point(g, field_owner, x, y);

                // Ruch jest możliwy kiedy po usunięciu danego pola właściciel
                // pola ma dalej ilość obszarów mniejszą lub równą maksymalnej.
                bool move_possible =
                        get_player(g, field_owner)->owned_areas <= g->max_areas;

                // Przywróć grę do stanu przed złotym ruchem.
                gamma_move(g, field_owner, x, y);

                if (move_possible) {
                    return true;
                }
            }
        }
    }

    return false;
}

uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {
    if (g == NULL || !correct_player(g, player)) {
        return 0;
    }

    return get_player(g, player)->owned_fields;
}

/** @brief Zwraca ilość wolnych pól, które sąsiadują z polami gracza.
 * Zwraca ilość wolnych pól, które sąsiadują z conajmniej jednym polem
 * w posiadaniu gracza.
 * @param[in] g        – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player   – indeks gracza.
 * @return ilość wolnych pól, które sąsiadują z polami gracza.
 */
static uint64_t count_neighbour_free_fields(gamma_t *g, uint32_t player) {
    uint64_t free_fields = 0;

    for (uint32_t x = 0; x < g->width; x++) {
        for (uint32_t y = 0; y < g->height; y++) {
            if (is_owned(g, NO_PLAYER, x, y) &&
                own_neighbours(g, player, x, y)) {

                free_fields++;
            }
        }
    }

    return free_fields;
}

uint64_t gamma_free_fields(gamma_t *g, uint32_t player) {
    if (g == NULL || !correct_player(g, player)) {
        return 0;
    }

    // Jeśli gracz ma dostępne obszary, zwróć wszystkie wolne pola.
    if (!has_maximum_areas(g, player)) {
        return g->free_fields;
    }

    return count_neighbour_free_fields(g, player);
}


/** @brief Konwertuje liczbe na napis.
 * Konwertuje liczbę @p number na napis i zapisuje go w @p dest.
 * Tworzy napis tak, że zajmuje on tyle miejsca ile określone
 * w @p spacing, czyli dodaje tyle spacji na początku liczby,
 * aby długość napisu była równa @p spacing.
 * @param[in,out] dest  – zmienna, w której zapisana będzie liczba,
 * @param[in] number    – liczba, która jest przekonwertowana na napis,
 * @param[in] spacing   – długość stworzonego napisu.
 * @return Napis, w którym zapisana jest liczba.
 */
static char *number_to_string(char *dest, uint32_t number, int spacing) {
    if (number == 0) {
        sprintf(dest, "%*s", spacing, ".");
        return dest;
    }

    sprintf(dest, "%*u", spacing, number);
    return dest;
}

/** @brief Liczy długość rozstawu pojedyńczego pola na planszy.
 * Liczy jak dużo miejsca w reprezentacji planszy ma
 * zajmować pojedyńcze pole.
 * @param[in] g    – wskaźnik na strukturę przechowującą stan gry.
 * @return Długość rozstawu pojedyńczego pola na planszy.
 */
static int calculate_spacing(gamma_t *g) {
    int spacing = 1;
    if (g->number_of_players >= 10) {
        uint32_t number = g->number_of_players;
        while (number >= 10) {
            number /= 10;
            spacing++;
        }

        // Add 1 place for space between fields.
        spacing++;
    }

    return spacing;
}

char *gamma_board(gamma_t *g) {
    if (g == NULL) {
        return NULL;
    }

    int spacing = calculate_spacing(g);

    // Rozmiar planszy to rozmiar potrzebny na każde pole i
    // liczba znaków nowej linii.
    uint64_t board_size =
            ((uint64_t) g->width * (uint64_t) g->height * (uint64_t) spacing)
            + g->height;

    char *char_board = malloc((board_size + 1) * sizeof(char));
    if (char_board == NULL) {
        return NULL;
    }
    char *number = malloc((spacing + 1) * sizeof(char));
    if (number == NULL) {
        free(char_board);
        return NULL;
    }
    *char_board = 0;
    *number = 0;

    // Iteracja od końca wysokości, aby punkt (0, 0) był
    // wypisany w lewym dolnym rogu.
    for (uint32_t y = g->height - 1; y < g->height; y--) {
        for (uint32_t x = 0; x < g->width; x++) {
            number = number_to_string(number, g->board[x][y]->owner, spacing);
            strcat(char_board, number);
        }
        strcat(char_board, "\n");
    }
    free(number);
    return char_board;
}