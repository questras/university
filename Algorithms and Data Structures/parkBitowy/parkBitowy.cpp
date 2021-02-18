#include<iostream>
using namespace std;


struct Node {
    int previous;
    int right;
    int left;

    int depth;
    int size;
    int rsize;
    int lsize;
};

Node nodes[500042]; // todo: change to 500042
int n, m;

void setup_depth(int root, int current_depth) {
    if (root == -1) {
        return;
    }

    nodes[root].depth = current_depth;
    setup_depth(nodes[root].left, current_depth + 1);
    setup_depth(nodes[root].right, current_depth + 1);
}

int setup_size(int root) {
    if (root == -1) {
        return -1;
    }

    nodes[root].rsize = setup_size(nodes[root].right) + 1;
    nodes[root].lsize = setup_size(nodes[root].left) + 1;
    nodes[root].size = max(nodes[root].rsize, nodes[root].lsize);

    return nodes[root].size;
}

bool search_up(int node, int sought) {
    int current_depth = 0;
    int prev = node;
    int curr = nodes[node].previous;

    while (curr != -1) {
        current_depth ++;
        if (nodes[curr].right != prev) {
            if (nodes[curr].rsize + current_depth >= sought) {
                // found!
                current_depth++;
                curr = nodes[curr].right;
                while (current_depth < sought) {
                    if (nodes[curr].lsize > nodes[curr].rsize) {
                        curr = nodes[curr].left;
                    }
                    else {
                        curr = nodes[curr].right;
                    }
                    current_depth ++;
                }
                cout<<curr<<endl;
                return true;
            }
        }
        else {
            if (nodes[curr].lsize + current_depth >= sought) {
                // found!
                current_depth++;
                curr = nodes[curr].left;
                while (current_depth < sought) {
                    if (nodes[curr].lsize > nodes[curr].rsize) {
                        curr = nodes[curr].left;
                    }
                    else {
                        curr = nodes[curr].right;
                    }
                    current_depth ++;
                }
                cout<<curr<<endl;
                return true;
            }
        }

        prev = curr;
        curr = nodes[prev].previous;
    }

    return false;
}

bool search_down(int node) {
    
}

int main() {
    ios_base::sync_with_stdio(false);

    cin>>n;
    nodes[1].previous = -1;
    for (int i = 1; i < n + 1; i++)
    {
        int a, b;
        cin>>a>>b;

        nodes[i].left = a;
        nodes[i].right = b;

        if (a != -1) {
            nodes[a].previous = i;
        }
            
        if (b != -1) {
            nodes[b].previous = i;
        }
            
    }

    setup_depth(1, 0);
    setup_size(1);

    cin>>m;
    for (int i = 0; i < m; i++)
    {
        int node, len;
        cin>>node>>len;

        if (nodes[node].depth >= len) {
            int current = node;
            while (len > 0) {
                len --;
                current = nodes[current].previous;
            }
            cout<<current<<endl;
        }
        else if (nodes[node].rsize >= len) {
            int current = nodes[node].right;
            len --;
            while (len > 0) {
                len --;
                if (nodes[current].lsize > nodes[current].rsize) {
                    current = nodes[current].left;
                }
                else {
                    current = nodes[current].right;
                }
            }
            cout<<current<<endl;
        }
        else if (nodes[node].lsize >= len) {
            int current = nodes[node].left;
            len --;
            while (len > 0) {
                len --;
                if (nodes[current].lsize > nodes[current].rsize) {
                    current = nodes[current].left;
                }
                else {
                    current = nodes[current].right;
                }
            }
            cout<<current<<endl;
        }
        else {
            bool res = search_up(node, len);
            if (!res) {
                cout<<-1<<endl;
            }
        }
    }
}
