#include <iostream>
using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;

    if (n == 1)
    {
        int val;
        cin >> val;

        int k;
        cin >> k;
        for (int i = 0; i < k; i++)
        {
            int howMany;
            cin >> howMany;
            if (val % 2 == 1)
            {
                cout << val << endl;
            }
            else
            {
                cout << -1 << endl;
            }
        }
        return 0;
    }

    int t[n];
    int najmn_parz_suf[n];
    int najmn_nieparz_suf[n];
    int najw_parz_pref[n];
    int najw_nieparz_pref[n];

    for (int i = 0; i < n; i++)
    {
        cin >> t[i];
        najmn_parz_suf[i] = -1;
        najmn_nieparz_suf[i] = -1;
        najw_parz_pref[i] = -1;
        najw_nieparz_pref[i] = -1;
    }

    // sufiksy
    if (t[n - 1] % 2 == 1)
    {
        najmn_nieparz_suf[n - 1] = t[n - 1];
    }
    else
    {
        najmn_parz_suf[n - 1] = t[n - 1];
    }

    for (int i = n - 2; i >= 0; i--)
    {
        if (t[i] % 2 == 1)
        {
            najmn_nieparz_suf[i] = t[i];
            najmn_parz_suf[i] = najmn_parz_suf[i + 1];
        }
        else
        {
            najmn_parz_suf[i] = t[i];
            najmn_nieparz_suf[i] = najmn_nieparz_suf[i + 1];
        }
    }

    // prefiksy
    if (t[0] % 2 == 1)
    {
        najw_nieparz_pref[0] = t[0];
    }
    else
    {
        najw_parz_pref[0] = t[0];
    }

    for (int i = 1; i < n; i++)
    {
        if (t[i] % 2 == 1)
        {
            najw_nieparz_pref[i] = t[i];
            najw_parz_pref[i] = najw_parz_pref[i - 1];
        }
        else
        {
            najw_parz_pref[i] = t[i];
            najw_nieparz_pref[i] = najw_nieparz_pref[i - 1];
        }
    }

    long long int sufiksy[n];
    sufiksy[n - 1] = (long long int)t[n - 1];
    for (int i = n - 2; i >= 0; i--)
    {
        sufiksy[i] = sufiksy[i + 1] + (long long int)t[i];
    }

    int k;
    cin >> k;

    for (int ii = 0; ii < k; ii++)
    {
        int howMany;
        cin >> howMany;
        int idx = n - howMany;

        long long int result = sufiksy[idx];

        if (result % 2 == 1)
        {
            cout << result << endl;
        }
        else if (idx == 0)
        {
            cout << -1 << endl;
        }
        else
        {
            long long int final_result = -1;

            if (najmn_parz_suf[idx] != -1 && najw_nieparz_pref[idx - 1] != -1)
            {
                final_result = max(final_result, result - (long long int)najmn_parz_suf[idx] + (long long int)najw_nieparz_pref[idx - 1]);
            }

            if (najmn_nieparz_suf[idx] != -1 && najw_parz_pref[idx - 1] != -1)
            {
                final_result = max(final_result, result - (long long int)najmn_nieparz_suf[idx] + (long long int)najw_parz_pref[idx - 1]);
            }

            cout << final_result << endl;
        }
    }
}
