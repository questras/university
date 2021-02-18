#include<iostream>
using namespace std;

const int N = 100000 + 42;

int n;
int from[N], to[N], weights[N];

int main() {
    ios_base::sync_with_stdio(false);

    cin>>n;
    for (int i = 0; i < n; i++)
    {
        cin>>from[i]>>to[i]>>weights[i];
    }
    if (n%2 == 1) {
        cout<<"-1\n";
    }

    
    

}
