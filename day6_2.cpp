#include <iostream>
#include <fstream>
#include <string>
#include <set>

int main() {
    std::fstream input_file;
    input_file.open("day6_input.txt", std::ios::in);
    int n_unique = 14;

    if (input_file.is_open()) {
        std::string s;
        while (std::getline(input_file, s)) {
            for (int i = 0; i < s.size() - n_unique - 3; i++) {
                std::string substring = s.substr(i, n_unique);
                std::set<char> unique;
                for (char ch : substring) {
                    unique.insert(ch);
                }
                if (unique.size() == n_unique) {
                    std::cout << "Result:" << std::endl;
                    std::cout << i + n_unique << std::endl;
                    std::cout << substring << std::endl;
                    input_file.close();
                    return 0;
                }
            }
        }
        input_file.close();
    }
    return 0;
}
