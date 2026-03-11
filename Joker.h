#ifndef JOKER_H
#define JOKER_H

#include "piece.h"
#include "Board.hpp"
#include <memory>

class Joker : public Piece {
private:
    int transformsRemaining;
    PieceType currentMimic;
    bool isTransformedThisTurn;
    
public:
    Joker(Color c, Position pos)
        : Piece(c, PieceType::JOKER, pos, true),
          transformsRemaining(2),
          currentMimic(PieceType::PAWN),
          isTransformedThisTurn(false) {}
    
    Joker(const Joker& other)
        : Piece(other.color, other.type, other.position, true),
          transformsRemaining(other.transformsRemaining),
          currentMimic(other.currentMimic),
          isTransformedThisTurn(other.isTransformedThisTurn) {
        moveCount = other.moveCount;
    }
    
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const override;
    
    bool transform(PieceType targetType);
    bool canTransform() const { return transformsRemaining > 0; }
    int getTransformsRemaining() const { return transformsRemaining; }
    PieceType getCurrentMimic() const { return currentMimic; }
    void resetToPawn();
    void endTurnReset();
    
    bool hasSpecialAbility() const override { return true; }
    void useSpecialAbility(int row, int col, Board& board) override;
    
    std::string getSymbol() const override;
    std::vector<Position> getPossibleMoves(int row, int col, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
    
    std::string serialize() const override;
    
    bool getIsTransformedThisTurn() const { return isTransformedThisTurn; }
    void setTransformedThisTurn(bool transformed) { isTransformedThisTurn = transformed; }
};

#endif