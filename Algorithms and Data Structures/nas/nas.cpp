#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
using namespace std;


int n, l;
vector<unordered_set<int>> sets;

int main() {
ios_base::sync_with_stdio(false);
    cin>>n>>l;

    for (int i = 0; i < l; i++)
    {
        sets.push_back(unordered_set<int>());
    }
    

    int a;
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < l; k++)
        {
            cin>>a;
            sets[k].emplace(a);
        }
    }

    // for (int i = 0; i < l; i++)
    // {
    //     for (auto x : sets[i]) {
    //         cout<<x<<" ";
    //     }
    //     cout<<endl;
    // }

    int result = 1;
    unordered_map<int, int> necklaces;

    for (auto x : sets[0]) {
        necklaces.insert({x,1});
    }

    int toDelete = -1;
    for (int i = 1; i < l; i++)
    {
        
        // cout<<"map\n";
        for (auto &x: necklaces) {
            if (toDelete != -1) {
                necklaces.erase(toDelete);
                toDelete = -1;
            }
            auto it = sets[i].find(x.first);
            // cout<<x.first<<" "<<x.second<<endl;

            if (it == sets[i].end()) {
                // cout<<x.first<<" not found in set."<<endl;
                result = max(result, x.second);
                // necklaces.erase(x.first);
                toDelete = x.first;
            }
            else {
                // x.second += 1;
                necklaces[x.first]++;
            }

        }
        if (toDelete != -1) {
            necklaces.erase(toDelete);
            toDelete = -1;
        }

        for(auto &x: sets[i]) {
            auto it2 = necklaces.find(x);
            if (it2 == necklaces.end()) {
                // cout<<x<<" added to map\n";
                necklaces.insert({x, 1});
            }
        }
    }

    
    
    for(auto &x : necklaces) {
        result = max(result, x.second);
    }

    cout<<result<<endl;
    
    
    
}
