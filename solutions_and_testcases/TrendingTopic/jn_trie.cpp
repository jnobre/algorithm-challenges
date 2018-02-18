#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

// Compare for the sort function()
// Sorts .first in decreasing order and .second in increasing order.
bool comparator(const pair<int, const string*>& lhs, const pair<int, const string*>& rhs) {
    if (lhs.first == rhs.first)
       return *lhs.second < *rhs.second;
    return lhs.first > rhs.first;
}


const int MAXWORDS = 100000;
map<string, int> word_codes[30];
string words[MAXWORDS];
int counts[MAXWORDS];
int nwords = 0;

struct TrieNode {
    map<char, int> next;
    bool isFinal;
    int code;

    TrieNode() : isFinal(false) {}
};

vector<TrieNode> trie;

void trie_init(vector<TrieNode>& trie) {
    trie = vector<TrieNode>(1);
}

int trie_get_code(int node, const string& w, int pos) {
    if (pos == (int)w.size()) {
        if (!trie[node].isFinal) {
            trie[node].code = nwords;
            words[nwords] = w;
            //word_codes[w] = nwords;
            ++nwords;
            trie[node].isFinal = true;
        }
        return trie[node].code;
    }

    map<char,int>::iterator next_it = trie[node].next.find(w[pos]);
    if (next_it == trie[node].next.end()) {
        trie.push_back(TrieNode());
        int next = trie.size()-1;
        trie[node].next[w[pos]] = next;
        return trie_get_code(next, w, pos+1);
    }
    return trie_get_code(next_it->second, w, pos+1);
}

inline int get_code2(const string& w) {
    int c = w[0]-'a';
    map<string, int>::iterator r = word_codes[c].find(w);
    if (r == word_codes[c].end()) {
        int code = nwords++;
        words[code] = w.c_str();
        word_codes[c][w] = code;
        return code;
    } else
        return r->second;
}


inline int get_code(const string& w) {
    int code = trie_get_code(0, w, 0);
    return code;
}

int main(int argc, char *argv[]) {
    trie_init(trie);
    vector<int> days[7];
    int LAST = 7-1;

    while (true) {
        char action_char[100];
        if (scanf("%s", action_char) != 1)
            break;
        string action = action_char;
        
        if (action == "<text>") { // read a text
            for (vector<int>::iterator it = days[0].begin();
                 it != days[0].end();
                 ++it) {
                --counts[*it];
            }

            for (int i = 0; i < LAST; ++i)
                days[i].swap(days[i+1]);
            days[LAST].clear();

            while (true) {
                char word_char[100];
                if (scanf("%s", word_char) != 1)
                    return -1;
                if (word_char[0] == '<') // </text>
                    break;
                string word = word_char;
                if (word.size() < 4)
                    continue;
                int code = get_code(word);
                days[LAST].push_back(code);
                counts[code]++;
            }
        
        } else { // print trending topics
            int how_many;
            if (scanf("%d%*s", &how_many) != 1)
                return -1;
            printf("<top %d>\n", how_many);

            int max_cnt = 0;
            vector<int> cnt(1000, 0);
            for (int i = 0; i < nwords; ++i) {
                cnt[counts[i]]++;
                max_cnt = max(max_cnt, counts[i]);
            }

            int thres = max_cnt+1;
            while (thres > 0 && how_many > 0) {
                --thres;
                how_many -= cnt[thres];
            }
            thres = max(1, thres);

            // get all the words above the threshold
            vector<pair<int, const string*> > result;
            for (int i = 0; i < nwords; ++i)
                if (counts[i] >= thres)
                    result.push_back(pair<int, const string*>(counts[i], &words[i]));

            // prints the result in order
            sort(result.begin(), result.end(), comparator);
            for (vector<pair<int,const string*> >::iterator it = result.begin();
                 it != result.end();
                 ++it)
                    printf("%s %d\n", it->second->c_str(), it->first);
 
            printf("</top>\n");
        }

    } 

    return EXIT_SUCCESS;
}
