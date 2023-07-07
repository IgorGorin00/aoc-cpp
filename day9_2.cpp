#include <cstdlib>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <boost/lexical_cast.hpp>

bool need_to_move_tail (std::pair<int, int>& head, std::pair<int, int>& tail) {
    if (head.first - tail.first == 2 || head.first - tail.first == -2 ||
        head.second - tail.second == 2 || head.second - tail.second == -2) {
        return true;
    }
    return false;
}


void move_head(char& direction, std::pair<int, int>& head) {
    if (direction == 'R') {head.first++;}
    else if (direction == 'L') {head.first--;}
    else if (direction == 'U') {head.second++;}
    else if (direction == 'D') {head.second--;}
}

void move_tail_straight(std::pair<int, int>& head, std::pair<int, int>& tail) {
    if (head.first == tail.first) { 
        if (head.second > tail.second) {tail.second++;}
        else {tail.second--;}
    }
    if (head.second == tail.second) {
        if (head.first > tail.first) {tail.first++;}
        else {tail.first--;}
    }
}

void move_tail_diagonally(std::pair<int, int>& head, std::pair<int, int>& tail) {
    if (head.first > tail.first) {tail.first++;} 
    else {tail.first--;}
    if (head.second > tail.second) {tail.second++;}
    else {tail.second--;}
}



int main() {
    std::fstream input_file;
    input_file.open("day9_input.txt", std::ios::in);
    if (input_file.is_open()) {
        std::string s;
        std::vector<std::pair<char, int>> head_moves;
        std::set<std::pair<int, int>> tail_positions_set;
        
        std::vector<std::pair<int, int>> rope;
        const int num_knots_in_rope = 10;
        for (int i = 0; i < num_knots_in_rope; i++) {
            rope.push_back(std::pair{0, 0});
        }
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
                move_head(direction, rope[0]);
                for (int i = 0; i < rope.size() - 1; i++) {
                    auto& head = rope[i];
                    auto& tail = rope[i + 1];
                    if (need_to_move_tail(head, tail)) {
                        if (head.first != tail.first && head.second != tail.second) {move_tail_diagonally(head, tail);}
                        while (need_to_move_tail(head, tail)) {move_tail_straight(head, tail);}
                    }

                }
                tail_positions_set.insert(rope[rope.size() - 1]);

            }
        }


        std::cout << "Result:" << std::endl << tail_positions_set.size() << std::endl;

        input_file.close();
    } else {
        std::cout << "Input open failed" << std::endl;
    }
    return 0;
}
