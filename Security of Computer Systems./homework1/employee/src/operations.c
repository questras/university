#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "operations.h"
#include "utils.h"
#include "files.h"

#define CREDIT_OPTION 1
#define DEPOSIT_OPTION 2

#define UPDATE_OPTION_SUM 1
#define UPDATE_OPTION_PERCENTAGE 2
#define UPDATE_OPTION_END 3

struct passwd *choose_client() {
    char *client = NULL;
    printf("Enter client login: ");
    read_word(&client);

    return getpwnam(client);
}

void proceed_choose_client(char **client) {
    struct passwd *pwd = choose_client();

    if (pwd == NULL) {
        (*client) = NULL;
        print_error();
        printf("No such client.\n");
    } else {
        (*client) = malloc(strlen(pwd->pw_name));
        strcpy(*client, pwd->pw_name);

        print_info();
        printf("Setting client as '%s'\n", *client);
    }
}

void proceed_show_credits_and_deposits(char *client) {
    if (client == NULL) {
        print_error();
        printf("No client is selected.\n");
        return;
    }

    struct passwd *pwd = getpwnam(client);
    if (pwd == NULL) {
        print_error();
        printf("Incorrect client.\n");
        return;
    }

    show_client_files(pwd);
}

void print_add_option_menu() {
    printf("%d. Credit\n", CREDIT_OPTION);
    printf("%d. Deposit\n", DEPOSIT_OPTION);
}

double input_sum() {
    double sum;

    printf("Enter sum in format <number> or <number>.<number>:\n");
    scanf("%lf", &sum);
    getchar(); // Remove endline.
    print_info();
    printf("Entered sum is: %lf\n", sum);

    return sum;
}

double input_percentage() {
    double percentage;

    printf("Enter percentage in format <number> or <number>.<number>:\n");
    scanf("%lf", &percentage);
    getchar(); // Remove endline.
    print_info();
    printf("Entered percentage is: %lf\n", percentage);

    return percentage;
}

void input_date(char **date) {
    printf("Enter date in format DD.MM.YYYY:\n");
    read_word(date);
    if (!is_date_valid(*date)) {
        print_error();
        printf("Invalid date: '%s'.\n", *date);
        (*date) = NULL;
    }
}

bool are_arguments_invalid(double sum, double percentage, const char *date) {
    return sum < 0 || percentage < 0 || date == NULL;
}

void proceed_add_credit_or_deposit() {
    print_add_option_menu();
    int input = input_choose_option();
    if (input != CREDIT_OPTION && input != DEPOSIT_OPTION) return;

    struct passwd *pw = choose_client();
    if (pw == NULL) {
        print_error();
        printf("No such client.\n");
        return;
    }

    double initial_sum = input_sum();
    double initial_percentage = input_percentage();
    char *start_date = NULL;
    input_date(&start_date);

    if (are_arguments_invalid(initial_sum, initial_percentage, start_date)) {
        print_error();
        printf("Invalid argument(s), operation interrupted.\n");
        return;
    }

    unsigned long id = next_id();
    bool success = add_new_file(pw, start_date, initial_sum, initial_percentage, id, input == CREDIT_OPTION);

    if (success) {
        print_info();
        printf("Added successfully!\n");
    }
    else {
        print_error();
        printf("Adding was unsuccessful.\n");
    }
}

void print_update_option_menu() {
    printf("%d. Credit\n", CREDIT_OPTION);
    printf("%d. Deposit\n", DEPOSIT_OPTION);
}

void print_update_menu() {
    printf("%d. Add new sum, date and percentage.\n", UPDATE_OPTION_SUM);
    printf("%d. Add new date and percentage.\n", UPDATE_OPTION_PERCENTAGE);
    printf("%d. Add new date (end deposit/credit).\n", UPDATE_OPTION_END);
}

bool proceed_update_sum(unsigned long id, bool is_credit) {
    char *date = NULL;
    input_date(&date);
    double sum = input_sum();
    double percentage = input_percentage();

    if (are_arguments_invalid(sum, percentage, date)) {
        print_error();
        printf("Invalid argument(s), operation interrupted.\n");
        return false;
    }

    return update_file_with_sum(id, date, sum, percentage, is_credit);
}

bool proceed_update_percentage(unsigned long id, bool is_credit) {
    char *date = NULL;
    input_date(&date);
    double percentage = input_percentage();

    if (are_arguments_invalid(0, percentage, date)) {
        print_error();
        printf("Invalid argument(s), operation interrupted.\n");
        return false;
    }

    return update_file_with_percentage(id, date, percentage, is_credit);
}

bool proceed_update_end(unsigned long id, bool is_credit) {
    char *date = NULL;
    input_date(&date);

    if (are_arguments_invalid(0, 0, date)) {
        print_error();
        printf("Invalid argument(s), operation interrupted.\n");
        return false;
    }

    return end_file(id, date, is_credit);
}

void proceed_update_credit_or_deposit() {
    print_update_option_menu();
    int input = input_choose_option();
    if (input != CREDIT_OPTION && input != DEPOSIT_OPTION) return;

    list_all_files(input == CREDIT_OPTION);
    unsigned long id = input_id();

    print_update_menu();
    int update_type_input = input_choose_option();

    bool success = false;
    switch (update_type_input) {
        case UPDATE_OPTION_SUM:
            success = proceed_update_sum(id, input == CREDIT_OPTION);
            break;
        case UPDATE_OPTION_PERCENTAGE:
            success = proceed_update_percentage(id, input == CREDIT_OPTION);
            break;
        case UPDATE_OPTION_END:
            success = proceed_update_end(id, input == CREDIT_OPTION);
            break;
        default:
            print_error();
            printf("Incorrect option.\n");
            return;
    }

    if (success) {
        print_info();
        printf("Updated successfully!\n");
    } else {
        print_error();
        printf("Could not update.\n");
    }
}
