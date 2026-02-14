#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Board.hpp"

class GameEngine {
private:
    Board* board;
    Color currentTurn;

public:
    GameEngine(Board* b) : board(b), currentTurn(Color::WHITE) {}

    void switchPlayerTurn() {
        currentTurn = (currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }

    Color getCurrentTurn() const { return currentTurn; }
};

#endif
