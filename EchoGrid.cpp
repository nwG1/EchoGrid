#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#define NOMINMAX   // Prevents windows.h from defining min() and max() macros
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
const char P1_SYMBOL = 'X'; // Human Player
const char P2_SYMBOL = 'O'; // Human Player 2 or AI

// --- Function Prototypes ---
void setConsoleColor(int color);
void printTitle();
void printBoard(const std::vector<char>& board);
void clearScreen();
void pause(int milliseconds);
int getValidInput(const std::vector<char>& board, bool isEmptyRequired);
bool checkWin(const std::vector<char>& board, char playerSymbol);
bool checkDraw(const std::vector<char>& board);
int findBestMove(std::vector<char> board, char playerSymbol); // AI Logic
void performAITurn(std::vector<char>& board, int& currentPlayer); // AI Turn Handler

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::vector<char> board = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    int currentPlayer = 1;
    bool vsAI = false;

    printTitle();
    setConsoleColor(COLOR_WHITE);

    // --- Game Mode Selection ---
    std::cout << "\n\n Choose your opponent:\n";
    std::cout << " 1. Play against another Human\n";
    std::cout << " 2. Play against the AI\n";
    std::cout << " Your choice: ";
    int gameMode;
    while (!(std::cin >> gameMode) || (gameMode != 1 && gameMode != 2)) {
        std::cout << "Invalid choice. Please enter 1 or 2: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    vsAI = (gameMode == 2);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    std::cout << "\n Welcome to the EchoGrid. Where every move can echo into victory... or defeat.\n";
    std::cout << " The rules are different here. Victory requires luck, guts, and strategy.\n\n";
    std::cout << " Press Enter to see the rules...";
    std::cin.get();

    // --- Rule Explanation ---
    clearScreen();
    setConsoleColor(COLOR_YELLOW);
    std::cout << "================================ R U L E S ================================\n\n";
    setConsoleColor(COLOR_WHITE);
    std::cout << " 1. To start, both players roll a die. Highest roller goes first.\n\n";
    std::cout << " 2. On your turn, you toss a coin. Winning grants you a POWER TURN.\n";
    std::cout << "    - POWER TURN: You can [Place] on an empty square OR try to [Conquer]\n";
    std::cout << "      an opponent's square.\n";
    std::cout << "    - NORMAL TURN: You can only [Place] on an empty square.\n\n";
    setConsoleColor(COLOR_GREEN);
    std::cout << " >> CONQUER RULE: When you attempt to conquer, the defender gets a\n";
    std::cout << " >>               'Defense Toss'. If they win the toss, they keep their\n";
    std::cout << " >>               space and your turn ends! If they lose, you take it.\n\n";
    setConsoleColor(COLOR_RED);
    std::cout << " >> WARNING: Attempting to Conquer an invalid square (empty or your own)\n";
    std::cout << " >>          results in forfeiting your turn!\n\n";
    setConsoleColor(COLOR_WHITE);
    std::cout << " 3. The first player to get three in a row wins!\n\n";
    setConsoleColor(COLOR_YELLOW);
    std::cout << "===========================================================================\n\n";
    std::cout << "Press Enter to begin...";
    std::cin.get();

    // --- Starting Dice Roll ---
    // (Dice Roll logic remains the same)
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
        std::cout << (vsAI ? "AI (Red Side)" : "Player 2 (Red Side)");
        std::cout << " is rolling...";
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
            std::cout << (vsAI ? "The AI" : "Red Side") << " wins the roll and will go first!\n";
        }
        else {
            setConsoleColor(COLOR_YELLOW);
            std::cout << "It's a tie! Rerolling...\n";
            pause(2000);
        }
    } while (p1_roll == p2_roll);
    setConsoleColor(COLOR_WHITE);
    std::cout << "\nPress Enter to start...";
    std::cin.get();


    // --- Main Game Loop ---
    while (true) {
        clearScreen();
        printBoard(board);

        if (vsAI && currentPlayer == 2) {
            performAITurn(board, currentPlayer);
        }
        else {
            // --- Human Turn Logic ---
            char currentSymbol = (currentPlayer == 1) ? P1_SYMBOL : P2_SYMBOL;
            setConsoleColor(currentPlayer == 1 ? COLOR_BLUE : COLOR_RED);
            std::cout << (currentPlayer == 1 ? "Blue Side's Turn (X)\n" : "Red Side's Turn (O)\n");

            setConsoleColor(COLOR_WHITE);
            std::cout << "First, the coin toss. Call it! (1 for Heads, 2 for Tails): ";
            int coinCall;
            while (!(std::cin >> coinCall) || (coinCall != 1 && coinCall != 2)) {
                std::cout << "Invalid choice. Please enter 1 or 2: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
                std::cout << "1. Place mark | 2. Conquer opponent's square\nYour choice: ";
                int action;
                while (!(std::cin >> action) || (action != 1 && action != 2)) {
                    std::cout << "Invalid choice. Enter 1 or 2: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (action == 1) { // Place
                    std::cout << "Choose an empty square (1-9): ";
                    int move = getValidInput(board, true);
                    board[move - 1] = currentSymbol;
                }
                else { // Conquer
                    std::cout << "Choose an opponent's square to CONQUER (1-9): ";
                    int move = getValidInput(board, false);
                    char opponentSymbol = (currentPlayer == 1) ? P2_SYMBOL : P1_SYMBOL;

                    if (board[move - 1] == opponentSymbol) {
                        // --- NEW DEFENSE TOSS MECHANIC ---
                        setConsoleColor(COLOR_YELLOW);
                        std::cout << "\nTHE DEFENDER HAS A CHANCE TO SAVE THE SQUARE!\n";
                        pause(1500);

                        int defenseCall;
                        if (vsAI && currentPlayer == 1) { // AI is defending
                            setConsoleColor(COLOR_RED);
                            std::cout << "The AI is making its defense call...\n";
                            defenseCall = rand() % 2 + 1;
                            pause(2000);
                        }
                        else { // Human is defending
                            setConsoleColor(currentPlayer == 1 ? COLOR_RED : COLOR_BLUE);
                            std::cout << "Defender, call it! (1 for Heads, 2 for Tails): ";
                            while (!(std::cin >> defenseCall) || (defenseCall != 1 && defenseCall != 2)) {
                                std::cout << "Invalid call. 1 for Heads, 2 for Tails: ";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }

                        int defenseToss = rand() % 2 + 1;
                        std::cout << "The defense toss is...";
                        pause(2000);
                        std::cout << " " << (defenseToss == 1 ? "Heads!" : "Tails!") << "\n";

                        if (defenseCall == defenseToss) {
                            setConsoleColor(COLOR_GREEN);
                            std::cout << "\nDEFENSE SUCCESSFUL! The square is safe!\n";
                            pause(2500);
                        }
                        else {
                            setConsoleColor(COLOR_RED);
                            std::cout << "\nDEFENSE FAILED! The square has been conquered!\n";
                            board[move - 1] = currentSymbol;
                            pause(2500);
                        }
                    }
                    else {
                        setConsoleColor(COLOR_RED);
                        std::cout << "\nInvalid target! That's not an opponent's square.\n";
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
        }

        // --- Check for Game Over ---
        if (checkWin(board, P1_SYMBOL)) {
            clearScreen(); printBoard(board);
            setConsoleColor(COLOR_BLUE); std::cout << "\nBLUE SIDE IS VICTORIOUS!\n";
            break;
        }
        if (checkWin(board, P2_SYMBOL)) {
            clearScreen(); printBoard(board);
            setConsoleColor(COLOR_RED); std::cout << "\nRED SIDE IS VICTORIOUS!\n";
            break;
        }
        if (checkDraw(board)) {
            clearScreen(); printBoard(board);
            setConsoleColor(COLOR_YELLOW); std::cout << "\nTHE BATTLE ENDS IN A DRAW!\n";
            break;
        }

        // --- Switch Player ---
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    setConsoleColor(COLOR_WHITE);
    std::cout << "\n\nThanks for playing EchoGrid!\n";
    std::cin.get();
    return 0;
}

// --- Function Definitions ---
// (printTitle, printBoard, clearScreen, etc. remain the same)
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
        // We use std::cin here for human input, but clear the buffer afterwards
        if (!(std::cin >> move)) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (move < 1 || move > 9) {
            std::cout << "Invalid input. Please enter a number between 1 and 9: ";
        }
        else if (isEmptyRequired && (board[move - 1] == 'X' || board[move - 1] == 'O')) {
            std::cout << "That square is already taken. Choose an empty one: ";
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer for next input
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

// --- AI LOGIC FUNCTIONS ---
void performAITurn(std::vector<char>& board, int& currentPlayer) {
    setConsoleColor(COLOR_RED);
    std::cout << "AI's Turn (O)\n";
    pause(1000);

    setConsoleColor(COLOR_WHITE);
    std::cout << "The AI is calling the coin toss...";
    int coinCall = rand() % 2 + 1;
    pause(2000);

    int coinResult = rand() % 2 + 1;
    std::cout << " It's " << (coinResult == 1 ? "Heads!" : "Tails!") << "\n\n";

    bool powerTurn = (coinCall == coinResult);

    if (powerTurn) {
        setConsoleColor(COLOR_GREEN);
        std::cout << "The AI won the toss! It's a POWER TURN!\n";
        pause(1500);

        // AI Decision: Place vs Conquer
        // Simple logic: If there's an opponent piece, consider conquering. Otherwise, always place.
        int opponentPieces = 0;
        int targetSquare = -1;
        for (int i = 0; i < board.size(); ++i) {
            if (board[i] == P1_SYMBOL) {
                opponentPieces++;
                targetSquare = i + 1; // Find a potential target
            }
        }

        bool tryConquer = (opponentPieces > 0 && (rand() % 3 == 0)); // 1 in 3 chance to try conquering

        if (tryConquer && targetSquare != -1) {
            std::cout << "The AI chooses to CONQUER square " << targetSquare << "!\n";
            pause(2000);

            // Human defends
            setConsoleColor(COLOR_YELLOW);
            std::cout << "\nYOUR CHANCE TO DEFEND!\n";
            setConsoleColor(COLOR_BLUE);
            std::cout << "Call it! (1 for Heads, 2 for Tails): ";
            int defenseCall;
            while (!(std::cin >> defenseCall) || (defenseCall != 1 && defenseCall != 2)) {
                std::cout << "Invalid call. 1 for Heads, 2 for Tails: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            int defenseToss = rand() % 2 + 1;
            std::cout << "The defense toss is...";
            pause(2000);
            std::cout << " " << (defenseToss == 1 ? "Heads!" : "Tails!") << "\n";

            if (defenseCall == defenseToss) {
                setConsoleColor(COLOR_GREEN);
                std::cout << "\nDEFENSE SUCCESSFUL! You saved your square!\n";
            }
            else {
                setConsoleColor(COLOR_RED);
                std::cout << "\nDEFENSE FAILED! The AI conquered your square!\n";
                board[targetSquare - 1] = P2_SYMBOL;
            }
            pause(2500);

        }
        else {
            std::cout << "The AI chooses to place a mark.\n";
            pause(1000);
            int move = findBestMove(board, P2_SYMBOL);
            if (move != -1) {
                board[move] = P2_SYMBOL;
                std::cout << "The AI places its mark on square " << move + 1 << ".\n";
                pause(1500);
            }
        }
    }
    else {
        setConsoleColor(COLOR_YELLOW);
        std::cout << "The AI lost the toss. It's a normal turn.\n";
        pause(1500);
        int move = findBestMove(board, P2_SYMBOL);
        if (move != -1) {
            board[move] = P2_SYMBOL;
            std::cout << "The AI places its mark on square " << move + 1 << ".\n";
            pause(1500);
        }
    }
}

int findBestMove(std::vector<char> board, char playerSymbol) {
    // 1. Check for a winning move
    for (int i = 0; i < 9; ++i) {
        if (board[i] != P1_SYMBOL && board[i] != P2_SYMBOL) {
            std::vector<char> tempBoard = board;
            tempBoard[i] = P2_SYMBOL;
            if (checkWin(tempBoard, P2_SYMBOL)) {
                return i; // Return winning move index
            }
        }
    }

    // 2. Check to block the opponent's winning move
    for (int i = 0; i < 9; ++i) {
        if (board[i] != P1_SYMBOL && board[i] != P2_SYMBOL) {
            std::vector<char> tempBoard = board;
            tempBoard[i] = P1_SYMBOL;
            if (checkWin(tempBoard, P1_SYMBOL)) {
                return i; // Return blocking move index
            }
        }
    }

    // 3. Take the center if available
    if (board[4] != P1_SYMBOL && board[4] != P2_SYMBOL) {
        return 4;
    }

    // 4. Take one of the corners
    int corners[] = { 0, 2, 6, 8 };
    std::vector<int> availableCorners;
    for (int i : corners) {
        if (board[i] != P1_SYMBOL && board[i] != P2_SYMBOL) {
            availableCorners.push_back(i);
        }
    }
    if (!availableCorners.empty()) {
        return availableCorners[rand() % availableCorners.size()];
    }

    // 5. Take any available square
    std::vector<int> availableMoves;
    for (int i = 0; i < 9; ++i) {
        if (board[i] != P1_SYMBOL && board[i] != P2_SYMBOL) {
            availableMoves.push_back(i);
        }
    }
    if (!availableMoves.empty()) {
        return availableMoves[rand() % availableMoves.size()];
    }

    return -1; // Should not happen in a normal game
}

