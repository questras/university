/*
 * Module with interface of Binary Search Tree(BST)
 * */

#ifndef FORESTS_BINARYSEARCHTREE_H
#define FORESTS_BINARYSEARCHTREE_H

typedef struct Node* Tree;

struct Node {
    char *name;
    struct Node *left, *right;
    struct Node *value;
};

// Return new empty Node with name=[name] and value=NULL.
Tree newNode(char *name);

// Print names of [root] and its child-nodes in lexicographic
// order: left child-node first, then [root], then right child-node.
void printTree(Tree root);

// Return Node in [root] with name=[name]
// or NULL if such Node does not exist.
Tree findElement(Tree root, char *name);

// Return [root] with added empty Node with name=[name].
Tree insertElement(Tree root, char *name);

// Return [root] with deleted Node with name=[name],
// or just [root] if Node with name=[name] is not found in [root].
Tree deleteElement(Tree root, char *name);

// Free [root] and call itself on
// [value], [left] and [right] of [root].
void freeTree(Tree root);

#endif //FORESTS_BINARYSEARCHTREE_H
