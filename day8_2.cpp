#include <algorithm>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <boost/lexical_cast.hpp>


std::vector<std::vector<int>> get_matrix(std::fstream& input_file) {
    
    std::vector<std::vector<int>> matrix;
    input_file.open("day8_input.txt", std::ios::in);
    if (input_file.is_open()) {
        
        std::string s;
        while (std::getline(input_file, s)) {
            int max_value = 0;
            std::vector<int> mat_row;
            for (auto& ch : s) {
                int n = boost::lexical_cast<int>(ch);
                mat_row.push_back(n);
            }
            matrix.push_back(mat_row);
        }
        input_file.close();
        
    } else {
        std::cout << "Input open failed" << std::endl;
    }
    return matrix;
}


std::map<std::pair<int, int>, int> get_trees_height_map(std::vector<std::vector<int>>& matrix) {
    
    std::map<std::pair<int, int>, int> trees_height_map;
    for (int i = 0; i < matrix.size(); i++) {
        int max_val = -1;
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] > max_val) {
                max_val = matrix[i][j];
                std::pair<int, int> visible_tree{i, j};
                trees_height_map[visible_tree] = max_val;
            }
        }
        max_val = -1;
        for (int j = matrix[i].size() - 1; j >= 0; j--) {
            if (matrix[i][j] > max_val) {
                max_val = matrix[i][j];
                std::pair<int, int> visible_tree{i, j};
                trees_height_map[visible_tree] = max_val;
            } 
        }
    }

    for (int j = 0; j < matrix[0].size(); j++) {
        int max_val = -1;
        for (int i = 0; i < matrix.size(); i++) {
            if (matrix[i][j] > max_val) {
                max_val = matrix[i][j];
                std::pair<int, int> visible_tree{i, j};
                trees_height_map[visible_tree] = max_val;
            } 
        }
        max_val = -1;
        for (int i = matrix.size() - 1; i >= 0; i--) {
            if (matrix[i][j] > max_val) {
                max_val = matrix[i][j];
                std::pair<int, int> visible_tree{i, j};
                trees_height_map[visible_tree] = max_val;
            } 
        }
    }
    return trees_height_map;
}


int get_vision(int& tree_height, std::vector<int>& tree_list) {
    int vision = 0;
    int index = 0;
    while (index != tree_list.size()) {
        vision++;
        if (tree_list[index] >= tree_height) {
            return vision;
        }
        index++;
    }
    return vision;
}


int get_result(std::vector<std::vector<int>>& matrix) {
    int result = 0;
    for (int i = 1; i < matrix.size() - 1; i++) {
        for (int j = 1; j < matrix[i].size() - 1; j++) {
            std::vector<int> trees_left;
            std::vector<int> trees_right;
            std::vector<int> trees_top;
            std::vector<int> trees_bottom;
            for (int j_ = j - 1; j_ >= 0; j_--) {
                trees_left.push_back(matrix[i][j_]);
            }
            for (int j_ = j + 1; j_ < matrix[i].size(); j_++) {
                trees_right.push_back(matrix[i][j_]);
            }
            for (int i_ = i - 1; i_ >= 0; i_--) {
                trees_top.push_back(matrix[i_][j]);
            }
            for (int i_ = i + 1; i_ < matrix.size(); i_++) {
                trees_bottom.push_back(matrix[i_][j]);
            }
            int left_v = get_vision(matrix[i][j], trees_left);
            int right_v = get_vision(matrix[i][j], trees_right);
            int top_v = get_vision(matrix[i][j], trees_top);
            int bot_v = get_vision(matrix[i][j], trees_bottom);
            int v_score = left_v * right_v * top_v * bot_v;
            if (v_score > result) {
                result = v_score;
            }
        }
    }
    return result;
}



int main() {
    std::fstream input_file;
    
    std::vector<std::vector<int>> matrix = get_matrix(input_file); 
    int res = get_result(matrix);
    std::cout << "Result:" << std::endl << res << std::endl;


    return 0;
}

