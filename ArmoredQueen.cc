#include "ArmoredQueen.h"
#include "Board.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <algorithm>

using namespace std;

ArmoredQueen::ArmoredQueen(Color color, Position pos) 
    : Piece(color, PieceType::ARMORED_QUEEN, pos, true), 
      armor(1), 
      wasAttackedThisTurn(false) {}

bool ArmoredQueen::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const {
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
        Piece* blockingPiece = board.getPieceAt(Position(currentRow, currentCol));
        if (blockingPiece != nullptr) {
            return false;
        }
        currentRow += rowStep;
        currentCol += colStep;
    }
    
    Piece* targetPiece = board.getPieceAt(Position(toRow, toCol));
    if (targetPiece) {
        return targetPiece->getColor() != color;
    }
    
    return true;
}

vector<Position> ArmoredQueen::getPossibleMoves(int row, int col, const Board& board) const {
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
                
                Piece* targetPiece = board.getPieceAt(Position(newRow, newCol));
                if (targetPiece != nullptr) {
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

bool ArmoredQueen::hasSpecialAbility() const {
    return true;
}

void ArmoredQueen::useSpecialAbility(int, int, Board&) {
    cout << "Armored Queen's armor activated!" << endl;
}

string ArmoredQueen::getSymbol() const {
    if (color == Color::WHITE) {
        if (armor == 2) return "♕2";
        else if (armor == 1) return "♕1";
        else return "♕0";
    } else {
        if (armor == 2) return "♛2";
        else if (armor == 1) return "♛1";
        else return "♛0";
    }
}

unique_ptr<Piece> ArmoredQueen::clone() const {
    auto cloned = make_unique<ArmoredQueen>(*this);
    cloned->armor = this->armor;
    cloned->wasAttackedThisTurn = this->wasAttackedThisTurn;
    return cloned;
}

string ArmoredQueen::serialize() const {
    return Piece::serialize() + " " + to_string(armor) + " " + 
           (wasAttackedThisTurn ? "1" : "0");
}

bool ArmoredQueen::attemptCapture() {
    if (wasAttackedThisTurn) {
        std::cout << "Armored Queen captured!" << std::endl;
        return true;
    }
    
    wasAttackedThisTurn = true;

    if(armor > 0){
        armor-- ;
        if (armor == 1) {
            std::cout << "Armored Queen hit! First layer broken. One layer remains." << std::endl;
        } else if (armor == 0) {
            std::cout << "Armored Queen hit! All armor broken. Next attack will capture." << std::endl;
        }
        return false;
    }
    std::cout << "Armored Queen captured! (No armor left)" << std::endl;
    return true ;
}

void ArmoredQueen::resetAttackFlag() {
    wasAttackedThisTurn = false; 
}
