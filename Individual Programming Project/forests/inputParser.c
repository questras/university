/*
 * Implementation of inputParser.h
 * */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "inputParser.h"
#include "operationsParser.h"
#include "BinarySearchTree.h"


// Return true if operation is one of operations:
// ADD_OPERATION, CHECK_OPERATION, DEL_OPERATION, PRINT_OPERATION
bool isValidOperation(char *operation) {
    if (strcmp(operation, ADD_OPERATION) == 0 ||
        strcmp(operation, CHECK_OPERATION) == 0 ||
        strcmp(operation, DEL_OPERATION) == 0 ||
        strcmp(operation, PRINT_OPERATION) == 0) {

        return true;
    }
    else {
        return false;
    }
}

// return false if any of characters in [name]
// has ASCII value below 33,
// return true otherwise.
bool isOkASCII(char *name) {
    unsigned long n = strlen(name);

    for (unsigned long i = 0; i < n; i++) {
        unsigned char c = name[i];

        if (c < 33) {
            return false;
        }
    }

    return true;
}

// Print "YES", "NO", "OK" or "ERROR" depending on [result] and [operation].
void printResult(int result, char *operation) {
    if (result == 1) {
        if (strcmp(operation, ADD_OPERATION) == 0 ||
            strcmp(operation, DEL_OPERATION) == 0) {

            printf("OK\n");
        }
        else if (strcmp(operation, CHECK_OPERATION) == 0) {
            printf("YES\n");
        }
    }
    else if (result == 0) {
        fprintf(stderr, "ERROR\n");
    }
    else if (result == -1) {
        if (strcmp(operation, CHECK_OPERATION) == 0) {
            printf("NO\n");
        }
    }
}

// Call specific operation depending on given operation [op] and
// arguments [arg1], [arg2], [arg3] and print its result.
void parseInput(Tree *root, char *op, char *arg1, char *arg2, char *arg3) {
    if (strcmp(op, CHECK_OPERATION) == 0) {
        int result = checkOperation(*root, arg1, arg2, arg3);
        printResult(result, CHECK_OPERATION);
    }
    else if (strcmp(op, ADD_OPERATION) == 0) {
        bool result = addOperation(root, arg1, arg2, arg3);
        printResult(result, ADD_OPERATION);
    }
    else if ((strcmp(op, DEL_OPERATION) == 0)) {
        bool result = deleteOperation(root, arg1, arg2, arg3);
        printResult(result, DEL_OPERATION);
    }
    else if ((strcmp(op, PRINT_OPERATION) == 0)) {
        bool result = printOperation(*root, arg1, arg2, arg3);
        printResult(result, PRINT_OPERATION);
    }
}

// Analyze input line, especially split it into operation and arguments.
// Call respective functions to carry out operation with arguments on [root].
void analyzeInput(Tree *root, char *input) {
    bool newLineAtEnd = true;

    if (input[0] == '#') {
        // Line with starting '#' is a comment and is ignored.
        return;
    }

    if (input[strlen(input) - 1] != '\n') {
        // There is no '\n' sign at the end of line.
        newLineAtEnd = false;
    }

    // Take operation from input line.
    char *operation = NULL;
    operation = strtok(input, DELIMITER);

    if (operation == NULL) {
        // Empty line is ignored.
        return;
    }
    else if (!newLineAtEnd) {
        // Correct line must end with \n, so error.
        fprintf(stderr, "ERROR\n");
        return;
    }

    if (isValidOperation(operation)) {
        // Split rest of input into arguments.
        char *firstArg = strtok(NULL, DELIMITER);
        char *secondArg = strtok(NULL, DELIMITER);
        char *thirdArg = strtok(NULL, DELIMITER);

        // Raise error if arguments do not have correct ASCII values.
        if ( (firstArg != NULL && !isOkASCII(firstArg)) ||
             (secondArg != NULL && !isOkASCII(secondArg)) ||
             (thirdArg != NULL && !isOkASCII(thirdArg)) ) {

            fprintf(stderr, "ERROR\n");
            return;
        }

        // If operation is correct, there is nothing left in input.
        if (strtok(NULL, DELIMITER) == NULL) {
            parseInput(root, operation, firstArg, secondArg, thirdArg);
        }
        else {
            // There is something left in input, so more than 3 arguments
            // were given, and it is incorrect.
            fprintf(stderr, "ERROR\n");
            return;
        }
    }
    else {
        fprintf(stderr, "ERROR\n");
        return;
    }
}