#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <ctime>
#include <unordered_map>

std::string GREEN = "\x1b[42m";
std::string YELLOW = "\x1b[43m";
std::string GREY = "\x1b[47m";
std::string NORMAL = "\x1b[0m";
std::string BLACK = "\x1b[30m";

std::unordered_map<char, int> get_chars(std::string word) {
    std::unordered_map<char, int> map;
    for (int i = 0; i < word.size(); i++){
        if (map.contains(word[i])){
            map[word[i]] ++;
        } else {
            map[word[i]] = 1;
        }
    }
    return map;
}

bool in_dictionary(std::vector<std::string> dictionary, std::string input){
    for (std::string word: dictionary){
        if (input.compare(word) == 0){
            return true;
        }
    }
    return false;
}

std::vector<std::string> get_words(int number){
    std::vector<std::string> string_vector;
    std::fstream file;
    std::filesystem::path path = std::filesystem::absolute(std::filesystem::path("resources") /
            std::filesystem::path("words"+ std::to_string(number) +".txt"));
    std::cout<<path<<std::endl;
    file.open(path, std::ios::in);
    std::string input;
    while (getline(file, input)){
        string_vector.push_back(input);
    }
    file.close();
    return string_vector;
}

void clear_screen(){
    std::cout<<"\033[2J\033[1;1H";
}

int main(int argc, char* args[]) {
    std::vector<std::string> words = get_words(argc > 1 ? std::stoi(args[1]) : 5);
    srand(std::time(nullptr)*std::time(nullptr));
    std::string word = words[(((double) rand())/ ((double) RAND_MAX)) * words.size()];

    bool playing = true;

    std::string input;
    std::vector<std::string> lines;
    while (playing){
        clear_screen();
        for (std::string line: lines){
            std::unordered_map<char, int> map = get_chars(word);
            for (int i = 0; i < line.size(); i ++){
                if (line[i] == word[i]){
                    std::cout<<GREEN<<BLACK<<line[i];
                    map[line[i]] --;
                    if (map[line[i]] <= 0){
                        map.erase(line[i]);
                    }
                } else {
                    if (map.contains(line[i])){
                        std::cout<<YELLOW<<BLACK<<line[i];
                        map[line[i]] --;
                        if (map[line[i]] <= 0){
                            map.erase(line[i]);
                        }
                    } else {
                        std::cout<<GREY<<BLACK<<line[i];
                    }
                }
            }
            std::cout<<NORMAL<<std::endl;
        }
        std::cin>>input;
        if (in_dictionary(words, input)){
            lines.push_back(input);
        }
        if (word.compare(input) == 0){
            playing = false;
            std::cout<<"WIN"<<std::endl;
        } else if (lines.size() > 5){
            playing = false;
            std::cout<<"LOSE, IT WAS: "<<word<<std::endl;
        }
    }

    return 0;
}
