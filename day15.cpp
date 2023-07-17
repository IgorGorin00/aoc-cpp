#include <cassert>
#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


struct Point {
    long x;
    long y;
    Point(long x_, long y_) : x(x_), y(y_) {}

    void print() const {
        std::cout << "(" << x << ", " << y << ")  ";
    }

    bool operator<(const Point& other) const {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }
};

long calculateManhattanDistance(const Point& p1, const Point& p2) {
    long x_diff = std::max(p1.x, p2.x) - std::min(p1.x, p2.x);
    long y_diff = std::max(p1.y, p2.y) - std::min(p1.y, p2.y);
    return x_diff + y_diff;
}


std::vector<Point> getScannedPoints(const Point& scanner, const Point& beacon, const long& needed_Y) {
    long distance = calculateManhattanDistance(scanner, beacon);
    long max_y_scanned = scanner.y + distance;
    long min_y_scanned = scanner.y - distance;
//  2000000
    std::vector<Point> scanned_points;
    if (needed_Y > max_y_scanned || needed_Y < min_y_scanned) {
        return scanned_points;
    } 
    long distance_to_needed_Y = std::max(scanner.y, needed_Y) - std::min(scanner.y, needed_Y);

    long awailable_distance_X = distance - distance_to_needed_Y;
    long max_reach_X = scanner.x + awailable_distance_X;
    long min_reach_x = scanner.x - awailable_distance_X;
    long amount_of_scanned_points = max_reach_X - min_reach_x;
    
    for (int x = min_reach_x; x <= max_reach_X; x++) {
        scanned_points.push_back(Point(x, needed_Y));
    }
    
    return scanned_points;
}


long getResult(const std::vector<std::pair<Point, Point>>& data) {
    const long needed_Y = 2000000;
    std::set<Point> scanned_at_needed_Y;
    for (const std::pair<Point, Point>& d : data) {
        const Point scanner = d.first;
        const Point beacon = d.second;
        std::vector<Point> scanned_per_scanner = getScannedPoints(scanner, beacon, needed_Y);
        for (const Point& p : scanned_per_scanner) {
            scanned_at_needed_Y.insert(p);
        }
    }

    
    
    for (const std::pair<Point, Point>& d : data) {
        const Point scanner = d.first;
        const Point beacon = d.second;
        if (scanned_at_needed_Y.find(beacon) != scanned_at_needed_Y.end()) {
            scanned_at_needed_Y.erase(beacon); 
        }
    }

    return scanned_at_needed_Y.size();
}

std::vector<std::pair<Point, Point>> getData(std::string fName) {
    std::fstream input_file;
    input_file.open(fName, std::ios::in);
    std::vector<std::pair<Point, Point>> data;
    std::string s;
    if (input_file.is_open()) {
        while (std::getline(input_file, s)) {
            std::istringstream ss(s);
            std::vector<std::string> words;
            std::string word;
            while (ss >> word) {
                words.push_back(word);
            }
            assert(words.size() == 10 && "Words size != 10");

            std::string sensor_x = words[2].substr(2);
            std::string sensor_y = words[3].substr(2);
            std::string beacon_x = words[8].substr(2);
            std::string beacon_y = words[9].substr(2);

            long s_x = std::stol(sensor_x);
            long s_y = std::stol(sensor_y);
            long b_x = std::stol(beacon_x);
            long b_y = std::stol(beacon_y);

            Point sensor(s_x, s_y);
            Point beacon(b_x, b_y);
            std::pair<Point, Point> p{sensor, beacon};
            data.push_back(p);
        }

        input_file.close();
    } else {
        throw std::runtime_error("Input open failed");
    }
    return data;
}


int main() {
    std::string file_name = "day15_input.txt";
    std::vector<std::pair<Point, Point>> data = getData(file_name); 
//  2000000
    long res = getResult(data);
    std::cout << "Result:" << std::endl << res << std::endl;
    return 0;
}
