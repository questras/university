/*
 * Module with interface of operation parser.
 * Operation parser parses operations and arguments and
 * carries out respective operations on
 * binary search trees (BST).
 *
 * Parameters in all functions:
 *   [root]: binary search tree(BST) with forests
 *   [forest]: name of forest
 *   [tree]: name of tree
 *   [animal]: name of animal
 *
 * In all comments:
 *   forest is Node with name=[forest]
 *   tree is Node with name=[tree]
 *   animal is Node with name=[animal]
 *
 * */

#ifndef FORESTS_OPERATIONSPARSER_H
#define FORESTS_OPERATIONSPARSER_H

#include "stdbool.h"
#include "BinarySearchTree.h"


/*
 * Add forest to [root] if it does not exist.
 * Add tree to forest if it does not exist.
 * Add animal to tree if it does not exist.
 *
 * If any of [forest], [tree], [animal] is NULL, ignore this argument,
 * if all are NULL, operation is incorrect.
 * If one of arguments is NULL, arguments after it should be NULL.
 * Otherwise operation is incorrect.
 *
 * returns:
 *   [true]: if operation is correct
 *   [false]: otherwise
 * */
bool addOperation(Tree *root, char *forest, char *tree, char *animal);


/*
 * Delete animal from tree in forest in [root]
 * or delete tree and its value from forest in [root] if [animal] is NULL
 * or delete forest and its value from [root] if [tree] is NULL
 * or delete every forest in [root] if all arguments are NULL.
 *
 * If one of arguments is NULL, arguments after it should be NULL.
 * Otherwise operation is incorrect.
 *
 * If one of forest, tree or animal does not exist, then nothing is
 * deleted, but operation is correct.
 *
 * returns:
 *   [true]: if operation is correct
 *   [false]: otherwise
 * */
bool deleteOperation(Tree *root, char *forest, char *tree, char *animal);


/*
 * Print all animals in tree if [tree] is not NULL,
 * or print all trees in forest if [forest] is not NULL,
 * or print all forests in [root] if all arguments are NULL.
 * Operation is incorrect if animal is not NULL.
 *
 * All names are printed in lexicographic order.
 *
 * If one of arguments is NULL, arguments after it should be NULL.
 * Otherwise operation is incorrect.
 *
 * If one of forest, tree or animal does not exist, then nothing is
 * printed, but operation is correct.
 *
 * returns:
 *   [true]: if operation is correct
 *   [false]: otherwise
 * */
bool printOperation(Tree root, char *forest, char *tree, char *animal);


/*
 * Check if animal exists in tree in forest,
 * or check if tree exists in forest if [animal] is NULL,
 * or check if forest exists in [root] if [tree] is NULL.
 *
 * Asterisk (*) is a valid argument, which means, that
 * search is made in all forests/trees respectively.
 * Asterisk cannot be the last not-null argument.
 *
 * If one of arguments is NULL, arguments after it should be NULL.
 * Otherwise operation is incorrect.
 * Operation is incorrect if all arguments are NULL.
 *
 * ----------------------
 * returns:
 *   1: searched argument is found
 *   0: operation is incorrect
 *   -1: searched argument is not found
 * */
int checkOperation(Tree root, char *forest, char *tree, char *animal);

#endif //FORESTS_OPERATIONSPARSER_H
