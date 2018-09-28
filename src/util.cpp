#include <bits/stdc++.h>
#include <openssl/md5.h>

using namespace std;

extern const int values[] = {19, -1, -1, -1, -1, -1, -1, -1, 23, -1, -1, 11, -1, 17, 3, 13, -1, 29, 5, 2, 7, -1, 31, -1, 37, -1};

// generate_hash() generates a hash value for a unique word
long long generate_hash(string word) {
    long long ret = 1;
    for (int i = 0; i < word.size(); i++) {
        ret *= values[word[i]-'a'];
    }
    return ret;
}

// to_md5() converts a word into a md5 digest
string to_md5(string word) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    char tmp_word[18];
    strcpy(tmp_word, word.c_str());
    
    MD5((unsigned char*)&tmp_word, strlen(tmp_word), (unsigned char*)&digest);

    char mdString[33];

    for(int i = 0; i < 16; i++){
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    }

    string ans = mdString;
    return ans;
}

// complement() calculates the "complement" of s in a word
// assumes s can be written using only characters in word
//
// e.g.: complement of "bcf" in "abcdef" is "aed"
string complement(string s, string word) {
    int cnt[27]; memset(cnt, 0, sizeof(cnt));
    for(int i = 0; i < word.size(); i++)
        cnt[word[i]-'a']++;
    for(int i = 0; i < s.size(); i++)
        cnt[s[i]-'a']--;
    string ans = "";
    for(int i = 0; i < 26; i++)
        for(int j = 0; j < cnt[i]; j++)
            ans.push_back(i+'a');
    return ans;
}
