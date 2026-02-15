#ifndef TYPES_HPP
#define TYPES_HPP

#include "Position.hpp"
#include "Piece.h"

enum Season {
    SPRING,   
    SUMMER,   
    AUTUMN,   
    WINTER    
};

enum GameStatus {
    PLAYING,
    CHECK,
    CHECKMATE,
    STALEMATE,
    DRAW
};

enum GameModeType {
    STANDARD,
    ENERGY,
    MISSION
};

struct MoveRecord {
    Move move;
    PieceType movedPiece;
    PieceType capturedPiece;
    Color playerColor;
    std::string notation;
    bool wasCastling;
    bool wasEnPassant;
    bool wasPromotion;
    
    MoveRecord() 
        : movedPiece(PieceType::PAWN),
          capturedPiece(PieceType::PAWN),
          playerColor(Color::WHITE),
          wasCastling(false),
          wasEnPassant(false),
          wasPromotion(false) {}
};

#endif 