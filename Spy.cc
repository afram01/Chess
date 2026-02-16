#include "Spy.h"
#include "Board.hpp"
#include "Types.hpp"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <random>

using namespace std;

Spy::Spy(Color realOwner, unique_ptr<Piece> pieceToDisguise) 
    : Piece(Color::WHITE, PieceType::SPY, pieceToDisguise->getPosition(), true),
      realOwner(realOwner),
      revealed(false),
      moveCounter(0)
{
    disguisedPiece = std::move(pieceToDisguise);
    

    color = disguisedPiece->getColor();
    
    disguisedPiece->setPosition(position);
}

bool Spy::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const {
    if (!disguisedPiece) return false;
    
    return disguisedPiece->isValidMove(fromRow, fromCol, toRow, toCol, board);
}

vector<Position> Spy::getPossibleMoves(int row, int col, const Board& board) const {
    if (!disguisedPiece) return {};
    
    return disguisedPiece->getPossibleMoves(row, col, board);
}

bool Spy::hasSpecialAbility() const {
    return true;
}

void Spy::useSpecialAbility(int, int, Board&) {
    if (!revealed) {
        reveal();
    } else {
        cout << "This spy has already been revealed!" << endl;
    }
}

void Spy::incrementMoveCounter() {
    if (!revealed) {
        moveCounter++;
    }
}

void Spy::reveal() {
    if (!revealed) {
        revealed = true;
        
        map<PieceType, string> pieceNames = {
            {PieceType::PAWN, "Pawn"},
            {PieceType::ROOK, "Rook"},
            {PieceType::KNIGHT, "Knight"},
            {PieceType::BISHOP, "Bishop"},
            {PieceType::QUEEN, "Queen"},
            {PieceType::KING, "King"}
        };
        
        PieceType disguiseType = disguisedPiece->getType();
        string disguiseName = pieceNames.count(disguiseType) ? 
                             pieceNames[disguiseType] : "Unknown";
        
        cout << "\n===========================================" << endl;
        cout << "🔍 SPY REVEALED! 🔍" << endl;
        cout << "===========================================" << endl;
        cout << "Piece at position " << (char)('a' + position.col) << (8 - position.row) << endl;
        cout << "which was displayed as a " << disguiseName << " ";
        cout << (disguisedPiece->getColor() == Color::WHITE ? "White" : "Black") << "," << endl;
        cout << "is actually owned by player ";
        cout << (realOwner == Color::WHITE ? "White" : "Black") << "!" << endl;
        cout << "===========================================\n" << endl;
        
        color = realOwner;
    }
}

string Spy::getSymbol() const {
    if (!disguisedPiece) return "S?";
    
    string symbol = disguisedPiece->getSymbol();
    
    if (revealed) {
        if (!symbol.empty()) {
            char last = symbol.back();
            if (last == ' ' || last == '+' || (last >= '0' && last <= '9')) {
                symbol.pop_back();
            }
        }
        symbol += "!"; 
    }
    
    return symbol;
}

unique_ptr<Piece> Spy::clone() const {
    if (!disguisedPiece) return nullptr;
    
    auto clonedDisguise = disguisedPiece->clone();
    
    auto clonedSpy = make_unique<Spy>(realOwner, std::move(clonedDisguise));
    
    clonedSpy->revealed = revealed;
    clonedSpy->moveCount = moveCount;
    clonedSpy->color = color;
    clonedSpy->position = position;
    clonedSpy->moveCounter = moveCounter;
    
    return clonedSpy;
}

string Spy::serialize() const {
    if (!disguisedPiece) return "";
    
    return Piece::serialize() + " " +
           to_string(revealed ? 1 : 0) + " " +
           to_string(static_cast<int>(realOwner)) + " " +
           to_string(moveCounter) + " " +
           disguisedPiece->serialize();
}

bool Spy::isRevealed() const {
    return revealed;
}

Color Spy::getRealOwner() const {
    return realOwner;
}

PieceType Spy::getDisguisedAs() const {
    if (!disguisedPiece) return PieceType::PAWN;
    return disguisedPiece->getType();
}

Color Spy::getDisplayColor() const {

    if (!revealed && disguisedPiece) {
        return disguisedPiece->getColor();
    }
    return color;
}

int Spy::getMovesUntilReveal() const {
    return max(0, REVEAL_THRESHOLD - moveCounter);
}