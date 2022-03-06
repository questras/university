#ifndef EMPLOYEE_OPERATIONS_H
#define EMPLOYEE_OPERATIONS_H

// Perform operation to choose current client.
void proceed_choose_client(char **client);

// Perform operation to show credits and deposits of given client.
void proceed_show_credits_and_deposits(char *client);

// Perform operation to add credit or deposit.
void proceed_add_credit_or_deposit();

// Perform operation to update credit or deposit.
void proceed_update_credit_or_deposit();

#endif //EMPLOYEE_OPERATIONS_H
