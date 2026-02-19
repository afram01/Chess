#include "Queen.h"
#include "Board.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <algorithm>

using namespace std;

Queen::Queen(Color color, Position pos) 
    : Piece(color, PieceType::QUEEN, pos, false) {}

bool Queen::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const {
    if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7 ||
        toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) {
        return false;
    }
    
    if (fromRow == toRow && fromCol == toCol) {
        return false;
    }
    
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);
    
    bool isRookMove = (fromRow == toRow || fromCol == toCol);
    bool isBishopMove = (rowDiff == colDiff);
    
    if (!isRookMove && !isBishopMove) {
        return false;
    }
    
    int rowStep = 0;
    int colStep = 0;
    
    if (toRow > fromRow) rowStep = 1;
    else if (toRow < fromRow) rowStep = -1;
    
    if (toCol > fromCol) colStep = 1;
    else if (toCol < fromCol) colStep = -1;
    
    int currentRow = fromRow + rowStep;
    int currentCol = fromCol + colStep;
    
    while (currentRow != toRow || currentCol != toCol) {
        Position currentPos(currentRow, currentCol);
        if (board.getPieceAt(currentPos) != nullptr) {
            return false;
        }
        currentRow += rowStep;
        currentCol += colStep;
    }
    
    Position targetPos(toRow, toCol);
    Piece* targetPiece = board.getPieceAt(targetPos);
    if (targetPiece) {
        return targetPiece->getColor() != color;
    }
    
    return true;
}

vector<Position> Queen::getPossibleMoves(int row, int col, const Board& board) const {
    vector<Position> possibleMoves;
    
    int directions[8][2] = {
        {-1,  0},
        { 1,  0},
        { 0, -1},
        { 0,  1},
        {-1, -1},
        {-1,  1},
        { 1, -1},
        { 1,  1}
    };
    
    for (const auto& dir : directions) {
        for (int steps = 1; steps <= 7; steps++) {
            int newRow = row + dir[0] * steps;
            int newCol = col + dir[1] * steps;
            
            if (newRow < 0 || newRow > 7 || newCol < 0 || newCol > 7) {
                break;
            }
            
            if (isValidMove(row, col, newRow, newCol, board)) {
                possibleMoves.push_back(Position(newRow, newCol));
                
                Position destPos(newRow, newCol);
                if (board.getPieceAt(destPos) != nullptr) {
                    break;
                }
            } 
            else {
                break;
            }
        }
    }
    
    return possibleMoves;
}

bool Queen::hasSpecialAbility() const {
    return false;
}

string Queen::getSymbol() const {
    return (color == Color::WHITE) ? "♕ " : "♛ ";
}

unique_ptr<Piece> Queen::clone() const {
    return make_unique<Queen>(*this);
}

string Queen::serialize() const {
    return Piece::serialize();
}