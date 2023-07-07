#include <cstdlib>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <boost/lexical_cast.hpp>

void update_positions(std::pair<int, int>& head_pos, std::pair<int, int>& tail_pos) {

    if (head_pos.first - tail_pos.first == 2 && head_pos.second - tail_pos.second == 1) {
        tail_pos.first++;
        tail_pos.second++;
    } else if (head_pos.first - tail_pos.first == 2 && head_pos.second - tail_pos.second == -1) {
        tail_pos.first++;
        tail_pos.second--;
    } else if (head_pos.first - tail_pos.first == -2 && head_pos.second - tail_pos.second == 1) {
        tail_pos.first--;
        tail_pos.second++;
    } else if (head_pos.first - tail_pos.first == -2 && head_pos.second - tail_pos.second == -1) {
        tail_pos.first--;
        tail_pos.second--;
    } else if (head_pos.first - tail_pos.first == 1 && head_pos.second - tail_pos.second == 2) {
        tail_pos.first++;
        tail_pos.second++;
    } else if (head_pos.first - tail_pos.first == 1 && head_pos.second - tail_pos.second == -2) {
        tail_pos.first++;
        tail_pos.second--;
    } else if (head_pos.first - tail_pos.first == -1 && head_pos.second - tail_pos.second == 2) {
        tail_pos.first--;
        tail_pos.second++;
    } else if (head_pos.first - tail_pos.first == -1 && head_pos.second - tail_pos.second == -2) {
        tail_pos.first--;
        tail_pos.second--;
    } else if (head_pos.first - tail_pos.first == 2) {
        tail_pos.first ++;
    } else if (head_pos.first - tail_pos.first == -2) {
        tail_pos.first--;
    } else if (head_pos.second - tail_pos.second == 2) {
        tail_pos.second++;
    } else if (head_pos.second - tail_pos.second == -2) {
        tail_pos.second--;
    }
}

int main() {
    std::fstream input_file;
    input_file.open("day9_input.txt", std::ios::in);
    if (input_file.is_open()) {
        std::string s;
        std::vector<std::pair<char, int>> head_moves;
        std::set<std::pair<int, int>> tail_positions_set;
        std::pair<int, int> head_pos{0, 0}; // x and y
        std::pair<int, int> tail_pos{0, 0};
        while (std::getline(input_file, s)) {
            char direction;
            std::string to_number;
            for (int i = 0; i < s.size(); i++){
                if (i == 0) {direction = s[i];}
                else if (i > 1) {to_number += s[i];}
            }
            
            int n = 0;
            try {n = boost::lexical_cast<int>(to_number);} 
            catch(...) {std::cout << "Bad" << std::endl; return 1;}
            for (int i = 0; i < n; i++) {
                if (direction == 'R') {head_pos.first++;}
                else if (direction == 'U') {head_pos.second++;}
                else if (direction == 'L') {head_pos.first--;}
                else if (direction == 'D') {head_pos.second--;}
                else {std::cout << "Bad" << std::endl; exit(1);}
                update_positions(head_pos, tail_pos);
                tail_positions_set.insert(tail_pos);

            }
        }


        std::cout << "Result:" << std::endl << tail_positions_set.size() << std::endl;

        input_file.close();
    } else {
        std::cout << "Input open failed" << std::endl;
    }
    return 0;
}
