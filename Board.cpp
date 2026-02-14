#include "Board.hpp"
#include <sstream>
#include <algorithm>


Board::Board() : 
    enPassantAvailable(false),
    whiteKingMoved(false), blackKingMoved(false),
    whiteKingsideRookMoved(false), whiteQueensideRookMoved(false),
    blackKingsideRookMoved(false), blackQueensideRookMoved(false),
    bombThreshold(5),  
    whiteQueenCanDoubleMove(false), blackQueenCanDoubleMove(false),
    currentSeason(Season::NONE),
    movesSinceSeasonChange(0),
    movesPerSeasonChange(20) 
{
    clear();
}

Board::Board(const Board& other) : 
    enPassantAvailable(other.enPassantAvailable),
    enPassantSquare(other.enPassantSquare),
    whiteKingMoved(other.whiteKingMoved),
    blackKingMoved(other.blackKingMoved),
    whiteKingsideRookMoved(other.whiteKingsideRookMoved),
    whiteQueensideRookMoved(other.whiteQueensideRookMoved),
    blackKingsideRookMoved(other.blackKingsideRookMoved),
    blackQueensideRookMoved(other.blackQueensideRookMoved),
    bombThreshold(other.bombThreshold),
    pieceStationaryCounter(other.pieceStationaryCounter),
    whiteQueenCanDoubleMove(other.whiteQueenCanDoubleMove),
    blackQueenCanDoubleMove(other.blackQueenCanDoubleMove),
    currentSeason(other.currentSeason),
    movesSinceSeasonChange(other.movesSinceSeasonChange),
    movesPerSeasonChange(other.movesPerSeasonChange),
    armoredQueenHits(other.armoredQueenHits),
    jokerTransformsUsed(other.jokerTransformsUsed)
{
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (other.grid[r][c]) {
                grid[r][c] = other.grid[r][c]->clone();
            }
        }
    }
    
    moveHistory = other.moveHistory;
    
    if (other.lastCapturedPiece) {
        lastCapturedPiece = other.lastCapturedPiece->clone();
    }
    lastCapturedPosition = other.lastCapturedPosition;
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        clear();
        
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (other.grid[r][c]) {
                    grid[r][c] = other.grid[r][c]->clone();
                }
            }
        }
        
        moveHistory = other.moveHistory;
        enPassantSquare = other.enPassantSquare;
        enPassantAvailable = other.enPassantAvailable;
        
        whiteKingMoved = other.whiteKingMoved;
        blackKingMoved = other.blackKingMoved;
        whiteKingsideRookMoved = other.whiteKingsideRookMoved;
        whiteQueensideRookMoved = other.whiteQueensideRookMoved;
        blackKingsideRookMoved = other.blackKingsideRookMoved;
        blackQueensideRookMoved = other.blackQueensideRookMoved;
        
        bombThreshold = other.bombThreshold;
        pieceStationaryCounter = other.pieceStationaryCounter;
        
        whiteQueenCanDoubleMove = other.whiteQueenCanDoubleMove;
        blackQueenCanDoubleMove = other.blackQueenCanDoubleMove;
        
        currentSeason = other.currentSeason;
        movesSinceSeasonChange = other.movesSinceSeasonChange;
        movesPerSeasonChange = other.movesPerSeasonChange;
        
        armoredQueenHits = other.armoredQueenHits;
        jokerTransformsUsed = other.jokerTransformsUsed;
        
        if (other.lastCapturedPiece) {
            lastCapturedPiece = other.lastCapturedPiece->clone();
        }
        lastCapturedPosition = other.lastCapturedPosition;
    }
    return *this;
}


void Board::initialize() {
    clear();
    
    for (int c = 0; c < 8; c++) {
        grid[1][c] = std::make_unique<Pawn>(Color::WHITE, Position(1, c));
    }
    
    for (int c = 0; c < 8; c++) {
        grid[6][c] = std::make_unique<Pawn>(Color::BLACK, Position(6, c));
    }
    
    grid[0][0] = std::make_unique<Rook>(Color::WHITE, Position(0, 0));
    grid[0][7] = std::make_unique<Rook>(Color::WHITE, Position(0, 7));
    grid[7][0] = std::make_unique<Rook>(Color::BLACK, Position(7, 0));
    grid[7][7] = std::make_unique<Rook>(Color::BLACK, Position(7, 7));
    
    grid[0][1] = std::make_unique<Knight>(Color::WHITE, Position(0, 1));
    grid[0][6] = std::make_unique<Knight>(Color::WHITE, Position(0, 6));
    grid[7][1] = std::make_unique<Knight>(Color::BLACK, Position(7, 1));
    grid[7][6] = std::make_unique<Knight>(Color::BLACK, Position(7, 6));
    
    grid[0][2] = std::make_unique<Bishop>(Color::WHITE, Position(0, 2));
    grid[0][5] = std::make_unique<Bishop>(Color::WHITE, Position(0, 5));
    grid[7][2] = std::make_unique<Bishop>(Color::BLACK, Position(7, 2));
    grid[7][5] = std::make_unique<Bishop>(Color::BLACK, Position(7, 5));
    
    grid[0][3] = std::make_unique<Queen>(Color::WHITE, Position(0, 3));
    grid[7][3] = std::make_unique<Queen>(Color::BLACK, Position(7, 3));
    
    grid[0][4] = std::make_unique<King>(Color::WHITE, Position(0, 4));
    grid[7][4] = std::make_unique<King>(Color::BLACK, Position(7, 4));
    
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteKingsideRookMoved = false;
    whiteQueensideRookMoved = false;
    blackKingsideRookMoved = false;
    blackQueensideRookMoved = false;
}

void Board::initializeWithSpecialPieces() {
    clear();
    
    grid[1][0] = std::make_unique<SoldierPlus>(Color::WHITE, Position(1, 0));
    grid[1][1] = std::make_unique<Pawn>(Color::WHITE, Position(1, 1));
    grid[1][2] = std::make_unique<Pawn>(Color::WHITE, Position(1, 2));
    grid[1][3] = std::make_unique<Joker>(Color::WHITE, Position(1, 3));
    grid[1][4] = std::make_unique<Pawn>(Color::WHITE, Position(1, 4));
    grid[1][5] = std::make_unique<Pawn>(Color::WHITE, Position(1, 5));
    grid[1][6] = std::make_unique<Pawn>(Color::WHITE, Position(1, 6));
    grid[1][7] = std::make_unique<Pawn>(Color::WHITE, Position(1, 7));
    
    grid[6][0] = std::make_unique<Pawn>(Color::BLACK, Position(6, 0));
    grid[6][1] = std::make_unique<Pawn>(Color::BLACK, Position(6, 1));
    grid[6][2] = std::make_unique<Pawn>(Color::BLACK, Position(6, 2));
    grid[6][3] = std::make_unique<Joker>(Color::BLACK, Position(6, 3));
    grid[6][4] = std::make_unique<SoldierPlus>(Color::BLACK, Position(6, 4));
    grid[6][5] = std::make_unique<Pawn>(Color::BLACK, Position(6, 5));
    grid[6][6] = std::make_unique<Pawn>(Color::BLACK, Position(6, 6));
    grid[6][7] = std::make_unique<Pawn>(Color::BLACK, Position(6, 7));
    
    grid[0][0] = std::make_unique<Rook>(Color::WHITE, Position(0, 0));
    grid[0][7] = std::make_unique<Rook>(Color::WHITE, Position(0, 7));
    grid[7][0] = std::make_unique<Rook>(Color::BLACK, Position(7, 0));
    grid[7][7] = std::make_unique<Rook>(Color::BLACK, Position(7, 7));
    
    grid[0][1] = std::make_unique<Knight>(Color::WHITE, Position(0, 1));
    grid[0][6] = std::make_unique<Knight>(Color::WHITE, Position(0, 6));
    grid[7][1] = std::make_unique<Knight>(Color::BLACK, Position(7, 1));
    grid[7][6] = std::make_unique<Knight>(Color::BLACK, Position(7, 6));
    
    grid[0][2] = std::make_unique<Bishop>(Color::WHITE, Position(0, 2));
    grid[0][5] = std::make_unique<Bishop>(Color::WHITE, Position(0, 5));
    grid[7][2] = std::make_unique<Bishop>(Color::BLACK, Position(7, 2));
    grid[7][5] = std::make_unique<Bishop>(Color::BLACK, Position(7, 5));
    
    grid[0][3] = std::make_unique<ArmoredQueen>(Color::WHITE, Position(0, 3));
    grid[7][3] = std::make_unique<ArmoredQueen>(Color::BLACK, Position(7, 3));
    
    grid[0][4] = std::make_unique<King>(Color::WHITE, Position(0, 4));
    grid[7][4] = std::make_unique<King>(Color::BLACK, Position(7, 4));
    
    grid[2][4] = std::make_unique<Spy>(Color::WHITE, Color::BLACK, Position(2, 4));
    grid[5][3] = std::make_unique<Spy>(Color::BLACK, Color::WHITE, Position(5, 3));
    
    armoredQueenHits[Position(0, 3)] = 0;
    armoredQueenHits[Position(7, 3)] = 0;
    jokerTransformsUsed[Position(1, 3)] = 0;
    jokerTransformsUsed[Position(6, 3)] = 0;
    
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteKingsideRookMoved = false;
    whiteQueensideRookMoved = false;
    blackKingsideRookMoved = false;
    blackQueensideRookMoved = false;
}

void Board::clear() {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            grid[r][c].reset();
        }
    }
    moveHistory.clear();
    enPassantAvailable = false;
    enPassantSquare = Position();
    pieceStationaryCounter.clear();
    armoredQueenHits.clear();
    jokerTransformsUsed.clear();
    lastCapturedPiece.reset();
    
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteKingsideRookMoved = false;
    whiteQueensideRookMoved = false;
    blackKingsideRookMoved = false;
    blackQueensideRookMoved = false;
    
    whiteQueenCanDoubleMove = false;
    blackQueenCanDoubleMove = false;
    
    currentSeason = Season::NONE;
    movesSinceSeasonChange = 0;
}
Piece* Board::getPieceAt(Position pos) const {
    if (!pos.isValid()) return nullptr;
    return grid[pos.row][pos.col].get();
}

Piece* Board::getPieceAt(int row, int col) const {
    return getPieceAt(Position(row, col));
}

void Board::setPieceAt(Position pos, std::unique_ptr<Piece> piece) {
    if (pos.isValid()) {
        grid[pos.row][pos.col] = std::move(piece);
        if (grid[pos.row][pos.col]) {
            grid[pos.row][pos.col]->setPosition(pos);
        }
    }
}

std::unique_ptr<Piece> Board::removePieceAt(Position pos) {
    if (!pos.isValid()) return nullptr;
    
    pieceStationaryCounter.erase(pos);
    armoredQueenHits.erase(pos);
    jokerTransformsUsed.erase(pos);
    
    return std::move(grid[pos.row][pos.col]);
}
