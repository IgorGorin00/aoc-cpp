#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <cassert>
#include <set>

int get_char_value(char c) {
    int code = int(c);
    if (code >= 97 && code <= 122) {
        return code - 96;
    } else if (code >= 65 && code <= 90) {
        return code - 38; 
    }
    return 0;
}

char get_full_intersection(
        const std::set<char>& set1, 
        const std::set<char>& set2, 
        const std::set<char>& set3) {
    std::set<char> intersetction;
    std::set_intersection(
            set1.begin(), set1.end(),
            set2.begin(), set2.end(),
            std::inserter(intersetction, intersetction.begin())
            );
    std::set<char> full_intersection;
    std::set_intersection(
            intersetction.begin(), intersetction.end(),
            set3.begin(), set3.end(),
            std::inserter(full_intersection, full_intersection.begin())
            );
    assert(full_intersection.size() == 1 && "Intersection len > 1");
    char unique_value = *full_intersection.begin();
    return unique_value;
}


int main() {

    std::fstream input_file;
    input_file.open("day3_input.txt", std::ios::in);
    if (input_file.is_open()) {


        int total_value = 0;
        std::string s;
        int current_count = 0;
        std::vector<std::set<char>> vals;
        while (std::getline(input_file, s)) {
            std::set<char> unique_vals;
            for (char ch : s) {
                unique_vals.insert(ch);
            }
            vals.push_back(unique_vals);
            current_count++;
            if (current_count == 3) {
                current_count = 0;
                char full_intersection = get_full_intersection(
                        vals[0], vals[1], vals[2]);
                total_value += get_char_value(full_intersection);
                vals.clear();
            }
        }
        std::cout << total_value << std::endl;
        input_file.close();
    }
    std::cout << "finished\n";
    return 0;
}
