#ifndef BOARD_HPP
#define BOARD_HPP

#include "piece.h"
#include "Position.hpp"
#include "Move.hpp"
#include <vector>
#include <memory>
#include <array>



class Board {
private:
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> grid;
    
    std::vector<Move> moveHistory;
    
public:
    Board();
    Board(const Board& other);  
    Board& operator=(const Board& other); 

    void setupStandardBoard();
    

    void setupSpecialBoard();
    

    void clear();
    

    Piece* getPiece(int row, int col) const;
    Piece* getPieceAt(Position pos) const;

    void setPiece(int row, int col, std::unique_ptr<Piece> piece);
    void setPieceAt(Position pos, std::unique_ptr<Piece> piece);

    std::unique_ptr<Piece> removePiece(int row, int col);
    std::unique_ptr<Piece> removePieceAt(Position pos);
    

    bool isSquareEmpty(Position pos) const;
    bool isSquareEmpty(int row, int col) const;
    

    void executeMove(const Move& move);
    void executeMove(Position from, Position to);

    void undoLastMove();
    

    const std::vector<Move>& getMoveHistory() const { return moveHistory; }
    
    void addMoveToHistory(const Move& move);

    void clearHistory();

    Position findKing(Color color) const;

    std::vector<Piece*> getAllPieces(Color color) const;

    int countPieces(Color color) const;

    int getMaterialValue(Color color) const;
    

    Board createCopy() const;
};

#endif 
