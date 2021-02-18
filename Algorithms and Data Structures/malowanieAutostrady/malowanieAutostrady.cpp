#include <iostream>
using namespace std;

struct Node
{
    int leftInv;
    int rightInv;
    int qt;
    char type; // 'n', 'b', 'w'
    Node *leftSubtree;
    Node *rightSubtree;
};

int n, m;
pair<int, int> tree_size;
int nodes_number;
int leafs_number;
int height;
Node *tree;

// gets number of nodes and height;
pair<int, int> get_tree_size(int data_size)
{
    int res = 1;
    int height = 0;

    while (data_size > res)
    {
        res *= 2;
        height++;
    }

    return make_pair(res * 2, height);
}


int get_divider(int left, int right)
{
    return 1 + left + (right - left - 1) / 2;
}

Node *build_tree(int left, int right)
{
    Node *node = new Node();
    node->leftInv = left;
    node->rightInv = right;
    node->qt = 0;
    node->type = 'n';
    node->leftSubtree = nullptr;
    node->rightSubtree = nullptr;

    if (left != right)
    {
        int divider = get_divider(left, right);
        Node *ltree = build_tree(left, divider - 1);
        Node *rtree = build_tree(divider, right);
        node->leftSubtree = ltree;
        node->rightSubtree = rtree;
    }

    return node;
}

int solve_left(Node *t, int left, char type)
{
    if (t->leftInv == t->rightInv)
    {
        if (type == 'b')
        {
            t->qt = 0;
            t->type = 'b';
            return 0;
        }

        t->qt = 1;
        t->type = 'w';
        return 1;
    }

    if (t->type != 'n') {
        t->leftSubtree->type = t->type;
        t->leftSubtree->qt = (t->type == 'w') * (t->leftSubtree->rightInv -
                                                    t->leftSubtree->leftInv + 1);
        t->rightSubtree->type = t->type;
        t->rightSubtree->qt = (t->type == 'w') * (t->rightSubtree->rightInv -
                                                    t->rightSubtree->leftInv + 1);
    }

    t->type = 'n';
    int divider = get_divider(t->leftInv, t->rightInv);
    if (left < divider)
    {
        t->rightSubtree->type = type;
        t->rightSubtree->qt = (type == 'w') * (t->rightSubtree->rightInv -
                                               t->rightSubtree->leftInv + 1);
        t->qt = solve_left(t->leftSubtree, left, type) + t->rightSubtree->qt;
    }
    else
    {
        t->qt = t->leftSubtree->qt + solve_left(t->rightSubtree, left, type);
    }

    return t->qt;
}

int solve_right(Node *t, int right, char type)
{
    if (t->leftInv == t->rightInv)
    {
        if (type == 'b')
        {
            t->qt = 0;
            t->type = 'b';
            return 0;
        }

        t->qt = 1;
        t->type = 'w';
        return 1;
    }

    if (t->type != 'n') {
        t->leftSubtree->type = t->type;
        t->leftSubtree->qt = (t->type == 'w') * (t->leftSubtree->rightInv -
                                                    t->leftSubtree->leftInv + 1);
        t->rightSubtree->type = t->type;
        t->rightSubtree->qt = (t->type == 'w') * (t->rightSubtree->rightInv -
                                                    t->rightSubtree->leftInv + 1);
    }

    t->type = 'n';
    int divider = get_divider(t->leftInv, t->rightInv);
    if (right >= divider)
    {
        t->leftSubtree->type = type;
        t->leftSubtree->qt = (type == 'w') * (t->leftSubtree->rightInv -
                                              t->leftSubtree->leftInv + 1);
        t->qt = solve_right(t->rightSubtree, right, type) + t->leftSubtree->qt;
    }
    else
    {
        t->qt = t->rightSubtree->qt + solve_right(t->leftSubtree, right, type);
    }

    return t->qt;
}

int solve(Node *t, int left, int right, char type)
{
    if (left == t->leftInv && right == t->rightInv)
    {
        t->type = type;
        t->qt = (type == 'w') * (right - left + 1);
    }
    else if (t->type == 'n')
    {
        int divider = get_divider(t->leftInv, t->rightInv);

        // Both to left subtree
        if (left < divider && right < divider)
        {
            t->qt = t->rightSubtree->qt + solve(t->leftSubtree, left, right, type);
        }
        // Both to right subtree
        else if (left >= divider && right >= divider)
        {
            t->qt = t->leftSubtree->qt + solve(t->rightSubtree, left, right, type);
        }
        // Split
        else
        {
            t->qt = solve_left(t->leftSubtree, left, type) +
                    solve_right(t->rightSubtree, right, type);
        }
    }
    else
    {
        if (t->type == type)
        {
            return t->qt;
        }
        else
        {
            t->leftSubtree->type = t->type;
            t->leftSubtree->qt = (t->type == 'w') * (t->leftSubtree->rightInv - 
                                                    t->leftSubtree->leftInv + 1);
            t->rightSubtree->type = t->type;
            t->rightSubtree->qt = (t->type == 'w') * (t->rightSubtree->rightInv - 
                                                      t->rightSubtree->leftInv + 1);
            t->type = 'n';
            
            t->qt = solve(t, left, right, type);
        }
    }

    return t->qt;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n >> m;
    tree_size = get_tree_size(n);
    nodes_number = tree_size.first;
    height = tree_size.second;
    leafs_number = nodes_number / 2;
    tree = build_tree(1, leafs_number);
    for (int i = 0; i < m; i++)
    {
        int a, b;
        char t;

        cin >> a >> b >> t;
        if (t == 'C')
        {
            t = 'b';
        }
        else
        {
            t = 'w';
        }
        int sol = solve(tree, a, b, t);
        cout << sol << '\n';
    }

    return 0;
}
