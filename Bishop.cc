#include <iostream>
#include <cmath>
#include <memory>
#include <algorithm>
#include "Bishop.h"

using namespace std;

Bishop::Bishop(Color color, Position pos) 
    : Piece(color, PieceType::BISHOP, pos , false) {}

bool Bishop::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const {
    if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7 ||
        toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) {
        return false;
    }
    
    if (fromRow == toRow && fromCol == toCol) {
        return false;
    }
    
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);
    
    if (rowDiff != colDiff) {
        return false;
    }
    
    if (board.getCurrentSeason() == SUMMER) {
        if (rowDiff > 3) {
            return false;
        }
    }
    
    int rowStep = (toRow > fromRow) ? 1 : -1;
    int colStep = (toCol > fromCol) ? 1 : -1;
    
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

vector<Position> Bishop::getPossibleMoves(int row, int col, const Board& board) const {
    vector<Position> possibleMoves;
    
    int maxSteps = 7; 
    if (board.getCurrentSeason() == SUMMER) {
        maxSteps = 3; 
    }
    
    int directions[4][2] = {
        {-1, -1},
        {-1,  1},
        { 1, -1}, 
        { 1,  1} 
    };
    
    for (const auto& dir : directions) {
        for (int steps = 1; steps <= maxSteps; steps++) {
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
            } else {
                break;
            }
        }
    }
    
    return possibleMoves;
}

bool Bishop::hasSpecialAbility() const {
    return false;
}

string Bishop::getSymbol() const {
    return (color == Color::WHITE) ? "♗ " : "♝ ";
}

unique_ptr<Piece> Bishop::clone() const {
    return make_unique<Bishop>(*this);
}

string Bishop::serialize() const {
    return Piece::serialize();
}