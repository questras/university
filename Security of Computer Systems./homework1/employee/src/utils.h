#ifndef EMPLOYEE_UTILS_H
#define EMPLOYEE_UTILS_H

#include <stdbool.h>
#include <time.h>

// Read word from line removing the endline sign.
void read_word(char **word);

// Read integer option inserted by user.
int input_choose_option();

// Read id represented as unsigned long from user.
unsigned long input_id();

bool is_correct_day(char left, char right);

bool is_correct_month(char left, char right);

// Year presented as "abcd".
bool is_correct_year(char a, char b, char c, char d);

// Format dd.mm.YYYY is correct.
bool is_date_valid(char *date);

// Get next value of id.
unsigned long next_id();

void print_info();

void print_error();

#endif //EMPLOYEE_UTILS_H
