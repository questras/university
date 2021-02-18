#include <iostream>
using namespace std;

int n, k;
const int N = 20042;
const int maxTreeSize = 65537;
int t[N];
long long int tree[maxTreeSize];
int prev_invs[N];
int curr_invs[N];
int mod = 1e9;

int get_tree_size()
{
    int current = 2;
    while (current < n)
    {
        current *= 2;
    }

    return current * 2;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n >> k;
    for (int i = 0; i < n; i++)
    {
        cin >> t[i];
    }

    int treeSize = get_tree_size();

    for (int i = 0; i < n; i++)
    {
        prev_invs[i] = 1;
    }

    for (int i = 2; i < k + 1; i++)
    {
        for (int k = 0; k < treeSize + 1; k++)
        {
            tree[k] = 0;
        }

        for (int k = 0; k < n; k++)
        {
            int currentLeft = 1;
            int currentRight = (treeSize) / 2;
            int currentIndex = 1;
            long long int right_sum = 0;

            while (currentIndex * 2 + 1 < treeSize + 1)
            {
                int half = currentLeft + (currentRight - currentLeft) / 2;
                if (t[k] <= half)
                {
                    // To the left subtree
                    currentIndex *= 2;
                    currentRight = half;
                    right_sum = (right_sum + tree[currentIndex + 1]) % mod;
                }
                else
                {
                    // To the right subtree
                    currentIndex = currentIndex * 2 + 1;
                    currentLeft = half + 1;
                }
            }
            curr_invs[k] = right_sum;

            long long int curr_to_add = prev_invs[k];
            while (currentIndex >= 1)
            {
                tree[currentIndex] += curr_to_add;
                currentIndex /= 2;
            }
        }

        for (int k = 0; k < n; k++)
        {
            prev_invs[k] = curr_invs[k];
        }
    }

    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum = (sum + curr_invs[i]) % mod;
    }
    cout << sum << endl;
}
