#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <boost/lexical_cast.hpp>

void check_cycle(int& cycle_n, int& val_of_X, int& cycle_strength) {
    const std::vector<int> cycles_to_check{20, 60, 100, 140, 180, 220};
    for (auto& n : cycles_to_check) {
        if (cycle_n == n) {
            cycle_strength += n * val_of_X;
        }
    }
}


int main() {
    std::fstream input_file;
    input_file.open("day10_input.txt", std::ios::in);
    if (input_file.is_open()) {
        std::string s;

        int val_of_X = 1;
        int cycle_n = 0;
        int cycle_strength = 0;

        while (std::getline(input_file, s)) {
            if (s == "noop") {
                cycle_n++;
                check_cycle(cycle_n, val_of_X, cycle_strength);
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
                    cycle_n++;
                    check_cycle(cycle_n, val_of_X, cycle_strength);
                    cycle_n++;
                    check_cycle(cycle_n, val_of_X, cycle_strength);
                    val_of_X += n;
                }

            }
        }
    
        std::cout << "Result:" << std::endl << cycle_strength << std::endl;



        input_file.close();
    } else {
        std::cout << "Input open failed" << std::endl;
        exit(1);
    }

    return 0;
}
