#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"
#include "Board.hpp"
#include "Types.hpp"
#include <vector>

class Bishop : public Piece
{
public:
    Bishop(Color color, Position pos);
    virtual bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board &board) const override;
    virtual bool hasSpecialAbility() const override;
    virtual std::string getSymbol() const override;
    virtual std::unique_ptr<Piece> clone() const override;
    virtual std::vector<Position> getPossibleMoves(int row, int col, const Board &board) const override;
    virtual std::string serialize() const override;

};

#endif