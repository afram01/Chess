#include "Pawn.h"
#include "Board.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <cmath>

using namespace std;

Pawn::Pawn(Color color, Position pos) 
    : Piece(color, PieceType::PAWN, pos, false) {}

bool Pawn::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const {
    if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7 ||
        toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) {
        return false;
    }
    
    if (fromRow == toRow && fromCol == toCol) {
        return false;
    }
    
    int direction = (color == Color::WHITE) ? -1 : 1;
    int startRow   = (color == Color::WHITE) ? 6 : 1;
    
    if (board.getCurrentSeason() == SPRING) {
        Position fromPos(fromRow, fromCol);
        if (!board.canPieceMoveInSeason(fromPos)) {
            return false;
        }
    }
    
    int rowDiff = toRow - fromRow;
    int colDiff = abs(toCol - fromCol);
    
    if (rowDiff == direction && colDiff == 0) {
        Position targetPos(toRow, toCol);
        return board.getPieceAt(targetPos) == nullptr;
    }
    
    if (rowDiff == 2 * direction && colDiff == 0 && fromRow == startRow) {
        Position oneStep(fromRow + direction, fromCol);
        Position twoStep(toRow, toCol);
        
        if (board.getPieceAt(oneStep) == nullptr && board.getPieceAt(twoStep) == nullptr) {
            return true;
        }
    }
    
    if (rowDiff == direction && colDiff == 1) {
        Position targetPos(toRow, toCol);
        Piece* targetPiece = board.getPieceAt(targetPos);
        
        if (targetPiece && targetPiece->getColor() != color) {
            return true;
        }
        
        if (!targetPiece && board.isEnPassantAvailable()) {
            Position enPassantSquare = board.getEnPassantSquare();
            
            if (toRow == enPassantSquare.row && toCol == enPassantSquare.col) {
                Position enemyPawnPos(fromRow, toCol);
                Piece* enemyPawn = board.getPieceAt(enemyPawnPos);
                
                if (enemyPawn && enemyPawn->getColor() != color &&
                    (enemyPawn->getType() == PieceType::PAWN || 
                     enemyPawn->getType() == PieceType::SPECIAL_PAWN)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

vector<Position> Pawn::getPossibleMoves(int row, int col, const Board& board) const {
    vector<Position> possibleMoves;
    
    int direction = (color == Color::WHITE) ? -1 : 1;
    int startRow   = (color == Color::WHITE) ? 6 : 1;
    
    if (board.getCurrentSeason() == SPRING) {
        Position currentPos(row, col);
        if (!board.canPieceMoveInSeason(currentPos)) {
            return possibleMoves;
        }
    }
    
    int newRow = row + direction;
    if (newRow >= 0 && newRow <= 7) {
        Position forwardPos(newRow, col);
        if (board.getPieceAt(forwardPos) == nullptr) {
            possibleMoves.push_back(forwardPos);
            
            if (row == startRow) {
                int doubleRow = row + 2 * direction;
                if (doubleRow >= 0 && doubleRow <= 7) {
                    Position doublePos(doubleRow, col);
                    if (board.getPieceAt(doublePos) == nullptr) {
                        possibleMoves.push_back(doublePos);
                    }
                }
            }
        }
    }
    
    int captureDirections[2] = {-1, 1};
    for (int dc : captureDirections) {
        int newCol = col + dc;
        if (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <= 7) {
            Position targetPos(newRow, newCol);
            Piece* targetPiece = board.getPieceAt(targetPos);
            
            if (targetPiece && targetPiece->getColor() != color) {
                possibleMoves.push_back(targetPos);
            }
            
            if (board.isEnPassantAvailable()) {
                Position epSquare = board.getEnPassantSquare();
                
                if (targetPos.row == epSquare.row && targetPos.col == epSquare.col) {
                    Position enemyPawnPos(row, newCol);
                    Piece* enemyPawn = board.getPieceAt(enemyPawnPos);
                    
                    const auto& history = board.getMoveHistory();
                    if (!history.empty()) {
                        Move lastMove = history.back();
                        if (lastMove.to.row == enemyPawnPos.row && 
                            lastMove.to.col == enemyPawnPos.col &&
                            abs(lastMove.to.row - lastMove.from.row) == 2) {
                            possibleMoves.push_back(targetPos);
                        }
                    }
                }
            }
        }
    }

    return possibleMoves;
}

bool Pawn::hasSpecialAbility() const {
    return true;
}

void Pawn::useSpecialAbility(int row, int col, Board& board) {
    if ((color == Color::WHITE && row == 0) || (color == Color::BLACK && row == 7)) {
        cout << "Promotion!" << endl 
             << "Select which piece do you want:" << endl
             << "1. Queen" << endl
             << "2. Rook" << endl
             << "3. Bishop" << endl
             << "4. Knight" << endl
             << "Enter your choice (1-4): ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        PieceType newType;
        switch (choice) {
            case 1: newType = PieceType::QUEEN; break;
            case 2: newType = PieceType::ROOK; break;
            case 3: newType = PieceType::BISHOP; break;
            case 4: newType = PieceType::KNIGHT; break;
            default: newType = PieceType::QUEEN;
        }
        
        board.promotePawn(Position(row, col), newType);
    }
}

string Pawn::getSymbol() const {
    return (color == Color::WHITE) ? "♙ " : "♟ ";
}

unique_ptr<Piece> Pawn::clone() const {
    return make_unique<Pawn>(*this);
}

string Pawn::serialize() const {
    return Piece::serialize();
}
