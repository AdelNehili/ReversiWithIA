#include <iostream>
#include <algorithm>
#include "game.h"

void printBoard(int matrix[8][8]){
	std::cout << "\n  a b c d e f g h  \n";
	for (int i = 0; i < 8; i++) {
		std::cout << i+1 << " ";
		for (int j = 0; j < 8; j++) {
			if (matrix[i][j] == 1) {
				std::cout << "X ";
			} else if (matrix[i][j] == -1) {
				std::cout << "O ";
			} else {
				std::cout << ". ";
			}
		}
		std::cout << i+1 << "\n";
	}
	std::cout << "  a b c d e f g h  \n\n";
}

void printScore(int matrix[8][8]){
	int scoreP1 = 0;
	int scoreP2 = 0;
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (matrix[i][j] == 1){
				scoreP1++;
			} else if (matrix[i][j] == -1) {
				scoreP2++;
			}
		}
	}
	std::cout << "GAME OVER\n" << "score is :\n" << "P1 : " << scoreP1 << "\nP2 : " << scoreP2 << std::endl;
}

bool MOVE_TEST(int matrix[8][8], int player, int row, int col){
    if (matrix[row][col] == 0) {
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				int k = 1;
				while ( col+k*i >= 0 && col+k*i <= 7 && row+k*j >= 0 && row+k*j <= 7 && !(i==0 && j==0)) {
					// tant que les indices restent bien dans les limites du tableau, et que i et j ne sont pas nuls en même temps
					if (matrix[row+k*j][col+k*i] == -player) {
						k++;
					} else if (matrix[row+k*j][col+k*i] == player && k>1) {
						return true;
					} else {
						break;
					}
				}
			}
		}
	}
	return false;
}

bool MOVE_EXISTS(int matrix[8][8], int player){
    for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (MOVE_TEST(matrix, player, i, j)) {
				return true;
			}
		}
	}
	return false;
}

bool TERMINAL_TEST(int matrix[8][8]){
    if (MOVE_EXISTS(matrix, 1) || MOVE_EXISTS(matrix, -1)) {
		return false;
	}
	return true;
}

std::string ACTIONS(int matrix[8][8], int player){
	std::string actions = "";
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (MOVE_TEST(matrix, player, i, j)) {
				actions += std::to_string(i);
				actions += std::to_string(j);
				// format : row, col
			}
		}
	}
	return actions;
}

int EVAL(int weight[8][8], int matrix[8][8], int MAX){
	int sum = 0;
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (matrix[i][j] == MAX) {
				sum += weight[i][j]; // DOIT TJRS REPRÉSENTER L'UTILITÉE POUR MAX !!! (JOUEUR QUI INVOQUE MINIMAX) donc il faut retravailler le code int player
			}
		}
	}
	return sum;
}


bool CUTOFF_TEST(int matrix[8][8], int depth){
	int max_depth = 6;
	if ( (depth > max_depth) || TERMINAL_TEST(matrix) ){
		return true;
	}
	return false;
}

int MAX_VALUE(int depth, int weight[8][8], int matrix[8][8], int player, int MAX){
	if ( CUTOFF_TEST(matrix, depth) ){
		// std::cout << "MAX_VALUE EVAL : " << EVAL(weight, matrix, MAX) << std::endl;
		return EVAL(weight, matrix, MAX);
	}
	int v = -10000;
	player = -player;
	std::string actions = ACTIONS(matrix, player);
	for (int i = 0; i < actions.length(); i += 2) {
		int row = (int)actions[i]  - (int)('0');
		int col = (int)actions[i + 1] - (int)('0');

		int matrix_copy[8][8] = {0};
		for (int x = 0 ; x < 8; x++){
  			for (int y = 0 ; y < 8; y++){
    			matrix_copy[x][y] = matrix[x][y];
  			}
		}
		EXECUTE_MOVE(matrix_copy, player, row, col);
		// std::cout << "MAX_VALUE depth : " << depth << std::endl;
		// printBoard(matrix_copy); // DEBUG

		v = std::max(v, MIN_VALUE(depth + 1, weight, matrix_copy, player, MAX));
	}
	// std::cout << "MAX_VALUE v : " << v << std::endl;
	return v;
}



int MIN_VALUE(int depth, int weight[8][8], int matrix[8][8], int player, int MAX){
	if ( CUTOFF_TEST(matrix, depth) ){
		// std::cout << "MIN_VALUE EVAL : " << EVAL(weight, matrix, MAX) << std::endl;
		return EVAL(weight, matrix, MAX);
	}
	int v = 10000;
	player = -player;
	std::string actions = ACTIONS(matrix, player);
	for (int i = 0; i < actions.length(); i += 2) {
		int row = (int)actions[i]  - (int)('0');
		int col = (int)actions[i + 1] - (int)('0');

		int matrix_copy[8][8] = {0};
		for (int x = 0 ; x < 8; x++){
  			for (int y = 0 ; y < 8; y++){
    			matrix_copy[x][y] = matrix[x][y];
  			}
		}
		EXECUTE_MOVE(matrix_copy, player, row, col);
		// std::cout << "MIN_VALUE depth : " << depth << std::endl;
		// printBoard(matrix_copy); // DEBUG

		v = std::min(v, MAX_VALUE(depth + 1, weight, matrix_copy, player, MAX));
	}
	return v;
}


std::string MINIMAX_DECISION(int weight[8][8], int matrix[8][8], int player){
	std::string action = "ER";
	std::string actions = ACTIONS(matrix, player);
	int v = -10001;
	int depth = 1;
	int MAX = player;
	for (int i = 0; i < actions.length(); i += 2) {
		// std::cout << "row : " << actions[i] << "; col : " << actions[i+1] << std::endl;
		int row = (int)actions[i]  - (int)('0');
		int col = (int)actions[i + 1] - (int)('0');

		int matrix_copy[8][8] = {0};
		for (int x = 0 ; x < 8; x++){
  			for (int y = 0 ; y < 8; y++){
    			matrix_copy[x][y] = matrix[x][y];
  			}
		}
		EXECUTE_MOVE(matrix_copy, player, row, col);
		// std::cout << "depthOOOO : " << depth << std::endl;
		// printBoard(matrix_copy); // DEBUG

		// std::cout << "bobo : " << MIN_VALUE(depth + 1, weight, matrix, player) << std::endl;
		int temp = MIN_VALUE(depth + 1, weight, matrix_copy, player, MAX);
		if (temp > v) {
			v = temp;
			action = "";
			action += std::to_string(row);
			action += std::to_string(col);
		}
	}
	return action;
}


void EXECUTE_MOVE(int matrix[8][8], int player, int row, int col){
	bool MoveIsValid = false;
	if (matrix[row][col] == 0) {
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				int k = 1;
				while ( col+k*i >= 0 && col+k*i <= 7 && row+k*j >= 0 && row+k*j <= 7 && !(i==0 && j==0)) {
					// tant que les indices restent bien dans les limites du tableau, et que i et j ne sont pas nuls en même temps
					if (matrix[row+k*j][col+k*i] == -player) {
						k++;
					} else if (matrix[row+k*j][col+k*i] == player && k>1) {
						for (int l = k-1; l >= 0; l--) {
								matrix[row+l*j][col+l*i] = player;
						}
						MoveIsValid = true;
						break;
					} else {
						break;
					}
				}
			}
		}
	}
}
