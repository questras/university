#include<iostream>
#include<vector>
using namespace std;

int max_val = 100000000;
int values[100000 + 42];
int depend_value[100000 + 42];
vector<int> dependencies[100000 + 42];
int n, m, k;

void count_dependency(int idx) {
    if (depend_value[idx] == -1) {
        for (int i = 0; i < dependencies[idx].size(); i++)
        {
            count_dependency(dependencies[idx][i]);
            depend_value[idx] = max(depend_value[idx], depend_value[dependencies[idx][i]]);
        }
        depend_value[idx] = max(depend_value[idx], values[idx]);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin>>n>>m>>k;

    for (int i = 0; i < n; i++)
    {
        cin>>values[i];
        depend_value[i] = -1;
    }

    int a, b;
    for (int i = 0; i < m; i++)
    {
        cin>>a>>b;
        dependencies[a-1].push_back(b-1);
    }

    for (int i = 0; i < n; i++)
    {
        count_dependency(i);
    }
    
    // for (int i = 0; i < n; i++)
    // {

    //     cout<<depend_value[i]<<endl;
    // }

    int beg = 1, end = max_val, result = max_val, mid, curr;

    while (beg <= end)
    {
        mid = (beg+end) / 2;
        curr = 0;

        for (int i = 0; i < n; i++)
        {
            if (depend_value[i] <= mid) {
                curr++;
            }
        }

        if (curr >= k) {
            result = mid;
            end = mid - 1;
        }
        else {
            beg = mid + 1;
        }
    }

    cout<<result<<"\n";
}
