#ifndef KING_H
#define KING_H

#include "piece.h"
#include <vector>

class King : public Piece {
public:
    King(Color color, Position pos);
    
    virtual bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const override;
    virtual bool hasSpecialAbility() const override { return true; }
    virtual void useSpecialAbility(int row, int col, Board& board) override;
    virtual std::string getSymbol() const override;
    virtual std::unique_ptr<Piece> clone() const override;
    virtual std::string serialize() const override;
    virtual std::vector<Position> getPossibleMoves(int row, int col, const Board& board) const override;
private:
    bool isCastleMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool canCastle(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const;
};

#endif