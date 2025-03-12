#include <iostream>
#include "main.h"

using namespace std;

vector<Move> playerMoves;
int playerWins = 0;
int computerWins = 0;
int totalGames = 0;

const int MAX_MATCHES = 5;
const int MIN_MATCHES = 1;
const int MAX_SEARCH_SIZE = 3000;

int main() {
    std::cout << "\033[2J\033[1;1H";
    system("clear");

    cout << "Welcome to Rock, Paper, Scissors! If you didn't know already, this game learns from your patterns to beat you over time!" << endl;
    vector<Move> moves;
    Move computerMove = Invalid;
    Move lastMove = Invalid;

    bool isRunning = true;
    while (isRunning) {
        std::cout << "\033[2J\033[1;1H";
        system("clear");
        printStats();
        cout << endl;

        if (lastMove != Invalid) {
            printWinner(lastMove, computerMove);
        }

        moves = getPlayerMove();

        for (const Move &move : moves) {
            totalGames++;
            lastMove = move;

            if (move == Quit) {
                isRunning = false;
                break;
            }

            computerMove = getComputerMove();
            playerMoves.push_back(move);

            int winner = getWinner(move, computerMove);
            if (winner > 0)
                playerWins++;
            else if (winner < 0)
                computerWins++;
        }
    }


    return 0;
}

Move getBeatingMove(Move moveToBeat) {
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

Move findPattern(const int searchSize) {
    vector<Move> pattern;
    for (int j = playerMoves.size() - 1; j >= max(static_cast<int>(playerMoves.size())-1-searchSize, 0); j--) {
        pattern.insert(pattern.begin(), playerMoves[j]);
    }

    int matches = 0;
    for (int i = max(static_cast<int>(playerMoves.size()) - 1 - searchSize, 0); i >= min(searchSize, MAX_SEARCH_SIZE); i--) {
        vector<Move> subPattern;
        for (int j = i; j >= max(i-searchSize, 0); j--) {
            subPattern.insert(subPattern.begin(), playerMoves[j]);
        }

        // Check if subPattern is equal to pattern
        if (subPattern == pattern) {
            // cout << "Pattern found! " << getMoveString(pattern) << endl;
            return getBeatingMove(playerMoves[min(i+1, static_cast<int>(playerMoves.size())-1)]);
        }
    }

    return Inconclusive;
}

Move getComputerMove() {
    for (int i = MAX_MATCHES; i >= MIN_MATCHES ; i--) {
        Move patternMove  = findPattern(i);
        if (patternMove != Inconclusive) {
            return patternMove;
        }
    }

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

        for (int i = 0; i < playerInput.size(); i++) {
            auto c = playerInput[i];
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

int getWinner(Move playerMove, Move computerMove) {
    if (playerMove == Invalid || playerMove == Inconclusive) {
        return 0;
    }
    if (playerMove == computerMove) {
        return 0;
    } else if (playerMove == Rock && computerMove == Scissors) {
        return 1;
    } else if (playerMove == Paper && computerMove == Rock) {
        return 1;
    } else if (playerMove == Scissors && computerMove == Paper) {
        return 1;
    } else {
        return -1;
    }
}

void printWinner(Move playerMove, Move computerMove) {
    int winner = getWinner(playerMove, computerMove);
    if (winner == 0) {
        cout << "It's a tie! You both chose " << moveToString(playerMove) << "." << endl;
    } else if (winner == 1) {
        cout << "You win! " << moveToString(playerMove) << " beats " << moveToString(computerMove) << "." << endl;
    } else {
        cout << "You lose! " << moveToString(computerMove) << " beats " << moveToString(playerMove) << "." << endl;
    }
}

void printStats() {
    cout << "Player: " << playerWins << ", Computer: " << computerWins << ", Total Games: " << totalGames << endl;
}

string moveToString(Move move) {
    if (move == Rock) {
        return "Rock";
    } else if (move == Paper) {
        return "Paper";
    } else {
        return "Scissors";
    }
}

string getMoveString(std::vector<Move> moves) {
    string moveString = "";
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