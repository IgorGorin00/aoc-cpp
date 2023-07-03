#include <iostream>
#include <fstream>
#include <string>
#include <map>

int check_result(char they_play, char you_play) {
    int result;
    if(
            (they_play == 'A' && you_play == 'Y') ||
            (they_play == 'B' && you_play == 'Z') ||
            (they_play == 'C' && you_play == 'X')
      ) {
        result = 6;
    } else if(
            (they_play == 'A' && you_play == 'X') ||
            (they_play == 'B' && you_play == 'Y') ||
            (they_play == 'C' && you_play == 'Z')
            ) {
        result = 3;
    } else {
        result = 0;
    }
    return result;
}


int main() {
    char rock_they = 'A';
    char paper_they = 'B';
    char scissors_they = 'C';

    char rock_you = 'X';
    char paper_you = 'Y';
    char scisoors_you = 'Z';

    int rock_score = 1;
    int paper_score = 2;
    int scissors_score = 3;

    int loss_score = 0;
    int draw_score = 3;
    int win_score = 6;

    std::map<char, int> play_scores;
    play_scores['X'] = 1;
    play_scores['Y'] = 2;
    play_scores['Z'] = 3;

    std::fstream input_file;
    input_file.open("day2_input.txt", std::ios::in);
    if (input_file.is_open()) {
        int total_score = 0;
        int i = 0;
        int early_stop = 10;
        std::string s;
        while (std::getline(input_file, s)) {
            char they_play = s[0];
            char you_play = s[2];
            int result_score = check_result(they_play, you_play);
            int play_score = play_scores[you_play];
            total_score += result_score;
            total_score += play_score;
        }
        
        std::cout << total_score << "\n";
        input_file.close();
    }


    return 0;
}
