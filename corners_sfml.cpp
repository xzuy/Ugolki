#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <windows.h>

using namespace std;

const int BOARD_SIZE = 8;
const int CORNER_SIZE = 4;
const int CELL_SIZE = 160;
const int BORDER = 80;

const int WINDOW_SIZE = BOARD_SIZE * CELL_SIZE + BORDER * 2;
const int STATUS_HEIGHT = 80;
const int WINDOW_WIDTH = WINDOW_SIZE;
const int WINDOW_HEIGHT = WINDOW_SIZE + STATUS_HEIGHT;

enum Piece { EMPTY = '.', WHITE = 'W', BLACK = 'B' };

vector<vector<char>> board(BOARD_SIZE, vector<char>(BOARD_SIZE, EMPTY));
vector<vector<int>> skipTurns(BOARD_SIZE, vector<int>(BOARD_SIZE, 0));
vector<vector<bool>> inOpponentCorner(BOARD_SIZE, vector<bool>(BOARD_SIZE, false));

sf::Font font;
sf::Text statusText;

void initBoard() {
    for (int i = 0; i < CORNER_SIZE; ++i)
        for (int j = 0; j < CORNER_SIZE - i; ++j) {
            board[i][j] = WHITE;
            skipTurns[i][j] = 0;
        }

    for (int i = 0; i < CORNER_SIZE; ++i)
        for (int j = 0; j < CORNER_SIZE - i; ++j) {
            int x = BOARD_SIZE - 1 - i;
            int y = BOARD_SIZE - 1 - j;
            board[x][y] = BLACK;
            skipTurns[x][y] = 0;
        }
}

bool isInside(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

bool isValidMove(int x1, int y1, int x2, int y2, char player) {
    if (!isInside(x1, y1) || !isInside(x2, y2))
        return false;
    if (board[x1][y1] != player || board[x2][y2] != EMPTY)
        return false;

    if (player == BLACK && inOpponentCorner[x1][y1])
        return false;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1))
        return true;

    if ((dx == 2 && dy == 0) || (dx == 0 && dy == 2)) {
        int mx = (x1 + x2) / 2;
        int my = (y1 + y2) / 2;
        if (board[mx][my] != EMPTY)
            return true;
    }

    return false;
}

bool makeMove(int x1, int y1, int x2, int y2, char player) {
    if (isValidMove(x1, y1, x2, y2, player)) {
        board[x2][y2] = player;
        board[x1][y1] = EMPTY;

        if (player == BLACK) {
            skipTurns[x2][y2] = 5;
            skipTurns[x1][y1] = 0;
        } else if (player == WHITE) {
            skipTurns[x2][y2] = 0;
            skipTurns[x1][y1] = 0;
        }

        if (player == BLACK) {
            // Black pieces entering the white corner (top-left triangle)
            if (x2 >= 0 && x2 <= 3 && y2 >= 0 && y2 <= 3 - x2) {
                inOpponentCorner[x2][y2] = true;
                skipTurns[x2][y2] = 0;
                cout << "Black piece at (" << x2 + 1 << "," << y2 + 1 << ") entered white's corner and can no longer move\n";
            }
        }

        return true;
    }
    return false;
}

bool checkWin(char player) {
    int count = 0;

    if (player == WHITE) {
        // Верхний правый угол (треугольник)
        for (int x = BOARD_SIZE - CORNER_SIZE; x < BOARD_SIZE; ++x) {
            for (int y = BOARD_SIZE - CORNER_SIZE; y < BOARD_SIZE; ++y) {
                if ((x + y) >= (BOARD_SIZE - 1) + (BOARD_SIZE - CORNER_SIZE)) {
                    if (board[x][y] == WHITE)
                        ++count;
                }
            }
        }
        return count >= 6;  // Победа, если занято не менее 6 клеток
    } 
    else if (player == BLACK) {
        // Нижний левый угол (треугольник)
        count = 0;
        for (int x = 0; x < CORNER_SIZE; ++x) {
            for (int y = 0; y < CORNER_SIZE; ++y) {
                if ((x + y) <= CORNER_SIZE - 1) {
                    if (board[x][y] == BLACK)
                        ++count;
                }
            }
        }
        return count >= 6;
    }

    return false;
}


int distanceToCorner(int x, int y, char player) {
    int targetX = (player == BLACK) ? 0 : BOARD_SIZE - 1;
    int targetY = (player == BLACK) ? 0 : BOARD_SIZE - 1;
    return abs(targetX - x) + abs(targetY - y);
}

void decrementSkipTurns() {
    for (int x = 0; x < BOARD_SIZE; ++x)
        for (int y = 0; y < BOARD_SIZE; ++y)
            if (skipTurns[x][y] > 0)
                skipTurns[x][y]--;
}

bool makeAIMove() {
    struct Move {
        int x1, y1, x2, y2, dist;
    };

    const int dxs[4] = {1, -1, 0, 0};
    const int dys[4] = {0, 0, 1, -1};

    vector<Move> possibleMoves;

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            if (board[x][y] == BLACK && skipTurns[x][y] == 0 && !inOpponentCorner[x][y]) {
                for (int dir = 0; dir < 4; ++dir) {
                    int nx = x + dxs[dir];
                    int ny = y + dys[dir];
                    if (isValidMove(x, y, nx, ny, BLACK)) {
                        possibleMoves.push_back({x, y, nx, ny, distanceToCorner(nx, ny, BLACK)});
                    }
                    nx = x + 2 * dxs[dir];
                    ny = y + 2 * dys[dir];
                    if (isValidMove(x, y, nx, ny, BLACK)) {
                        possibleMoves.push_back({x, y, nx, ny, distanceToCorner(nx, ny, BLACK)});
                    }
                }
            }
        }
    }

    if (possibleMoves.empty())
        return false;

    Move bestMove = possibleMoves[0];
    for (auto& m : possibleMoves) {
        if (m.dist < bestMove.dist) {
            bestMove = m;
        }
    }

    decrementSkipTurns();

    makeMove(bestMove.x1, bestMove.y1, bestMove.x2, bestMove.y2, BLACK);

    char colFrom = 'A' + bestMove.y1;
    int rowFrom = bestMove.x1 + 1;
    char colTo = 'A' + bestMove.y2;
    int rowTo = bestMove.x2 + 1;

    cout << "AI move: " << colFrom << rowFrom << " -> " << colTo << rowTo << "\n";

    statusText.setString("AI move: " + string(1, colFrom) + to_string(rowFrom) + " -> " + string(1, colTo) + to_string(rowTo));

    return true;
}

void drawBoard(sf::RenderWindow& window, int selectedX, int selectedY, bool pieceSelected) {
    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            cellShape.setPosition(BORDER + j * CELL_SIZE + 1.f, BORDER + (BOARD_SIZE - 1 - i) * CELL_SIZE + 1.f);

            if ((i + j) % 2 == 0)
                cellShape.setFillColor(sf::Color(220, 220, 220));
            else
                cellShape.setFillColor(sf::Color(180, 180, 180));

            window.draw(cellShape);

            if (board[i][j] == WHITE || board[i][j] == BLACK) {
                sf::CircleShape pieceShape(CELL_SIZE / 2.f - 20.f);
                pieceShape.setPosition(BORDER + j * CELL_SIZE + 20.f, BORDER + (BOARD_SIZE - 1 - i) * CELL_SIZE + 20.f);
                pieceShape.setFillColor(board[i][j] == WHITE ? sf::Color::White : sf::Color::Black);
                window.draw(pieceShape);
            }
        }
    }

    if (pieceSelected && isInside(selectedX, selectedY)) {
        sf::RectangleShape highlight(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
        highlight.setPosition(BORDER + selectedY * CELL_SIZE + 1.f, BORDER + (BOARD_SIZE - 1 - selectedX) * CELL_SIZE + 1.f);
        highlight.setFillColor(sf::Color(255, 255, 0, 100));
        window.draw(highlight);
    }

    sf::Text coordText;
    coordText.setFont(font);
    coordText.setCharacterSize(40);
    coordText.setFillColor(sf::Color::Black);

    for (int i = 0; i < BOARD_SIZE; ++i) {
        char c = 'A' + i;

        coordText.setString(c);
        coordText.setPosition(BORDER + i * CELL_SIZE + CELL_SIZE / 2.f - 12.f, BORDER / 4.f + 5.f);
        window.draw(coordText);

        coordText.setPosition(BORDER + i * CELL_SIZE + CELL_SIZE / 2.f - 12.f, BORDER + BOARD_SIZE * CELL_SIZE + BORDER / 12.f);
        window.draw(coordText);
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        int n = i + 1;

        coordText.setString(to_string(n));
        coordText.setPosition(BORDER / 2.f, BORDER + (BOARD_SIZE - 1 - i) * CELL_SIZE + CELL_SIZE / 2.f - 24.f);
        window.draw(coordText);

        coordText.setPosition(BORDER + BOARD_SIZE * CELL_SIZE + BORDER / 3.f, BORDER + (BOARD_SIZE - 1 - i) * CELL_SIZE + CELL_SIZE / 2.f - 24.f);
        window.draw(coordText);
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    cout << "=== Corners Game with Coordinate Frame ===\n";
    initBoard();

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Corners Game 8x8");

    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Failed to load font arial.ttf" << endl;
        return -1;
    }

    statusText.setFont(font);
    statusText.setCharacterSize(24);
    statusText.setFillColor(sf::Color::Black);
    statusText.setPosition(BORDER, WINDOW_SIZE + 10.f);
    statusText.setString("Your move");

    int selectedX = -1, selectedY = -1;
    bool pieceSelected = false;
    bool playerTurn = true;

    int totalMoves = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (playerTurn && event.type == sf::Event::MouseButtonPressed) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                int gridX = (BOARD_SIZE - 1) - ((mouseY - BORDER) / CELL_SIZE);
                int gridY = (mouseX - BORDER) / CELL_SIZE;

                if (isInside(gridX, gridY)) {
                    if (!pieceSelected && board[gridX][gridY] == WHITE) {
                        selectedX = gridX;
                        selectedY = gridY;
                        pieceSelected = true;
                        statusText.setString("Selected: " + string(1, 'A' + selectedY) + to_string(selectedX + 1));
                    } else if (pieceSelected) {
                        if (makeMove(selectedX, selectedY, gridX, gridY, WHITE)) {
                            pieceSelected = false;
                            statusText.setString("Move made");
                            playerTurn = false;
                            totalMoves++;
                        } else {
                            statusText.setString("Invalid move");
                        }
                    }
                }
            }
        }

        if (!playerTurn && totalMoves < 40) {
            bool aiMoved = makeAIMove();
            if (!aiMoved) {
                statusText.setString("AI has no moves");
            }
            playerTurn = true;
            totalMoves++;
        }

        if (checkWin(WHITE)) {
            statusText.setString("White wins!");
            window.clear(sf::Color::White);
            drawBoard(window, selectedX, selectedY, pieceSelected);
            window.draw(statusText);
            window.display();
            Sleep(3000);
            break;
        } else if (checkWin(BLACK)) {
            statusText.setString("Black wins!");
            window.clear(sf::Color::White);
            drawBoard(window, selectedX, selectedY, pieceSelected);
            window.draw(statusText);
            window.display();
            Sleep(3000);
            break;
        }

        if (totalMoves >= 40) {
            int whiteCount = 0, blackCount = 0;

            // Count white pieces in black corner (bottom-right)
            for (int i = BOARD_SIZE - CORNER_SIZE; i < BOARD_SIZE; ++i)
                for (int j = BOARD_SIZE - CORNER_SIZE; j < BOARD_SIZE; ++j)
                    if ((i + j) >= 2 * BOARD_SIZE - CORNER_SIZE - 1 && board[i][j] == WHITE)
                        whiteCount++;

            // Count black pieces in white corner (top-left)
            for (int i = 0; i < CORNER_SIZE; ++i)
                for (int j = 0; j < CORNER_SIZE; ++j)
                    if (i + j <= CORNER_SIZE - 1 && board[i][j] == BLACK)
                        blackCount++;

            string result;
            if (whiteCount > blackCount)
                result = "WHITE wins (you)!";
            else if (blackCount > whiteCount)
                result = "BLACK wins (AI)!";
            else
                result = "Draw!";

            statusText.setString("Game over. " + result + " Press any key or click to exit.");

            window.clear(sf::Color::White);
            drawBoard(window, selectedX, selectedY, pieceSelected);
            window.draw(statusText);
            window.display();

            bool waiting = true;
            while (waiting) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        waiting = false;
                        window.close();
                    } else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
                        waiting = false;
                    }
                }
            }

            break;
        }

        window.clear(sf::Color::White);
        drawBoard(window, selectedX, selectedY, pieceSelected);
        window.draw(statusText);
        window.display();
    }

    return 0;
}
