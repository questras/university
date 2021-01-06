/*
 * Implementation of operationsParser.h
 * */

#include <stdbool.h>
#include <string.h>

#include "operationsParser.h"


// Return greater of integers [a] and [b].
int max(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}

bool isAsterisk(char *arg) {
    if (arg != NULL && strcmp(arg, "*") == 0) {
        return true;
    }
    else {
        return false;
    }
}


// 'Add' operation with one argument.
bool addOneArg(Tree *root, char *forest) {
    *root = insertElement(*root, forest);
    return true;
}

// 'Add' operation with two arguments.
bool addTwoArgs(Tree *root, char *forest, char *tree) {
    Tree tempForest = findElement(*root, forest);
    if (tempForest == NULL) {
        *root = insertElement(*root, forest);
        tempForest = findElement(*root, forest);
    }

    tempForest->value = insertElement(tempForest->value, tree);

    return true;
}

// 'Add' operation with three arguments.
bool addThreeArgs(Tree *root, char *forest, char *tree, char *animal) {
    Tree tempForest = findElement(*root, forest);
    if (tempForest == NULL) {
        *root = insertElement(*root, forest);
        tempForest = findElement(*root, forest);
    }

    Tree tempTree = findElement(tempForest->value, tree);
    if (tempTree == NULL) {
        tempForest->value = insertElement(tempForest->value, tree);
        tempTree = findElement(tempForest->value, tree);
    }

    tempTree->value = insertElement(tempTree->value, animal);

    return true;
}

bool addOperation(Tree *root, char *forest, char *tree, char *animal) {
    if (forest == NULL && tree == NULL && animal == NULL) {
        return false;
    }
    else if (tree == NULL && animal == NULL) {
        return addOneArg(root, forest);
    }
    else if (animal == NULL) {
        return addTwoArgs(root, forest, tree);
    }
    else if (forest != NULL && tree != NULL) {
        return addThreeArgs(root, forest, tree, animal);
    }
    else {
        return false;
    }
}


// 'Delete' operation with one argument.
bool deleteOneArg(Tree *root, char *forest) {
    *root = deleteElement(*root, forest);

    return true;
}

// 'Delete' operation with two arguments.
bool deleteTwoArgs(Tree *root, char *forest, char *tree) {
    Tree tempForest = findElement(*root, forest);
    if (tempForest == NULL) {
        return true;
    }

    tempForest->value = deleteElement(tempForest->value, tree);

    return true;
}

// 'Delete' operation with three arguments.
bool deleteThreeArgs(Tree *root, char *forest, char *tree, char *animal) {
    Tree tempForest = findElement(*root, forest);
    if (tempForest == NULL) {
        return true;
    }

    Tree tempTree = findElement(tempForest->value, tree);
    if (tempTree == NULL) {
        return true;
    }

    tempTree->value = deleteElement(tempTree->value, animal);

    return true;
}

bool deleteOperation(Tree *root, char *forest, char *tree, char *animal) {
    if (forest == NULL && tree == NULL && animal == NULL) {
        freeTree(*root);
        *root = NULL;

        return true;
    }
    else if (tree == NULL && animal == NULL) {
        return deleteOneArg(root, forest);
    }
    else if (animal == NULL) {
        return deleteTwoArgs(root, forest, tree);
    }
    else if (forest != NULL && tree != NULL) {
        return deleteThreeArgs(root, forest, tree, animal);
    }
    else {
        return false;
    }
}


// 'Print' operation with one argument.
bool printOneArg(Tree root, char *forest) {
    Tree tempForest = findElement(root, forest);

    if (tempForest != NULL) {
        printTree(tempForest->value);
    }

    return true;
}

// 'Print' operation with two arguments.
bool printTwoArgs(Tree root, char *forest, char *tree) {
    Tree tempForest = findElement(root, forest);
    if (tempForest != NULL) {
        Tree tempTree = findElement(tempForest->value, tree);
        if (tempTree != NULL) {
            printTree(tempTree->value);
        }

        return true;
    }

    return true;
}

bool printOperation(Tree root, char *forest, char *tree, char *animal) {
    if (animal != NULL) {
        return false;
    }
    else if (forest == NULL && tree == NULL) {
        // No argument given so print all forests in [root].
        printTree(root);
        return true;
    }
    else if (tree == NULL) {
        return printOneArg(root, forest);
    }
    else {
        return printTwoArgs(root, forest, tree);
    }
}


// Case: CHECK * tree
// Return 1 if found, -1 otherwise
int checkOneAsteriskOneArg(Tree root, char *tree) {
    if (root == NULL) {
        return -1;
    }

    Tree tempForest = findElement(root->value, tree);
    if (tempForest == NULL) {
        // CHECK * tree for left and right child node in [root]
        int a = checkOneAsteriskOneArg(root->left, tree);
        int b = checkOneAsteriskOneArg(root->right, tree);

        return max(a, b);
    }
    else {
        return 1;
    }
}

// Case: CHECK * * animal
// Return 1 if found, -1 otherwise
int checkTwoAsterisks(Tree root, char *animal) {
    if (root == NULL) {
        return -1;
    }

    // CHECK * Y for current forest in [root].
    int res = checkOneAsteriskOneArg(root->value, animal);

    if (res == -1) {
        // CHECK * * animal for left and right child node in [root].
        int a = checkTwoAsterisks(root->left, animal);
        int b = checkTwoAsterisks(root->right, animal);

        return max(a, b);
    }
    else {
        return 1;
    }
}

// Case: CHECK * tree animal
// Return 1 if found, -1 otherwise
int checkOneAsteriskTwoArgs(Tree root, char *tree, char *animal) {
    if (root == NULL) {
        return -1;
    }

    Tree tempTree = findElement(root->value, tree);
    if (tempTree == NULL) {
        int a = checkOneAsteriskTwoArgs(root->left, tree, animal);
        int b = checkOneAsteriskTwoArgs(root->right, tree, animal);

        return max(a, b);
    }
    else {
        Tree tempAnimal = findElement(tempTree->value, animal);

        if (tempAnimal == NULL) {
            int a = checkOneAsteriskTwoArgs(root->left, tree, animal);
            int b = checkOneAsteriskTwoArgs(root->right, tree, animal);

            return max(a, b);
        }
        else {
            return 1;
        }
    }
}

// 'Check' operation with asterisk (*) as argument.
int checkAsterisk(Tree root, char *forest, char *tree, char *animal) {
    if (forest == NULL) {
        return 0;
    }
    else if (tree == NULL) {
        // Error, because when tree is NULL then animal should be NULL,
        // so forest is '*', and last argument cannot be '*'.
        return 0;
    }
    else if (animal == NULL) {
        if (isAsterisk(tree)) {
            // Last argument is asterisk, not allowed
            return 0;
        }
        else {
            // case: CHECK * tree
            return checkOneAsteriskOneArg(root, tree);
        }
    }
    else {
        if (isAsterisk(animal)) {
            // Last argument is asterisk, not allowed
            return 0;
        }
        else if (isAsterisk(forest) && isAsterisk(tree)) {
            // case: CHECK * * animal
            return checkTwoAsterisks(root, animal);
        }
        else if (isAsterisk(forest)) {
            // case: CHECK * tree animal
            return checkOneAsteriskTwoArgs(root, tree, animal);
        }
        else if (isAsterisk(tree)) {
            // case: CHECK forest * animal
            Tree tempForest = findElement(root, forest);
            if (tempForest == NULL) {
                return -1;
            }
            else {
                // CHECK forest * animal is the same as
                // CHECK * animal called with [forest] as root
                return checkOneAsteriskOneArg(tempForest->value, animal);
            }
        }
        else {
            return 0;
        }
    }
}

// 'Check' operation with 1 not-null argument.
int checkOneArg(Tree root, char *forest) {
    if (findElement(root, forest) == NULL) {
        return -1;
    }
    else {
        return 1;
    }
}

// 'Check' operation with 2 not-null arguments.
int checkTwoArgs(Tree root, char *forest, char *tree) {
    Tree tempForest = findElement(root, forest);
    if (tempForest == NULL) {
        return -1;
    }
    else if (findElement(tempForest->value, tree) == NULL) {
        return -1;
    }
    else {
        return 1;
    }
}

// 'Check' operation with 3 not-null arguments.
int checkThreeArgs(Tree root, char *forest, char *tree, char *animal) {
    Tree tempForest = findElement(root, forest);
    if (tempForest == NULL) {
        return -1;
    }
    else {
        Tree tempTree = findElement(tempForest->value, tree);
        if (tempTree == NULL) {
            return -1;
        }
        else if (findElement(tempTree->value, animal) == NULL) {
            return -1;
        }
        else {
            return 1;
        }
    }
}

int checkOperation(Tree root, char *forest, char *tree, char *animal) {
    if (isAsterisk(forest) || isAsterisk(tree) || isAsterisk(animal)) {
        return checkAsterisk(root, forest, tree, animal);
    }
    else if (forest == NULL) {
        return 0;
    }
    else if (tree == NULL && animal == NULL) {
        return checkOneArg(root, forest);
    }
    else if (animal == NULL) {
        return checkTwoArgs(root, forest, tree);
    }
    else {
        return checkThreeArgs(root, forest, tree, animal);
    }
}