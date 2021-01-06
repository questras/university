/*
 * Implementation of Binary Search Tree (BST).
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BinarySearchTree.h"


// Return new string with value in [string].
char *stringDuplicate(const char *string) {
    char *duplicatedString = malloc(strlen(string) + 1);

    if (duplicatedString == NULL) {
        // Unable to allocate memory.
        exit(1);
    }

    strcpy(duplicatedString, string);

    return duplicatedString;
}

// Return new empty Node with name=[name] and value=NULL.
Tree newNode(char *name) {
    Tree temp = calloc(1, sizeof(struct Node));

    if (temp == NULL) {
        // Unable to allocate memory.
        exit(1);
    }

    temp->name = stringDuplicate(name);
    temp->left = NULL;
    temp->right = NULL;
    temp->value = NULL;

    return temp;
}

// Print names of [root] and its child-nodes in lexicographic
// order: left child-node first, then [root], then right child-node.
void printTree(Tree root) {
    if (root == NULL) {
        return;
    }
    else {
        // Printing in lexicographic order so print
        // left subtree first, then node, then right subtree.
        printTree(root->left);
        printf("%s\n", root->name);
        printTree(root->right);
    }
}

// Return Node in [root] with name=[name]
// or NULL if such Node does not exist.
Tree findElement(Tree root, char *name) {
    if (root == NULL) {
        return NULL;
    }
    else if (strcmp(root->name, name) < 0) {
        // Name in [root] is smaller lexicographically so search
        // in its right subtree.
        return findElement(root->right, name);
    }
    else if (strcmp(root->name, name) > 0) {
        // Name in [root] is greater lexicographically so search
        // in its left subtree.
        return findElement(root->left, name);
    }
    else {
        return root;
    }
}

// Return [root] with added empty Node with name=[name].
Tree insertElement(Tree root, char *name) {
    if (root == NULL) {
        return newNode(name);
    }
    else if (strcmp(root->name, name) < 0) {
        // Name in [root] is smaller lexicographically so search
        // in its right subtree.
        root->right = insertElement(root->right, name);
    }
    else if (strcmp(root->name, name) > 0) {
        // Name in [root] is greater lexicographically so search
        // in its left subtree.
        root->left = insertElement(root->left, name);
    }

    return root;
}

// Return Node with smallest name(i.e left-most Node)
// in [root].
Tree smallestNameNode(Tree root) {
    if (root == NULL) {
        return root;
    }

    Tree leftMostNode = root;
    // Find left-most Node
    while (leftMostNode->left != NULL) {
        leftMostNode = leftMostNode->left;
    }

    return leftMostNode;
}

// Return [root] with deleted Node with name=[name],
// or just [root] if Node with name=[name] is not found in [root].
Tree deleteElement(Tree root, char *name) {
    if (root == NULL) {
        // Node with name=[name] not found
        return root;
    }

    if (strcmp(name, root->name) < 0) {
        // If searched name is smaller lexicographically,
        // search for node in left subtree
        root->left = deleteElement(root->left, name);

        return root;
    }
    else if (strcmp(name, root->name) > 0) {
        // If searched name is greater lexicographically,
        // search for node in right subtree
        root->right = deleteElement(root->right, name);

        return root;
    }
    else {
        // Node with name=[name] found.

        if (root->left == NULL) {
            // There is no left subtree so free Node
            // and return its right subtree.

            Tree tempTree = root->right;

            freeTree(root->value);
            free(root->name);
            free(root);

            return tempTree;
        }
        else if (root->right == NULL) {
            // There is no right subtree so free Node
            // and return its left subtree.

            Tree tempTree = root->left;

            freeTree(root->value);
            free(root->name);
            free(root);

            return tempTree;
        }
        else {
            // Node with both subtrees

            // Get Node with smallest name to the right of current Node
            Tree tempTree = smallestNameNode(root->right);

            // Set root to Node with smallest name to the right
            // and delete this Node.
            freeTree(root->value);
            free(root->name);

            root->name = stringDuplicate(tempTree->name);
            root->value = tempTree->value;
            tempTree->value = NULL;

            root->right = deleteElement(root->right, tempTree->name);

            return root;
        }
    }
}

// Free [root] and call itself on
// [value], [left] and [right] of [root].
void freeTree(Tree root) {
    if (root == NULL) {
        return;
    }

    // free subtrees and tree in [value]
    freeTree(root->left);
    freeTree(root->right);
    freeTree(root->value);

    free(root->name);
    free(root);
}