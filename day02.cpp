#include <fstream>
#include <iostream>

int part1(char opponent, char move) {
	// convert to numbers 0-2 for easier comparison
	int opponent_number = opponent - 65;
	int move_number = move - 88;

	// add points based on the move you make
	int score = move_number + 1;
	// 3 points for a draw
	if (move_number == opponent_number)
		return score + 3;
	// 6 points for a win
	if (move_number - opponent_number == 1 || move_number - opponent_number == -2)
		return score + 6;
	// no additional points for a loss
	return score;
}

int part2(char opponent, char outcome) {
	// convert to number 0-2
	int opponent_number = opponent - 65;

	int score = 0;
	// no additional points for a loss
	if (outcome == 'X') {
		// -1 is not wrapped to 0 so treat as special case
		if (opponent_number == 0) 
			return 3;
		return ((opponent_number - 1) % 3) + 1;
	}
	// 3 points for a draw
	else if (outcome == 'Y')
		return opponent_number + 1 + 3;
	// 6 points for a win
	return ((opponent_number + 1) % 3) + 1 + 6;
}

int main() {
	std::ifstream file("resources/day02.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	std::string line;
	int score1 = 0;
	int score2 = 0;

	while (getline(file, line)) {
		score1 += part1(line[0], line[2]);
		score2 += part2(line[0], line[2]);
	}
	file.close();

	std::cout << "Part 1: " << score1 << "\n";
	std::cout << "Part 1: " << score2 << "\n";

	return 0;
}
