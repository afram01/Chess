#ifndef MOVE_VALIDATOR_HPP
#define MOVE_VALIDATOR_HPP

#include "Board.hpp"
#include "Move.hpp"
#include "Position.hpp"
#include "piece.h"
#include <vector>


class MoveValidator {
private:

    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteKingsideRookMoved;
    bool whiteQueensideRookMoved;
    bool blackKingsideRookMoved;
    bool blackQueensideRookMoved;

    
    Position enPassantSquare;
    bool enPassantAvailable;
    
public:
    MoveValidator();

    bool isMoveLegal(const Move& move, const Board& board, Color playerColor) const;

    std::vector<Move> getAllLegalMoves(const Board& board, Color color) const;

    std::vector<Move> getLegalMovesForPiece(Position pos, const Board& board) const;

    bool isInCheck(const Board& board, Color playerColor) const;

    bool isCheckmate(const Board& board, Color playerColor) const;

    bool isStalemate(const Board& board, Color playerColor) const;
    

    bool isSquareAttacked(Position pos, Color byColor, const Board& board) const;
    

    bool canCastleKingside(Color color, const Board& board) const;
    bool canCastleQueenside(Color color, const Board& board) const;

    void updateCastlingRights(const Move& move, const Board& board);
    
 
    void setCastlingRight(Color color, bool kingside, bool value);
    

    
    void setEnPassantSquare(Position pos);
    Position getEnPassantSquare() const { return enPassantSquare; }
    bool isEnPassantAvailable() const { return enPassantAvailable; }
    void clearEnPassant();
    

    bool isEnPassantMove(const Move& move, const Board& board) const;
    

    bool canPromote(Position pos, const Board& board) const;

private:

    bool wouldBeInCheckAfterMove(const Move& move, const Board& board, Color playerColor) const;
};

#endif 
