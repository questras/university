#include "auth.h"
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>

#define TIMESTAMP_THRESHOLD 15

unsigned long max(unsigned long a, unsigned long b) {
    return a > b ? a : b;
}

unsigned long min(unsigned long a, unsigned long b) {
    return a > b ? b : a;
}

unsigned long get_timestamp_from_user() {
    unsigned long input;
    printf("Enter current timestamp: ");
    scanf("%lu", &input);

    return input;
}

bool timestamp_authenticate() {
    unsigned long user_timestamp = get_timestamp_from_user();
    unsigned long current_timestamp = (unsigned long) time(NULL);
    printf("Current timestamp is: %lu\n", current_timestamp);

    return (max(user_timestamp, current_timestamp) - min(user_timestamp, current_timestamp)) < TIMESTAMP_THRESHOLD;
}

static struct pam_conv login_conv = {
        misc_conv,
        NULL
};

bool login_employee() {
    pam_handle_t *pamh = NULL;
    int retval;
    char *username = NULL;

    retval = pam_start("login", username, &login_conv, &pamh);
    if (pamh == NULL || retval != PAM_SUCCESS) {
        fprintf(stderr, "Error when starting: %d\n", retval);
        return false;
    }

    retval = pam_authenticate(pamh, 0);
    if (retval != PAM_SUCCESS) {
        return false;
    }

    if (!timestamp_authenticate()) {
        return false;
    }
    pam_end(pamh, PAM_SUCCESS);
    return true;
}
