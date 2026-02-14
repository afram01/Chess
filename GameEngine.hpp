#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include "Board.hpp"

struct Move {
    Position from;
    Position to;
};

class GameEngine {
private:
    Board* board;
    Color currentTurn;

public:
    GameEngine(Board* b) : board(b), currentTurn(Color::WHITE) {}

    void processMove(Move move) {
        if (validateMove(move)) {
            board->movePiece(move.from, move.to);
            switchPlayerTurn();
        }
    }

    void switchPlayerTurn() {
        currentTurn = (currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }

    bool validateMove(Move move) {
        Piece* p = board->getPieceAt(move.from.x, move.from.y);
        return (p != nullptr && p->getColor() == currentTurn);
    }
};

#endif
