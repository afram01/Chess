#ifndef ARMORED_QUEEN_H
#define ARMORED_QUEEN_H

#include "piece.h"

class ArmoredQueen : public Piece {
private:
    int armor;  
    bool wasAttackedThisTurn;  

public:
    ArmoredQueen(Color color, Position pos);
    
    virtual bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const override;
    virtual bool hasSpecialAbility() const override;
    virtual void useSpecialAbility(int row, int col, Board& board) override;
    virtual std::string getSymbol() const override;
    virtual std::unique_ptr<Piece> clone() const override;
    virtual std::vector<Position> getPossibleMoves(int row, int col, const Board& board) const override;
    virtual std::string serialize() const override;
    
    void resetAttackFlag();
    bool hasArmor() const { return armor > 0; }
    int getArmor() const { return armor; }
    void setArmor(int a) { armor = a; }
    bool attemptCapture();
    bool getWasAttackedThisTurn() const { return wasAttackedThisTurn; }
};

#endif