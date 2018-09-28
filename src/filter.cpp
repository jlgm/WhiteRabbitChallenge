#include <bits/stdc++.h>

using namespace std;

string t = "poultryoutwitsants";
string s;

map<string, bool> mark;

bool inT(char c) {
    for(int i = 0; i < t.size(); i++)
        if (t[i] == c) return true;
    return false;
}

int count(char c) {
    int ret =0;
    for(int i = 0; i < t.size(); i++)
        ret += (t[i]==c);
    return ret;
}

bool can(string s) {
    if (s.size() > t.size()) return false;
    for(int i = 0; i < s.size(); i++) {
        if (!inT(s[i])) return false;
        int cnt = 1;
        for(int j = 0; j < s.size(); j++) {
            if (j == i) continue;
            if (s[j] == s[i]) cnt++;
        }
        if (cnt > count(s[i])) return false;
    }
    return true;
}

int main() {

    while(cin >> s)
        if (can(s)) {
            if (!mark[s]) {
                cout << s << endl;
            }
            mark[s] = true;
        }

    return 0;
}
