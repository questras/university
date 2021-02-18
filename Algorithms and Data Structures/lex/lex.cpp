#include<iostream>
#include<algorithm>
#include<map>
#include<vector>
using namespace std;

const int N = 300000 + 42;
const int LOGMAX = 30;
int powers[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288};
int logs[N];
int n, m;
string s;

// pair<int, int> tymczasowa[N];
// pair<int, int> tymczasowa_sorted[N];

int nazwa[N][LOGMAX];

int power(int i) {
    return powers[i];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin>>n>>m;
    cin>>s;
    int l;
    for (int i = 0; i < n; i++)
    {
        l = s[i];
        nazwa[i][0] = l-96; 
    }

    logs[0] = 0;
    logs[1] = 0;
    for (int i = 2; i < N; i++)
    {
        logs[i] = logs[i / 2] + 1;
    }


    
    int k = 1, akt;
    while (power(k) <= n) {
        int rozmiar = n - power(k) + 1;
        vector<pair<int, int>> tymczasowa;
        vector<pair<int, int>> tymczasowa_sorted;
        for (int i = 0; i < rozmiar; i++)
        {
            tymczasowa.push_back({nazwa[i][k-1], nazwa[i + power(k-1)][k-1]});
            tymczasowa_sorted.push_back(tymczasowa.at(i));
        }

        sort(tymczasowa_sorted.begin(), tymczasowa_sorted.end());
        
        
        map<pair<int, int>, int> m;
        akt = 1;
        
        m[tymczasowa_sorted[0]] = akt;
                

        for (int i = 1; i < rozmiar; i++)
        {
            if (tymczasowa_sorted[i] != tymczasowa_sorted[i-1]) {
                akt++;
                m[tymczasowa_sorted[i]] = akt;
            }
        }


        for (int i = 0; i < rozmiar; i++)
        {
            nazwa[i][k] = m[tymczasowa[i]];
        }

        k++;
    }

    

    int a,b,c,d,len1, len2,log1, log2;
    for (int i = 0; i < m; i++)
    {
        cin>>a>>b>>c>>d;
        len1 = b-a+1;
        len2 = d-c+1;
        int diff = len2 - len1;
        
        if (diff > 0) {
            d -= diff;
        }
        else if (diff < 0) {
            b += diff;
        }

        log1 = logs[b-a+1];
        log2 = logs[d-c+1];

        pair<int, int> p1 = make_pair(nazwa[a-1][log1], nazwa[b-power(log1)][log1]);
        pair<int, int> p2 = make_pair(nazwa[c-1][log2], nazwa[d-power(log2)][log2]);

        if (p1 < p2) {
            cout<<"<\n";
        }
        else if (p2 < p1) {
            cout<<">\n";
        }
        else {
            if (len2 > len1) {
                cout<<"<\n";
            }
            else if (len1 > len2) {
                cout<<">\n";
            }
            else {
                cout<<"=\n";
            }
        }
    }
}
