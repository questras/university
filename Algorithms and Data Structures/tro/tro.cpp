#include<iostream>
using namespace std;

//1
//2 1+1
//3 1+2
//4 1+3, 2+2
//5 1+4, 2+3
//6 1+5, 2+4, 3+3
//7 1+6, 2+5, 3+4
//8 1+7, 2+6, 3+5, 4+4
//9 1+8, 2+7, 3+6, 4+5
//10 1+9, 2+8, 3+7, 4+6, 5+5 

const int N = 500000 + 42;

int n;
int values[N];
long long prefixes[11][N];


long long handle(int numb, int indeks_of_n) {
    if (numb <= 1) {
        return 0;
    }

    long long res = 0;
    for (int i = 1; i <= numb / 2; i++)
    {
        int second = numb - i;
        if (second == i) {
            res += (1+(prefixes[i][indeks_of_n]-1)) * (prefixes[i][indeks_of_n]-1) / 2; //prefixes[i][indeks_of_n] * (prefixes[second][indeks_of_n]-1); 
        }
        else {
            res += prefixes[i][indeks_of_n] * prefixes[second][indeks_of_n]; 
            // indeks_of_n because prefixes are normal indexes + 1, normally it would be indeks_of_n-1
        }
    }

    return res;
    
}



int main() {
    ios_base::sync_with_stdio(false);
    cin>>n;

    for (int i = 0; i < n; i++)
    {
        cin>>values[i];
        // cout<<values[i]<<" ";
    }
    // cout<<endl;

    for (int i = 0; i < 11; i++)
    {
        prefixes[i][0] = 0;
    }
    

    for (int i = 1; i < n + 1; i++)
    {
        for (int k = 1; k < 11; k++)
        {
            prefixes[k][i] = prefixes[k][i-1];
            if (values[i-1] == k) {
                prefixes[k][i]++;
            }   
        }
    }

    // for (int i = 1; i < 11; i++)
    // {
    //     cout<<i<<": ";
    //     for (int k = 1; k < n + 1; k++)
    //     {
    //         cout<<prefixes[i][k]<<" ";
    //     }
    //     cout<<endl;
    // }

    long long res = 0;
    for (int i = n-1; i >= 0; i--)
    {
        res += handle(values[i], i);
    }
    cout<<res<<"\n";

}
