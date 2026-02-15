#include "Board.hpp"
#include "piece.h"
#include "Types.hpp"
#include "SpecialPawn.h"
#include "ArmoredQueen.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Queen.h"
#include "Bishop.h"
#include "Joker.h"
#include "Spy.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

Board::Board() 
    : enPassantAvailable(false),
      whiteKingMoved(false), blackKingMoved(false),
      whiteKingsideRookMoved(false), whiteQueensideRookMoved(false),
      blackKingsideRookMoved(false), blackQueensideRookMoved(false),
      bombThreshold(5),
      whiteQueenCanDoubleMove(false), blackQueenCanDoubleMove(false),
      currentSeason(SPRING),
      movesSinceSeasonChange(0),
      movesPerSeasonChange(20),
      currentTurnNumber(0),
      spyRevealThreshold(3)
{
    grid.resize(8);
    for (int i = 0; i < 8; ++i) {
        grid[i].resize(8);
    }
}

void Board::setupBoard() {
    clear();
    
    for (int c = 0; c < 8; ++c) {
        grid[1][c] = std::make_unique<Pawn>(Color::BLACK, Position{1, c});
        grid[6][c] = std::make_unique<Pawn>(Color::WHITE, Position{6, c});
    }

    grid[0][0] = std::make_unique<Rook>(Color::BLACK, Position{0, 0});
    grid[0][7] = std::make_unique<Rook>(Color::BLACK, Position{0, 7});
    grid[7][0] = std::make_unique<Rook>(Color::WHITE, Position{7, 0});
    grid[7][7] = std::make_unique<Rook>(Color::WHITE, Position{7, 7});

    grid[0][1] = std::make_unique<Knight>(Color::BLACK, Position{0, 1});
    grid[0][6] = std::make_unique<Knight>(Color::BLACK, Position{0, 6});
    grid[7][1] = std::make_unique<Knight>(Color::WHITE, Position{7, 1});
    grid[7][6] = std::make_unique<Knight>(Color::WHITE, Position{7, 6});

    grid[0][2] = std::make_unique<Bishop>(Color::BLACK, Position{0, 2});
    grid[0][5] = std::make_unique<Bishop>(Color::BLACK, Position{0, 5});
    grid[7][2] = std::make_unique<Bishop>(Color::WHITE, Position{7, 2});
    grid[7][5] = std::make_unique<Bishop>(Color::WHITE, Position{7, 5});

    grid[0][3] = std::make_unique<Queen>(Color::BLACK, Position{0, 3});
    grid[0][4] = std::make_unique<King>(Color::BLACK, Position{0, 4});
    grid[7][3] = std::make_unique<Queen>(Color::WHITE, Position{7, 3});
    grid[7][4] = std::make_unique<King>(Color::WHITE, Position{7, 4});
}

void Board::setupBoardWithSpecialPieces() {
    clear();
    
    grid[6][0] = std::make_unique<SpecialPawn>(Color::WHITE, Position{6, 0});  
    grid[6][1] = std::make_unique<Pawn>(Color::WHITE, Position{6, 1});
    grid[6][2] = std::make_unique<Pawn>(Color::WHITE, Position{6, 2});
    grid[6][3] = std::make_unique<Joker>(Color::WHITE, Position{6, 3});
    grid[6][4] = std::make_unique<Pawn>(Color::WHITE, Position{6, 4});
    grid[6][5] = std::make_unique<Pawn>(Color::WHITE, Position{6, 5});
    grid[6][6] = std::make_unique<Pawn>(Color::WHITE, Position{6, 6});
    grid[6][7] = std::make_unique<Pawn>(Color::WHITE, Position{6, 7});
    
    grid[1][0] = std::make_unique<Pawn>(Color::BLACK, Position{1, 0});
    grid[1][1] = std::make_unique<Pawn>(Color::BLACK, Position{1, 1});
    grid[1][2] = std::make_unique<Pawn>(Color::BLACK, Position{1, 2});
    grid[1][3] = std::make_unique<Joker>(Color::BLACK, Position{1, 3});
    grid[1][4] = std::make_unique<SpecialPawn>(Color::BLACK, Position{1, 4});  
    grid[1][5] = std::make_unique<Pawn>(Color::BLACK, Position{1, 5});
    grid[1][6] = std::make_unique<Pawn>(Color::BLACK, Position{1, 6});
    grid[1][7] = std::make_unique<Pawn>(Color::BLACK, Position{1, 7});
    
    grid[0][0] = std::make_unique<Rook>(Color::BLACK, Position{0, 0});
    grid[0][7] = std::make_unique<Rook>(Color::BLACK, Position{0, 7});
    grid[7][0] = std::make_unique<Rook>(Color::WHITE, Position{7, 0});
    grid[7][7] = std::make_unique<Rook>(Color::WHITE, Position{7, 7});

    grid[0][1] = std::make_unique<Knight>(Color::BLACK, Position{0, 1});
    grid[0][6] = std::make_unique<Knight>(Color::BLACK, Position{0, 6});
    grid[7][1] = std::make_unique<Knight>(Color::WHITE, Position{7, 1});
    grid[7][6] = std::make_unique<Knight>(Color::WHITE, Position{7, 6});

    grid[0][2] = std::make_unique<Bishop>(Color::BLACK, Position{0, 2});
    grid[0][5] = std::make_unique<Bishop>(Color::BLACK, Position{0, 5});
    grid[7][2] = std::make_unique<Bishop>(Color::WHITE, Position{7, 2});
    grid[7][5] = std::make_unique<Bishop>(Color::WHITE, Position{7, 5});

    grid[0][3] = std::make_unique<ArmoredQueen>(Color::BLACK, Position{0, 3});
    grid[7][3] = std::make_unique<ArmoredQueen>(Color::WHITE, Position{7, 3});
    
    grid[0][4] = std::make_unique<King>(Color::BLACK, Position{0, 4});
    grid[7][4] = std::make_unique<King>(Color::WHITE, Position{7, 4});
    
    grid[2][4] = std::make_unique<Spy>(Color::WHITE, std::make_unique<Pawn>(Color::BLACK, Position{2, 4})); 
    grid[5][3] = std::make_unique<Spy>(Color::BLACK, std::make_unique<Pawn>(Color::WHITE, Position{5, 3}));  

    jokerTransformsRemaining[Position(1, 3)] = 2;
    jokerTransformsRemaining[Position(6, 3)] = 2;
    spyMoveCounter[Position(2, 4)] = 0;
    spyMoveCounter[Position(5, 3)] = 0;
}

void Board::clear() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            grid[r][c].reset();
        }
    }
    moveHistory.clear();
    pieceStationaryCounter.clear();
    lastMoveTurn.clear();
    jokerTransformsRemaining.clear();
    jokerCurrentMimic.clear();
    spyMoveCounter.clear();
    
    enPassantAvailable = false;
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteKingsideRookMoved = false;
    whiteQueensideRookMoved = false;
    blackKingsideRookMoved = false;
    blackQueensideRookMoved = false;
    whiteQueenCanDoubleMove = false;
    blackQueenCanDoubleMove = false;
    currentTurnNumber = 0;
    movesSinceSeasonChange = 0;
}

Piece* Board::getPiece(int row, int col) const {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return nullptr;
    return grid[row][col].get();
}

Piece* Board::getPieceAt(Position pos) const {
    return getPiece(pos.row, pos.col);
}

void Board::setPiece(int row, int col, std::unique_ptr<Piece> piece) {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        grid[row][col] = std::move(piece);
    }
}

void Board::setPieceAt(Position pos, std::unique_ptr<Piece> piece) {
    setPiece(pos.row, pos.col, std::move(piece));
}

std::unique_ptr<Piece> Board::removePiece(int row, int col) {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        Position pos(row, col);
        pieceStationaryCounter.erase(pos);
        lastMoveTurn.erase(pos);
        jokerTransformsRemaining.erase(pos);
        jokerCurrentMimic.erase(pos);
        spyMoveCounter.erase(pos);
        
        return std::move(grid[row][col]);
    }
    return nullptr;
}

std::unique_ptr<Piece> Board::removePieceAt(Position pos) {
    return removePiece(pos.row, pos.col);
}

bool Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    return movePiece(Move(Position(fromRow, fromCol), Position(toRow, toCol)));
}

bool Board::movePiece(const Move& move) {
    Piece* piece = getPieceAt(move.from);
    if (!piece) return false;

    if (!piece->isValidMove(move.from.row, move.from.col, move.to.row, move.to.col, *this)) {
        return false;
    }

    Piece* targetPiece = getPieceAt(move.to);
    if (targetPiece) {
        if (targetPiece->getType() == PieceType::ARMORED_QUEEN) {
            if (!tryCaptureArmoredQueen(move.to)) {
                std::cout << "Armored Queen survived the attack! " 
                          << std::endl;
                return false;  
            }
            lastCapturedPiece = removePieceAt(move.to);
            lastCapturedPosition = move.to;
            targetPiece = nullptr;
        }
        else {
            PieceType capturedType = targetPiece->getType();
            lastCapturedPiece = removePieceAt(move.to);
            lastCapturedPosition = move.to;
            
            if (capturedType == PieceType::QUEEN || 
                capturedType == PieceType::ARMORED_QUEEN) {
                enableQueenDoubleMove(piece->getColor());
            }
        }
    }

    auto movingPiece = removePieceAt(move.from);
    setPieceAt(move.to, std::move(movingPiece));
    piece = getPieceAt(move.to);  
    piece->incrementMoveCount();
    
    lastMoveTurn[move.to] = currentTurnNumber;

    if (piece->getType() == PieceType::KING) {
        if (piece->getColor() == Color::WHITE) whiteKingMoved = true;
        else blackKingMoved = true;
    }
    if (piece->getType() == PieceType::ROOK) {
        if (piece->getColor() == Color::WHITE) {
            if (move.from.row == 7 && move.from.col == 0) whiteQueensideRookMoved = true;
            if (move.from.row == 7 && move.from.col == 7) whiteKingsideRookMoved = true;
        } else {
            if (move.from.row == 0 && move.from.col == 0) blackQueensideRookMoved = true;
            if (move.from.row == 0 && move.from.col == 7) blackKingsideRookMoved = true;
        }
    }

    pieceStationaryCounter.erase(move.to);
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Position pos(r, c);
            if (getPieceAt(pos) && pos != move.to) {
                pieceStationaryCounter[pos]++;
            }
        }
    }

    std::vector<Position> bombPositions;
    for (const auto& entry : pieceStationaryCounter) {
        if (entry.second >= bombThreshold) {
            bombPositions.push_back(entry.first);
        }
    }
    for (const auto& bombPos : bombPositions) {
        std::cout << "Explosion in" << (char)('a' + bombPos.col) << (bombPos.row + 1) << "!" << std::endl;
        int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
        for (auto& dir : directions) {
            Position target(bombPos.row + dir[0], bombPos.col + dir[1]);
            if (target.isValid()) removePieceAt(target);
        }
        removePieceAt(bombPos);
    }

    updateSpyCounters();

    if (piece->getType() == PieceType::PAWN || piece->getType() == PieceType::SPECIAL_PAWN) {
        if ((piece->getColor() == Color::WHITE && move.to.row == 0) ||
            (piece->getColor() == Color::BLACK && move.to.row == 7)) {
            Color c = piece->getColor();
            removePieceAt(move.to);
            setPieceAt(move.to, std::make_unique<Queen>(c, move.to));
            std::cout << "Pawn promoted to Queen!" << std::endl;
        }
    }

    movesSinceSeasonChange++;
    if (movesSinceSeasonChange >= movesPerSeasonChange) {
        updateSeasonCycle();
    }

    moveHistory.push_back(move);
    return true;
}

bool Board::isSquareAttacked(Position pos, Color byColor) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = getPiece(r, c);
            if (piece && piece->getColor() == byColor) {
                if (piece->isValidMove(r, c, pos.row, pos.col, *this)) {
                    return true;
                }
            }
        }
    }
    return false;
}

Position Board::findKing(Color color) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = getPiece(r, c);
            if (piece && piece->getType() == PieceType::KING && piece->getColor() == color) {
                return Position(r, c);
            }
        }
    }
    return Position(-1, -1);
}

bool Board::isInCheck(Color player) const {
    Position kingPos = findKing(player);
    if (!kingPos.isValid()) return false;
    
    Color opponent = (player == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return isSquareAttacked(kingPos, opponent);
}

bool Board::isCheckmate(Color player) const {
    if (!isInCheck(player)) return false;
    return getAllLegalMoves(player).empty();
}

bool Board::isStalemate(Color player) const {
    if (isInCheck(player)) return false;
    return getAllLegalMoves(player).empty();
}

std::vector<Move> Board::getAllLegalMoves(Color color) const {
    std::vector<Move> legalMoves;
    
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = getPiece(r, c);
            if (piece && piece->getColor() == color) {
                auto pieceMoves = getLegalMovesForPiece(Position(r, c));
                legalMoves.insert(legalMoves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }
    
    return legalMoves;
}

std::vector<Move> Board::getLegalMovesForPiece(Position pos) const {
    std::vector<Move> legalMoves;
    
    Piece* piece = getPieceAt(pos);
    if (!piece) return legalMoves;
    
    if (!canPieceMoveInSeason(pos)) {
        return legalMoves;
    }
    
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Move move(pos, Position(r, c));
            if (piece->isValidMove(pos.row, pos.col, r, c, *this)) {
                if (isMoveLegal(move, piece->getColor())) {
                    legalMoves.push_back(move);
                }
            }
        }
    }
    
    return legalMoves;
}

bool Board::isMoveLegal(const Move& move, Color color) const {
    Board tempBoard;
    tempBoard.grid.resize(8);
    for (int i = 0; i < 8; ++i) {
        tempBoard.grid[i].resize(8);
    }
    
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (grid[r][c]) {
                tempBoard.grid[r][c] = grid[r][c]->clone();
            }
        }
    }
    
    auto movingPiece = tempBoard.removePieceAt(move.from);
    tempBoard.setPieceAt(move.to, std::move(movingPiece));
    
    return !tempBoard.isInCheck(color);
}

bool Board::canCastleKingside(Color color) const {
    int row = (color == Color::WHITE) ? 7 : 0;
    
    if (color == Color::WHITE) {
        if (whiteKingMoved || whiteKingsideRookMoved) return false;
    } else {
        if (blackKingMoved || blackKingsideRookMoved) return false;
    }
    
    Piece* king = getPiece(row, 4);
    Piece* rook = getPiece(row, 7);
    
    if (!king || king->getType() != PieceType::KING) return false;
    if (!rook || rook->getType() != PieceType::ROOK) return false;
    
    if (getPiece(row, 5) || getPiece(row, 6)) return false;
    
    if (isInCheck(color)) return false;
    
    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (isSquareAttacked(Position(row, 5), opponent)) return false;
    if (isSquareAttacked(Position(row, 6), opponent)) return false;
    
    return true;
}

bool Board::canCastleQueenside(Color color) const {
    int row = (color == Color::WHITE) ? 7 : 0;
    
    if (color == Color::WHITE) {
        if (whiteKingMoved || whiteQueensideRookMoved) return false;
    } else {
        if (blackKingMoved || blackQueensideRookMoved) return false;
    }
    
    Piece* king = getPiece(row, 4);
    Piece* rook = getPiece(row, 0);
    
    if (!king || king->getType() != PieceType::KING) return false;
    if (!rook || rook->getType() != PieceType::ROOK) return false;
    
    if (getPiece(row, 1) || getPiece(row, 2) || getPiece(row, 3)) return false;
    
    if (isInCheck(color)) return false;
    
    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (isSquareAttacked(Position(row, 2), opponent)) return false;
    if (isSquareAttacked(Position(row, 3), opponent)) return false;
    
    return true;
}

void Board::performCastling(const Move& move) {
    int row = move.from.row;
    
    auto king = removePieceAt(move.from);
    setPieceAt(move.to, std::move(king));
    
    if (move.to.col == 6) {  
        auto rook = removePiece(row, 7);
        setPiece(row, 5, std::move(rook));
    } else { 
        auto rook = removePiece(row, 0);
        setPiece(row, 3, std::move(rook));
    }
    
    moveHistory.push_back(move);
}

void Board::promotePawn(Position pos, PieceType newType) {
    Piece* pawn = getPieceAt(pos);
    if (!pawn) return;
    if (pawn->getType() != PieceType::PAWN && pawn->getType() != PieceType::SPECIAL_PAWN) return;
    
    Color color = pawn->getColor();
    removePieceAt(pos);
    
    switch (newType) {
        case PieceType::QUEEN:
            setPieceAt(pos, std::make_unique<Queen>(color, pos));
            break;
        case PieceType::ROOK:
            setPieceAt(pos, std::make_unique<Rook>(color, pos));
            break;
        case PieceType::BISHOP:
            setPieceAt(pos, std::make_unique<Bishop>(color, pos));
            break;
        case PieceType::KNIGHT:
            setPieceAt(pos, std::make_unique<Knight>(color, pos));
            break;
        default:
            setPieceAt(pos, std::make_unique<Queen>(color, pos));
    }
}

bool Board::canPromote(Position pos) const {
    Piece* piece = getPieceAt(pos);
    if (!piece) return false;
    if (piece->getType() != PieceType::PAWN && piece->getType() != PieceType::SPECIAL_PAWN) return false;
    
    if (piece->getColor() == Color::WHITE && pos.row == 0) return true;
    if (piece->getColor() == Color::BLACK && pos.row == 7) return true;
    
    return false;
}

void Board::setEnPassantSquare(Position pos) {
    enPassantSquare = pos;
    enPassantAvailable = true;
}

void Board::clearEnPassant() {
    enPassantAvailable = false;
    enPassantSquare = Position();
}

void Board::undoLastMove() {
    if (moveHistory.empty()) return;
    
    Move lastMove = moveHistory.back();
    moveHistory.pop_back();
    
    auto piece = removePieceAt(lastMove.to);
    if (piece) {
        setPieceAt(lastMove.from, std::move(piece));
    }
    
    if (lastCapturedPiece && lastCapturedPosition.isValid()) {
        setPieceAt(lastCapturedPosition, std::move(lastCapturedPiece));
        lastCapturedPosition = Position();
    }
    
    if (movesSinceSeasonChange > 0) {
        movesSinceSeasonChange--;
    }
}

void Board::undoMultipleMoves(int n) {
    for (int i = 0; i < n && !moveHistory.empty(); ++i) {
        undoLastMove();
    }
}


void Board::enableQueenDoubleMove(Color color) {
    if (color == Color::WHITE) {
        whiteQueenCanDoubleMove = true;
        std::cout << "Massacre! The white queen can move twice." << std::endl;
    } else {
        blackQueenCanDoubleMove = true;
        std::cout << "Massacre! The black queen can move twice." << std::endl;
    }
}

bool Board::canQueenDoubleMove(Color color) const {
    return (color == Color::WHITE) ? whiteQueenCanDoubleMove : blackQueenCanDoubleMove;
}

void Board::resetQueenDoubleMove(Color color) {
    if (color == Color::WHITE) whiteQueenCanDoubleMove = false;
    else blackQueenCanDoubleMove = false;
}


Season Board::getCurrentSeason() const {
    return currentSeason;
}

void Board::setCurrentSeason(Season s) {
    currentSeason = s;
}

void Board::updateSeasonCycle() {
    Season seasons[] = {SPRING, SUMMER, AUTUMN, WINTER};
    
    for (int i = 0; i < 4; ++i) {
        if (seasons[i] == currentSeason) {
            currentSeason = seasons[(i + 1) % 4];
            movesSinceSeasonChange = 0;
            
            const char* names[] = {"Spring", "Summer", "Autumn", "Winter"};
            std::cout << "Season changed to:" << names[(i + 1) % 4] << std::endl;
            break;
        }
    }
}

bool Board::canPieceMoveInSeason(Position pos) const {
    Piece* piece = getPieceAt(pos);
    if (!piece) return false;
    
    PieceType type = piece->getType();
    
    if (currentSeason == SPRING && (type == PieceType::PAWN || type == PieceType::SPECIAL_PAWN)) {
        auto it = lastMoveTurn.find(pos);
        if (it == lastMoveTurn.end()) return true;
        
        int turnsSinceLastMove = currentTurnNumber - it->second;
        return turnsSinceLastMove >= 2;
    }
    
    if (currentSeason == AUTUMN && type == PieceType::KNIGHT) {
        auto it = lastMoveTurn.find(pos);
        if (it == lastMoveTurn.end()) return true;
        
        int turnsSinceLastMove = currentTurnNumber - it->second;
        return turnsSinceLastMove >= 2;
    }
    
    return true;
}

int Board::getMaxMovementInSeason(PieceType type) const {
    if (currentSeason == SUMMER && type == PieceType::BISHOP) {
        return 3;
    }
    
    if (currentSeason == WINTER && type == PieceType::ROOK) {
        return 4;
    }
    
    return -1;  
}

    
bool Board::transformJoker(Position pos, PieceType targetType) {
    Piece* piece = getPieceAt(pos);
    if (!piece || piece->getType() != PieceType::JOKER) {
        return false;
    }
    
    auto it = jokerTransformsRemaining.find(pos);
    int remaining = (it != jokerTransformsRemaining.end()) ? it->second : 2;
    
    if (remaining <= 0) {
        return false;
    }
    
    jokerCurrentMimic[pos] = targetType;
    jokerTransformsRemaining[pos] = remaining - 1;
    
    std::cout << "Joker transformed! Remaining transformations: " << (remaining - 1) << std::endl;
    
    return true;
}

void Board::resetJokerTransform(Position pos) {
    jokerCurrentMimic.erase(pos);
}

int Board::getJokerTransformsRemaining(Position pos) const {
    auto it = jokerTransformsRemaining.find(pos);
    if (it == jokerTransformsRemaining.end()) return 2;
    return it->second;
}

bool Board::isJokerTransformed(Position pos) const {
    return jokerCurrentMimic.find(pos) != jokerCurrentMimic.end();
}

PieceType Board::getJokerCurrentMimic(Position pos) const {
    auto it = jokerCurrentMimic.find(pos);
    if (it == jokerCurrentMimic.end()) return PieceType::JOKER;
    return it->second;
}


void Board::updateSpyCounters() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Position pos(r, c);
            Piece* piece = getPieceAt(pos);
            
            if (piece && piece->getType() == PieceType::SPY) {
                spyMoveCounter[pos]++;
                
                if (spyMoveCounter[pos] >= spyRevealThreshold) {
                    std::cout << "Spy in" << (char)('a' + c) << (r + 1) 
                              << " Revealed!" << std::endl;
                }
            }
        }
    }
}

int Board::getSpyMovesUntilReveal(Position pos) const {
    auto it = spyMoveCounter.find(pos);
    int moves = (it != spyMoveCounter.end()) ? it->second : 0;
    return std::max(0, spyRevealThreshold - moves);
}


void Board::printBoard() const {
    std::cout << "\n   a  b  c  d  e  f  g  h\n";
    for (int r = 0; r < 8; ++r) {
        std::cout << (8 - r) << " ";
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p) {
                std::cout << std::setw(2) << p->getSymbol() << " ";
            } else {
                std::cout << " . ";
            }
        }
        std::cout << (8 - r) << "\n";
    }
    std::cout << "   a  b  c  d  e  f  g  h\n\n";
}


std::string Board::serialize() const {
    std::ostringstream ss;
    
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (grid[r][c]) {
                ss << static_cast<int>(grid[r][c]->getType()) << ","
                   << static_cast<int>(grid[r][c]->getColor()) << ";";
            } else {
                ss << "-;";
            }
        }
    }
    
    return ss.str();
}

void Board::deserialize(const std::string& data) {
    clear();
    std::istringstream ss(data);
}

bool Board::tryCaptureArmoredQueen(Position pos) {
    Piece* piece = getPieceAt(pos);
    if (!piece || piece->getType() != PieceType::ARMORED_QUEEN) {
        return true;
    }
    
    ArmoredQueen* armoredQueen = dynamic_cast<ArmoredQueen*>(piece);
    if (!armoredQueen) {
        return true;
    }
    
    return armoredQueen->attemptCapture();
}

void Board::resetArmoredQueenAttackFlags(Color color) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = getPiece(r, c);
            if (piece && piece->getType() == PieceType::ARMORED_QUEEN && 
                piece->getColor() == color) {
                ArmoredQueen* armoredQueen = dynamic_cast<ArmoredQueen*>(piece);
                if (armoredQueen) {
                    armoredQueen->resetAttackFlag();
                }
            }
        }
    }
}