#include<iostream>
#include<set>
#include<vector>
using namespace std;

const int N = 200000 + 42;

int n;
int fac_types[N];

typedef struct Node {
    vector<int> neighbours;
    int typ;
    int best_path[11];

} node_t;

node_t facs[N];

void calculate(int current, int previous) {
    for (auto neigh : facs[current].neighbours) {
        if (neigh != previous) {
            calculate(neigh, current);
        }
    }

    for (int t = 1; t < 11; t++)
    {
        for (auto neigh : facs[current].neighbours) {
            if (neigh != previous) {
                if (facs[neigh].typ == t || facs[neigh].best_path[t] != 0) {
                    facs[current].best_path[t] = max(facs[current].best_path[t], facs[neigh].best_path[t]+1);
                }
            }
        }
    }
}

int calculate_result(int current, int previous) {
    int res = 0;
    for (auto neigh : facs[current].neighbours) {
        if (neigh != previous) {
            res = max(calculate_result(neigh, current), res);
        }
    }

    for (int t = 1; t < 11; t++)
    {
        int max1=0, max2=0;
        for (auto neigh : facs[current].neighbours) {
            if (neigh != previous) {
                if (facs[neigh].typ == t || facs[neigh].best_path[t] != 0) {
                    if (facs[neigh].best_path[t] + 1 > max1) {
                        max2 = max1;
                        max1 = facs[neigh].best_path[t] + 1;
                    }
                    else if (facs[neigh].best_path[t] + 1 > max2) {
                        max2 = facs[neigh].best_path[t] + 1;
                    }
                }
            }
        }
        if (max2 != 0) {
            res = max(res, max1+max2);
        }
        else if (max2 == 0 && max1 != 0 && facs[current].typ == t) {
            res = max(res, max1);
        }
    }

    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin>>n;
    for (int i = 0; i < n; i++)
    {
        int t;
        cin>>t;
        facs[i].typ =  t;
        for (int k = 0; k < 11; k++)
        {
            facs[i].best_path[k] = 0;
        }
    }

    for (int i = 0; i < n - 1; i++)
    {
        int a, b;
        cin>>a>>b;
        // decrement to match indexes
        a--;
        b--;

        facs[a].neighbours.push_back(b);
        facs[b].neighbours.push_back(a);
    }

    calculate(0, -1);
    int result = calculate_result(0, -1);
    cout<<result<<"\n";
}
