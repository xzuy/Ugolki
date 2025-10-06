#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <SFML/Graphics.hpp>
#include <vector>

const int BOARD_SIZE = 8;
const int CELL_SIZE = 160;
const int BORDER = 80;
const int WINDOW_WIDTH = BOARD_SIZE * CELL_SIZE + BORDER * 2;
const int WINDOW_HEIGHT = BOARD_SIZE * CELL_SIZE + BORDER * 2;

std::vector<std::vector<char>> board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '.'));
int selectedX = -1;
int selectedY = -1;
bool pieceSelected = false;

void drawBoard(sf::RenderWindow& window, int selX, int selY, bool selPiece) {
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            cell.setPosition(BORDER + j * CELL_SIZE + 1.f, BORDER + (BOARD_SIZE - 1 - i) * CELL_SIZE + 1.f);
            cell.setFillColor((i + j) % 2 == 0 ? sf::Color(180, 180, 180) : sf::Color(220, 220, 220));
            window.draw(cell);
        }
    }

    if (selPiece && selX >= 0 && selY >= 0) {
        sf::RectangleShape hl(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
        hl.setPosition(BORDER + selY * CELL_SIZE + 1.f, BORDER + (BOARD_SIZE - 1 - selX) * CELL_SIZE + 1.f);
        hl.setFillColor(sf::Color(255, 255, 0, 100));
        window.draw(hl);
    }
}

// --- Тест ---
TEST_CASE("drawBoard renders without crashing") {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "DrawBoard Test");

    selectedX = 2;
    selectedY = 3;
    pieceSelected = true;

    CHECK_NOTHROW(drawBoard(window, selectedX, selectedY, pieceSelected));
}
