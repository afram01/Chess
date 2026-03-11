#ifndef SPECIALPAWN_H
#define SPECIALPAWN_H

#include "piece.h"
#include "Board.hpp"

class SpecialPawn : public Piece {
public:
    SpecialPawn(Color c, Position pos)
        : Piece(c, PieceType::SPECIAL_PAWN, pos, true) {}

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const override;
    std::string getSymbol() const override;
    std::vector<Position> getPossibleMoves(int row, int col, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
};

#endif