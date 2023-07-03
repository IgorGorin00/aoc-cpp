#include <iostream>
#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <cassert>

int main() {
    std::fstream input_file;
    int total_count = 0; 
    input_file.open("day4_input.txt", std::ios::in);
    if (input_file.is_open()) {
        std::string s;
        char comma = ',';
        char dash = '-';
        while (std::getline(input_file, s)) {
            size_t found_comma = s.find(comma);
            if (found_comma != std::string::npos) {
                std::string first_pair = s.substr(0, found_comma);
                std::string second_pair = s.substr(found_comma + 1);
                std::vector<std::string> pairs{first_pair, second_pair};
                std::vector<int> nums;
                for (auto pair : pairs) {
                    size_t found_dash = pair.find(dash);
                    if (found_dash != std::string::npos) {
                        std::string first_n = pair.substr(0, found_dash);
                        std::string second_n = pair.substr(found_dash + 1);
                        try {
                            int n1 = boost::lexical_cast<int>(first_n);
                            int n2 = boost::lexical_cast<int>(second_n);
                            nums.push_back(n1);
                            nums.push_back(n2);
                        } catch(...) {
                            std::cout << "Error while casting to int" << std::endl;
                            return 1;
                        }
                    }
                }
                assert(nums.size() == 4 && "Nums len != 4");
                if (
                        (nums[0] >= nums[2] && nums[1] <= nums[3]) ||
                        (nums[0] <= nums[2] && nums[1] >= nums[3])
                        ) {
                    total_count += 1;
                }
            }
        }
        input_file.close();
    }
    std::cout << total_count << std::endl;
    std::cout << "finished" << std::endl;

    return 0;
}
