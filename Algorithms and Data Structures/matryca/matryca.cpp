#include<iostream>
#include<climits>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    string input;
    cin>>input;

    char last_letter;

    int i = 0;
    while (i < input.length() && input[i] == '*')
    {
        i++;
    }
    
    if (i == input.length()) {
        cout<<1<<endl;
        return 0;
    }

    last_letter = input[i];
    i++;
    int result = INT_MAX;
    int current = 0;

    bool flag = false;
    while (i < input.length())
    {
        if (input[i] == '*') {
            current++;
        }
        else if (input[i] == last_letter) {
            current = 0;
        }
        else {
            flag = true;
            result = min(result, current);
            last_letter = input[i];
            current = 0;
        }
        i++;
    }

    if (flag) {
        cout<<input.length() - result<<endl;
    }
    else {
        cout<<1<<endl;
    }
    
}
