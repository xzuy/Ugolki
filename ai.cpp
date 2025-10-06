/**
 * @file ai.cpp
 * @brief Реализация искусственного интеллекта для настольной игры
 * 
 * Данный файл содержит реализацию AI для игры, включая алгоритм минимакс
 * с альфа-бета отсечением для выбора оптимальных ходов.
 */

#include "ai.h"
#include <algorithm>
#include <cmath>

using namespace std;

const int BOARD_SIZE = 8;        ///< Размер игрового поля
const int CORNER_SIZE = 4;       ///< Размер угловой зоны для победы

vector<vector<char>> board(BOARD_SIZE, vector<char>(BOARD_SIZE, '.'));          ///< Игровое поле
vector<vector<bool>> inOpponentCorner(BOARD_SIZE, vector<bool>(BOARD_SIZE, false)); ///< Флаги нахождения в углу противника
int blackMoves = 20;             ///< Оставшиеся ходы черных
int whiteMoves = 20;             ///< Оставшиеся ходы белых
vector<string> moveHistory;      ///< История ходов
size_t moveNumber = 0;           ///< Номер текущего хода

/**
 * @brief Проверяет, находится ли точка внутри игрового поля
 * 
 * @param x Координата X
 * @param y Координата Y
 * @return true если точка внутри поля, false в противном случае
 */
bool isInside(int x, int y) { 
    if (x < 0 || x >= BOARD_SIZE) return false;
    if (y < 0 || y >= BOARD_SIZE) return false;
    return true;
}

/**
 * @brief Проверяет валидность хода
 * 
 * @param x1 Начальная координата X
 * @param y1 Начальная координата Y
 * @param x2 Конечная координата X
 * @param y2 Конечная координата Y
 * @param player Игрок ('B' или 'W')
 * @return true если ход валиден, false в противном случае
 */

bool isValidMove(int x1, int y1, int x2, int y2, char player) {
    if (!isInside(x1, y1) || !isInside(x2, y2)) return false;
    if (board[x1][y1] != player || board[x2][y2] != '.') return false;
    if (player == 'B' && inOpponentCorner[x1][y1]) return false;

    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) return true;
    if ((dx == 2 && dy == 0) || (dx == 0 && dy == 2)) {
        int mx = (x1 + x2) / 2, my = (y1 + y2) / 2;
        if (board[mx][my] != '.') return true;
    }
    return false;
}

/**
 * @brief Выполняет ход на игровом поле
 * 
 * @param x1 Начальная координата X
 * @param y1 Начальная координата Y
 * @param x2 Конечная координата X
 * @param y2 Конечная координата Y
 * @param player Игрок ('B' или 'W')
 * @return true если ход выполнен успешно, false в противном случае
 */
 
bool makeMove(int x1, int y1, int x2, int y2, char player) {
    if (!isValidMove(x1, y1, x2, y2, player)) return false;
    board[x2][y2] = player;
    board[x1][y1] = '.';
    if (player == 'B' && x2 <= 3 && y2 <= 3 - x2) inOpponentCorner[x2][y2] = true;
    return true;
}

/**
 * @brief Проверяет условие победы для игрока
 * 
 * @param player Игрок ('B' или 'W')
 * @return true если игрок победил, false в противном случае
 */
bool checkWin(char player) {
    int count = 0;
    if (player == 'W') {
        for (int x = BOARD_SIZE - CORNER_SIZE; x < BOARD_SIZE; ++x)
            for (int y = BOARD_SIZE - CORNER_SIZE; y < BOARD_SIZE; ++y)
                if ((x + y) >= BOARD_SIZE - 1 + BOARD_SIZE - CORNER_SIZE && board[x][y] == 'W') ++count;
        return count >= 6;
    } else {
        for (int x = 0; x < CORNER_SIZE; ++x)
            for (int y = 0; y < CORNER_SIZE; ++y)
                if ((x + y) <= CORNER_SIZE - 1 && board[x][y] == 'B') ++count;
        return count >= 6;
    }
    return false;
}

/**
 * @brief Вычисляет расстояние от фишки до целевого угла
 * 
 * @param x Координата X фишки
 * @param y Координата Y фишки
 * @param player Игрок ('B' или 'W')
 * @return Расстояние в клетках до целевого угла
 */
int distanceToCorner(int x, int y, char player) {
    int targetX, targetY;
    
    if (player == 'B') {
        targetX = 0;
        targetY = 0;
    } else {
        targetX = BOARD_SIZE - 1;
        targetY = BOARD_SIZE - 1;
    }
    
    int distanceX = abs(targetX - x);
    int distanceY = abs(targetY - y);
    
    return distanceX + distanceY;
}

/**
 * @brief Оценивает текущее состояние игрового поля
 * 
 * Функция вычисляет эвристическую оценку позиции для алгоритма минимакс.
 * Положительные значения выгодны черным, отрицательные - белым.
 * 
 * @param boardState Текущее состояние поля
 * @param remainingBlackMoves Оставшиеся ходы черных
 * @param remainingWhiteMoves Оставшиеся ходы белых
 * @return Целочисленная оценка позиции
 */
int evaluateBoard(const vector<vector<char>> &boardState, int remainingBlackMoves, int remainingWhiteMoves) {
    int score = 0;
    for (int x = 0; x < BOARD_SIZE; ++x)
        for (int y = 0; y < BOARD_SIZE; ++y) {
            if (boardState[x][y] == 'B') {
                score -= distanceToCorner(x, y, 'B') * 10;
                if (x <= 3 && y <= 3 && (x + y) <= CORNER_SIZE - 1) score += 50;
            } else if (boardState[x][y] == 'W') {
                score += distanceToCorner(x, y, 'W') * 10;
                if (x >= BOARD_SIZE - 4 && y >= BOARD_SIZE - 4 && (x + y) >= 2*BOARD_SIZE - CORNER_SIZE - 1) score -= 50;
            }
        }
    if (remainingBlackMoves <= 5)
        for (int x = 0; x < CORNER_SIZE; ++x)
            for (int y = 0; y < CORNER_SIZE; ++y)
                if ((x + y) <= CORNER_SIZE - 1 && boardState[x][y] != 'B') score -= 20;
    return score;
}

/**
 * @brief Генерирует все возможные ходы для игрока
 * 
 * @param player Игрок ('B' или 'W')
 * @return Вектор всех возможных ходов
 */
vector<Move> generateMoves(char player) {
    vector<Move> moves;
    const int dxs[4] = {1, -1, 0, 0};
    const int dys[4] = {0, 0, 1, -1};
    for (int x = 0; x < BOARD_SIZE; x++)
        for (int y = 0; y < BOARD_SIZE; y++)
            if (board[x][y] == player)
                for (int dir = 0; dir < 4; dir++) {
                    int nx = x + dxs[dir], ny = y + dys[dir];
                    if (isValidMove(x, y, nx, ny, player)) moves.push_back({x,y,nx,ny});
                    nx = x + 2*dxs[dir]; ny = y + 2*dys[dir];
                    if (isValidMove(x, y, nx, ny, player)) moves.push_back({x,y,nx,ny});
                }
    return moves;
}

/**
 * @brief Реализует алгоритм минимакс с альфа-бета отсечением
 * 
 * @param depth Глубина поиска
 * @param isMaximizing true если максимизирующий игрок (черные), false если минимизирующий (белые)
 * @param alpha Значение альфа для отсечения
 * @param beta Значение бета для отсечения
 * @param remainingBlackMoves Оставшиеся ходы черных
 * @param remainingWhiteMoves Оставшиеся ходы белых
 * @return Оценка позиции для текущего игрока
 */
int minimax(int depth, bool isMaximizing, int alpha, int beta, int remainingBlackMoves, int remainingWhiteMoves) {
    if (depth == 0 || checkWin('B') || checkWin('W'))
        return evaluateBoard(board, remainingBlackMoves, remainingWhiteMoves);

    char player = isMaximizing ? 'B' : 'W';
    vector<Move> moves = generateMoves(player);
    if (moves.empty()) return evaluateBoard(board, remainingBlackMoves, remainingWhiteMoves);

    if (isMaximizing) {
        int maxEval = -1000000;
        for (auto &m : moves) {
            auto backupBoard = board;
            auto backupCorner = inOpponentCorner;
            makeMove(m.x1, m.y1, m.x2, m.y2, player);
            int eval = minimax(depth-1,false,alpha,beta,remainingBlackMoves-1,remainingWhiteMoves);
            board = backupBoard; inOpponentCorner = backupCorner;
            maxEval = max(maxEval,eval);
            alpha = max(alpha,eval);
            if(beta<=alpha) break;
        }
        return maxEval;
    } else {
        int minEval = 1000000;
        for(auto &m : moves){
            auto backupBoard = board;
            auto backupCorner = inOpponentCorner;
            makeMove(m.x1,m.y1,m.x2,m.y2,player);
            int eval = minimax(depth-1,true,alpha,beta,remainingBlackMoves,remainingWhiteMoves-1);
            board = backupBoard; inOpponentCorner = backupCorner;
            minEval = min(minEval,eval);
            beta = min(beta,eval);
            if(beta<=alpha) break;
        }
        return minEval;
    }
}

#include <chrono>

/**
 * @brief Выполняет ход искусственного интеллекта
 * 
 * Функция выбирает лучший ход для черных с использованием алгоритма минимакс
 * с ограничением по времени (500 мс).
 * 
 * @return true если ход выполнен успешно, false если нет возможных ходов
 */
bool makeAIMove() {
    vector<Move> moves = generateMoves('B');
    if (moves.empty()) return false;

    using namespace chrono;
    auto start = high_resolution_clock::now();
    int timeLimitMs = 500; 

    int bestValue = -1000000;
    Move bestMove = moves[0];

    for (auto &m : moves) {
        auto backupBoard = board;
        auto backupCorner = inOpponentCorner;

        makeMove(m.x1, m.y1, m.x2, m.y2, 'B');

        int moveValue = minimax(2, false, -1000000, 1000000, blackMoves - 1, whiteMoves);
        

        board = backupBoard;
        inOpponentCorner = backupCorner;

        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = m;
        }

        auto now = high_resolution_clock::now();
        if (duration_cast<milliseconds>(now - start).count() > timeLimitMs) {
            break; 
        }
    }

    makeMove(bestMove.x1, bestMove.y1, bestMove.x2, bestMove.y2, 'B');
    blackMoves--;

    char colFrom = 'A' + bestMove.y1, colTo = 'A' + bestMove.y2;
    int rowFrom = bestMove.x1 + 1, rowTo = bestMove.x2 + 1;
    moveNumber++;
    moveHistory.push_back(to_string(moveNumber) + ". AI: " +
                          string(1, colFrom) + to_string(rowFrom) +
                          " -> " + string(1, colTo) + to_string(rowTo));

    return true;
}