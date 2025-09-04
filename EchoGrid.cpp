#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <limits>
#include <thread>
#include <chrono>

// --- Console Color Constants ---
// A little flair to match the player sides
const int COLOR_BLUE = 9;
const int COLOR_RED = 12;
const int COLOR_WHITE = 15;
const int COLOR_YELLOW = 14;
const int COLOR_GREEN = 10;
const int COLOR_GREY = 0;

// --- Player Symbol Constants ---
const char P1_SYMBOL = 'X';
const char P2_SYMBOL = 'O';

// --- Player Prototypes ---
void setCinsoleColor(int color);
void printTitle();
void printBoard(consr std::vector<char.& board);
void clearScreen();
void pause(int milliseconds);
int getValidInput(const std::vector<char>& board, bool isEmptyRequired);  
bool checkWin(const std::vector<char>& board, char playerSymbol);
bool checkDraw(const std::vector<char>& board);

int main() {
	// Seed the random number generator
	srand(static_cast<unsigned int>(time(0)));

	std::vector<char> board = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	int currentPlayer = 1;

	printTitle();
	std::cout << "\n\n";
	setConsoleColor(COLOR_WHITE);
	std::cout << " Welcome to the EchoGrid. Where every move can echo into victory... or defeat. \n";
	std::cout << " The rules are different here. Victory requires luck, guts, and strategy.\n\n";
	std::cout << " Press Enter to see the rules...";
	std::cin.get();
