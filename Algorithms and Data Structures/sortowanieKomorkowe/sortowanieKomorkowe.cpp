#include <iostream>
using namespace std;

int last_left[1005][1005];
int last_right[1005][1005];
int t[1005];
int mod = 1e9;

int main()
{
    ios_base::sync_with_stdio(false);

    int n;
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        cin >> t[i];
    }

    for (int i = 0; i < n; i++)
    {
        last_left[0][i] = 1;
        last_right[0][i] = 0;
    }

    for (int dl = 1; dl < n; dl++)
    {
        for (int szer = 0; szer < n - dl; szer++)
        {
            int ile = 0;
            // left
            if (t[szer] < t[szer + 1])
            {
                ile += last_left[dl - 1][szer + 1];
            }
            if (t[szer] < t[szer + dl])
            {
                ile += last_right[dl - 1][szer + 1];
            }
            last_left[dl][szer] = ile % mod;

            ile = 0;
            // right
            if (t[szer + dl] > t[szer])
            {
                ile += last_left[dl - 1][szer];
            }
            if (t[szer + dl] > t[szer + dl - 1])
            {
                ile += last_right[dl - 1][szer];
            }
            last_right[dl][szer] = ile % mod;
        }
    }

    cout << (last_left[n - 1][0] + last_right[n - 1][0]) % mod << endl;
}