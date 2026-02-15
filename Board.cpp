#include "Board.hpp"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "SpecialPawn.h"
#include "ArmoredQueen.h"
#include "Spy.h"
#include "Joker.h"



Board::Board() {
}

Board::Board(const Board& other) {

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (other.grid[r][c]) {
                grid[r][c] = other.grid[r][c]->clone();
            }
        }
    }
    
    moveHistory = other.moveHistory;
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {

        clear();
        

        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                if (other.grid[r][c]) {
                    grid[r][c] = other.grid[r][c]->clone();
                }
            }
        }
        
        moveHistory = other.moveHistory;
    }
    return *this;
}


void Board::setupStandardBoard() {
    clear();

    for (int c = 0; c < 8; ++c) {
        grid[1][c] = std::make_unique<Pawn>(Color::BLACK, Position(1, c));
        grid[6][c] = std::make_unique<Pawn>(Color::WHITE, Position(6, c));
    }
    

    grid[0][0] = std::make_unique<Rook>(Color::BLACK, Position(0, 0));
    grid[0][7] = std::make_unique<Rook>(Color::BLACK, Position(0, 7));
    grid[7][0] = std::make_unique<Rook>(Color::WHITE, Position(7, 0));
    grid[7][7] = std::make_unique<Rook>(Color::WHITE, Position(7, 7));

    grid[0][1] = std::make_unique<Knight>(Color::BLACK, Position(0, 1));
    grid[0][6] = std::make_unique<Knight>(Color::BLACK, Position(0, 6));
    grid[7][1] = std::make_unique<Knight>(Color::WHITE, Position(7, 1));
    grid[7][6] = std::make_unique<Knight>(Color::WHITE, Position(7, 6));

    grid[0][2] = std::make_unique<Bishop>(Color::BLACK, Position(0, 2));
    grid[0][5] = std::make_unique<Bishop>(Color::BLACK, Position(0, 5));
    grid[7][2] = std::make_unique<Bishop>(Color::WHITE, Position(7, 2));
    grid[7][5] = std::make_unique<Bishop>(Color::WHITE, Position(7, 5));

    grid[0][3] = std::make_unique<Queen>(Color::BLACK, Position(0, 3));
    grid[0][4] = std::make_unique<King>(Color::BLACK, Position(0, 4));
    grid[7][3] = std::make_unique<Queen>(Color::WHITE, Position(7, 3));
    grid[7][4] = std::make_unique<King>(Color::WHITE, Position(7, 4));
}

void Board::setupSpecialBoard() {
    clear();
    
    grid[6][0] = std::make_unique<SpecialPawn>(Color::WHITE, Position(6, 0));
    grid[6][1] = std::make_unique<Pawn>(Color::WHITE, Position(6, 1));
    grid[6][2] = std::make_unique<Pawn>(Color::WHITE, Position(6, 2));
    grid[6][3] = std::make_unique<Joker>(Color::WHITE, Position(6, 3));
    grid[6][4] = std::make_unique<Pawn>(Color::WHITE, Position(6, 4));
    grid[6][5] = std::make_unique<Pawn>(Color::WHITE, Position(6, 5));
    grid[6][6] = std::make_unique<Pawn>(Color::WHITE, Position(6, 6));
    grid[6][7] = std::make_unique<Pawn>(Color::WHITE, Position(6, 7));
    
    grid[1][0] = std::make_unique<Pawn>(Color::BLACK, Position(1, 0));
    grid[1][1] = std::make_unique<Pawn>(Color::BLACK, Position(1, 1));
    grid[1][2] = std::make_unique<Pawn>(Color::BLACK, Position(1, 2));
    grid[1][3] = std::make_unique<Joker>(Color::BLACK, Position(1, 3));
    grid[1][4] = std::make_unique<SpecialPawn>(Color::BLACK, Position(1, 4));
    grid[1][5] = std::make_unique<Pawn>(Color::BLACK, Position(1, 5));
    grid[1][6] = std::make_unique<Pawn>(Color::BLACK, Position(1, 6));
    grid[1][7] = std::make_unique<Pawn>(Color::BLACK, Position(1, 7));
    
    grid[0][0] = std::make_unique<Rook>(Color::BLACK, Position(0, 0));
    grid[0][7] = std::make_unique<Rook>(Color::BLACK, Position(0, 7));
    grid[7][0] = std::make_unique<Rook>(Color::WHITE, Position(7, 0));
    grid[7][7] = std::make_unique<Rook>(Color::WHITE, Position(7, 7));
    

    grid[0][1] = std::make_unique<Knight>(Color::BLACK, Position(0, 1));
    grid[0][6] = std::make_unique<Knight>(Color::BLACK, Position(0, 6));
    grid[7][1] = std::make_unique<Knight>(Color::WHITE, Position(7, 1));
    grid[7][6] = std::make_unique<Knight>(Color::WHITE, Position(7, 6));

    grid[0][2] = std::make_unique<Bishop>(Color::BLACK, Position(0, 2));
    grid[0][5] = std::make_unique<Bishop>(Color::BLACK, Position(0, 5));
    grid[7][2] = std::make_unique<Bishop>(Color::WHITE, Position(7, 2));
    grid[7][5] = std::make_unique<Bishop>(Color::WHITE, Position(7, 5));

    grid[0][3] = std::make_unique<ArmoredQueen>(Color::BLACK, Position(0, 3));
    grid[7][3] = std::make_unique<ArmoredQueen>(Color::WHITE, Position(7, 3));

    grid[0][4] = std::make_unique<King>(Color::BLACK, Position(0, 4));
    grid[7][4] = std::make_unique<King>(Color::WHITE, Position(7, 4));

    grid[2][4] = std::make_unique<Spy>(Color::WHITE, Position(2, 4));
    grid[5][3] = std::make_unique<Spy>(Color::BLACK, Position(5, 3));
}

void Board::clear() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            grid[r][c].reset();
        }
    }
    moveHistory.clear();
}


Piece* Board::getPiece(int row, int col) const {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        return nullptr;
    }
    return grid[row][col].get();
}

Piece* Board::getPieceAt(Position pos) const {
    return getPiece(pos.row, pos.col);
}

void Board::setPiece(int row, int col, std::unique_ptr<Piece> piece) {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        grid[row][col] = std::move(piece);
        if (grid[row][col]) {
            grid[row][col]->setPosition(Position(row, col));
        }
    }
}

void Board::setPieceAt(Position pos, std::unique_ptr<Piece> piece) {
    setPiece(pos.row, pos.col, std::move(piece));
}

std::unique_ptr<Piece> Board::removePiece(int row, int col) {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        return std::move(grid[row][col]);
    }
    return nullptr;
}

std::unique_ptr<Piece> Board::removePieceAt(Position pos) {
    return removePiece(pos.row, pos.col);
}

bool Board::isSquareEmpty(Position pos) const {
    return getPieceAt(pos) == nullptr;
}

bool Board::isSquareEmpty(int row, int col) const {
    return getPiece(row, col) == nullptr;
}

void Board::executeMove(const Move& move) {
    executeMove(move.from, move.to);
}

void Board::executeMove(Position from, Position to) {
    auto piece = removePieceAt(from);
    if (piece) {
        piece->setPosition(to);
        piece->setHasMoved(true);
        piece->incrementMoveCount();
        setPieceAt(to, std::move(piece));
    }
}

void Board::undoLastMove() {
    if (moveHistory.empty()) return;
    
    Move lastMove = moveHistory.back();
    moveHistory.pop_back();

    auto piece = removePieceAt(lastMove.to);
    if (piece) {
        piece->setPosition(lastMove.from);
        setPieceAt(lastMove.from, std::move(piece));
    }
}


void Board::addMoveToHistory(const Move& move) {
    moveHistory.push_back(move);
}

void Board::clearHistory() {
    moveHistory.clear();
}


Position Board::findKing(Color color) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = getPiece(r, c);
            if (piece && piece->getType() == PieceType::KING && piece->getColor() == color) {
                return Position(r, c);
            }
        }
    }
    return Position(-1, -1);
}

std::vector<Piece*> Board::getAllPieces(Color color) const {
    std::vector<Piece*> pieces;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = getPiece(r, c);
            if (piece && piece->getColor() == color) {
                pieces.push_back(piece);
            }
        }
    }
    return pieces;
}

int Board::countPieces(Color color) const {
    return getAllPieces(color).size();
}

int Board::getMaterialValue(Color color) const {
    int value = 0;
    const int PIECE_VALUES[] = {
        0,  
        9,   
        5,   
        3,  
        3, 
        1,  
        2,  
        10, 
        2,  
        3   
    };
    
    for (const auto& piece : getAllPieces(color)) {
        int typeIndex = static_cast<int>(piece->getType());
        if (typeIndex >= 0 && typeIndex < 10) {
            value += PIECE_VALUES[typeIndex];
        }
    }
    
    return value;
}

Board Board::createCopy() const {
    return Board(*this);
}
