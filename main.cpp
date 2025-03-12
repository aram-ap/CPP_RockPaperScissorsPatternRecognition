#include <iostream>
#include "main.h"

using namespace std;

vector<Move> playerMoves;
int playerWins = 0;
int computerWins = 0;
int totalGames = 0;

constexpr int MAX_MATCHES = 5;
constexpr int MIN_MATCHES = 1;
constexpr int MAX_SEARCH_SIZE = 3000;

int main() {
    std::cout << "\033[2J\033[1;1H";
    system("clear");
    cout << "Welcome to Rock, Paper, Scissors! If you didn't know already, this game learns from your patterns to beat you over time!" << endl;

    Move computerMove = Invalid;
    Move lastMove = Invalid;

    bool isRunning = true;
    while (isRunning) {
        // Clear the console
        std::cout << "\033[2J\033[1;1H";
        system("clear");

        // Print stats
        printStats();
        cout << endl;

        // Print results of last game
        if (lastMove != Invalid) {
            printWinner(lastMove, computerMove);
        }

        // Get player moves. Typically only one move is returned, but players can chain moves together

        for (vector<Move> moves = getPlayerMove(); const Move &move : moves) {
            totalGames++;
            lastMove = move;

            if (move == Quit) {
                isRunning = false;
                break;
            }

            computerMove = getComputerMove();

            // Update player moves after computer move is determined to avoid cheating
            playerMoves.push_back(move);

            if (const int winner = getWinner(move, computerMove); winner > 0)
                playerWins++;
            else if (winner < 0)
                computerWins++;
        }
    }

    return 0;
}

Move getBeatingMove(const Move moveToBeat) {
    switch (moveToBeat) {
        case Rock:
            return Paper;
        case Paper:
            return Scissors;
        case Scissors:
            return Rock;
        default:
            return static_cast<Move>(rand() % 3);
    }
}

/**
 * Note: This function is not optimized and won't work well for large search sizes
 * @param searchSize the size of the pattern to search for
 * @return The ideal move if a pattern is found, Inconclusive otherwise
 */
Move findPattern(const int searchSize) {
    // Get the last searchSize moves
    vector<Move> pattern;
    for (int j = static_cast<int>(playerMoves.size()) - 1; j >= max(static_cast<int>(playerMoves.size())-1-searchSize, 0); j--) {
        pattern.insert(pattern.begin(), playerMoves[j]);
    }

    // For every value in player moves, check if the pattern is found
    for (int i = max(static_cast<int>(playerMoves.size()) - 1 - searchSize, 0); i >= min(searchSize, MAX_SEARCH_SIZE); i--) {
        // Get the pattern from i to i-searchSize
        vector<Move> subPattern;
        for (int j = i; j >= max(i-searchSize, 0); j--) {
            subPattern.insert(subPattern.begin(), playerMoves[j]);
        }

        // Check if subPattern is equal to pattern
        if (subPattern == pattern) {
            return getBeatingMove(playerMoves[min(i+1, static_cast<int>(playerMoves.size())-1)]);
        }
    }

    return Inconclusive;
}

Move getComputerMove() {
    // Check for patterns of decreasing size
    for (int i = MAX_MATCHES; i >= MIN_MATCHES ; i--) {
        if (const Move patternMove  = findPattern(i); patternMove != Inconclusive) {
            return patternMove;
        }
    }

    // If no pattern is found, return a random move
    return static_cast<Move>(rand() % 3);
}

vector<Move> getPlayerMove() {
    string playerInput;
    Move move = Invalid;
    vector<Move> inputs;

    while (move == Invalid) {
        cout << "Enter your move (1 = Rock, 2 = Paper, 3 = Scissors) (-1 to quit): ";
        cin >> playerInput;

        if (playerInput == "-1") {
            return {Quit};
        }

        for (const char c : playerInput) {
            if (c == '1') {
                move = Rock;
                inputs.push_back(move);
            } else if (c == '2') {
                move = Paper;
                inputs.push_back(move);
            } else if (c == '3') {
                move = Scissors;
                inputs.push_back(move);
            } else {
                cout << "Invalid move. Please try again." << endl;
            }
        }
    }

    return inputs;
}

int getWinner(const Move playerMove, const Move computerMove) {
    if (playerMove == Invalid || playerMove == Inconclusive) return 0;

    if (playerMove == computerMove) { return 0; }
    if (playerMove == Rock && computerMove == Scissors) { return 1; }
    if (playerMove == Paper && computerMove == Rock) { return 1; }
    if (playerMove == Scissors && computerMove == Paper) { return 1; }

    return -1;
}

void printWinner(const Move playerMove, const Move computerMove) {
    if (const int winner = getWinner(playerMove, computerMove); winner == 0) {
        cout << "It's a tie! You both chose " << moveToString(playerMove) << "." << endl;
    } else if (winner == 1) {
        cout << "You win! " << moveToString(playerMove) << " beats " << moveToString(computerMove) << "." << endl;
    } else {
        cout << "You lose! " << moveToString(computerMove) << " beats " << moveToString(playerMove) << "." << endl;
    }
}

void printStats() {
    cout << "| Player: " << playerWins << " | Computer: " << computerWins << " | Total Games: " << totalGames << " |" << endl;
}

string moveToString(const Move move) {
    if (move == Rock) {
        return "Rock";
    } else if (move == Paper) {
        return "Paper";
    } else {
        return "Scissors";
    }
}

string getMoveString(const std::vector<Move> &moves) {
    string moveString;
    for (const Move &move : moves) {
        switch (move) {
            case Rock:
                moveString += "R";
                break;
            case Paper:
                moveString += "P";
                break;
            case Scissors:
                moveString += "S";
                break;
            default:
                break;
        }
    }
    return moveString;
}