#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "ai.h"
#include <vector>

extern std::vector<std::vector<char>> board;
extern std::vector<std::vector<bool>> inOpponentCorner;
extern int blackMoves;
extern int whiteMoves;
extern size_t moveNumber;
extern const int BOARD_SIZE;
extern const int CORNER_SIZE;

TEST_CASE("isValidMove basic checks") {

    for(int i=0; i<BOARD_SIZE; ++i)
        for(int j=0; j<BOARD_SIZE; ++j) {
            board[i][j] = '.';
            inOpponentCorner[i][j] = false;
        }

    board[2][2] = 'B';
    board[2][3] = 'W';
    board[4][4] = 'B';
    inOpponentCorner[4][4] = true;

    SUBCASE("Valid single-step moves") {
        CHECK(isValidMove(2, 2, 2, 3, 'B') == false); 
        CHECK(isValidMove(2, 2, 3, 2, 'B') == true);
        CHECK(isValidMove(2, 2, 1, 2, 'B') == true);
        CHECK(isValidMove(2, 2, 2, 1, 'B') == true);
        CHECK(isValidMove(2, 2, 2, 4, 'B') == true); 
    }

    SUBCASE("Jump over piece") {
        CHECK(isValidMove(2, 2, 2, 4, 'B') == true); // 
        CHECK(isValidMove(2, 2, 4, 2, 'B') == false);
    }

    SUBCASE("Out of bounds") {
        CHECK(isValidMove(-1, 0, 0, 0, 'B') == false);
        CHECK(isValidMove(0, 0, 0, 8, 'B') == false);
    }

    SUBCASE("Blocked in opponent corner") {
        CHECK(isValidMove(4, 4, 4, 5, 'B') == false);
    }
}
