#ifndef TYPES_HPP
#define TYPES_HPP

#include "Position.hpp"
#include "piece.h"

enum Season
{
    SPRING,
    SUMMER,
    AUTUMN,
    WINTER
};

enum GameStatus
{
    PLAYING,
    CHECK,
    CHECKMATE,
    STALEMATE,
    DRAW,
    MISSION_WIN,
    MISSION_FAIL
};

enum MissionType
{
    CAPTURE_QUEEN,
    REACH_SQUARE,
    ESCAPE_CHECK
};

enum GameModeType
{
    STANDARD,
    ENERGY,
    MISSION
};

struct MoveRecord
{
    Move move;
    PieceType movedPiece;
    PieceType capturedPiece;
    Color playerColor;
    std::string notation;
    bool wasCastling;
    bool wasEnPassant;
    bool wasPromotion;

    int prevTurnCount;
    int prevWhiteEnergy;
    int prevBlackEnergy;
    int prevMovesLeft;
    Season prevSeason;
    bool prevCanMoveAgain;
    GameStatus prevStatus;
    Color prevCurrentTurn;

    MoveRecord()
        : movedPiece(PieceType::PAWN),
          capturedPiece(PieceType::PAWN),
          playerColor(Color::WHITE),
          wasCastling(false),
          wasEnPassant(false),
          wasPromotion(false),
          prevTurnCount(0),
          prevWhiteEnergy(100),
          prevBlackEnergy(100),
          prevMovesLeft(0),
          prevSeason(SPRING),
          prevCanMoveAgain(false),
          prevStatus(PLAYING),
          prevCurrentTurn(Color::WHITE)
    {
    }
};

#endif
