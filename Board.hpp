#ifndef BOARD_H
#define BOARD_H

#include "Common.h"
#include "Piece.h"
#include <array>
#include <map>

enum class Season { 
    NONE,
    SPRING,  
    SUMMER, 
    AUTUMN,  
    WINTER  
};

class Board {
private:
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> grid;
    
    std::vector<Move> moveHistory;
    
    Position enPassantSquare;
    bool enPassantAvailable;
    
    std::unique_ptr<Piece> lastCapturedPiece;
    Position lastCapturedPosition;
    
    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteKingsideRookMoved;
    bool whiteQueensideRookMoved;
    bool blackKingsideRookMoved;
    bool blackQueensideRookMoved;
    
    std::map<Position, int> pieceStationaryCounter;
    int bombThreshold; 
    
    bool whiteQueenCanDoubleMove;
    bool blackQueenCanDoubleMove;
    
    Season currentSeason;
    int movesSinceSeasonChange;
    int movesPerSeasonChange;
    
    std::map<Position, int> armoredQueenHits; 
    

    std::map<Position, int> jokerTransformsUsed; 

};

#endif
