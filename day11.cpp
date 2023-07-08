#include <boost/lexical_cast/bad_lexical_cast.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <variant>
#include <vector>
#include <string>
#include <boost/lexical_cast.hpp>
#include <map>
#include <limits>

template <typename T>
void print_vector(const std::vector<T>& vec) {
    for (auto& e : vec) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

int modify_str_number(std::string& n) {
    int number = 0;
    try {
        number = std::stoi(n);
    } catch(std::invalid_argument) {
        std::cout << "Int conversion failed with " + n << std::endl;
        exit(1);
    }
    return number;
}


std::vector<std::vector<std::string>> get_split_strings(std::vector<std::string>& strings_per_monkey) {
    std::vector<std::vector<std::string>> split_strings;
    for (auto& s : strings_per_monkey) {
        std::istringstream ss(s);
        std::string word;
        std::vector<std::string> words;
        while (ss >> word) {
            words.push_back(word);

        }
        split_strings.push_back(words);
    }
    return split_strings;
}

using monkey_map_val = std::variant<int, std::vector<int>, std::pair<std::string, std::string>>;
std::map<std::string, monkey_map_val> process_strings(std::vector<std::string>& strings_per_monkey) {
    if (strings_per_monkey.size() != 6) {
        std::cout << "Not 6 strings per monky, but " << strings_per_monkey.size()  << std::endl;
        exit(1);
    }
    

    std::vector<std::vector<std::string>> split_strings = get_split_strings(strings_per_monkey);
    std::map<std::string, monkey_map_val> monkey_map; 

    int monkey_n = -1;
    if (split_strings[0][0] == "Monkey") {
        monkey_n = modify_str_number(split_strings[0][1]);
    } else {
        std::cout << "Invaling 1st string" << std::endl;
        print_vector(split_strings[0]);
        exit(1);
    }
    monkey_map["number"] = monkey_n;

    
    std::vector<int> monkey_items;
    if (split_strings[1][0] == "Starting") {
        for (int i = 2; i < split_strings[1].size(); i++) {
            int item = modify_str_number(split_strings[1][i]);
            monkey_items.push_back(item);
        }
    } else {
        std::cout << "Invalid 2nd string" << std::endl;
        print_vector(split_strings[1]);
        std::cout << split_strings[1][0] << std::endl;
        exit(1);
    }
    monkey_map["items"] = monkey_items;


    std::pair<std::string, std::string> monkey_operation;
    if (split_strings[2][0] == "Operation:") {
        int s_size = split_strings[2].size();
        if (split_strings[2][s_size - 2] == "*") {
            monkey_operation.first = "multiply";
        } else if (split_strings[2][s_size - 2] == "+") {
            monkey_operation.first = "add";
        } else {
            std::cout << "Invalid operation parameter" << std::endl;
            exit(1);
        }
        monkey_operation.second = split_strings[2][s_size - 1];

    } else {
        std::cout << "Invalid 3rd string" << std::endl;
        print_vector(split_strings[2]);
        exit(1);
    }
    monkey_map["operation"] = monkey_operation;

    
    int monkey_test_if_divisible = -1;
    if (split_strings[3][0] == "Test:") {
        monkey_test_if_divisible = modify_str_number(split_strings[3][split_strings[3].size() - 1]);
    } else {
        std::cout << "Invalid 4th string" << std::endl;
        exit(1);
    }
    monkey_map["test"] = monkey_test_if_divisible;


    int monkey_if_true = -1;
    if (split_strings[4][0] == "If" && split_strings[4][1] == "true:") {
        monkey_if_true = modify_str_number(split_strings[4][split_strings.size() - 1]);
    } else {
        std::cout << "Invalid 5th string" << std::endl;
        print_vector(split_strings[4]);
        exit(1);
    }
    monkey_map["if_true"] = monkey_if_true;
    

    int monkey_if_false = -1;
    if (split_strings[5][0] == "If" && split_strings[5][1] == "false:") {
        monkey_if_false = modify_str_number(split_strings[5][split_strings.size() - 1]);
    } else {
        std::cout << "Invalid 6th string" << std::endl;
        print_vector(split_strings[5]);
        exit(1);
    }
    monkey_map["if_false"] = monkey_if_false;
    
    return monkey_map;
}

std::vector<std::map<std::string, monkey_map_val>> get_data(std::fstream& input_file) {
    
    input_file.open("day11_input.txt", std::ios::in);
    std::vector<std::map<std::string, monkey_map_val>> monkey_maps;
    std::vector<std::string> strings_per_monkey;
    if (input_file.is_open()) {
        
        std::string s;
        while (std::getline(input_file, s)) {
            //monkey x[items]
            //monkey x[operation]
            //monkey x[test]
            //monkey x[if_true]
            //monkey x[if_false]
            if (!s.empty()) {
                strings_per_monkey.push_back(s);
            } else {

                std::map<std::string, monkey_map_val> monkey_map = process_strings(strings_per_monkey);
                monkey_maps.push_back(monkey_map);
                strings_per_monkey.clear();
            }
        }
        std::map<std::string, monkey_map_val> monkey_map = process_strings(strings_per_monkey);
        monkey_maps.push_back(monkey_map);
        strings_per_monkey.clear();

        input_file.close();
    } else {
        std::cout << "Input open failed" << std::endl;
        exit(1);
    }
    return monkey_maps;
}


void increase_worry_level(int& worry_level, std::pair<std::string, std::string> operation) {
    int n = -1;
    if (operation.first == "add") {
        if (operation.second != "old") {
            n = modify_str_number(operation.second);
        } else {
            n = worry_level;
        }
        worry_level += n;
    } else if (operation.first == "multiply") {
        if (operation.second != "old") {
            n = modify_str_number(operation.second);
        } else {
            n = worry_level;
        }
        worry_level *= n;
    } else {
        std::cout << "Invalid operation: " << operation.first << std::endl;
        exit(1);
    }
}

bool play_test(int& worry_level, int& test_if_divisible) {
    return worry_level % test_if_divisible == 0;
}


void decrease_worry_level(int& worry_level) {
    worry_level /= 3;
    worry_level = std::floor(worry_level);
}

void throw_item(int& item, int& monkey_to_throw, std::vector<std::map<std::string, monkey_map_val>>& monkey_maps) {
    auto& map_of_monkey_to_throw = monkey_maps[monkey_to_throw];
    auto& monkey_items = std::get<std::vector<int>>(map_of_monkey_to_throw["items"]);
    monkey_items.push_back(item);
}

void print_monkey_map(std::map<std::string, monkey_map_val>& m_m) {
        int monkey_n = -1;
        if (std::holds_alternative<int>(m_m["number"])) {
            monkey_n = std::get<int>(m_m["number"]);
        } else {
            std::cout << "Monkey n is not a number!" << std::endl;
            exit(1);
        }

        std::vector<int> monkey_items;
        if (std::holds_alternative<std::vector<int>>(m_m["items"])) {
            monkey_items = std::get<std::vector<int>>(m_m["items"]);
        } else {
            std::cout << "Monkey items is not std::vector<int>!" << std::endl;
            exit(1);
        } 

        std::pair<std::string, std::string> monkey_operation;
        if (std::holds_alternative<std::pair<std::string, std::string>>(m_m["operation"])) {
            monkey_operation = std::get<std::pair<std::string, std::string>>(m_m["operation"]);
        } else {
            std::cout << "Monkey operation is not a pair of strings!" << std::endl;
            exit(1);
        }

        int monkey_test_if_divisible = -1;
        if (std::holds_alternative<int>(m_m["test"])) {
            monkey_test_if_divisible = std::get<int>(m_m["test"]);
        } else {
            std::cout << "Monkey test_if_divisible is not int" << std::endl;
            exit(1);
        }

        int monkey_if_true = -1;
        if (std::holds_alternative<int>(m_m["if_true"])) {
            monkey_if_true = std::get<int>(m_m["if_true"]);
        } else {
            std::cout << "Monkey if_ture is not an int" << std::endl;
            exit(1);
        }

        int monkey_if_false = -1;
        if (std::holds_alternative<int>(m_m["if_false"])) {
            monkey_if_false = std::get<int>(m_m["if_false"]);
        } else {
            std::cout << "Monkey if_false is not an int" << std::endl;
            exit(1);
        }
        std::cout << "monkey_n: " << monkey_n << std::endl;
        std::cout << "monkey_items: ";
        print_vector(monkey_items);
        std::cout << "monkey_operation: " << monkey_operation.first << " " << monkey_operation.second << std::endl;
        std::cout << "monkey_test: " << monkey_test_if_divisible << std::endl;
        std::cout << "monkey_if_true: " << monkey_if_true << std::endl;
        std::cout << "monkey_if_false: " << monkey_if_false << std::endl;
        std::cout << std::endl << std::endl;
}

void play_round(
        std::vector<std::map<std::string, monkey_map_val>>& monkey_maps, 
        std::vector<int>& monkey_inspected_n_itemss
    ) {
    for (auto& m_m : monkey_maps) {
        
        auto& monkey_n = std::get<int>(m_m["number"]);
        auto& monkey_items = std::get<std::vector<int>>(m_m["items"]);
        auto& monkey_operation = std::get<std::pair<std::string, std::string>>(m_m["operation"]);
        auto& monkey_test_if_divisible = std::get<int>(m_m["test"]);
        auto& monkey_if_true = std::get<int>(m_m["if_true"]);
        auto& monkey_if_false = std::get<int>(m_m["if_false"]);

        for (int& item : monkey_items) {
            int& worry_level = item;

            increase_worry_level(worry_level, monkey_operation);
            decrease_worry_level(worry_level);
            bool test_result = play_test(worry_level, monkey_test_if_divisible);
            if (test_result) {
                int& monkey_to_throw = monkey_if_true;
                throw_item(worry_level, monkey_to_throw, monkey_maps);
            } else {
                int& monkey_to_throw = monkey_if_false;
                throw_item(worry_level, monkey_to_throw, monkey_maps);
            }
            monkey_inspected_n_itemss[monkey_n]++;
        }
        monkey_items.clear();
//        for (auto& m_m : monkey_maps) {
//            auto& monkey_n = std::get<int>(m_m["number"]);
//            auto& monkey_items = std::get<std::vector<int>>(m_m["items"]);
//            std::cout << "Monkey " << monkey_n << " items" << std::endl;
//            print_vector(monkey_items);
//        }
    }
    

}

int main() {
    std::fstream input_file;
    std::vector<std::map<std::string, monkey_map_val>> monkey_maps = get_data(input_file);  

    int rounds_to_play = 20;
    std::vector<int> monkey_inspected_n_items{0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < rounds_to_play; i++) {
        play_round(monkey_maps, monkey_inspected_n_items);
    }

    std::cout << "Monkey at following index inspected N items:" << std::endl;
    print_vector(monkey_inspected_n_items);
    int top_1 = -1;
    int top_2 = -1;
    for (auto& n : monkey_inspected_n_items) {
        if (n > top_1 && n > top_2) {
            top_2 = top_1;
            top_1 = n;
        } else if (n > top_2) {
            top_2 = n;
        }
    }

    long long res = top_1 * top_2;

    std::cout << "Top 1: " << top_1 << " Top 2: " << top_2 << std::endl;
    std::cout << "Monkey business level (result):" << std::endl;
    std::cout << res << std::endl;

    return 0;
}
