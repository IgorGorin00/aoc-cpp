#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <boost/lexical_cast.hpp>

template<typename T>
void print_vector(const std::vector<T>& vec) {
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::fstream input_file;
    input_file.open("day5_input.txt", std::ios::in);


    std::vector<std::vector<char>> stacks;
    stacks.resize(9);
    if (input_file.is_open()) {
        std::string s;
        while (std::getline(input_file, s)) {
            if (s.rfind('[', 0) == 0) {
                std::cout << s << std::endl;
                std::vector<char> to_push{s[1], s[5], s[9], s[13], s[17], s[21], s[25], s[29], s[33]};
                for (int i = 0; i < to_push.size(); i++) {
                    if (to_push[i] != ' ') {
                        stacks[i].push_back(to_push[i]);
                    }
                }
            } else if (s.empty()) {
                for (auto& stack : stacks) {
                    std::reverse(stack.begin(), stack.end());
                }

                std::cout << "\nReversed" << std::endl;
                for (const auto& stack : stacks) {
                    print_vector(stack);
                }
                std::cout << std::endl;
            } else if (s.rfind("move", 0) == 0) {
                std::istringstream ss(s);
                std::string word;
                std::vector<int> nums;
                nums.reserve(3);
                while (ss >> word) {
                    try {
                        int number = boost::lexical_cast<int>(word);
                        nums.push_back(number);
                    } catch(...) {}
                }
                for (int i = 0; i < nums[0]; i++) {
                    std::vector<char>& stack_to_take = stacks[nums[1] - 1];
                    std::vector<char>& stack_to_push = stacks[nums[2] - 1];
                    char temp = stack_to_take[stack_to_take.size() - 1];

                    stack_to_take.pop_back();
                    stack_to_push.push_back(temp);
                }
            }
        }
        input_file.close();

        std::cout << "After operations" << std::endl;
        for (const auto& stack : stacks) {
            print_vector(stack);
        }
        
        std::cout << "\nResult: " << std::endl;
        for (const auto& stack : stacks) {
            std::cout << stack[stack.size() - 1];
        }
        std::cout << std::endl;
    }
    return 0;
}

