#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

const int BOARD_SIZE = 8;
const int CORNER_SIZE = 3;
char board[BOARD_SIZE][BOARD_SIZE];

void initBoard() {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = '.';

    for (int i = 0; i < CORNER_SIZE; ++i)
        for (int j = 0; j < CORNER_SIZE - i; ++j)
            board[i][j] = 'W';

    for (int i = 0; i < CORNER_SIZE; ++i)
        for (int j = 0; j < CORNER_SIZE - i; ++j)
            board[BOARD_SIZE - 1 - i][BOARD_SIZE - 1 - j] = 'B';
}

TEST_CASE("initBoard sets up the board correctly") {
    initBoard();

    for (int i = 0; i < CORNER_SIZE; ++i)
        for (int j = 0; j < CORNER_SIZE - i; ++j)
            CHECK(board[i][j] == 'W');

    for (int i = 0; i < CORNER_SIZE; ++i)
        for (int j = 0; j < CORNER_SIZE - i; ++j)
            CHECK(board[BOARD_SIZE - 1 - i][BOARD_SIZE - 1 - j] == 'B');

	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {

			bool in_white_corner = (i < CORNER_SIZE && j < CORNER_SIZE - i);
			bool in_black_corner = false;
			for (int bi = 0; bi < CORNER_SIZE; ++bi)
				for (int bj = 0; bj < CORNER_SIZE - bi; ++bj)
					if (i == BOARD_SIZE - 1 - bi && j == BOARD_SIZE - 1 - bj)
						in_black_corner = true;

			if (!in_white_corner && !in_black_corner)
				CHECK(board[i][j] == '.');
		}
	}

}
