#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <windows.h> // For console colors and Sleep()
#include <limits>  // For numeric_limits
#include <thread>  // For this_thread::sleep_for
#include <chrono>  // For milliseconds

// --- Console Color Constants ---
const int COLOR_BLUE = 9;
const int COLOR_RED = 12;
const int COLOR_WHITE = 15;
const int COLOR_YELLOW = 14;
const int COLOR_GREEN = 10;
const int COLOR_GREY = 8;

// --- Player Symbol Constants ---
const char P1_SYMBOL = 'X';
const char P2_SYMBOL = 'O';

// --- Function Prototypes ---
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
    setConsoleColor(COLOR_WHITE
        std::cout << " Welcome to the EchoGrid. Where every move can echo into victory... or defeat.\n";
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

    // --- Starting Dice Roll ---
    clearScreen();
    std::cout << "Let's see who goes first. The dice will decide!\n";
    pause(2000);

    int p1_roll, p2_roll;
    do {
        std::cout << "\nPlayer 1 (";
        setConsoleColor(COLOR_BLUE);
        std::cout << "Blue Side";
        setConsoleColor(COLOR_WHITE);
        std::cout << ") is rolling...";
        pause(1500);
        p1_roll = rand() % 6 + 1;
        std::cout << " a " << p1_roll << "!\n";

        std::cout << "Player 2 (";
        setConsoleColor(COLOR_RED);
        std::cout << "Red Side";
        setConsoleColor(COLOR_WHITE);
        std::cout << ") is rolling...";
        pause(1500);
        p2_roll = rand() % 6 + 1;
        std::cout << " a " << p2_roll << "!\n\n";

        if (p1_roll > p2_roll) {
            currentPlayer = 1;
            setConsoleColor(COLOR_BLUE);
            std::cout << "Blue Side wins the roll and will go first!\n";
        }
        else if (p2_roll > p1_roll) {
            currentPlayer = 2;
            setConsoleColor(COLOR_RED);
            std::cout << "Red Side wins the roll and will go first!\n";
        }
        else {
            setConsoleColor(COLOR_YELLOW);
            std::cout << "It's a tie! Let's roll again to be sure.\n";
            pause(2000);
        }
    } while (p1_roll == p2_roll);

    setConsoleColor(COLOR_WHITE);
    std::cout << "\nPress Enter to start the game...";
    std::cin.get();

    // --- Main Game Loop ---
    while (true) {
        clearScreen();
        printBoard(board);

        // --- Announce Turn ---
        char currentSymbol = (currentPlayer == 1) ? P1_SYMBOL : P2_SYMBOL;
        if (currentPlayer == 1) {
            setConsoleColor(COLOR_BLUE);
            std::cout << "Blue Side's Turn (" << currentSymbol << ")\n";
        }
        else {
            setConsoleColor(COLOR_RED);
            std::cout << "Red Side's Turn (" << currentSymbol << ")\n";
        }
        setConsoleColor(COLOR_WHITE);

        // --- Coin Toss Mechanic ---
        std::cout << "First, the coin toss. Call it! (1 for Heads, 2 for Tails): ";
        int coinCall;
        while (!(std::cin >> coinCall) || (coinCall != 1 && coinCall != 2)) {
            std::cout << "Invalid choice. Please enter 1 for Heads or 2 for Tails: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        int coinResult = rand() % 2 + 1;
        std::cout << "Flipping the coin...";
        pause(2000);
        std::cout << " It's " << (coinResult == 1 ? "Heads!" : "Tails!") << "\n\n";

        bool powerTurn = (coinCall == coinResult);
        bool turnSkipped = false;

        if (powerTurn) {
            setConsoleColor(COLOR_GREEN);
            std::cout << "You won the toss! It's a POWER TURN!\n";
            setConsoleColor(COLOR_WHITE);
            std::cout << "Choose your action:\n";
            std::cout << "1. Place your mark on an empty square.\n";
            std::cout << "2. Conquer an opponent's square.\n";
            std::cout << "Your choice: ";
            int action;
            while (!(std::cin >> action) || (action != 1 && action != 2)) {
                std::cout << "Invalid choice. Please enter 1 to Place or 2 to Conquer: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            if (action == 1) { // Place
                std::cout << "Choose an empty square (1-9): ";
                int move = getValidInput(board, true);
                board[move - 1] = currentSymbol;
            }
            else { // Conquer
                std::cout << "Choose an opponent's square to CONQUER (1-9): ";
                int move = getValidInput(board, false); // Get any valid number
                char opponentSymbol = (currentPlayer == 1) ? P2_SYMBOL : P1_SYMBOL;
                if (board[move - 1] == opponentSymbol) {
                    board[move - 1] = currentSymbol;
                    setConsoleColor(COLOR_GREEN);
                    std::cout << "\nSuccess! The square has been conquered!\n";
                    pause(2000);
                }
                else {
                    setConsoleColor(COLOR_RED);
                    std::cout << "\nInvalid target! That square doesn't belong to your opponent.\n";
                    std::cout << "Your turn is forfeited!\n";
                    turnSkipped = true;
                    pause(2500);
                }
            }
        }
        else {
            setConsoleColor(COLOR_YELLOW);
            std::cout << "You lost the toss. It's a normal turn.\n";
            setConsoleColor(COLOR_WHITE);
            std::cout << "Choose an empty square to place your mark (1-9): ";
            int move = getValidInput(board, true);
            board[move - 1] = currentSymbol;
        }

        // --- Check for Game Over ---
        if (!turnSkipped && checkWin(board, currentSymbol)) {
            clearScreen();
            printBoard(board);
            if (currentPlayer == 1) {
                setConsoleColor(COLOR_BLUE);
                std::cout << "\nBLUE SIDE IS VICTORIOUS!\n";
            }
            else {
                setConsoleColor(COLOR_RED);
                std::cout << "\nRED SIDE IS VICTORIOUS!\n";
            }
            break;
        }

        if (checkDraw(board)) {
            clearScreen();
            printBoard(board);
            setConsoleColor(COLOR_YELLOW);
            std::cout << "\nTHE BOARD IS FULL! THE BATTLE ENDS IN A DRAW!\n";
            break;
        }

        // --- Switch Player ---
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    setConsoleColor(COLOR_WHITE);
    std::cout << "\n\nThanks for playing EchGrid!\n";
    return 0;
}

// --- Function Definitions ---

void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void printTitle() {
    clearScreen();
    setConsoleColor(COLOR_YELLOW);
    std::cout << R"(
