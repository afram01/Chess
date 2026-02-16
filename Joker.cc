#include "Joker.h"
#include "Board.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>

using namespace std;

bool Joker::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const Board& board) const {
    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
        toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) {
        return false;
    }
    
    if (fromRow == toRow && fromCol == toCol) {
        return false;
    }
    
    Piece* targetPiece = board.getPiece(toRow, toCol);
    if (targetPiece != nullptr && targetPiece->getColor() == color) {
        return false;
    }
    
    switch (currentMimic) {
        case PieceType::KING:
            return (abs(toRow - fromRow) <= 1 && abs(toCol - fromCol) <= 1);
            
        case PieceType::QUEEN:
        case PieceType::ARMORED_QUEEN:
            if (fromRow == toRow || fromCol == toCol || 
                abs(toRow - fromRow) == abs(toCol - fromCol)) {
                int rowStep = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
                int colStep = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;
                int currentRow = fromRow + rowStep;
                int currentCol = fromCol + colStep;
                while (currentRow != toRow || currentCol != toCol) {
                    if (board.getPiece(currentRow, currentCol) != nullptr) return false;
                    currentRow += rowStep;
                    currentCol += colStep;
                }
                return true;
            }
            return false;
            
        case PieceType::ROOK:
            if (fromRow == toRow || fromCol == toCol) {
                int rowStep = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
                int colStep = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;
                int currentRow = fromRow + rowStep;
                int currentCol = fromCol + colStep;
                while (currentRow != toRow || currentCol != toCol) {
                    if (board.getPiece(currentRow, currentCol) != nullptr) return false;
                    currentRow += rowStep;
                    currentCol += colStep;
                }
                return true;
            }
            return false;
            
        case PieceType::BISHOP:
            if (abs(toRow - fromRow) == abs(toCol - fromCol)) {
                int rowStep = (toRow > fromRow) ? 1 : -1;
                int colStep = (toCol > fromCol) ? 1 : -1;
                int currentRow = fromRow + rowStep;
                int currentCol = fromCol + colStep;
                while (currentRow != toRow && currentCol != toCol) {
                    if (board.getPiece(currentRow, currentCol) != nullptr) return false;
                    currentRow += rowStep;
                    currentCol += colStep;
                }
                return true;
            }
            return false;
            
        case PieceType::KNIGHT: {
            int rowDiff = abs(toRow - fromRow);
            int colDiff = abs(toCol - fromCol);
            return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
        }
            
        case PieceType::PAWN:
        case PieceType::SPECIAL_PAWN: {
            int direction = (color == Color::WHITE) ? -1 : 1;
            int startRow  = (color == Color::WHITE) ? 6 : 1;
            
            if (fromCol == toCol) {
                if (toRow == fromRow + direction && board.getPiece(toRow, toCol) == nullptr) {
                    return true;
                }
                if (!hasMoved() && fromRow == startRow && 
                    toRow == fromRow + 2 * direction && 
                    board.getPiece(toRow, toCol) == nullptr &&
                    board.getPiece(fromRow + direction, fromCol) == nullptr) {
                    return true;
                }
            }
            else if (abs(toCol - fromCol) == 1 && toRow == fromRow + direction) {
                if (targetPiece != nullptr && targetPiece->getColor() != color) {
                    return true;
                }
                if (board.isEnPassantAvailable()) {
                    Position enPassantSquare = board.getEnPassantSquare();
                    if (toRow == enPassantSquare.row && toCol == enPassantSquare.col) {
                        int enemyPawnRow = fromRow;
                        int enemyPawnCol = toCol;
                        Piece* enemyPawn = board.getPiece(enemyPawnRow, enemyPawnCol);
                        if (enemyPawn != nullptr && 
                            enemyPawn->getColor() != color && 
                            (enemyPawn->getType() == PieceType::PAWN || 
                             enemyPawn->getType() == PieceType::SPECIAL_PAWN)) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
            
        case PieceType::SPY:
            if (fromCol == toCol) {
                if (abs(toRow - fromRow) == 1 && board.getPiece(toRow, toCol) == nullptr) {
                    return true;
                }
                if (!hasMoved() && abs(toRow - fromRow) == 2 && 
                    board.getPiece(toRow, toCol) == nullptr) {
                    int middleRow = (fromRow + toRow) / 2;
                    if (board.getPiece(middleRow, fromCol) == nullptr) {
                        return true;
                    }
                }
            }
            else if (abs(toCol - fromCol) == 1 && abs(toRow - fromRow) == 1) {
                if (targetPiece != nullptr && targetPiece->getColor() != color) {
                    return true;
                }
            }
            return false;
            
        default:
            return false;
    }
}

bool Joker::transform(PieceType targetType) {
    if (!canTransform() || targetType == PieceType::JOKER) {
        return false;
    }
    
    currentMimic = targetType;
    transformsRemaining--;
    isTransformedThisTurn = true;
    return true;
}

void Joker::resetToPawn() {
    currentMimic = PieceType::PAWN;
    isTransformedThisTurn = false;
}

void Joker::endTurnReset() {
    if (isTransformedThisTurn) {
        currentMimic = PieceType::PAWN;
        isTransformedThisTurn = false;
    }
}

void Joker::useSpecialAbility(int, int, Board&) {
}

string Joker::getSymbol() const {
    string baseSymbol;
    
    if (color == Color::WHITE) {
        switch (currentMimic) {
            case PieceType::KING:         baseSymbol = "♔"; break;
            case PieceType::QUEEN:        baseSymbol = "♕"; break;
            case PieceType::ROOK:         baseSymbol = "♖"; break;
            case PieceType::BISHOP:       baseSymbol = "♗"; break;
            case PieceType::KNIGHT:       baseSymbol = "♘"; break;
            case PieceType::PAWN:         baseSymbol = "♙"; break;
            case PieceType::SPECIAL_PAWN: baseSymbol = "♙"; break;
            case PieceType::ARMORED_QUEEN:baseSymbol = "♕"; break;
            case PieceType::SPY:          baseSymbol = "S"; break;
            case PieceType::JOKER:        baseSymbol = "J"; break;
            default:                      baseSymbol = "J";
        }
    } else {
        switch (currentMimic) {
            case PieceType::KING:         baseSymbol = "♚"; break;
            case PieceType::QUEEN:        baseSymbol = "♛"; break;
            case PieceType::ROOK:         baseSymbol = "♜"; break;
            case PieceType::BISHOP:       baseSymbol = "♝"; break;
            case PieceType::KNIGHT:       baseSymbol = "♞"; break;
            case PieceType::PAWN:         baseSymbol = "♟"; break;
            case PieceType::SPECIAL_PAWN: baseSymbol = "♟"; break;
            case PieceType::ARMORED_QUEEN:baseSymbol = "♛"; break;
            case PieceType::SPY:          baseSymbol = "s"; break;
            case PieceType::JOKER:        baseSymbol = "j"; break;
            default:                      baseSymbol = "j";
        }
    }
    
    if (currentMimic != PieceType::PAWN && currentMimic != PieceType::JOKER) {
        return baseSymbol + "*";
    }
    
    return baseSymbol + " ";
}

vector<Position> Joker::getPossibleMoves(int row, int col, const Board& board) const {
    vector<Position> moves;
    
    switch (currentMimic) {
        case PieceType::KING:
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    if (dr == 0 && dc == 0) continue;
                    int newRow = row + dr;
                    int newCol = col + dc;
                    if (isValidMove(row, col, newRow, newCol, board)) {
                        moves.push_back(Position(newRow, newCol));
                    }
                }
            }
            break;
            
        case PieceType::KNIGHT: {
            int knightMoves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                                     {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
            for (int i = 0; i < 8; i++) {
                int newRow = row + knightMoves[i][0];
                int newCol = col + knightMoves[i][1];
                if (isValidMove(row, col, newRow, newCol, board)) {
                    moves.push_back(Position(newRow, newCol));
                }
            }
            break;
        }
            
        case PieceType::PAWN:
        case PieceType::SPECIAL_PAWN:
        case PieceType::SPY: {
            int direction = (color == Color::WHITE) ? -1 : 1;
            int startRow  = (color == Color::WHITE) ? 6 : 1;
            
            if (isValidMove(row, col, row + direction, col, board)) {
                moves.push_back(Position(row + direction, col));
            }
            if (!hasMoved() && row == startRow && 
                isValidMove(row, col, row + 2 * direction, col, board)) {
                moves.push_back(Position(row + 2 * direction, col));
            }
            if (isValidMove(row, col, row + direction, col - 1, board)) {
                moves.push_back(Position(row + direction, col - 1));
            }
            if (isValidMove(row, col, row + direction, col + 1, board)) {
                moves.push_back(Position(row + direction, col + 1));
            }
            
            if (currentMimic == PieceType::SPY) {
                if (isValidMove(row, col, row - direction, col, board)) {
                    moves.push_back(Position(row - direction, col));
                }
                if (!hasMoved() && isValidMove(row, col, row - 2 * direction, col, board)) {
                    moves.push_back(Position(row - 2 * direction, col));
                }
                if (isValidMove(row, col, row - direction, col - 1, board)) {
                    moves.push_back(Position(row - direction, col - 1));
                }
                if (isValidMove(row, col, row - direction, col + 1, board)) {
                    moves.push_back(Position(row - direction, col + 1));
                }
            }
            break;
        }
            
        default: {
            int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1},
                                    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
            int startDir = 0;
            int endDir = 8;
            
            if (currentMimic == PieceType::ROOK) {
                endDir = 4;
            } else if (currentMimic == PieceType::BISHOP) {
                startDir = 4;
            }
            
            for (int dir = startDir; dir < endDir; dir++) {
                for (int step = 1; step < 8; step++) {
                    int newRow = row + directions[dir][0] * step;
                    int newCol = col + directions[dir][1] * step;
                    
                    if (isValidMove(row, col, newRow, newCol, board)) {
                        moves.push_back(Position(newRow, newCol));
                        Piece* target = board.getPiece(newRow, newCol);
                        if (target != nullptr && target->getColor() != color) {
                            break;
                        }
                    } else {
                        break;
                    }
                }
            }
            break;
        }
    }
    
    return moves;
}

unique_ptr<Piece> Joker::clone() const {
    return make_unique<Joker>(*this);
}

string Joker::serialize() const {
    string result = Piece::serialize();
    result += " " + to_string(transformsRemaining) + " " +
              to_string(static_cast<int>(currentMimic)) + " " +
              to_string(isTransformedThisTurn ? 1 : 0);
    return result;
}
