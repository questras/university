#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>
using namespace std;

vector<bool> participants_to_matches[40005];

int main()
{
    ios_base::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;

    int player;
    for (int i = 0; i < m; i++)
    {
        for (int k = 0; k < n / 2; k++)
        {
            cin >> player;
            participants_to_matches[player - 1].push_back(true);
        }

        for (int k = 0; k < n / 2; k++)
        {
            cin >> player;
            participants_to_matches[player - 1].push_back(false);
        }
    }

    unordered_set<vector<bool>> s;

    for (int i = 0; i < n; i++)
    {
        if (s.count(participants_to_matches[i]) == 1)
        {
            cout << "NIE" << endl;

            return 0;
        }
        else
        {
            s.insert(participants_to_matches[i]);
        }
    }
    cout << "TAK" << endl;
}
