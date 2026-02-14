#ifndef BOARD_H
#define BOARD_H

#include "Piece.hpp"

class Board {
private:
    Piece* grid[8][8];

public:
    Board() {
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                grid[i][j] = nullptr;
    }

    Piece* getPieceAt(int x, int y) {
        if (x < 0 || x >= 8 || y < 0 || y >= 8) return nullptr;
        return grid[x][y];
    }

    bool movePiece(Position from, Position to) {
        Piece* p = getPieceAt(from.x, from.y);
        if (!p) return false;
        
        grid[to.x][to.y] = p;
        grid[from.x][from.y] = nullptr;
        return true;
    }
};

#endif
