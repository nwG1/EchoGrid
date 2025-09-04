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
void setConsoleColor(int color);
void printTitle();
void printBoard(const std::vector<char>& board);
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
    
    // --- Rule Explanation ---
    clearScreen();
    setConsoleColor(COLOR_YELLOW);
    std::cout << "================================ R U L E S ================================\n\n";
    setConsoleColor(COLOR_WHITE);
    std::cout << " 1. To start, both players roll a die. The highest roller gets the first turn.\n\n";
    std::cout << " 2. On your turn, you will first toss a coin.\n";
    std::cout << "    - If you WIN the toss, you get a POWER TURN!\n";
    std::cout << "      You can either [Place] your mark on an EMPTY square OR...\n";
    std::cout << "      You can attempt to [Conquer] a square already held by your OPPONENT!\n\n";
    std::cout << "    - If you LOSE the toss, you have a NORMAL TURN.\n";
    std::cout << "      You can only [Place] your mark on an EMPTY square.\n\n";
    setConsoleColor(COLOR_RED);
    std::cout << " >> WARNING: If you attempt to [Conquer] a square that isn't held by the\n";
    std::cout << " >>          opponent (e.g., an empty square), YOU FORFEIT YOUR TURN!\n\n";
    setConsoleColor(COLOR_WHITE);
    std::cout << " 3. The first player to get three of their marks in a row wins!\n\n";
    setConsoleColor(COLOR_YELLOW);
    std::cout << "===========================================================================\n\n";
    std::cout << "Press Enter to begin the showdown...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

