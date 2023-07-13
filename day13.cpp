#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <variant>
#include <ranges>


namespace detail {
    struct packet : std::variant<int, std::vector<packet>> {
        using val = int;
        using seq = std::vector<packet>;

        using variant::variant;
        explicit packet(val v) : variant(v) { }
        explicit packet(seq const& v) : variant(v) { }

        friend auto operator<(packet const& a, packet const& b) -> bool;
    };

    struct comparison_visitor {
        auto operator()(packet::val a, packet::val b) const -> bool {
            return a < b;
        }

        auto operator()(packet::seq const& a, packet::seq const& b) const -> bool {
            return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
        }
        
        auto operator()(packet::val a, std::vector<packet> const& b) const -> bool {
            return (*this)(packet::seq{a}, b);
        }
        auto operator()(std::vector<packet> const& a, packet::val b) -> bool {
            return (*this)(a, packet::seq{b});
        }
    };

    auto operator<(packet const& a, packet const& b) -> bool {
        return std::visit(comparison_visitor{}, a, b);
    }

    auto parse(std::string_view& s) -> packet {
        if (s[0] == '[' && s[1] == ']') {
            s.remove_prefix(2);
            return packet{packet::seq{}}; // empty sequence
        }
        if (s[0] == '[') {
            packet::seq seq;
            while (s[0] != ']') {
                s.remove_prefix(1);
                seq.push_back(parse(s));
            }
            s.remove_prefix(1);
            return seq;
        }
        auto const* it = s.begin();
        while (it != s.end()) {
            std::string c(&(*it), 1);
            if (c == "," || c == "]") {
                break;
            }
            it++;
        }
        std::string_view v{s.begin(), static_cast<std::size_t>(std::distance(s.data(), it))};
        auto res = std::stoi(std::string(v));
        s.remove_prefix(v.size());
        return packet{res};
    }


    auto parse(std::string const& s) -> packet {
        std::string_view v{s.data(), s.size()};
        return parse(v);
    }
}


std::vector<std::string> get_data(std::string fName) {
    std::fstream input_file;
    input_file.open(fName, std::ios::in);
    std::vector<std::string> data;
    if (input_file.is_open()) {
        std::string s;
        while (std::getline(input_file, s)) {
            data.push_back(s);
        }
        input_file.close();
    } else {
        throw std::runtime_error("Input open failed!");
    }
    return data;
}


int main() {
    std::string fName = "day13_input.txt";
    auto data = get_data(fName);
    

    using detail::packet;
    using detail::parse;
    int result = 0;    
    auto p1{parse("[[2]]")};
    auto p2{parse("[[6]]")};
    auto i1{0}, i2{0};
    for (auto i = 0, j = 0; i < data.size(); i+=3, j++) {
        auto a = parse(data[i]);
        auto b = parse(data[i+1]);
        result += (j + 1) * (a < b);
        i1 += (a < p1) + (b < p1);
        i2 += (a < p2) + (b < p2);
    }
    
    std::cout << result << " " << (i1+1)*(i2+2) << std::endl; 
    return 0;
}

