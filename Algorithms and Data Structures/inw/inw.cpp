#include<iostream>
#include<vector>
#include<map>
#include<stack>
#include<algorithm>
using namespace std;

using numb_t = int;

const int N = 1000000 + 42;

int n;

numb_t vertices[N];


bool cmp(pair<numb_t, numb_t> &p1, pair<numb_t, numb_t> &p2) {
    if (p1.second == p2.second) {
        return p1.first < p2.first;
    }

    return p1.second < p2.second;
}


numb_t fu[N];
numb_t ranks[N];
stack<numb_t> st;

numb_t find(numb_t key) {
    if (fu[key] == key) {
        return key;
    }

    fu[key] = find(fu[key]);
    return fu[key];
}

void union_(numb_t key1, numb_t key2) {
    numb_t parent_key1 = find(key1);
    numb_t parent_key2 = find(key2);

    if (parent_key1 == parent_key2) {
        return;
    }

    numb_t r1 = ranks[parent_key1];
    numb_t r2 = ranks[parent_key2];

    if (r1 < r2) {
        fu[parent_key1] = parent_key2;
    }
    else if (r1 > r2) {
        fu[parent_key2] = parent_key1;
    }
    else {
        fu[parent_key1] = parent_key2;
        ranks[parent_key2] = r2 + 1;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin>>n;
    for (int i = 0; i < n; i++)
    {
        cin>>vertices[i];
        fu[vertices[i]] = vertices[i];
        ranks[vertices[i]] = 0;
    }

    for (int i = 0; i < n; i++) {
        if (st.empty()) {
            st.push(vertices[i]);
        }
        else {
            numb_t biggest = st.top();
            numb_t current = biggest;
            if (vertices[i] < current) {
                st.pop();
                union_(current, vertices[i]);

                while (!st.empty()) {
                    current = st.top();

                    if (vertices[i] < current) {
                        st.pop();
                        union_(vertices[i], current);
                    }
                    else {
                        break;
                    }

                    biggest = max(biggest, current);
                }

                st.push(biggest);
            }
            else {
                st.push(vertices[i]);
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        find(vertices[i]);
    }
    

    vector<pair<numb_t, numb_t>> sorted;
    for (int i = 0; i < n; i++)
    {
        sorted.push_back({vertices[i], fu[vertices[i]]});
    }
    sort(sorted.begin(), sorted.end(), cmp);

    int distinct = 1;
    for (int i = 1; i < n; i++)
    {
        if (sorted[i].second != sorted[i-1].second) {
            distinct++;
        }
    }

    cout<<distinct<<"\n";

    
    int first = 0, last = 0;
    for (int i = 1; i < n; i++)
    {
        if (sorted[i].second != sorted[i-1].second) {
            cout<<last-first + 1<<" ";
            for (int k = first; k < last + 1; k++)
            {
                cout<<sorted[k].first<<" ";
            }
            cout<<"\n";
            first = i;
        }
        last = i;
    }
    cout<<last-first + 1<<" ";
    for (int k = first; k < last + 1; k++)
    {
        cout<<sorted[k].first<<" ";
    }
    cout<<"\n";
}
