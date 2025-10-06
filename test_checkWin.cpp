#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "ai.h"
#include <vector>
#include <string>

extern const int BOARD_SIZE;
extern const int CORNER_SIZE;
extern std::vector<std::vector<char>> board;
extern std::vector<std::vector<bool>> inOpponentCorner;
extern int blackMoves;
extern int whiteMoves;
extern std::vector<std::string> moveHistory;
extern size_t moveNumber;

// --- Тесты ---
TEST_SUITE("checkWin") {

    TEST_CASE("Black wins with exactly 6 pieces in target triangle") {
        board = std::vector<std::vector<char>>(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '.'));
        board[0][0] = 'B';
        board[0][1] = 'B';
        board[0][2] = 'B';
        board[1][0] = 'B';
        board[1][1] = 'B';
        board[2][0] = 'B';

        CHECK(checkWin('B') == true);
        CHECK(checkWin('W') == false);
    }


    TEST_CASE("White wins with exactly 6 pieces in target triangle") {
        board = std::vector<std::vector<char>>(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '.'));

        board[7][7] = 'W';
        board[7][6] = 'W';
        board[7][5] = 'W';
        board[6][7] = 'W';
        board[6][6] = 'W';
        board[5][7] = 'W';

        CHECK(checkWin('W') == true);
        CHECK(checkWin('B') == false);
    }

    TEST_CASE("White does not win with only 5 pieces in triangle") {
        board = std::vector<std::vector<char>>(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '.'));
        board[7][7] = 'W';
        board[7][6] = 'W';
        board[7][5] = 'W';
        board[6][7] = 'W';
        board[6][6] = 'W';
        CHECK(checkWin('W') == false);
    }

    TEST_CASE("Pieces outside triangle don't count for black") {
        board = std::vector<std::vector<char>>(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '.'));
        board[0][0] = 'B';
        board[0][1] = 'B';
        board[0][2] = 'B';
        board[1][0] = 'B';
        board[1][1] = 'B';
        board[3][1] = 'B';
        board[2][2] = 'B';
        CHECK(checkWin('B') == false);
    }


    TEST_CASE("Mixed pieces don't cause false win") {
        board = std::vector<std::vector<char>>(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '.'));
        board[0][0] = 'B'; board[0][1] = 'B'; board[0][2] = 'B';
        board[1][0] = 'W'; board[1][1] = 'W'; board[2][0] = 'W';
        CHECK(checkWin('B') == false);
        CHECK(checkWin('W') == false);
    }

    TEST_CASE("Empty board - no winners") {
        board = std::vector<std::vector<char>>(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '.'));
        CHECK(checkWin('B') == false);
        CHECK(checkWin('W') == false);
    }

    TEST_CASE("Boundary conditions for black triangle") {
        board = std::vector<std::vector<char>>(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '.'));
        board[0][3] = 'B';
        board[1][2] = 'B';
        board[2][1] = 'B';
        board[3][0] = 'B';
        board[0][2] = 'B';
        board[1][1] = 'B';
        CHECK(checkWin('B') == true);
    }

    TEST_CASE("Boundary conditions for white triangle") {
        board = std::vector<std::vector<char>>(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '.'));
        board[7][4] = 'W';
        board[6][5] = 'W';
        board[5][6] = 'W';
        board[4][7] = 'W';
        board[7][5] = 'W';
        board[6][6] = 'W';
        CHECK(checkWin('W') == true);
    }
}
