/*
 * Module with interface of input analyzer.
 * */

#ifndef FORESTS_INPUTPARSER_H
#define FORESTS_INPUTPARSER_H

#include "BinarySearchTree.h"

#define ADD_OPERATION "ADD"
#define DEL_OPERATION "DEL"
#define CHECK_OPERATION "CHECK"
#define PRINT_OPERATION "PRINT"
#define DELIMITER " \t\v\f\r\n"


/*
 * Analyze input line, especially split it into operation and arguments.
 * Call respective functions to carry out operation with arguments on [root].
 * Acceptable operations with <arguments> are:
 *   ADD <forest>
 *   ADD <forest> <tree>
 *   ADD <forest> <tree> <animal>
 *   DEL
 *   DEL <forest>
 *   DEL <forest> <tree>
 *   DEL <forest> <tree> <animal>
 *   PRINT
 *   PRINT <forest>
 *   PRINT <forest> <tree>
 *   CHECK <forest>
 *   CHECK <forest> <tree>
 *   CHECK <forest> <tree> <animal>
 *
 * Line with only whitespaces is ignored.
 * Line starting with '#' is ignored.
 *
 * "OK" is printed if ADD or DEL operation was done correctly.
 * "ERROR" is printed to stderr if line contains incorrect input.
 * "YES" or "NO" is printed for CHECK operation.
 *
 * */
void analyzeInput(Tree *root, char *input);

#endif //FORESTS_INPUTPARSER_H
