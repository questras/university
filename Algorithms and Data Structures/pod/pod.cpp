#include<iostream>
using namespace std;

const int N = 1000000 + 42;
int n;
int values[N];
long long ones[N], twos[N], threes[N], oneTwos[N];

int main() {
    ios_base::sync_with_stdio(false);
    cin>>n;

    ones[0] = 0;
    twos[0] = 0;
    threes[0] = 0;

    oneTwos[0] = 0;
    oneTwos[1] = 0;
    for (int i = 0; i < n; i++)
    {
        cin>>values[i];

        ones[i+1] = ones[i] + (values[i] == 1);
        twos[i+1] = twos[i] + (values[i] == 2);
        threes[i+1] = threes[i] + (values[i] == 3);
    }

    for (int i = 2; i < n + 1; i++)
    {
        oneTwos[i] = oneTwos[i-1];
        if (values[i-1] == 2) {
            oneTwos[i] += ones[i-1];
        }
    }
    

    // for (int i = 1; i < n+1; i++)
    // {
    //     cout<<ones[i]<<" ";
    // }
    // cout<<endl;
    
    // for (int i = 1; i < n+1; i++)
    // {
    //     cout<<twos[i]<<" ";
    // }
    // cout<<endl;

    // for (int i = 1; i < n+1; i++)
    // {
    //     cout<<threes[i]<<" ";
    // }
    // cout<<endl;

    long long result = n;
    for (int i = 1; i < n; i++)
    {
        if (values[i] == 2) {
            result += ones[i];
        }
        else if (values[i] == 3) {
            result += twos[i] + ones[i];
            result += oneTwos[i];
        }
    }
    
    cout<<result<<endl;    
}
