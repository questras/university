#include<iostream>
#include<algorithm>
using namespace std;

const int N = 200000 + 42;

struct project {
    int left, right, value;
};

using proj =  struct project;

int n;
proj projects[N];


bool comp(proj &a, proj &b) {
    return a.right < b.right;
}

int find(int val) {
    int beg, end, mid;
    beg = 0, end = n-1;
    int result = -1;
    while (beg <= end) {
        mid = (beg + end) / 2;

        if (projects[mid].right == val) {
            return mid;
        }

        if (projects[mid].right < val) {
            result = mid;
            beg = mid + 1;
        }
        else {
            end = mid - 1;
        }
    }

    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin>>n;
    for (int i = 0; i < n; i++)
    {
        int a, b, c;
        cin>>a>>b>>c;
        projects[i] = {a, b, c};
    }

    sort(projects, projects + n, comp);

    // for (int i = 0; i < n; i++)
    // {
    //     cout<<projects[i].left<<" "<<projects[i].right<<" "<<projects[i].value<<endl;
    // }
    
    long long bests[N];
    bests[0] = projects[0].value;
    // cout<<bests[0]<<endl;
    for (int i = 1; i < n; i++)
    {
        int f = find(projects[i].left - 1);
        long long toAdd = (f == -1) ? 0 : bests[f];
        bests[i] = max(bests[i-1], projects[i].value + toAdd);
        // cout<<bests[i]<<endl;
    }

    
    cout<<bests[n-1]<<endl;
}
