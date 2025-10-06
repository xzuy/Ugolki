/**
 * @file main.cpp
 * @brief Основной файл проекта "UGOLKI".
 *
 * Содержит реализацию игрового окна, меню, доски, хода игрока и ИИ.
 */

#include <SFML/Graphics.hpp>
#include <windows.h>
#include <string>
#include "ai.h"

const int CELL_SIZE = 160;
const int BORDER = 80;
const int HISTORY_WIDTH = 400;
const int WINDOW_SIZE = BOARD_SIZE * CELL_SIZE + BORDER * 2;
const int WINDOW_WIDTH = WINDOW_SIZE + HISTORY_WIDTH;
const int WINDOW_HEIGHT = BOARD_SIZE * CELL_SIZE + BORDER * 2;
int scrollOffset = 0;
const int MAX_VISIBLE_MOVES = 50;
int selectedX = -1, selectedY = -1;
bool pieceSelected = false;
bool playerTurn = true;

sf::Font font;
sf::Texture boardBackgroundTexture;
sf::Sprite boardBackground;

/**
 * @brief Инициализирует игровую доску.
 *
 * Полностью очищает доску и расставляет начальные фишки
 * белого и черного цвета.
 */

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

/**
 * @brief Отрисовывает игровую доску и фигуры.
 *
 * @param window Ссылка на окно SFML для отрисовки.
 * @param selectedX Координата X выбранной клетки.
 * @param selectedY Координата Y выбранной клетки.
 * @param pieceSelected Флаг, выбран ли игроком элемент.
 *
 * Отображает клетки, фигуры, подсветку выбранной клетки и историю ходов.
 */
void drawBoard(sf::RenderWindow& window, int selectedX, int selectedY, bool pieceSelected) {
	sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++) {
			cell.setPosition(BORDER + j * CELL_SIZE + 1.f, BORDER + (BOARD_SIZE - 1 - i) * CELL_SIZE + 1.f);
			cell.setFillColor((i + j) % 2 == 0 ? sf::Color(180, 180, 180) : sf::Color(220, 220, 220));


			cell.setOutlineThickness(2.f);
			cell.setOutlineColor(sf::Color::Black);

			window.draw(cell);
			if (board[i][j] == 'W' || board[i][j] == 'B') {
				sf::CircleShape piece(CELL_SIZE / 2.f - 20.f);
				piece.setPosition(BORDER + j * CELL_SIZE + 20.f, BORDER + (BOARD_SIZE - 1 - i) * CELL_SIZE + 20.f);
				piece.setFillColor(board[i][j] == 'W' ? sf::Color::White : sf::Color::Black);
				window.draw(piece);
			}
		}
	if (pieceSelected && isInside(selectedX, selectedY)) {
		sf::RectangleShape hl(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
		hl.setPosition(BORDER + selectedY * CELL_SIZE + 1.f, BORDER + (BOARD_SIZE - 1 - selectedX) * CELL_SIZE + 1.f);
		hl.setFillColor(sf::Color(255, 255, 0, 100));
		window.draw(hl);
	}

	sf::Text coord, moveText;
	coord.setFont(font); coord.setCharacterSize(24); coord.setFillColor(sf::Color::Black);
	moveText.setFont(font); moveText.setCharacterSize(20); moveText.setFillColor(sf::Color::Black);

	for (int i = 0; i < BOARD_SIZE; i++) {
		char c = 'A' + i;
		coord.setString(c);
		coord.setPosition(BORDER + i * CELL_SIZE + CELL_SIZE / 2.f - 12.f, BORDER - 30.f); window.draw(coord);
		coord.setPosition(BORDER + i * CELL_SIZE + CELL_SIZE / 2.f - 12.f, BORDER + BOARD_SIZE * CELL_SIZE + 5.f); window.draw(coord);
		coord.setString(std::to_string(i + 1));
		coord.setPosition(BORDER - 30.f, BORDER + (BOARD_SIZE - 1 - i) * CELL_SIZE + CELL_SIZE / 2.f - 12.f); window.draw(coord);
		coord.setPosition(BORDER + BOARD_SIZE * CELL_SIZE + 5.f, BORDER + (BOARD_SIZE - 1 - i) * CELL_SIZE + CELL_SIZE / 2.f - 12.f); window.draw(coord);
	}


	int totalMoves = static_cast<int>(moveHistory.size());
	int start = std::max(0, totalMoves - MAX_VISIBLE_MOVES - scrollOffset);
	int end = std::min(totalMoves, start + MAX_VISIBLE_MOVES);

	for (int i = start; i < end; i++) {
		moveText.setString(moveHistory[i]);
		moveText.setPosition(WINDOW_SIZE + 20.f, BORDER + (i - start) * 25.f);
		window.draw(moveText);
	}

}
/**
 * @enum MenuState
 * @brief Состояния главного меню.
 */
enum class MenuState { MAIN, RULES };
/**
 * @brief Отображает главное меню и экран правил.
 *
 * @param window Ссылка на окно SFML для отрисовки.
 * @return true если пользователь начал игру, false если окно закрыто.
 */
bool showMenu(sf::RenderWindow& window) {
	sf::Font menuFont;
	if (!menuFont.loadFromFile("DejaVuSans-Bold.ttf")) {
		MessageBoxA(nullptr, "Failed to load font", "Error", MB_ICONERROR);
		return false;
	}

	sf::Texture menuTexture;
	menuTexture.loadFromFile("bg.png");
	sf::Sprite menuBackground(menuTexture);
	menuBackground.setScale(
		static_cast<float>(WINDOW_WIDTH) / menuTexture.getSize().x,
		static_cast<float>(WINDOW_HEIGHT) / menuTexture.getSize().y
	);


	sf::Text title("UGOLKI", menuFont, 72);
	title.setFillColor(sf::Color::Black);
	sf::FloatRect bounds = title.getLocalBounds();
	title.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	title.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 7.f);


	sf::RectangleShape startButton(sf::Vector2f(250.f, 80.f));
	startButton.setFillColor(sf::Color(200, 200, 200));
	startButton.setPosition(WINDOW_WIDTH / 2.f - 125.f, 300.f);

	sf::Text startButtonText("Start", menuFont, 36);
	startButtonText.setFillColor(sf::Color::Black);
	startButtonText.setPosition(WINDOW_WIDTH / 2.f - 50.f, 315.f);


	sf::RectangleShape rulesButton(sf::Vector2f(250.f, 80.f));
	rulesButton.setFillColor(sf::Color(200, 200, 200));
	rulesButton.setPosition(WINDOW_WIDTH / 2.f - 125.f, 400.f);

	sf::Text rulesButtonText("Rules", menuFont, 36);
	rulesButtonText.setFillColor(sf::Color::Black);
	rulesButtonText.setPosition(WINDOW_WIDTH / 2.f - 50.f, 415.f);


	sf::RectangleShape backButton(sf::Vector2f(250.f, 80.f));
	backButton.setFillColor(sf::Color(200, 200, 200));
	backButton.setPosition(WINDOW_WIDTH / 2.f - 125.f, 1000.f);

	sf::Text backButtonText("Back", menuFont, 36);
	backButtonText.setFillColor(sf::Color::Black);
	backButtonText.setPosition(WINDOW_WIDTH / 2.f - 50.f, 1015.f);


	sf::Text rulesText(
		"Rules of the game:\n\n"
		"1. Objective:\n"
		"   Move all your pieces from your starting corner to the opposite corner.\n\n"
		"2. Turn Order:\n"
		"   White (player) moves first, followed by Black (AI).\n\n"
		"3. Movement:\n"
		"   Pieces can move only horizontally or vertically, one cell at a time.\n"
		"   Diagonal moves are not allowed.\n\n"
		"4. Jumping:\n"
		"   You can jump over one adjacent piece (yours or opponent's) to an empty\n"
		"   cell directly behind it. Multiple consecutive jumps in a single move are allowed.\n\n"
		"5. End of Game:\n"
		"   The game ends when all pieces reach the opposite corners.\n"
		"   The player with more pieces in the destination corner wins.\n",
		menuFont, 36
	);
	rulesText.setFillColor(sf::Color::Black);
	rulesText.setPosition(100.f, 150.f);

	MenuState state = MenuState::MAIN;


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed) {
				float mx = static_cast<float>(event.mouseButton.x);
				float my = static_cast<float>(event.mouseButton.y);

				if (state == MenuState::MAIN) {
					// Start
					if (startButton.getGlobalBounds().contains(mx, my)) {
						return true;
					}
					// Rules
					if (rulesButton.getGlobalBounds().contains(mx, my)) {
						state = MenuState::RULES;
					}
				}
				else if (state == MenuState::RULES) {
					// Back
					if (backButton.getGlobalBounds().contains(mx, my)) {
						state = MenuState::MAIN;
					}
				}
			}
		}


		window.clear();
		window.draw(menuBackground);

		if (state == MenuState::MAIN) {
			window.draw(title);
			window.draw(startButton);
			window.draw(startButtonText);
			window.draw(rulesButton);
			window.draw(rulesButtonText);
		}
		else if (state == MenuState::RULES) {
			window.draw(rulesText);
			window.draw(backButton);
			window.draw(backButtonText);
		}

		window.display();
	}

	return false;
}
// экран окончания игры
/**
 * @brief Показывает экран окончания игры.
 *
 * @param window Ссылка на окно SFML.
 * @param resultText Текст результата (кто выиграл, счет).
 * @param font Шрифт для отображения текста.
 *
 * Отображает затемненный фон, панель с результатом и закрывается через 5 секунд.
 */
void showGameOver(sf::RenderWindow& window, const std::string& resultText, sf::Font& font) {

	sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)));
	overlay.setFillColor(sf::Color(0, 0, 0, 150));


	sf::RectangleShape panel(sf::Vector2f(600.f, 300.f));
	panel.setFillColor(sf::Color(255, 255, 255, 230));
	panel.setOrigin(panel.getSize().x / 2.f, panel.getSize().y / 2.f);
	panel.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);


	sf::Text result(resultText, font, 36);
	result.setFillColor(sf::Color::Black);
	sf::FloatRect textBounds = result.getLocalBounds();
	result.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
	result.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);


	sf::Clock clock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				return;
			}
		}

		window.clear();
		window.draw(boardBackground);
		drawBoard(window, -1, -1, false);
		window.draw(overlay);
		window.draw(panel);
		window.draw(result);
		window.display();

		if (clock.getElapsedTime().asSeconds() > 10.f) {
			return;
		}
	}
}
/**
 * @brief Сбрасывает игру в начальное состояние.
 *
 * Выполняет повторную инициализацию доски, сброс истории ходов и состояния управления.
 */
void resetGame() {

	initBoard();


	for (int i = 0; i < BOARD_SIZE; ++i)
		for (int j = 0; j < BOARD_SIZE; ++j)
			inOpponentCorner[i][j] = false;

	moveHistory.clear();
	moveNumber = 0;
	whiteMoves = blackMoves = 20;
	playerTurn = true;
	selectedX = selectedY = -1;
	pieceSelected = false;
}
/**
 * @brief Точка входа в Windows-приложение.
 *
 * Аналог стандартной функции `main`, но в WinAPI.
 * Используется как стартовая точка для GUI-программ под Windows.
 *
 * Создает окно, отображает меню, запускает игровой цикл.
 *
 * @param hInstance Дескриптор текущего экземпляра приложения.
 *        Используется системой Windows для идентификации программы
 *        и загрузки ресурсов (иконок, диалогов, строк и т.п.).
 *
 * @param hPrevInstance Дескриптор предыдущего экземпляра приложения.
 *        В современных версиях Windows всегда равен `NULL`
 *        и не используется (оставлен для совместимости).
 * @param lpCmdLine Аргументы командной строки.
 * @param nCmdShow Флаг отображения окна.
 * @return Код возврата (0 — успешное завершение).
 */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "UGOLKI");

	if (!showMenu(window)) {
		return 0;
	}

	boardBackgroundTexture.loadFromFile("bg.png");

	boardBackground.setTexture(boardBackgroundTexture);
	boardBackground.setScale(static_cast<float>(WINDOW_WIDTH) / boardBackgroundTexture.getSize().x,
		static_cast<float>(WINDOW_HEIGHT) / boardBackgroundTexture.getSize().y);


	initBoard();

	font.loadFromFile("DejaVuSans-Bold.ttf");



	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (playerTurn && whiteMoves > 0 && event.type == sf::Event::MouseButtonPressed) {
				int mouseX = event.mouseButton.x, mouseY = event.mouseButton.y;
				int gridX = (BOARD_SIZE - 1) - ((mouseY - BORDER) / CELL_SIZE);
				int gridY = (mouseX - BORDER) / CELL_SIZE;
				if (isInside(gridX, gridY)) {
					if (!pieceSelected && board[gridX][gridY] == 'W') {
						selectedX = gridX; selectedY = gridY; pieceSelected = true;
					}
					else if (pieceSelected) {
						if (makeMove(selectedX, selectedY, gridX, gridY, 'W')) {
							moveNumber++; whiteMoves--;
							moveHistory.push_back(std::to_string(moveNumber) + ". Player: " +
								std::string(1, 'A' + selectedY) + std::to_string(selectedX + 1) +
								" -> " + std::string(1, 'A' + gridY) + std::to_string(gridX + 1));
							pieceSelected = false;
							playerTurn = false;
						}
						else {
							pieceSelected = false;
						}
					}
				}
			}

			if (!playerTurn && blackMoves > 0) {
				if (!makeAIMove()) {
					moveNumber++;
				}
				playerTurn = true;
			}


			if (event.type == sf::Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					if (event.mouseWheelScroll.delta > 0) {
						if (scrollOffset < (int)moveHistory.size() - MAX_VISIBLE_MOVES)
							scrollOffset++;
					}
					else if (event.mouseWheelScroll.delta < 0) {
						if (scrollOffset > 0)
							scrollOffset--;
					}
				}
			}
		}

		window.clear();
		window.draw(boardBackground);
		drawBoard(window, selectedX, selectedY, pieceSelected);
		window.display();


		if (whiteMoves <= 0 && blackMoves <= 0) {
			int whiteCount = 0, blackCount = 0;
			for (int i = BOARD_SIZE - CORNER_SIZE; i < BOARD_SIZE; i++)
				for (int j = BOARD_SIZE - CORNER_SIZE; j < BOARD_SIZE; j++)
					if ((i + j) >= 2 * BOARD_SIZE - CORNER_SIZE - 1 && board[i][j] == 'W') // (i + j) >= 11
						whiteCount++;
			for (int i = 0; i < CORNER_SIZE; i++)
				for (int j = 0; j < CORNER_SIZE; j++)
					if ((i + j) <= CORNER_SIZE - 1 && board[i][j] == 'B')
						blackCount++;

			std::string result;
			if (whiteCount > blackCount) result = "WHITE wins (you)!";
			else if (blackCount > whiteCount) result = "BLACK wins (AI)!";
			else result = "Draw!";

			result += "\nWHITE: " + std::to_string(whiteCount) +
				" BLACK: " + std::to_string(blackCount);

			showGameOver(window, result, font);

			// возвращаемся в меню
			if (!showMenu(window)) return 0;

			resetGame();
		}
	}

	return 0;
}