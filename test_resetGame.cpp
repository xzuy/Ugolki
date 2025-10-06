#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "ai.h"

const int BOARD_SIZE = 8;
const int CORNER_SIZE = 4;
std::vector<std::vector<char>> board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '.'));
std::vector<std::vector<bool>> inOpponentCorner(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, false));
int blackMoves = 20;
int whiteMoves = 20;
std::vector<std::string> moveHistory;
size_t moveNumber = 0;

int selectedX;
int selectedY;
bool pieceSelected;
bool playerTurn;

void resetGame() {
    selectedX = -1;
    selectedY = -1;
    pieceSelected = false;
    playerTurn = true;
    blackMoves = 20;
    whiteMoves = 20;
    moveNumber = 0;
    moveHistory.clear();
    for(int i=0;i<BOARD_SIZE;i++)
        for(int j=0;j<BOARD_SIZE;j++)
            board[i][j] = '.';
}

TEST_CASE("resetGame resets game state correctly") {
    selectedX = 5; selectedY = 5; pieceSelected = true; playerTurn = false;
    blackMoves = 5; whiteMoves = 3; moveNumber = 10;
    moveHistory.push_back("fake move");
    board[0][0] = 'B';

    resetGame();

    CHECK(selectedX == -1);
    CHECK(selectedY == -1);
    CHECK(pieceSelected == false);
    CHECK(playerTurn == true);
    CHECK(blackMoves == 20);
    CHECK(whiteMoves == 20);
    CHECK(moveNumber == 0);
    CHECK(moveHistory.empty());
    for(int i=0;i<BOARD_SIZE;i++)
        for(int j=0;j<BOARD_SIZE;j++)
            CHECK(board[i][j] == '.');
}
