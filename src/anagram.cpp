#include <bits/stdc++.h>
#include <openssl/md5.h>
#include "util.cpp"

using namespace std;

unordered_map<long long, vector<string>> dic;
unordered_map<long long, int> comp;
vector<long long> keys;

const int MAX_WORDS = 5;
long long WORD_HASH;
bool finished;

string word, current_hash;

clock_t started, ended;

// solution_found() calculates the time spent until a solution was found
// also instructs the solve() function to stop working on current_hash
void solution_found(string &sentence) {
    cout << "solution found: " << sentence << " (" << current_hash << ")" << endl;
    ended = clock();
    double time_used = ((double) (ended - started)) / CLOCKS_PER_SEC;
    cout << time_used << "s" << endl;
    finished = true;
}

// check() tests different permutations of strings that are grouped
// Also verifies if their MD5 hash is the solution
void check(int *ids, vector<long long> &combination) {
    string sentence = "";
    for(int i = 0; i < combination.size(); i++)
        if (ids[i] >= dic[combination[i]].size()) {
            // id is bigger than the size of the vector
            // we should go back 
            return;
        } else {
            sentence += i?" ":"";
            sentence += dic[combination[i]][ids[i]];
        }
    string md5_hash = to_md5(sentence);
    if (md5_hash == current_hash) {
        solution_found(sentence);
    }
    else for(int i = 0; i < combination.size(); i++) {
        ids[i]++;
        check(ids, combination);
        ids[i]--;
    }
}

// test_sol() tests if a given candidate can be arranged into a solution
// Uses check() described above
void test_sol(long long &combination_hash, vector<long long> combination) {
    do {
        int ids[] = {0,0,0,0,0};
        check(ids, combination);
    } while(next_permutation(combination.begin(), combination.end()));
}

// solve() generates all possible k-combinations on the dictionary keys
// calls test_sol() when a combination is potential solution 
void solve(int idx, int k, long long cumulative, vector<long long> &combination) {
    if (finished) return; // if solution was found, no need to go deeper
    
    if (WORD_HASH%cumulative != 0) return;

    if (k == 0) { //k-combination. Verify if current combination is a candidate

        // cumulative is the product of every hash in this combination
        // comp[cumulative] is the hash of the "missing" word

        long long comp_hash = comp[cumulative];
        cumulative *= (long long)comp_hash;

        if (comp.find(comp_hash) == comp.end()) return;

        if (cumulative == WORD_HASH) {
            combination.push_back(comp_hash);
            test_sol(cumulative, combination);
            combination.pop_back();
        }
        return;
    }
    for(int i = idx; i <= keys.size() - k; i++) {
        combination.push_back(keys[i]);
        solve(i+1, k-1, cumulative*keys[i], combination);
        combination.pop_back();
    }
}

int main(int argc, char** argv) {

    // example word is hard-coded for simplicity
    word = "poultryoutwitsants";
    WORD_HASH = generate_hash(word);

    // generates dictionary
    string s;
    {
        while(cin >> s) {
            long long tmp = generate_hash(s);
            if (dic.find(tmp) == dic.end()) {
                dic[tmp] = vector<string>{s};
                comp[generate_hash(complement(s, word))] = tmp;
                keys.push_back(tmp);
            }
            else {
                dic[tmp].push_back(s);
            }
        }
    }
    
    // sort keys so combinations will always be sorted
    // (useful for next_permutation)
    sort(keys.begin(), keys.end());
 
    // tries to solve every md5 hash passed as argument
    for(int i = 1; i < argc; i++) {
        started = clock();
        finished = false;
        current_hash = argv[i];
        for(int j = 1; j < MAX_WORDS; j++) {
            vector<long long> v;
            solve(0,j,1,v);
        }
    }
    return 0;
}