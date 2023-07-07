#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <boost/lexical_cast.hpp>


void update_big_ass_string(std::string& big_ass_string, int cycle_n, int& val_of_X) {
    
    while (cycle_n > 39) {
        cycle_n -= 40;
    }

    if (val_of_X - cycle_n >= 2 || val_of_X - cycle_n <= -2) {
        big_ass_string += '.';
    } else {
        big_ass_string += '#';
    }

}


std::vector<std::string> split_string(std::string& s, int& substr_len) {
    if (s.size() != 240) { 
        std::cout << "String size is not 240" << std::endl;
        exit(1);
    }
    std::vector<std::string> result;
    
    for (int i = 0; i < s.size(); i += substr_len) {
        result.push_back(s.substr(i, substr_len));
    }

    return result;
}


int main() {
    std::fstream input_file;
    input_file.open("day10_input.txt", std::ios::in);
    if (input_file.is_open()) {
        std::string s;

        int val_of_X = 1;
        int cycle_n = 0;
        int cycle_strength = 0;
        std::string big_ass_string = "";

        while (std::getline(input_file, s)) {
            if (s == "noop") {
                update_big_ass_string(big_ass_string, cycle_n, val_of_X);
                cycle_n++;
            } else {
                std::istringstream ss(s);
                std::string word;
                std::vector<std::string> words;
                while (ss >> word) {
                    words.push_back(word);
                }
                if (words[0] != "addx") {
                    std::cout << "something wrong" << std::endl;
                    exit(1);
                } else {
                    int n = 0;
                    try {
                        n = boost::lexical_cast<int>(words[1]);
                    } catch(...) {
                        std::cout << "Int conversion failed" << std::endl;
                        exit(1);
                    }
                    update_big_ass_string(big_ass_string, cycle_n, val_of_X);
                    cycle_n++;
                    update_big_ass_string(big_ass_string, cycle_n, val_of_X);
                    cycle_n++;
                    val_of_X += n;
                }

            }
        }
        int substr_len = 40;
        std::vector<std::string> strings_to_display = split_string(big_ass_string, substr_len);
        for (auto& str_to_display : strings_to_display) {
            std::cout << str_to_display << std::endl;
        }



        input_file.close();
    } else {
        std::cout << "Input open failed" << std::endl;
        exit(1);
    }

    return 0;
}

