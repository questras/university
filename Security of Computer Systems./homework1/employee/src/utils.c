#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

void read_word(char **word) {
    size_t size = 0;

    ssize_t chars = getline(word, &size, stdin);
    if ((*word)[chars - 1] == '\n') {
        (*word)[chars - 1] = '\0';
    }
}

int input_choose_option() {
    printf("Choose option: ");

    int input;
    scanf("%d", &input);
    getchar(); // Remove newline.

    return input;
}

bool is_correct_day(char left, char right) {
    return (left == '0' && (right >= 49 && right <= 57)) ||
           ((left == '1' || left == '2') && left >= 48 && right <= 57) ||
           (left == '3' && (right == '0' || right == '1'));
}

bool is_correct_month(char left, char right) {
    return (left == '1' && (right == '0' || right == '1' || right == '2')) ||
           (left == '0' && right >= 49 && right <= 57);
}

bool is_correct_year(char a, char b, char c, char d) {
    return (a == '1' || a == '2') &&
           (b <= 57 && b >= 48) &&
           (c <= 57 && c >= 48) &&
           (d <= 57 && d >= 48);
}

bool is_date_valid(char *date) {
    return strlen(date) == 10 &&
           is_correct_day(date[0], date[1]) &&
           date[2] == '.' &&
           is_correct_month(date[3], date[4]) &&
           date[5] == '.' &&
           is_correct_year(date[6], date[7], date[8], date[9]);
}

unsigned long next_id() {
    return (unsigned long) time(NULL);
}

void print_info() {
    printf("[INFO] ");
}

void print_error() {
    printf("[ERROR] ");
}

unsigned long input_id() {
    printf("Enter id: ");

    unsigned long input;
    scanf("%lu", &input);
    getchar(); // Remove newline.

    print_info();
    printf("Entered id is: %lu\n", input);

    return input;
}
