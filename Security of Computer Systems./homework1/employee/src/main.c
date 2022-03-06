#include <stdio.h>

#include "utils.h"
#include "auth.h"
#include "definitions.h"
#include "operations.h"

void print_current_client(char *client) {
    if (client == NULL) {
        printf("*** No client chosen.\n");
    } else {
        printf("*** Current client is: %s\n", client);
    }
}

void print_menu() {
    printf("1. Choose client, on whom operations will be performed\n");
    printf("2. Show credits and deposits\n");
    printf("3. Add credit or deposit\n");
    printf("4. Edit credit or deposit\n");
}

int main() {
    // Login to app.
    bool success = DEBUG ? true : false;

    while (!success) {
        success = login_employee();

        if (success) {
            printf("Login successful.\n");
        } else {
            printf("Login failed.\n");
        }
    }

    // App loop.
    int option;
    char *client = NULL;

    for (;;) {
        print_current_client(client);
        print_menu();
        option = input_choose_option();

        switch (option) {
            case 1:
                proceed_choose_client(&client);
                break;
            case 2:
                proceed_show_credits_and_deposits(client);
                break;
            case 3:
                proceed_add_credit_or_deposit();
                break;
            case 4:
                proceed_update_credit_or_deposit();
                break;
            default:
                break;
        }
    }
}
