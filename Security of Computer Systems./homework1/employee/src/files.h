#ifndef EMPLOYEE_FILES_H
#define EMPLOYEE_FILES_H

#include <pwd.h>
#include <stdbool.h>

// Create new file representing credit or deposit with given data.
// Return true if operation was successful, false otherwise.
bool add_new_file(struct passwd *client, char *date, double sum, double percentage, unsigned long id, bool is_credit);

// Update with given data a file with given id representing credit ot deposit - version with specified sum.
// Return true if operation was successful, false otherwise.
bool update_file_with_sum(unsigned long id, char *date, double sum, double percentage, bool is_credit);

// Update with given data a file with given id representing credit ot deposit - version without specified sum.
// Return true if operation was successful, false otherwise.
bool update_file_with_percentage(unsigned long id, char *date, double percentage, bool is_credit);

// End credit or deposit represented by given id by adding date at the end of the file.
// Return true if operation was successful, false otherwise.
bool end_file(unsigned long id, char *date, bool is_credit);

// Print all files representing credits or deposits.
void list_all_files(bool is_credit);

// Print content of all deposits and credits represented by files owned by given client.
void show_client_files(struct passwd *client);

#endif //EMPLOYEE_FILES_H
