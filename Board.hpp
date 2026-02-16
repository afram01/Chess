#ifndef BOARD_HPP
#define BOARD_HPP

#include "piece.h"
#include "Position.hpp"
#include "Move.hpp"
#include "King.h"
#include"Types.hpp"


#include <vector>
#include <memory>
#include <string>
#include <map>

class Board {
private:
    std::vector<std::vector<std::unique_ptr<Piece>>> grid;
    
    std::vector<Move> moveHistory;
    
    Position enPassantSquare;
    bool enPassantAvailable;
    
    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteKingsideRookMoved;
    bool whiteQueensideRookMoved;
    bool blackKingsideRookMoved;
    bool blackQueensideRookMoved;
    
    std::unique_ptr<Piece> lastCapturedPiece;
    Position lastCapturedPosition;
    
    std::map<Position, int> pieceStationaryCounter;
    int bombThreshold;
    
    bool whiteQueenCanDoubleMove;
    bool blackQueenCanDoubleMove;
    
    Season currentSeason;
    int movesSinceSeasonChange;
    int movesPerSeasonChange;
    std::map<Position, int> lastMoveTurn;
    int currentTurnNumber;
    
    
    std::map<Position, int> jokerTransformsRemaining;
    std::map<Position, PieceType> jokerCurrentMimic;
    
    std::map<Position, int> spyMoveCounter;
    int spyRevealThreshold;
    
public:
    Board();
    
    void setupBoard();
    void setupBoardWithSpecialPieces();
    void setupCheckScenario();   
    void clear();
    
    Piece* getPiece(int row, int col) const;
    Piece* getPieceAt(Position pos) const;
    void setPiece(int row, int col, std::unique_ptr<Piece> piece);
    void setPieceAt(Position pos, std::unique_ptr<Piece> piece);
    std::unique_ptr<Piece> removePiece(int row, int col);
    std::unique_ptr<Piece> removePieceAt(Position pos);
    
    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);
    bool movePiece(const Move& move);
    
    bool isInCheck(Color player) const;
    bool isCheckmate(Color player) const;
    bool isStalemate(Color player) const;
    bool isSquareAttacked(Position pos, Color byColor) const;
    Position findKing(Color color) const;
    
    std::vector<Move> getAllLegalMoves(Color color) const;
    std::vector<Move> getLegalMovesForPiece(Position pos) const;
    bool isMoveLegal(const Move& move, Color color) const;
    
    bool canCastleKingside(Color color) const;
    bool canCastleQueenside(Color color) const;
    void performCastling(const Move& move);
    
    void promotePawn(Position pos, PieceType newType);
    bool canPromote(Position pos) const;
    
    void setEnPassantSquare(Position pos);
    Position getEnPassantSquare() const { return enPassantSquare; }
    bool isEnPassantAvailable() const { return enPassantAvailable; }
    void clearEnPassant();
    
    const std::vector<Move>& getMoveHistory() const { return moveHistory; }
    void undoLastMove();
    void undoMultipleMoves(int n);
    
    void setBombThreshold(int n) { bombThreshold = n; }
    int getBombThreshold() const { return bombThreshold; }
    
    void enableQueenDoubleMove(Color color);
    bool canQueenDoubleMove(Color color) const;
    void resetQueenDoubleMove(Color color);
    
    Season getCurrentSeason() const;
    void setCurrentSeason(Season s);
    bool canPieceMoveInSeason(Position pos) const;
    int getMaxMovementInSeason(PieceType type) const;
    void updateSeasonCycle();
    int getCurrentTurnNumber() const { return currentTurnNumber; }
    void incrementTurnNumber() { currentTurnNumber++; }
    void decrementTurnNumber() { if (currentTurnNumber > 0) currentTurnNumber--; }

    void resetArmoredQueenAttackFlags(Color color);
    bool tryCaptureArmoredQueen(Position pos);
    void updateSpyCounters();
    
    bool transformJoker(Position pos, PieceType targetType);
    void resetJokerTransform(Position pos);
    int getJokerTransformsRemaining(Position pos) const;
    bool isJokerTransformed(Position pos) const;
    PieceType getJokerCurrentMimic(Position pos) const;
    
    void setSpyRevealThreshold(int n) { spyRevealThreshold = n; }
    int getSpyMovesUntilReveal(Position pos) const;
    
    void printBoard() const;
    
    std::string serialize() const;
    void deserialize(const std::string& data);
};

#endif