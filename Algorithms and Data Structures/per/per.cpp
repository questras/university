#include<iostream>
using namespace std;

const int N = 100000 + 42;

int n;
long long values[N];

long long prefixes2[N];
long long sufixes2[N];

int main() {
    ios_base::sync_with_stdio(false);
    cin>>n;
    for (int i = 1; i < n + 1; i++)
    {
        cin>>values[i];
    }

    if (n < 4) {
        long long result = 0;
        for (int i = 1; i < n+1; i++)
        {
            result += values[i];
        }
        cout<<result<<endl;
        return 0;
    }

    prefixes2[0] = 0;
    prefixes2[1] = values[1];
    prefixes2[2] = values[1] + values[2];

    for (int i = 3; i < n + 1; i++)
    {
        prefixes2[i] = max(values[i] + values[i-1] + prefixes2[i-3], values[i] + prefixes2[i-2]);
        prefixes2[i] = max(prefixes2[i], prefixes2[i-1]);
    }

    // for (int i = 0; i < n+1; i++)
    // {
    //     cout<<prefixes2[i]<<" ";
    // }
    // cout<<endl;

    sufixes2[0] = 0;
    sufixes2[n] = values[n];
    sufixes2[n-1] = values[n] + values[n-1];
    for (int i = n-2; i > 0; i--) {
        sufixes2[i] = max(values[i] + values[i+1] + sufixes2[i+3 > n ? 0 : i+3], values[i] + sufixes2[i + 2]);
        sufixes2[i] = max(sufixes2[i], sufixes2[i+1]);
    }
    // for (int i = 0; i < n+1; i++)
    // {
    //     cout<<sufixes2[i]<<" ";
    // }
    // cout<<endl;

    long long result = 0;
    for (int i = 3; i < n + 1; i++)
    {
        int trio = values[i-2] + values[i-1] + values[i];
        result = max(result, trio + prefixes2[max(0, i-4)] + sufixes2[i+2 > n ? 0 : i+2]);
    }

    result = max(result, prefixes2[n]);
    cout<<result<<endl;
    
    
    
}
