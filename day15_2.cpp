#include <cassert>
#include <iostream>
#include <fstream>
#include <map>
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


long long getResult(std::vector<std::pair<Point, Point>> data) {
    std::set<long> aCoeffs;
    std::set<long> bCoeffs;
   
    std::map<Point, long> distanceMap;

    for (std::pair<Point, Point> const& d : data) {
        const Point& scanner = d.first;
        const Point& beacon = d.second;
        long mDistance = calculateManhattanDistance(scanner, beacon);
        distanceMap[scanner] = mDistance;
        aCoeffs.insert(scanner.y - scanner.x + mDistance + 1);
        aCoeffs.insert(scanner.y - scanner.x - mDistance - 1);
        bCoeffs.insert(scanner.y + scanner.x + mDistance + 1);
        bCoeffs.insert(scanner.y + scanner.x - mDistance - 1);
    }

    long bound = 4000000;
    long long res = 0;
    for (const long& a : aCoeffs) {
        for (const long& b : bCoeffs) {
            Point p{(b-a)/2, (a+b)/2};
            if (p.x > 0 && p.x < bound && p.y > 0 && p.y < bound) {
                int count_true = 0;
                for (std::pair<Point, long> const& d : distanceMap) {
                    const Point& scanner = d.first;
                    const long& scan_distance = d.second;
                    long current_distance = calculateManhattanDistance(p, scanner);
                    if (current_distance > scan_distance) {
                        count_true++;
                    }
                }
                if (count_true == distanceMap.size()) {
                    std::cout << "res found for point: ";
                    p.print();
                    std::cout << std::endl;
                    res = 4000000 * p.x + p.y;
                }
            }
        }
    }
    return res;
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
    long long res = getResult(data);
    std::cout << "Result:" << std::endl; 
    std::cout << res << std::endl;
    return 0;
}

