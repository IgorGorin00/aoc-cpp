#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::fstream input_file;

    input_file.open("day1_input.txt", std::ios::in);

    int best_count = 0;
    int current_count = 0;
    if (input_file.is_open()) {
        std::string s;
        while (std::getline(input_file, s)) {
            try {
                int i = boost::lexical_cast<int>(s);
                current_count += i;
            } catch(...) {
                //std::cout << "NOT A NUMBER" << "\n";
                if (current_count > best_count) {
                    best_count = current_count;
                }
                current_count = 0;
            }
        }
        input_file.close();
    }
    std::cout << best_count << "\n";

    return 0;
}

