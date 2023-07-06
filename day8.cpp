#include <algorithm>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <utility>
#include <boost/lexical_cast.hpp>


int get_indes(std::vector<int>& v, int& X) {
    auto it = std::find(v.begin(), v.end(), X);
    int index = -1;
    if (it != v.end()) {
        index = it - v.begin();
    }
    return index;
}


int main() {
    std::fstream input_file;
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

    std::vector<std::pair<int, int>> visible_trees;
    for (int i = 0; i < matrix.size(); i++) {
        int max_val = -1;
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] > max_val) {
                max_val = matrix[i][j];
                std::pair<int, int> visible_tree{i, j};
                visible_trees.push_back(visible_tree);
            }
        }
        max_val = -1;
        for (int j = matrix[i].size() - 1; j >= 0; j--) {
            if (matrix[i][j] > max_val) {
                max_val = matrix[i][j];
                std::pair<int, int> visible_tree{i, j};
                visible_trees.push_back(visible_tree);
            } 
        }
    }

    for (int j = 0; j < matrix[0].size(); j++) {
        int max_val = -1;
        for (int i = 0; i < matrix.size(); i++) {
            if (matrix[i][j] > max_val) {
                max_val = matrix[i][j];
                std::pair<int, int> visible_tree{i, j};
                visible_trees.push_back(visible_tree);
            } 
        }
        max_val = -1;
        for (int i = matrix.size() - 1; i >= 0; i--) {
            if (matrix[i][j] > max_val) {
                max_val = matrix[i][j];
                std::pair<int, int> visible_tree{i, j};
                visible_trees.push_back(visible_tree);
            } 
        }
    }
    std::set<std::pair<int, int>> visible_trees_set(visible_trees.begin(), visible_trees.end());

//    std::cout << visible_trees.size() << " " << visible_trees_set.size() << std::endl;
    std::cout << "Result:" << std::endl << visible_trees_set.size() << std::endl;

    return 0;
}
