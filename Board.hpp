#ifndef BOARD_H
#define BOARD_H

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
    
    std::map<Position, int> lastMoveTurn; 
    int currentTurnNumber; 
    
    std::map<Position, int> armoredQueenArmor; 
    
    std::map<Position, int> jokerTransformsRemaining; 
    std::map<Position, PieceType> jokerCurrentMimic; 
    
    std::map<Position, int> spyMoveCounter; 
    int spyRevealThreshold;  
    
public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& other);
    
 
    void initialize();
    void initializeWithSpecialPieces();
    void clear();

    Piece* getPieceAt(Position pos) const;
    Piece* getPieceAt(int row, int col) const;
    void setPieceAt(Position pos, std::unique_ptr<Piece> piece);
    std::unique_ptr<Piece> removePieceAt(Position pos);
    void movePiece(const Move& move);
    
    bool isSquareEmpty(Position pos) const;
    bool isSquareAttacked(Position pos, Color byColor) const;
    bool isInCheck(Color color) const;
    bool isCheckmate(Color color) const;
    bool isStalemate(Color color) const;
    
    Position findKing(Color color) const;
    std::vector<Move> getAllLegalMoves(Color color) const;
    std::vector<Move> getLegalMovesForPiece(Position pos) const;
    bool isMoveLegal(const Move& move, Color color) const;

    bool canCastleKingside(Color color) const;
    bool canCastleQueenside(Color color) const;
    void performCastling(const Move& move);
    void setCastlingRight(Color color, bool kingside, bool value);
    void updateCastlingRights(const Move& move);

    void setEnPassantSquare(Position pos);
    Position getEnPassantSquare() const { return enPassantSquare; }
    bool isEnPassantAvailable() const { return enPassantAvailable; }
 
    void addMoveToHistory(const Move& move);
    const std::vector<Move>& getMoveHistory() const { return moveHistory; }
    void undoLastMove();
    void undoMultipleMoves(int n);  
    

    std::vector<Piece*> getAllPieces(Color color) const;
    int countPieces(Color color) const;
    int getMaterialValue(Color color) const;
    std::map<PieceType, int> getPieceCounts(Color color) const;

    void promotePawn(Position pos, PieceType newType);
    bool canPromote(Position pos) const;
    
    std::vector<Position> getExplosionTargets(Position center) const;
    void explodePieces(const std::vector<Position>& positions);
    void updateStationaryCounters(const Move& move);
    void checkAndExplodeBombs();
    void setBombThreshold(int n) { bombThreshold = n; }
    int getBombThreshold() const { return bombThreshold; }
    
    void enableQueenDoubleMove(Color color);
    bool canQueenDoubleMove(Color color) const;
    void resetQueenDoubleMove(Color color);
    
    void advanceSeasonCycle();
    void applySeason(Season season);
    Season getCurrentSeason() const { return currentSeason; }
    bool canPieceMoveInSeason(Position pos) const; 
    int getMaxMovementInSeason(PieceType type) const;
    void setMovesPerSeasonChange(int n) { movesPerSeasonChange = n; }
    int getCurrentTurnNumber() const { return currentTurnNumber; }
    void incrementTurnNumber() { currentTurnNumber++; }

    bool tryAttackArmoredQueen(Position pos);
    int getArmoredQueenArmor(Position pos) const;
    void resetArmoredQueenArmor(Position pos, int armor = 2);

    bool transformJoker(Position pos, PieceType targetType); 
    void resetJokerTransform(Position pos); 
    int getJokerTransformsRemaining(Position pos) const;
    bool isJokerTransformed(Position pos) const;
    PieceType getJokerCurrentMimic(Position pos) const;
 
    void revealSpy(Position pos);
    void activateSpy(Position pos);  
    bool isSpyRevealed(Position pos) const;
    void updateSpyCounters(); 
    void setSpyRevealThreshold(int n) { spyRevealThreshold = n; }
    int getSpyMovesUntilReveal(Position pos) const;
    
    Piece* getLastCapturedPiece() const { return lastCapturedPiece.get(); }
    std::string serialize() const;
    void deserialize(const std::string& data);
};

#endif
