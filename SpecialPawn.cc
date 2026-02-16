#include "SpecialPawn.h"
#include "Board.hpp"
#include <cmath>

bool SpecialPawn::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const {
    int direction = (color == Color::WHITE) ? -1 : 1;
    int startRow  = (color == Color::WHITE) ? 6 : 1;

    if (fromCol == toCol && toRow == fromRow + direction) {
        if (board.getPiece(toRow, toCol) == nullptr) {
            return true;
        }
        else if (board.getPiece(toRow, toCol) && board.getPiece(toRow, toCol)->getColor() != color) {
            return true;
        }
    }

    if (fromCol == toCol && toRow == fromRow + 2 * direction && fromRow == startRow) {
        int midRow = fromRow + direction;
        return board.getPiece(midRow, toCol) == nullptr &&
               board.getPiece(toRow, toCol) == nullptr;
    }

    if (std::abs(toCol - fromCol) == 1 && toRow == fromRow + direction) {
        Piece* target = board.getPiece(toRow, toCol);
        if (target && target->getColor() != color) {
            return true; 
        }
        
        if (board.isEnPassantAvailable()) {
            Position ep = board.getEnPassantSquare();
            if (ep.row == toRow && ep.col == toCol) {
                return true;
            }
        }
    }

    return false;
}

std::string SpecialPawn::getSymbol() const {
    return (color == Color::WHITE) ? "♙+" : "♟+";
}

std::vector<Position> SpecialPawn::getPossibleMoves(int row, int col, const Board& board) const {
    std::vector<Position> moves;
    int direction = (color == Color::WHITE) ? -1 : 1;
    int startRow  = (color == Color::WHITE) ? 6 : 1;

    int newRow = row + direction;
    if (newRow >= 0 && newRow < 8) {
        Position forwardPos(newRow, col);
        Piece* forwardPiece = board.getPieceAt(forwardPos);
        
        if (!forwardPiece) {
            moves.push_back(forwardPos);
        }
        else if (forwardPiece && forwardPiece->getColor() != color) {
            moves.push_back(forwardPos);
        }

        if (col > 0) {
            Position leftDiagPos(newRow, col - 1);
            Piece* leftDiag = board.getPieceAt(leftDiagPos);
            if (leftDiag && leftDiag->getColor() != color) {
                moves.push_back(leftDiagPos);
            }
            if (board.isEnPassantAvailable()) {
                Position ep = board.getEnPassantSquare();
                if (ep.row == newRow && ep.col == col - 1) {
                    moves.push_back(ep);
                }
            }
        }

        if (col < 7) {
            Position rightDiagPos(newRow, col + 1);
            Piece* rightDiag = board.getPieceAt(rightDiagPos);
            if (rightDiag && rightDiag->getColor() != color) {
                moves.push_back(rightDiagPos);
            }
            if (board.isEnPassantAvailable()) {
                Position ep = board.getEnPassantSquare();
                if (ep.row == newRow && ep.col == col + 1) {
                    moves.push_back(ep);
                }
            }
        }
    }

    if (row == startRow) {
        int twoRow = row + 2 * direction;
        if (twoRow >= 0 && twoRow < 8) {
            int midRow = row + direction;
            if (!board.getPiece(midRow, col) && !board.getPiece(twoRow, col)) {
                moves.push_back(Position(twoRow, col));
            }
        }
    }

    return moves;
}

std::unique_ptr<Piece> SpecialPawn::clone() const {
    return std::make_unique<SpecialPawn>(*this);
}