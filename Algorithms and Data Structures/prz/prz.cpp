#include<iostream>
#include<unordered_set>
#include<set>
using namespace std;

const int N = 1000000 + 42;
const int M = 200000 + 42;

int n, m;

int furthestfromLeft, furthestFromRight;
set<pair<int, int>> intervals;
multiset<int> lefts;
multiset<int> rights;


int main() {
    ios_base::sync_with_stdio(false);
    cin>>n>>m;

    int left = 1, right = n;
    
    lefts.insert(0);
    rights.insert(n+1);

    for (int i = 0; i < m; i++)
    {
        char sign;
        int a, b;
        cin>>sign>>a>>b;

        if (sign == '+') {
            if (intervals.count({a, b}) == 0) {
                if (a == 1) {
                    lefts.insert(b);
                }
                else {
                    rights.insert(a);
                }
                
                intervals.insert({a, b});
            }
        }
        else {
            if (intervals.count({a, b}) > 0) {
                if (a == 1) {
                    lefts.erase(b);
                }
                else if (b == n) {
                    rights.erase(a);
                }

                intervals.erase({a, b});
            }
        }
        cout<<max(*(rights.begin()) - (*(lefts.rbegin()) + 1), 0)<<"\n";
    }
}
