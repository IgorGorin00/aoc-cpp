#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <cassert>

template <typename T>
void printVector(const std::vector<T>& vec) {
    for (auto const& e : vec) {
        std::cout << e;
    }
    std::cout << std::endl;
}

struct Point {
    int x;
    int y;
    Point(int x_, int y_) : x(x_), y(y_) {}

    void print() const {
        std::cout << "(" << x << ", " << y << ")  ";
    }
};


struct Shape {
    std::vector<Point> Points;
    Shape (const std::vector<Point>& points) {
        if (points.size() < 2) {
            Points = points;
            return;
        }

        for (size_t i = 0; i < points.size() - 1; i++) {
            const Point& p1 = points[i];
            const Point& p2 = points[i + 1];
            if (p1.x == p2.x) {
                for (int y = std::min(p1.y, p2.y); y <= std::max(p1.y, p2.y); y++) {
                    Points.emplace_back(p1.x, y);
                }
            } else if (p1.y == p2.y) {
                for (int x = std::min(p1.x, p2.x); x <= std::max(p1.x, p2.x); x++) {
                    Points.emplace_back(x, p1.y);
                }
            }
        }
    }
    
    void print() const {
        for (auto const& p : Points) {
            p.print();
        }
    }
};

struct Figure {
    std::vector<Shape> Shapes;
    int maxX = -1;
    int maxY = -1;
    Figure(std::vector<Shape> shapes) : Shapes(shapes) {
        for (const Shape& sh : Shapes) {
            for (const Point& p : sh.Points) {
                if (p.x > maxX) {
                    maxX = p.x;
                }
                if (p.y > maxY) {
                    maxY = p.y;
                }
            }
        }
    }; 

    void addSandPoint(const Point p) {
        std::vector<Point> point_as_vector{p};
        Shape shape{point_as_vector};
        std::cout << "Adding shape ";
        shape.print();
        std::cout << std::endl;
        std::cout << Shapes.size() << std::endl << std::endl;

        Shapes.push_back(shape);
    }

    bool collides_down(const Point& p) {
        for (const Shape& shape : Shapes) {
            for (const Point& shape_p : shape.Points) {
                if (p.y == shape_p.y - 1 && p.x == shape_p.x) {
         //           std::cout << "Collides down with " << shape_p.x << ", " << shape_p.y << std::endl;
                    return true;
                }
            }
        }
        return false;
    }

    bool collides_diag_left(const Point& p) {
        for (const Shape& shape : Shapes) {
            for (const Point& shape_p : shape.Points) {
                if (p.y == shape_p.y - 1 && p.x == shape_p.x + 1) {
         //           std::cout << "Collides diag_left with " << shape_p.x << ", " << shape_p.y << std::endl;
                    return true;
                } 
            }
        }
        return false;
    }
    
    bool collides_diag_right(const Point& p) {
        for (const Shape& shape : Shapes) {
            for (const Point& shape_p : shape.Points) {
                if (p.y == shape_p.y - 1 && p.x == shape_p.x - 1) {
         //           std::cout << "Collides diag_right with " << shape_p.x << ", " << shape_p.y << std::endl;
                    return true;
                } 
            }
        }
        return false;
    }

    void print() {
        for (auto const& sh : Shapes) {
            sh.print();
        }
    }

    std::pair<int, int> getMaxVals() {
        std::pair<int, int> res{maxX, maxY};
        return res;
    }

    void draw() {
        std::pair<int, int> const max_vals = getMaxVals();
        int const& max_X = max_vals.first;
        int const& max_Y = max_vals.second;
        
        std::vector<std::vector<char>> matrix(max_Y + 1, std::vector<char>(max_X + 1, '.'));

        for (const Shape& sh : Shapes) {
            for (const Point& p : sh.Points) {
                matrix[p.y][p.x] = '#';
            }
        }

        for (const auto& row : matrix) {
            printVector(row);
        }
    }
};

Point get_point(std::string word, char delim) {
    std::vector<std::string> nums;
    size_t start = 0;
    size_t end = word.find(delim);
    while (end != std::string::npos) {
        nums.push_back(word.substr(start, end - start));
        start = end + 1;
        end = word.find(delim, start);
    }
    nums.push_back(word.substr(start));
    assert(nums.size() == 2 && "Not two numbers passed!");
    auto& x_str = nums[0];
    auto& y_str = nums[1];
    int x = -1;
    int y = -1;
    try {
        x = std::stoi(x_str);
        y = std::stoi(y_str);
    } catch(std::invalid_argument) {
        std::cout << "Int conversion failed with " << x_str << " " << y_str << std::endl;
        throw(std::runtime_error("Fail"));
    }
    Point res(x, y);
    return res;
}

Figure getData(std::string fName) {
    std::fstream input_file;
    input_file.open(fName, std::ios::in);
    if (input_file.is_open()) {
        
        std::string s;
        char delim = ',';
        std::vector<Shape> shapes_parsed;
        std::vector<Point> points_parsed;
        while (std::getline(input_file, s)) {
            std::istringstream ss(s);
            std::vector<std::string> words;
            std::string word;
            while (ss >> word) {
                if (word != "->") {
                    Point p = get_point(word, delim);
                    points_parsed.push_back(p);
                }
            }
            Shape sh(points_parsed);
            shapes_parsed.push_back(sh);
            points_parsed.clear();
        }
        Figure fig(shapes_parsed);
        return fig;

        input_file.close();
    } else {
        throw std::runtime_error("Input open failed!");
    }
    return Figure(std::vector<Shape>());
}

bool sandDropsInVoid(const Point& sandPoint, const Figure& fig) {
    return sandPoint.y == fig.maxY;
}

int addSand(std::pair<int, int>& startPos, Figure fig) {
    bool sand_drops_in_void = false;
    Point sand_point(startPos.first, startPos.second);
    int sand_in_rest_count = 0;
    while (1) {
        if (fig.collides_down(sand_point)) {
            if (fig.collides_diag_left(sand_point)) {
                if (fig.collides_diag_right(sand_point)) {
                    fig.addSandPoint(sand_point);
                    sand_in_rest_count++;
                    if (sand_point.x == 500 && sand_point.y == 0) {
                        break;
                    }
                    sand_point.x = startPos.first;
                    sand_point.y = startPos.second;
                } else {
                    sand_point.y++;
                    sand_point.x++;
                }
            } else {
                sand_point.y++;
                sand_point.x--;
            } 
        } else {
            sand_point.y++;
        }
    }
    return sand_in_rest_count;
}

int main() {
    std::string file_name = "day14_input.txt";
    Figure fig = getData(file_name);



    std::pair<int, int> sand_starting_pos{500, 0};

    std::pair<int, int> max_vals = fig.getMaxVals();
    int& maxX = max_vals.first;
    int& maxY = max_vals.second;
    std::cout << "Max X: " << max_vals.first << " Max Y: " << max_vals.second << std::endl;
    // 534, 168
    std::vector<Point> floor_points{Point(maxX - 1500, maxY + 2), Point(maxX + 1500, maxY + 2)};
    Shape floor{floor_points};
    fig.Shapes.push_back(floor);
    int res = addSand(sand_starting_pos, fig);
    std::cout << "Result: ";
    std::cout << res << std::endl;
    return 0;
}
