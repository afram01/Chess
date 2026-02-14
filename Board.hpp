#ifndef BOARD_H
#define BOARD_H

#include "Piece.hpp"
#include <vector>

class Board {
private:
    Piece* grid[8][8];

public:
    Board() {
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                grid[i][j] = nullptr;
    }

    void setupBoard() {}
    Piece* getPieceAt(int x, int y) { return nullptr; }
    bool movePiece(Position from, Position to) { return false; }
    bool isInCheck(Color player) { return false; }
    bool isCheckmate(Color player) { return false; }
};

#endif
