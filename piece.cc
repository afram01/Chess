#include "piece.h"
#include <iostream>

Piece::Piece(Color c, PieceType t, Position pos, bool special) 
    : color(c), type(t), position(pos), isSpecial(special) {}

Color Piece::getColor() const { return color; }

PieceType Piece::getType() const { return type; }

bool Piece::getIsSpecial() const { return isSpecial; }

int Piece::getMoveCount() const { return moveCount; }

void Piece::incrementMoveCount() { moveCount++; }

bool Piece::hasSpecialAbility() const { return isSpecial ; }

void Piece::useSpecialAbility(int, int, Board&) {
}

bool Piece:: hasMoved() const { return moveCount > 0; }

Position Piece:: getPosition() const { return position; }

void Piece:: setPosition(Position pos) { position = pos; }

void Piece:: setHasMoved(bool moved) { 
    if (moved && moveCount == 0) moveCount = 1;
    else if (!moved) moveCount = 0;
}

std::vector<Position> Piece:: getPossibleMoves(const Board& board) const {
    return getPossibleMoves(position.row, position.col, board);
}

std::string Piece:: serialize() const {
    return  std::to_string(static_cast<int>(type)) + " " +
            std::to_string(static_cast<int>(color)) + " " +
            std::to_string(position.row) + " " +
            std::to_string(position.col) + " " +
            std::to_string(hasMoved() ? 1 : 0);
}