#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#define NOMINMAX   // This is the FIX: It prevents windows.h from defining min() and max() macros
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
    setConsoleColor(COLOR_WHITE);
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
    // This line was causing an error. We need to clear the input buffer before waiting for the next Enter.
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
    std::cout << "\n\nThanks for playing EchoGrid!\n";
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
  _______ ______ _    _  _____   ______ _____ __   __ ______ 
 |__   __|  ____| |  | |/ ____| |  ____/ ____|  \ |  |  ____|
    | |  | |__  | |__| | |  __  | |__ | |  __| \ \|  | |__   
    | |  |  __| |  __  | | |_ | |  __|| | |_ | |\   |  __|  
    | |  | |____| |  | | |__| | | |___| |__| | | \  | |____ 
    |_|  |______|_|  |_|\_____| |______\_____|_|  \_|______|
                                                          
)" << '\n';
}

void printBoard(const std::vector<char>& board) {
    setConsoleColor(COLOR_WHITE);
    std::cout << "\n     |     |     \n";
    std::cout << "  ";
    for (int i = 0; i < 3; ++i) {
        if (board[i] == P1_SYMBOL) setConsoleColor(COLOR_BLUE);
        else if (board[i] == P2_SYMBOL) setConsoleColor(COLOR_RED);
        else setConsoleColor(COLOR_GREY);
        std::cout << board[i];
        setConsoleColor(COLOR_WHITE);
        if (i < 2) std::cout << "  |  ";
    }
    std::cout << "\n_____|_____|_____\n";
    std::cout << "     |     |     \n";
    std::cout << "  ";
    for (int i = 3; i < 6; ++i) {
        if (board[i] == P1_SYMBOL) setConsoleColor(COLOR_BLUE);
        else if (board[i] == P2_SYMBOL) setConsoleColor(COLOR_RED);
        else setConsoleColor(COLOR_GREY);
        std::cout << board[i];
        setConsoleColor(COLOR_WHITE);
        if (i < 5) std::cout << "  |  ";
    }
    std::cout << "\n_____|_____|_____\n";
    std::cout << "     |     |     \n";
    std::cout << "  ";
    for (int i = 6; i < 9; ++i) {
        if (board[i] == P1_SYMBOL) setConsoleColor(COLOR_BLUE);
        else if (board[i] == P2_SYMBOL) setConsoleColor(COLOR_RED);
        else setConsoleColor(COLOR_GREY);
        std::cout << board[i];
        setConsoleColor(COLOR_WHITE);
        if (i < 8) std::cout << "  |  ";
    }
    std::cout << "\n     |     |     \n\n";
}

void clearScreen() {
    system("cls");
}

void pause(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int getValidInput(const std::vector<char>& board, bool isEmptyRequired) {
    int move;
    while (true) {
        std::cin >> move;
        if (std::cin.fail() || move < 1 || move > 9) {
            std::cout << "Invalid input. Please enter a number between 1 and 9: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if (isEmptyRequired && (board[move - 1] == 'X' || board[move - 1] == 'O')) {
            std::cout << "That square is already taken. Choose an empty one: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            // Clear the input buffer after a successful read to prevent issues on the next turn.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return move;
        }
    }
}

bool checkWin(const std::vector<char>& board, char playerSymbol) {
    const int wins[8][3] = { {0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6} };
    for (int i = 0; i < 8; ++i) {
        if (board[wins[i][0]] == playerSymbol && board[wins[i][1]] == playerSymbol && board[wins[i][2]] == playerSymbol) {
            return true;
        }
    }
    return false;
}

bool checkDraw(const std::vector<char>& board) {
    for (char c : board) {
        if (c != 'X' && c != 'O') {
            return false;
        }
    }
    return true;
}

