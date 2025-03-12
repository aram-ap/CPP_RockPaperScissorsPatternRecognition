//
// Created by Aram Aprahamian on 3/11/25.
//

#ifndef MAIN_H
#define MAIN_H

enum Move {
    Rock,
    Paper,
    Scissors,
    Invalid,
    Quit,
    Inconclusive
};

Move getComputerMove();

std::vector<Move> getPlayerMove();
int getWinner(Move playerMove, Move computerMove);
void printWinner(Move playerMove, Move computerMove);
void printStats();
void updateStats(int winner, Move playerMove, Move computerMove);
std::string moveToString(Move move);
std::string getMoveString(const std::vector<Move> &moves);

#endif //MAIN_H
