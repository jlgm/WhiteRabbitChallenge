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

    char md_string[33];

    for(int i = 0; i < 16; i++){
         sprintf(&md_string[i*2], "%02x", (unsigned int)digest[i]);
    }

    string ans = md_string;
    return ans;
}

