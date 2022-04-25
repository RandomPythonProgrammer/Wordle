#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <chrono>

using namespace std;
using namespace chrono;
using namespace filesystem;

const string GREEN = "\x1b[42m";
const string YELLOW = "\x1b[43m";
const string GREY = "\x1b[47m";
const string NORMAL = "\x1b[0m";
const string BLACK = "\x1b[30m";

unordered_map<char, int> get_chars(string word) {
    unordered_map<char, int> map;
    for (char & i : word) {
        if (map.contains(i)) {
            map[i]++;
        } else {
            map[i] = 1;
        }
    }
    return map;
}

bool in_dictionary(vector<string> dictionary, string input) {
    for (string word: dictionary) {
        if (input.compare(word) == 0) {
            return true;
        }
    }
    return false;
}

vector<string> get_words(int number) {
    vector<string> string_vector;
    fstream file;
    path filepath = absolute(path("dictionary.txt"));
    file.open(filepath, ios::in);
    string input;
    while (getline(file, input)) {
        if (input.size() == number) {
            transform(input.begin(), input.end(), input.begin(), ::tolower);
            string_vector.push_back(input);
        }
    }
    file.close();
    return string_vector;
}

void clear_screen() {
    cout << "\033[2J\033[1;1H";
}

int main(int argc, char *args[]) {
    vector<string> words = get_words(argc > 1 ? stoi(args[1]) : 5);
    srand(system_clock::now().time_since_epoch().count());
    string word = words[(long) ((((double) rand()) / RAND_MAX) * words.size())];

    bool playing = true;

    string input;
    vector<string> lines;
    while (playing) {
        clear_screen();
        for (string line: lines) {
            unordered_map<char, int> map = get_chars(word);
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == word[i]) {
                    cout << GREEN << BLACK << line[i];
                    map[line[i]]--;
                    if (map[line[i]] <= 0) {
                        map.erase(line[i]);
                    }
                } else {
                    if (map.contains(line[i])) {
                        cout << YELLOW << BLACK << line[i];
                        map[line[i]]--;
                        if (map[line[i]] <= 0) {
                            map.erase(line[i]);
                        }
                    } else {
                        cout << GREY << BLACK << line[i];
                    }
                }
            }
            cout << NORMAL << endl;
        }
        cin >> input;
        if (in_dictionary(words, input)) {
            lines.push_back(input);
        }
        if (word == input) {
            playing = false;
            cout << "WIN" << endl;
        } else if (lines.size() > 5) {
            playing = false;
            cout << "LOSE, IT WAS: " << word << endl;
        }
    }

    return 0;
}
