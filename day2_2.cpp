#include <iostream>
#include <fstream>
#include <string>
#include <map>


int check_result(char they_play, char you_play) {
    if (you_play == 'Z') {
        if (they_play == 'A') {
            return 2;
        } else if (they_play == 'B') {
            return 3;
        } else if (they_play == 'C') {
            return 1;
        }
    } else if (you_play == 'Y') {
        if (they_play == 'A') {
            return 1;
        } else if (they_play == 'B') {
            return 2;
        } else if (they_play == 'C') {
            return 3;
        }
    } else if (you_play == 'X') {
        if (they_play == 'A') {
            return 3;
        } else if (they_play == 'B') {
            return 1;
        } else if (they_play == 'C') {
            return 2;
        }
    }
    return 0;
}

int main() {
    char rock_they = 'A';
    char paper_they = 'B';
    char scissors_they = 'C';

    char you_lose = 'X';
    char you_draw = 'Y';
    char you_win = 'Z';

    int rock_score = 1;
    int paper_score = 2;
    int scissors_score = 3;

    int loss_score = 0;
    int draw_score = 3;
    int win_score = 6;

    std::map<char, int> result_scores;
    result_scores['X'] = 0;
    result_scores['Y'] = 3;
    result_scores['Z'] = 6;

    std::fstream input_file;
    input_file.open("day2_input.txt", std::ios::in);
    if (input_file.is_open()) {
        int total_score = 0;
        std::string s;
        while (std::getline(input_file, s)) {
            char they_play = s[0];
            char you_play = s[2];
            int play_score = check_result(they_play, you_play);
            int result_score = result_scores[you_play];
            total_score += result_score;
            total_score += play_score;
        }
        
        std::cout << total_score << "\n";
        input_file.close();
    }


    return 0;
}
