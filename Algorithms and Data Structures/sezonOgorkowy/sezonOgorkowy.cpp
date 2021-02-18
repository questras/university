// Michał Wiśniewski 418486
#include <iostream>
#include <cmath>
#include <climits>
using namespace std;


// przyklad
// 2 3 2 1 5 8
// 4 3 5 2 7 1
// 1 6 3 5 8 3


const int SIZE = 1000000 + 42;
const int LOG_SIZE = 30 + 10;

// sparse_table_min[i][j] to wartosc najmniejszego elementu
// przedzialu od i do i + 2^j
int sparse_table_min[SIZE][LOG_SIZE];
int sparse_table_max[SIZE][LOG_SIZE];
int t[SIZE];
int logs[SIZE];

int n, m, k, bigger, smaller;

int powers[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192,
                16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608,
                16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824};

int power(int n)
{
    return powers[n];
}

int get_min(int l, int p, int level)
{
    int j = logs[p - l + 1];

    int L = level * bigger + l;
    int P = level * bigger + p;
    P = P - power(j) + 1;

    return min(sparse_table_min[L][j], sparse_table_min[P][j]);
}

int get_max(int l, int p, int level)
{
    int j = logs[p - l + 1];

    int L = level * bigger + l;
    int P = level * bigger + p;
    P = P - power(j) + 1;

    return max(sparse_table_max[L][j], sparse_table_max[P][j]);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m >> k;
    int size = n * m;
    bigger = max(n, m);
    smaller = min(n, m);
    bool translation = n > m;

    for (int i = 0; i < size; i++)
    {
        cin >> t[i];
    }

    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < LOG_SIZE; k++)
        {
            sparse_table_min[i][k] = INT_MAX;
            sparse_table_max[i][k] = -1;
        }
    }

    logs[1] = 0;
    for (int i = 2; i < SIZE; i++)
    {
        logs[i] = logs[i / 2] + 1;
    }

    //  1 2
    //  3 4   --->  2 4 6
    //  5 6         1 3 5
    //
    // Wypelnienie tablicy dla dlugosci odcinka 1
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < m; k++)
        {
            int row, col;
            row = i;
            col = k;

            if (translation)
            {
                col = i;
                row = m - 1 - k;
            }

            sparse_table_min[row * bigger + col][0] = t[i * m + k];
            sparse_table_max[row * bigger + col][0] = t[i * m + k];
        }
    }

    int pow2 = 2;
    int idx = 1;

    while (pow2 <= bigger)
    {
        for (int i = 0; i < smaller; i++)
        {
            for (int k = 0; k < bigger - pow2 + 1; k++)
            {
                int sparse_index = i * bigger + k;
                sparse_table_min[sparse_index][idx] = min(sparse_table_min[sparse_index][idx - 1],
                                                          sparse_table_min[sparse_index + (pow2 / 2)][idx - 1]);

                sparse_table_max[sparse_index][idx] = max(sparse_table_max[sparse_index][idx - 1],
                                                          sparse_table_max[sparse_index + (pow2 / 2)][idx - 1]);
            }
        }
        pow2 *= 2;
        idx++;
    }

    int lrow, lcol, prow, pcol;
    int res_min, res_max;
    for (int i = 0; i < k; i++)
    {
        res_min = INT_MAX;
        res_max = -1;
        cin >> lrow >> lcol >> prow >> pcol;

        if (translation)
        {
            int pcol_temp = pcol;
            int lcol_temp = lcol;
            lcol = lrow;
            lrow = m - 1 - pcol_temp;
            pcol = prow;
            prow = m - 1 - lcol_temp;
        }

        for (int k = lrow; k <= prow; k++)
        {
            res_min = min(res_min, get_min(lcol, pcol, k));
            res_max = max(res_max, get_max(lcol, pcol, k));
        }
        cout << res_max - res_min << "\n";
    }
    // Złożoność czasowa: O(n*m*log(max(n, m)) + k*min(n, m))
    // Złożoność pamięciowa: O(n*m*log(max(n, m)))
}
