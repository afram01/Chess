#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Move.hpp"
#include "Position.hpp"

enum class Color {
    BLACK ,
    WHITE
};

enum class PieceType{
    KING ,
    QUEEN , 
    ROOK ,
    BISHOP ,
    KNIGHT ,
    PAWN ,
    SPECIAL_PAWN ,
    ARMORED_QUEEN ,
    SPY ,
    JOKER
};

class Board ;

class Piece {
protected :
    Color color ;
    PieceType type ;
    int moveCount = 0 ;
    Position position;
    bool isSpecial = false ;
    
public :

    Piece (Color c , PieceType t , Position pos, bool special = false) ;   
    virtual ~Piece () = default ;
    Color getColor() const ;
    void setColor(Color newColor) { color = newColor; }
    PieceType getType() const ;
    bool getIsSpecial() const ;
    int getMoveCount() const ;
    void incrementMoveCount() ;
    void decrementMoveCount() { if (moveCount > 0) moveCount--; }

    Position getPosition() const ;
    void setPosition(Position pos) ;
    void setHasMoved(bool moved) ;
    
    bool hasMoved() const ;

    virtual bool isValidMove(int fromRaw , int fromCol , int toRow , int toCol , const Board& board) const = 0 ; 
    virtual bool hasSpecialAbility() const ;
    virtual void useSpecialAbility(int row , int col , Board& board) ;
    virtual std:: string getSymbol() const = 0 ;
    virtual std::vector<Position> getPossibleMoves(int row, int col, const Board& board) const = 0;
    virtual std::unique_ptr<Piece> clone() const = 0 ; 
    virtual std::vector<Position> getPossibleMoves(const Board& board) const ;


    virtual std::string serialize() const ;
};

#endif