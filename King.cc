#include "King.h"
#include "Board.hpp"
#include <cmath>
#include <algorithm>

using namespace std;

King::King(Color color, Position pos) 
    : Piece(color, PieceType::KING, pos , true) {
}

bool King::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const {
    if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7 ||
        toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) {
        return false;
    }
    
    if (fromRow == toRow && fromCol == toCol) {
        return false;
    }
    
    if (isCastleMove(fromRow, fromCol, toRow, toCol)) {
        return canCastle(fromRow, fromCol, toRow, toCol, board);
    }
    
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);
    
    if (rowDiff > 1 || colDiff > 1) {
        return false;
    }
    
    Position targetPos(toRow, toCol);
    Piece* target = board.getPieceAt(targetPos);
    if (target && target->getColor() == color) {
        return false;
    }
    
    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    Position toPos(toRow, toCol);
    if (board.isSquareAttacked(toPos, opponent)) {
        return false;
    }
    
    return true;
}

bool King::isCastleMove(int fromRow, int fromCol, int toRow, int toCol) const {
    return (fromRow == toRow) && (abs(toCol - fromCol) == 2);
}

bool King::canCastle(int fromRow, int fromCol, int /*toRow*/, int toCol, const Board& board) const {
    if (moveCount > 0) {
        return false;
    }
    
    if (board.isInCheck(color)) {
        return false;
    }
    
    bool isKingside = (toCol > fromCol); 
    
    if (isKingside) {
        if (!board.canCastleKingside(color)) {
            return false;
        }
    } else {
        if (!board.canCastleQueenside(color)) {
            return false;
        }
    }
    
    int startCol = min(fromCol, toCol) + 1;
    int endCol = max(fromCol, toCol);
    
    for (int col = startCol; col < endCol; col++) {
        Position pathPos(fromRow, col);
        if (board.getPieceAt(pathPos) != nullptr) {
            return false;
        }
    }
    
    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    
    Position fromPos(fromRow, fromCol);
    if (board.isSquareAttacked(fromPos, opponent)) {
        return false;
    }
    
    for (int col = startCol; col < endCol; col++) {
        Position checkPos(fromRow, col);
        if (board.isSquareAttacked(checkPos, opponent)) {
            return false;
        }
    }
    
    return true;
}

vector<Position> King::getPossibleMoves(int row, int col, const Board& board) const {
    vector<Position> moves;
    
    if (row < 0 || row > 7 || col < 0 || col > 7) {
        return moves;
    }
    
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    
    for (const auto& dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];
        
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            if (isValidMove(row, col, newRow, newCol, board)) {
                moves.push_back(Position(newRow, newCol));
            }
        }
    }
    
    if (isValidMove(row, col, row, col + 2, board)) {
        moves.push_back(Position(row, col + 2));
    }
    
    if (isValidMove(row, col, row, col - 2, board)) {
        moves.push_back(Position(row, col - 2));
    }
    
    return moves;
}

void King::useSpecialAbility(int, int, Board&) {
    cout << "King's special ability: Castling available" << endl;
}

string King::getSymbol() const {
    return (color == Color::WHITE) ? "♔ " : "♚ "; 
}

unique_ptr<Piece> King::clone() const {
    return make_unique<King>(*this);
}

string King::serialize() const {
    return Piece::serialize();
}