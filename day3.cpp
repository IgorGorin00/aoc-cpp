#include <algorithm>
#include <iostream>
#include <fstream>
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

int main() {

    std::fstream input_file;
    input_file.open("day3_input.txt", std::ios::in);
    if (input_file.is_open()) {


        int total_value = 0;
        std::string s;
        while (std::getline(input_file, s)) {
            int half_len = s.size() / 2;
            std::string first_half = s.substr(0, half_len);
            std::string second_half = s.substr(half_len);
            assert(first_half.size() == second_half.size() && "Not equal splitting");
            std::set<char> first_half_set;
            std::set<char> second_half_set;
            for (int i = 0; i < first_half.size(); i++) {
                first_half_set.insert(first_half[i]);
                second_half_set.insert(second_half[i]);
            }
            std::set<char> intersection;
            std::set_intersection(
                    first_half_set.begin(), first_half_set.end(),
                    second_half_set.begin(), second_half_set.end(),
                    std::inserter(intersection, intersection.begin())
                    );

            assert(intersection.size() == 1 && "Intersection longer than 1");
            char ch = *intersection.begin();
            total_value += get_char_value(ch);

        }
        std::cout << total_value << std::endl;
        input_file.close();
    }
    std::cout << "finished\n";
    return 0;
}
