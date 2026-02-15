#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "Knight.h"
#include "Board.hpp"

using namespace std;

Knight :: Knight(Color c, Position pos) : 
    Piece(c, PieceType::KNIGHT, pos, false) {
}


bool Knight::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const {
    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
        toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) {
        return false;
    }
    
    if (fromRow == toRow && fromCol == toCol) {
        return false;
    }
    
    if (board.getCurrentSeason() == AUTUMN) {
        Position fromPos(fromRow, fromCol);
        if (!board.canPieceMoveInSeason(fromPos)) {
            return false;
        }
    }
    
    int rowDiff = abs(fromRow - toRow);
    int colDiff = abs(fromCol - toCol);
    
    if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))) {
        return false;
    }
    
    Position targetPos(toRow, toCol);
    const Piece* destPiece = board.getPieceAt(targetPos);
    if (destPiece && destPiece->getColor() == color) {
        return false;
    }
        
    return true;
}

vector<Position> Knight::getPossibleMoves(int row, int col, const Board& board) const {
    vector<Position> moves;
    
    if (board.getCurrentSeason() == AUTUMN) {
        Position currentPos(row, col);
        if (!board.canPieceMoveInSeason(currentPos)) {
            return moves; 
        }
    }
    
    int knightMoves[8][2] = {
        {-2, -1}, {-2, 1},
        {-1, -2}, {-1, 2},
        {1, -2}, {1, 2},
        {2, -1}, {2, 1}
    };
    
    for (const auto& move : knightMoves) {
        int newRow = row + move[0];
        int newCol = col + move[1];
        
        if (isValidMove(row, col, newRow, newCol, board)) {
            moves.push_back({newRow, newCol});
        }
    }
    
    return moves;
}

bool Knight::hasSpecialAbility() const {
    return false;
}

string Knight::getSymbol() const {
    return (color == Color::WHITE) ? "♘ " : "♞ ";
}

unique_ptr<Piece> Knight::clone() const {
    return make_unique<Knight>(*this);
}

string Knight::serialize() const {
    return Piece::serialize();
}