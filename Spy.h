#ifndef SPY_H
#define SPY_H

#include "piece.h"
#include <memory>
#include <random>

class Spy : public Piece {
private:
    Color realOwner; 
    Color disguisedOwner;  
    bool revealed;          
    std::unique_ptr<Piece> disguisedPiece; 
    int moveCounter;         
    static constexpr int REVEAL_THRESHOLD = 5; 

public:
    Spy(Color realOwner, std::unique_ptr<Piece> pieceToDisguise);
    
    virtual bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const override;
    virtual std::vector<Position> getPossibleMoves(int row, int col, const Board& board) const override;
    virtual bool hasSpecialAbility() const override;
    virtual void useSpecialAbility(int row, int col, Board& board) override;
    virtual std::string getSymbol() const override;
    virtual std::unique_ptr<Piece> clone() const override;
    virtual std::string serialize() const override;
    
    void incrementMoveCounter();
    
    void reveal();
    
    bool isRevealed() const;
    Color getRealOwner() const;
    Color getEffectiveOwner() const; 
    PieceType getDisguisedAs() const;
    Color getDisplayColor() const;
    int getMovesUntilReveal() const;
    
    const Piece* getDisguisedPiece() const { return disguisedPiece.get(); }
};

#endif