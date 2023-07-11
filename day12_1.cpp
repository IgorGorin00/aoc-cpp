#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <numeric>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <random>
#include <cassert>
#include <algorithm>
#include <numeric>


template <typename T>
void print_vector(std::vector<T>& v) {
    for (auto& e : v) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

std::vector<std::vector<int>> get_data(std::string file_name) {
    
    std::fstream input_file;
    std::vector<std::vector<int>> matrix;

    input_file.open(file_name, std::ios::in);
    if (input_file.is_open()) {
        
        std::string s;
        while(std::getline(input_file, s)) {
            std::vector<int> matrix_row;
            for (auto& ch : s) {
                int n = -1;
                n = (int)ch;
                if (n == -1) {
                    std::cout << "Something wrong with" << ch << std::endl;
                    throw std::runtime_error("Char parsing failed");
                }
                matrix_row.push_back(n);
            }
            matrix.push_back(matrix_row);
        }
        input_file.close();
    } else {
        throw std::runtime_error("Input open failed");
    }
    return matrix;
}

bool can_go_up(int& i, int& j, std::vector<std::vector<int>>& matrix) {
    if (i != 0) {
        if (matrix[i][j] - matrix[i - 1][j] >= -1) {
            return true;
        }
    }
    return false;
}

bool can_go_down(int& i, int& j, std::vector<std::vector<int>>& matrix) {
    if (i != matrix.size() - 1) {
        if (matrix[i][j] - matrix[i + 1][j] >= -1) {
            return true;
        }
    }
    return false;
}

bool can_go_left(int& i, int& j, std::vector<std::vector<int>>& matrix) {
    if (j != 0) {
        if (matrix[i][j] - matrix[i][j - 1] >= -1) {
            return true;
        }
    }
    return false;
}

bool can_go_right(int& i, int& j, std::vector<std::vector<int>>& matrix) {
    if (j != matrix[i].size() - 1) {
        if (matrix[i][j] - matrix[i][j + 1] >= -1) {
            return true;
        }
    }
    return false;
}

std::map<std::pair<int, int>, std::vector<char>> get_possible_actions(std::vector<std::vector<int>> matrix) {
    std::map<std::pair<int, int>, std::vector<char>> possible_actions;
    
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            std::pair<int, int> current_position{i, j};
            std::vector<char> actions;
            // Actions:
            // 0: D, 1: U, 2: L, 3: R;
            if (can_go_up(i, j, matrix)) {
                actions.push_back('U');
            }
            if (can_go_down(i, j, matrix)) {
                actions.push_back('D');
            }
            if (can_go_left(i, j, matrix)) {
                actions.push_back('L');
            }
            if (can_go_right(i, j, matrix)) {
                actions.push_back('R');
            }
            possible_actions[current_position] = actions;
        }
    }
    return possible_actions;
}


struct Node {
    int i, j;
    std::vector<Node*> neighbors;

    Node(int i_, int j_) : i(i_), j(j_) {}
};


std::map<std::pair<int, int>, Node*> buildGraph(
        const std::map<std::pair<int, int>, std::vector<char>>& possibleActions) {
    std::map<std::pair<int, int>, Node*> nodes;

    for (const auto& [position, actions] : possibleActions) {
        int i = position.first;
        int j = position.second;


        if (nodes.find(position) == nodes.end()) {
            nodes[position] = new Node(i, j);
        }
        Node* currentNode = nodes[position];
        for (char action : actions) {
            int destI = i;
            int destJ = j;
            if (action == 'U') {
                destI--;
            } else if (action == 'D') {
                destI++;
            } else if (action == 'L') {
                destJ--;
            } else if (action == 'R') {
                destJ++;
            }

            std::pair<int, int> destinationPosition = {destI, destJ};
            if (nodes.find(destinationPosition) == nodes.end()) {
                nodes[destinationPosition] = new Node(destI, destJ);
            }

            Node* destinationNode = nodes[destinationPosition];
            currentNode->neighbors.push_back(destinationNode);
        }

    }
    return nodes;
}



std::vector<Node*> BFSearh(Node* start, Node* end) {
    std::queue<Node*> q;
    std::unordered_set<Node*> visited;
    std::unordered_map<Node*, Node*> parent;
    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current == end) {
            std::vector<Node*> path;
            Node* node = end;
            while (node != nullptr) {
                path.push_back(node);
                node = parent[node];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (Node* neighbor : current->neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                q.push(neighbor);
                visited.insert(neighbor);
                parent[neighbor] = current;
            }
        }
    }
    return {};   
}


void checkNeighbors(const Node* n) {
    std::cout << n->i << ", " << n->j << std::endl;
    for (const Node* neighbor : n->neighbors) {
        std::cout << "(" << neighbor->i << ", " << neighbor->j << "); ";
    }
    std::cout << std::endl;
}

void printPath(const std::vector<Node*>& path) {
    if (path.empty()) {
        std::cout << "No path found" << std::endl;
        return;
    }
    std::cout << "Path: ";
    int counter = 0;
    for (const Node* node : path) {
        std::cout << "(" << node->i << ", " << node->j << ") " << counter << std::endl;
        counter++;
    }
    std::cout << std::endl;
    std::cout << "Path size: " << path.size() << std::endl;
}



int main() {

    std::string file_name = "day12_input.txt";
    std::vector<std::vector<int>> matrix = get_data(file_name);
    
    int starting_pos_char = (int)'S';
    int pos_goal = (int)'E';

    std::pair<int, int> start_state;
    std::pair<int, int> goal_state;


    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == starting_pos_char) {
                start_state.first = i;
                start_state.second = j;
                matrix[i][j] = (int)'a';
            }
            if (matrix[i][j] == pos_goal) {
                goal_state.first = i;
                goal_state.second = j;
                matrix[i][j] = (int)'z';
            }
        }
    }

    std::map<std::pair<int, int>, std::vector<char>> possible_actions = get_possible_actions(matrix);

    std::map<std::pair<int, int>, Node*> graph = buildGraph(possible_actions);
    Node* startNode = graph[start_state];
    Node* endNode = graph[goal_state];
    
    std::pair<int, int> key_to_check{20, 9};
    std::vector<Node*> path = BFSearh(startNode, endNode);
    printPath(path);
    std::cout << "Steps N (result):" << std::endl;
    std::cout << path.size() - 1 << std::endl; 

    for (auto& entry : graph) {
        delete entry.second;
    }


    return 0;
}
