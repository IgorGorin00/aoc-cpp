#include <algorithm>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct GraphNodeTemplate {
    std::string Head;
    int Value;
    std::vector<std::string> Neighbors;

    GraphNodeTemplate() = default;

    GraphNodeTemplate(
            std::string& head,
            int& val, 
            std::vector<std::string>& neighbors) : 
        Head(head), Value(val), Neighbors(neighbors) {}

    void print() {
        std::cout << "Head: " << Head << ", Value: " << Value << std::endl;
        std::cout << "Neighbors: [ ";
        for (const auto& s : Neighbors) {
            std::cout << s << " ";
        }
        std::cout << "]" << std::endl;
    }
};


std::map<std::string, GraphNodeTemplate> getData(const std::string& fName) {
    std::fstream input_file;
    input_file.open(fName, std::ios::in);
    std::map<std::string, GraphNodeTemplate> nodes;
    if (input_file.is_open()) {
        std::string s;
        
        while (std::getline(input_file, s)) {
            std::istringstream iss(s);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
            }
            std::string head = words[1];
            std::string val_str = words[4].substr(5);
            int val = std::stoi(val_str);
            std::vector<std::string> neighbors;
            for (int i = 9; i < words.size(); i++) {
                std::string cur_word = words[i];
                if (cur_word[cur_word.size() - 1] == ',') {
                    neighbors.push_back(cur_word.substr(0, 2));
                } else {
                    neighbors.push_back(cur_word);
                }
            }
            GraphNodeTemplate node(head, val, neighbors);

            nodes[head] = node;
        }
        input_file.close();
    } else {
        throw std::runtime_error("Input open failed");
    }
    return nodes;
}


struct Valve {
    std::string Name;
    int Rate;
    std::unordered_map<std::string, int> Edges;

    Valve() = default;

    Valve(std::string& n, int r, std::unordered_map<std::string, int> e) 
        : Name(n), Rate(r), Edges(e) {}

    void print() {
        std::cout << "Valve: " << Name << " Rate: " << Rate << "\n";
        std::cout << "Edges:\n";
        for (auto& [e_name, path_len] : Edges) {
            std::cout << "\t" << e_name << " " << path_len << "\n";
        }
    }
};

int BFSearch(
        const std::string& start_name, 
        const std::string& end_name, 
        const std::map<std::string, GraphNodeTemplate>& graph
        ) {
    std::queue<std::pair<std::string, int>> q; // node and distance
    std::unordered_set<std::string> visited;

    q.push({start_name, 0});
    visited.insert(start_name);
    while (!q.empty()) {
        std::string current_name = q.front().first;
        int distance = q.front().second;
        q.pop();
        
        if (current_name == end_name) {
            return distance + 1;
        }

        GraphNodeTemplate current = graph.at(current_name);

        for (const std::string neighbor : current.Neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                q.push({neighbor, distance + 1});
                visited.insert(neighbor);
            }
        }
    }
    return -1;
}

std::unordered_map<std::string, int> generatePaths(
        const GraphNodeTemplate& start_node, 
        const std::map<std::string, GraphNodeTemplate>& nodes
    ) {
    std::unordered_map<std::string, int> paths;
    for (const auto& [dest_name, dest_node] : nodes) {
        if (dest_node.Head == start_node.Head || dest_node.Value == 0) {
            continue;
        }
        int path_len = BFSearch(start_node.Head, dest_node.Head, nodes);
        if (path_len != -1) {
            paths[dest_node.Head] = path_len;
        }
    }
    return paths;
}

std::unordered_map<std::string, Valve> generateValvesMap(const std::map<std::string, GraphNodeTemplate>& nodes) {
    std::string start_node_name = "AA";
    std::unordered_map<std::string, Valve> valves;
    for (const auto& [node_name, node] : nodes) {
        if (node.Value == 0 && node_name != start_node_name) {
            continue;
        }
        std::string n = node.Head;
        int r = node.Value;
        std::unordered_map<std::string, int> edges = generatePaths(node, nodes);
        Valve new_valve{n, r, edges};
        valves[node_name] = new_valve;
    }
    return valves;
}

struct calcVal {
    int Steps = 0;
    int FlowRate = 0;
    int Flow = 0;
    int TotalFlow = 0;
};

struct qVal {
    Valve Node;
    std::unordered_set<std::string> Visited;
    calcVal Calculated;

    qVal(Valve& v) : Node(v) {}


};


calcVal calcFlow(qVal& current, Valve& edge, int rounds, calcVal& new_calculated) {

    new_calculated.Flow = (current.Calculated.FlowRate * current.Node.Edges.at(edge.Name)) + current.Calculated.Flow;

    new_calculated.FlowRate = current.Calculated.FlowRate + edge.Rate;

    new_calculated.TotalFlow = (new_calculated.FlowRate * (rounds - new_calculated.Steps)) + new_calculated.Flow;

    return new_calculated;
}

int findShortestPath(
        std::unordered_map<std::string, Valve>& valves, 
        std::string& start_node_name, int rounds
    ) {
    
    std::queue<qVal> q;
    q.push(qVal{valves.at(start_node_name)});

    qVal winner = q.front();
    
    while (!q.empty()) {
        qVal current = q.front();
        q.pop();
        current.Visited.insert(current.Node.Name);
        if (current.Calculated.TotalFlow > winner.Calculated.TotalFlow) {
            winner = current;
        }

        for (auto& [_, edge] : valves) {
            if (current.Visited.find(edge.Name) != current.Visited.end()
                || current.Node.Name == edge.Name
                || current.Calculated.Steps + current.Node.Edges[edge.Name] > rounds
            ) { continue; } 
            calcVal new_calculated;
            new_calculated.Steps = current.Calculated.Steps + current.Node.Edges.at(edge.Name);

            new_calculated = calcFlow(current, edge, rounds, new_calculated);

            if (new_calculated.TotalFlow < winner.Calculated.TotalFlow
                && new_calculated.Steps >= winner.Calculated.Steps
                ) { continue; }

            qVal to_push(edge);
            to_push.Visited = current.Visited;
            to_push.Calculated = new_calculated;
            q.push(to_push);
        }
    }
    return winner.Calculated.TotalFlow;
}


int main() {
    auto time_start = std::chrono::high_resolution_clock::now();
    std::string file_name = "day16_input.txt";
//    std::string file_name = "day16_test.txt";
    std::map<std::string, GraphNodeTemplate> nodes = getData(file_name);
    std::unordered_map<std::string, Valve> valves = generateValvesMap(nodes);
    
    std::string start_node_name = "AA";
    int rounds = 30;
    int res = findShortestPath(valves, start_node_name, rounds);
    auto time_end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start);
    double duration_seconds = duration.count() / 1e6;

    std::cout << "Result:\n" << res << std::endl;
    std::cout << "Time taken by function: " << duration.count() << " microseconds, which is " << duration_seconds << " seconds."<< std::endl;
    return 0;
}
