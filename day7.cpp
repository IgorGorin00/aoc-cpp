#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <variant>
#include <unordered_map>
#include <map>
#include <bits/stdc++.h>
#include <vector>

template <typename T>
void print_vector(const std::vector<T>& vec) {
    for (const auto& e : vec) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

std::vector<std::string> str_split(std::string& s, char del) {
    std::vector<std::string> words_vec;
    std::stringstream ss(s);
    std::string word;
    while (std::getline(ss, word, del)) {
        if (!word.empty()){
            words_vec.push_back(word);
        }
    }
    return words_vec;
}

std::string str_join(std::vector<std::string>& vec, char delim) {
    std::string file_path;
    if (vec[0] == std::string(1, '/')) {
        file_path += std::string(1, '/');
        for (size_t i = 1; i < vec.size(); i++) {
            file_path += vec[i] + delim;
        }
    } else {
        file_path += std::string(1, '/');
        for (const auto& word : vec) {
            file_path += word + delim;
        }
    }
    return file_path;
}

int main() {
    std::fstream input_file;
    input_file.open("day7_input.txt", std::ios::in);
    std::map<std::string, int> file_structure;
    std::vector<std::string> file_path;
    std::vector<int> dir_sizes;
    if (input_file.is_open()) {
        std::string s;
        while (std::getline(input_file, s)) {
            std::istringstream ss(s);
            std::string word;
            std::vector<std::string> words;
            while (ss >> word) {
                words.push_back(word);
            }
            // try converting first word to int and adding to
            // size of current dir, if fail - do nothing
            try {
                dir_sizes[dir_sizes.size() - 1] += std::stoi(words[0]);
            } catch (std::invalid_argument) {}

            if (words[0] == "$" && words[1] == "cd" && words.size() >= 3) {
                // if the input line is smth like `$ cd NAME`,
                // 1) add the NAME to the current directory path
                // 2) initialize size of dir to 0
                if (words[2] != "..") {
                    file_path.push_back(words[2]);
                    dir_sizes.push_back(0);
                } 
                // if the input line is `$ cd ..`,
                // 1) add the last element of directory path 
                //    and last size to map of file structure
                // 2) add the size of current dir to partent dir
                // 3) delete current dir and current file size from vector
                else {
                    std::string file_path_str = str_join(file_path, '/');
                    file_structure[file_path_str] = dir_sizes[dir_sizes.size() - 1];
                    dir_sizes[dir_sizes.size() - 2] += dir_sizes[dir_sizes.size() - 1];
                    file_path.pop_back();
                    dir_sizes.pop_back();
                }
            }
        }
        input_file.close();
    } else {
        std::cout << "Input open failed" << std::endl;
    }
//    for (int i = dir_sizes.size() - 1; i >= 0; i--) {
//        std::vector<std::string> file_path_slice(file_path.begin(), file_path.begin() + i + 1);
//        std::string file_path_str = str_join(file_path_slice, '/');
//        file_structure[file_path_str] = dir_sizes[i];
//    }

    if (dir_sizes.size() == 2) {
        file_structure[file_path[0] + file_path[1] + '/'] = dir_sizes[1];
        dir_sizes[0] += dir_sizes[1];
        file_structure[file_path[0]] = dir_sizes[0];
    }

    int result = 0;
    for (const auto& pair : file_structure) {
        std::cout << pair.first << ": " << pair.second << std::endl;
        if (pair.second <= 100000) {
            result += pair.second;
        }
    }
    std::cout << std::endl << "Result:" << std::endl;
    std::cout << result << std::endl;

    return 0;
}
