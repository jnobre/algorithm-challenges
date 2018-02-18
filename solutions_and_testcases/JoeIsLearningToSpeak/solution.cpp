#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
using namespace std;

bool is_delimiter(const char& c) {
    switch (c) {
        case '.': case ',': case ':': case ';': case '!': case '?':
            return true;
    }
    return false;
}

void split(const string& s, queue<string>& result) {
    string current = "";
    int size = s.size();
    for (int i = 0; i < size; ++i)
        if (is_delimiter(s[i])) {
            if (!current.empty())
                result.push(current);
            current = "";
            result.push(".");
        } else if (s[i] == '*') {
            if (!current.empty())
                result.push(current);
            current = "";
            result.push("*");
        } else if (s[i] == '#') {
            if (!current.empty())
                result.push(current);
            current = "";
            result.push("#");
        } else
            current += s[i];
    if (!current.empty())
        result.push(current);
}

bool extract_ngrams(const vector<string>& s, const int n,
                    vector<set<string> >& result) {
    int size = s.size();
    bool not_found = false;
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j <= size-i; ++j) {
            string current = s[j];
            for (int k = 1; k < i; ++k)
                current += " " + s[j+k];
            if (i > 1 && result[i].count(current) == 0)
                not_found = true;
            result[i].insert(current);
        }
    }
    return not_found;
}

int main(int argc, char *argv[]) {
    int n, c=1;
    while (cin >> n) {
        queue<string> tokens;
        // read all the tokens until '#'
        while (true) {
            string buffer;
            cin >> buffer;
            split(buffer, tokens);
            if (tokens.size() > 0 && tokens.back() == "#")
                break;
        }

        // process the previous knowledge
        vector<set<string> > ngrams(6);
        ngrams[1].insert("joe"); // joe knows his name
        while (true) {
            vector<string> sentence;
            string current = tokens.front();
            tokens.pop();

            while (current != "*" && current != ".") {
                transform(current.begin(), current.end(),
                          current.begin(), ::tolower);  // string to lower!
                sentence.push_back(current);
                current = tokens.front();
                tokens.pop();
            }

            if (!sentence.empty())
                extract_ngrams(sentence, n, ngrams);

            if (current == "*")
                break;
        }

        if (c > 1)
            cout << endl;
        cout << "Learning case " << c++ << endl;
        /* for (int i = 1; i <= n; ++i) {
            cerr << "size: " << i << endl;
            for (set<string>::iterator it = ngrams[i].begin();
                 it != ngrams[i].end();
                 ++it)
                cerr << *it << endl;
        } */

        // process the sentences
        while (true) {
            vector<string> sentence;
            string current = tokens.front();
            tokens.pop();

            while (current != "#" && current != ".") {
                sentence.push_back(current);
                current = tokens.front();
                tokens.pop();
            }

            if (!sentence.empty()) {
                int size = sentence.size();
                vector<string> original(size);
                for (int i = 0; i < size; ++i) {
                    original[i] = sentence[i];
                    transform(sentence[i].begin(), sentence[i].end(),
                              sentence[i].begin(), ::tolower);
                    if (ngrams[1].count(sentence[i]) == 0) {
                        cout << "What does the word \"" << original[i]
                        //cout << "What does \"" << original[i]
                             << "\" mean?" << endl;
                        ngrams[1].insert(sentence[i]);
                    }
                }
                if (extract_ngrams(sentence, n, ngrams)) {
                    cout << "What does the sentence \"" << original[0];
                    //cout << "What does \"" << original[0];
                    for (int i = 1; i < size; ++i)
                        cout << " " << original[i];
                    cout << "\" mean?" << endl;
                }
            }
            if (current == "#")
                break;
        }
    }
    return EXIT_SUCCESS;
}
