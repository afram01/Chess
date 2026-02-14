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

