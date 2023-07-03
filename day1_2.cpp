#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>

int main() {
    std::fstream input_file;
    
    std::vector<int> counts;

    input_file.open("day1_input.txt", std::ios::in);

    int current_count = 0;
    if (input_file.is_open()) {
        std::string s;
        while (std::getline(input_file, s)) {
            try {
                int i = boost::lexical_cast<int>(s);
                current_count += i;
            } catch(...) {
                counts.push_back(current_count);
                current_count = 0;
            }
        }
        input_file.close();
    }

    std::sort(counts.begin(), counts.end());
    std::cout << counts[counts.size() - 1] << "\n\n";
    std::cout << counts[counts.size() - 2] << "\n\n";
    std::cout << counts[counts.size() - 3] << "\n\n";
    int total_value = counts[counts.size() - 1] + counts[counts.size() - 2] + counts[counts.size() - 3];
    std::cout << total_value << "\n";

    return 0;
}

