#include "Rook.h"
#include "Board.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <algorithm>

using namespace std;

Rook::Rook(Color color, Position pos) 
    : Piece(color, PieceType::ROOK, pos, false) {}

bool Rook::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const {
    if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7 ||
        toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) {
        return false;
    }
    
    if (fromRow == toRow && fromCol == toCol) {
        return false;
    }
    
    if (fromRow != toRow && fromCol != toCol) {
        return false;
    }
    
    if (board.getCurrentSeason() == WINTER) {
        int distance = abs(toRow - fromRow) + abs(toCol - fromCol);
        if (distance > 4) {
            return false;
        }
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

vector<Position> Rook::getPossibleMoves(int row, int col, const Board& board) const {
    vector<Position> possibleMoves;
    
    int maxSteps = 7;
    if (board.getCurrentSeason() == WINTER) {
        maxSteps = 4;
    }
    
    int directions[4][2] = {
        {-1,  0},
        { 1,  0},
        { 0, -1},
        { 0,  1}
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

bool Rook::hasSpecialAbility() const {
    return false;
}

string Rook::getSymbol() const {
    return (color == Color::WHITE) ? "♖ " : "♜ ";
}

unique_ptr<Piece> Rook::clone() const {
    return make_unique<Rook>(*this);
}

string Rook::serialize() const {
    return Piece::serialize();
}