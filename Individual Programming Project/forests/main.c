#define  _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BinarySearchTree.h"
#include "inputParser.h"


int main() {
    Tree root = NULL;
    char *input = NULL;
    size_t size = 0;

    // Read all lines from standard input.
    while (getline(&input, &size, stdin) != EOF) {
        analyzeInput(&root, input);
    }

    // Free all allocated memory.
    free(input);
    freeTree(root);
    
    return 0;
}
