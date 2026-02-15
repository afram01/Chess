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
      bombThreshold(20),
      whiteQueenCanDoubleMove(false), blackQueenCanDoubleMove(false),
      currentSeason(SPRING),
      movesSinceSeasonChange(0),
      movesPerSeasonChange(20),
      currentTurnNumber(0),
      spyRevealThreshold(3)
{
    grid.resize(8);
    for (int i = 0; i < 8; ++i)
        grid[i].resize(8);
}


void Board::setupBoard() {
    clear();
    for (int c = 0; c < 8; ++c) {
        grid[1][c] = std::make_unique<Pawn>(Color::BLACK, Position{1, c});
        grid[6][c] = std::make_unique<Pawn>(Color::WHITE, Position{6, c});
    }
    grid[0][0] = std::make_unique<Rook>(Color::BLACK, Position{0,0});
    grid[0][7] = std::make_unique<Rook>(Color::BLACK, Position{0,7});
    grid[7][0] = std::make_unique<Rook>(Color::WHITE, Position{7,0});
    grid[7][7] = std::make_unique<Rook>(Color::WHITE, Position{7,7});

    grid[0][1] = std::make_unique<Knight>(Color::BLACK, Position{0,1});
    grid[0][6] = std::make_unique<Knight>(Color::BLACK, Position{0,6});
    grid[7][1] = std::make_unique<Knight>(Color::WHITE, Position{7,1});
    grid[7][6] = std::make_unique<Knight>(Color::WHITE, Position{7,6});

    grid[0][2] = std::make_unique<Bishop>(Color::BLACK, Position{0,2});
    grid[0][5] = std::make_unique<Bishop>(Color::BLACK, Position{0,5});
    grid[7][2] = std::make_unique<Bishop>(Color::WHITE, Position{7,2});
    grid[7][5] = std::make_unique<Bishop>(Color::WHITE, Position{7,5});

    grid[0][3] = std::make_unique<Queen>(Color::BLACK, Position{0,3});
    grid[0][4] = std::make_unique<King>(Color::BLACK, Position{0,4});
    grid[7][3] = std::make_unique<Queen>(Color::WHITE, Position{7,3});
    grid[7][4] = std::make_unique<King>(Color::WHITE, Position{7,4});
}

void Board::setupBoardWithSpecialPieces() {
    clear();
    grid[6][0] = std::make_unique<SpecialPawn>(Color::WHITE, Position{6,0});
    grid[6][1] = std::make_unique<Pawn>(Color::WHITE, Position{6,1});
    grid[6][2] = std::make_unique<Pawn>(Color::WHITE, Position{6,2});
    grid[6][3] = std::make_unique<Joker>(Color::WHITE, Position{6,3});
    grid[6][4] = std::make_unique<Pawn>(Color::WHITE, Position{6,4});
    grid[6][5] = std::make_unique<Pawn>(Color::WHITE, Position{6,5});
    grid[6][6] = std::make_unique<Pawn>(Color::WHITE, Position{6,6});
    grid[6][7] = std::make_unique<Pawn>(Color::WHITE, Position{6,7});

    grid[1][0] = std::make_unique<Pawn>(Color::BLACK, Position{1,0});
    grid[1][1] = std::make_unique<Pawn>(Color::BLACK, Position{1,1});
    grid[1][2] = std::make_unique<Pawn>(Color::BLACK, Position{1,2});
    grid[1][3] = std::make_unique<Joker>(Color::BLACK, Position{1,3});
    grid[1][4] = std::make_unique<SpecialPawn>(Color::BLACK, Position{1,4});
    grid[1][5] = std::make_unique<Pawn>(Color::BLACK, Position{1,5});
    grid[1][6] = std::make_unique<Pawn>(Color::BLACK, Position{1,6});
    grid[1][7] = std::make_unique<Pawn>(Color::BLACK, Position{1,7});

    grid[0][0] = std::make_unique<Rook>(Color::BLACK, Position{0,0});
    grid[0][7] = std::make_unique<Rook>(Color::BLACK, Position{0,7});
    grid[7][0] = std::make_unique<Rook>(Color::WHITE, Position{7,0});
    grid[7][7] = std::make_unique<Rook>(Color::WHITE, Position{7,7});

    grid[0][1] = std::make_unique<Knight>(Color::BLACK, Position{0,1});
    grid[0][6] = std::make_unique<Knight>(Color::BLACK, Position{0,6});
    grid[7][1] = std::make_unique<Knight>(Color::WHITE, Position{7,1});
    grid[7][6] = std::make_unique<Knight>(Color::WHITE, Position{7,6});

    grid[0][2] = std::make_unique<Bishop>(Color::BLACK, Position{0,2});
    grid[0][5] = std::make_unique<Bishop>(Color::BLACK, Position{0,5});
    grid[7][2] = std::make_unique<Bishop>(Color::WHITE, Position{7,2});
    grid[7][5] = std::make_unique<Bishop>(Color::WHITE, Position{7,5});

    grid[0][3] = std::make_unique<ArmoredQueen>(Color::BLACK, Position{0,3});
    grid[7][3] = std::make_unique<ArmoredQueen>(Color::WHITE, Position{7,3});

    grid[0][4] = std::make_unique<King>(Color::BLACK, Position{0,4});
    grid[7][4] = std::make_unique<King>(Color::WHITE, Position{7,4});

    grid[2][4] = std::make_unique<Spy>(Color::WHITE, std::make_unique<Pawn>(Color::BLACK, Position{2,4}));
    grid[5][3] = std::make_unique<Spy>(Color::BLACK, std::make_unique<Pawn>(Color::WHITE, Position{5,3}));

    jokerTransformsRemaining[Position(1,3)] = 2;
    jokerTransformsRemaining[Position(6,3)] = 2;
    spyMoveCounter[Position(2,4)] = 0;
    spyMoveCounter[Position(5,3)] = 0;
}

void Board::clear() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            grid[r][c].reset();

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
Piece* Board::getPieceAt(Position pos) const { return getPiece(pos.row, pos.col); }

void Board::setPiece(int row, int col, std::unique_ptr<Piece> piece) {
    if (row >= 0 && row < 8 && col >= 0 && col < 8)
        grid[row][col] = std::move(piece);
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

    if (!piece->isValidMove(move.from.row, move.from.col, move.to.row, move.to.col, *this))
        return false;

    Piece* targetPiece = getPieceAt(move.to);
    if (targetPiece) {
        if (targetPiece->getType() == PieceType::ARMORED_QUEEN) {
            if (!tryCaptureArmoredQueen(move.to)) {
                std::cout << "Armored Queen survived the attack!" << std::endl;
                return false;
            }
            lastCapturedPiece = removePieceAt(move.to);
            lastCapturedPosition = move.to;
            enableQueenDoubleMove(piece->getColor());
            targetPiece = nullptr;
        } else {
            PieceType capturedType = targetPiece->getType();
            lastCapturedPiece = removePieceAt(move.to);
            lastCapturedPosition = move.to;
            if (capturedType == PieceType::QUEEN || capturedType == PieceType::ARMORED_QUEEN)
                enableQueenDoubleMove(piece->getColor());
        }
    }

    auto movingPiece = removePieceAt(move.from);
    setPieceAt(move.to, std::move(movingPiece));
    piece = getPieceAt(move.to);
    piece->incrementMoveCount();

    lastMoveTurn[move.to] = currentTurnNumber;
    lastMoveTurn.erase(move.from);

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
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            Position pos(r, c);
            if (getPieceAt(pos) && pos != move.to)
                pieceStationaryCounter[pos]++;
        }

    std::vector<Position> bombPositions;
    for (const auto& entry : pieceStationaryCounter)
        if (entry.second >= bombThreshold)
            bombPositions.push_back(entry.first);
    for (const auto& bombPos : bombPositions) {
        std::cout << "Explosion at " << (char)('a'+bombPos.col) << (8-bombPos.row) << "!" << std::endl;
        int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
        for (auto& dir : directions) {
            Position target(bombPos.row+dir[0], bombPos.col+dir[1]);
            if (target.isValid()) removePieceAt(target);
        }
        removePieceAt(bombPos);
    }

    updateSpyCounters();

    if (piece->getType() == PieceType::PAWN || piece->getType() == PieceType::SPECIAL_PAWN) {
        if ((piece->getColor() == Color::WHITE && move.to.row == 0) ||
            (piece->getColor() == Color::BLACK && move.to.row == 7)) {
            Color c = piece->getColor();
            std::cout << "Pawn promoted! Choose piece:\n";
            std::cout << "1. Queen  2. Rook  3. Bishop  4. Knight\nEnter choice (1-4): ";
            int choice; std::cin >> choice; std::cin.ignore();
            PieceType newType;
            switch (choice) {
                case 2: newType = PieceType::ROOK;   break;
                case 3: newType = PieceType::BISHOP; break;
                case 4: newType = PieceType::KNIGHT; break;
                default: newType = PieceType::QUEEN;
            }
            removePieceAt(move.to);
            switch (newType) {
                case PieceType::ROOK:   setPieceAt(move.to, std::make_unique<Rook>(c, move.to));   break;
                case PieceType::BISHOP: setPieceAt(move.to, std::make_unique<Bishop>(c, move.to)); break;
                case PieceType::KNIGHT: setPieceAt(move.to, std::make_unique<Knight>(c, move.to)); break;
                default:                setPieceAt(move.to, std::make_unique<Queen>(c, move.to));  break;
            }
            std::cout << "Pawn promoted!" << std::endl;
        }
    }

    movesSinceSeasonChange++;
    if (movesSinceSeasonChange >= movesPerSeasonChange)
        updateSeasonCycle();

    moveHistory.push_back(move);
    return true;
}


bool Board::isSquareAttacked(Position pos, Color byColor) const {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->getColor() == byColor)
                if (p->isValidMove(r, c, pos.row, pos.col, *this))
                    return true;
        }
    return false;
}

Position Board::findKing(Color color) const {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->getType() == PieceType::KING && p->getColor() == color)
                return Position(r, c);
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
    std::vector<Move> legal;
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->getColor() == color) {
                auto pieceMoves = getLegalMovesForPiece(Position(r, c));
                legal.insert(legal.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    return legal;
}

std::vector<Move> Board::getLegalMovesForPiece(Position pos) const {
    std::vector<Move> legal;
    Piece* piece = getPieceAt(pos);
    if (!piece) return legal;
    if (!canPieceMoveInSeason(pos)) return legal;

    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            Move move(pos, Position(r, c));
            if (piece->isValidMove(pos.row, pos.col, r, c, *this))
                if (isMoveLegal(move, piece->getColor()))
                    legal.push_back(move);
        }
    return legal;
}


Board Board::makeTempCopy() const {
    Board tmp;

    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (grid[r][c])
                tmp.grid[r][c] = grid[r][c]->clone();


    tmp.enPassantAvailable        = enPassantAvailable;
    tmp.enPassantSquare           = enPassantSquare;
    tmp.whiteKingMoved            = whiteKingMoved;
    tmp.blackKingMoved            = blackKingMoved;
    tmp.whiteKingsideRookMoved    = whiteKingsideRookMoved;
    tmp.whiteQueensideRookMoved   = whiteQueensideRookMoved;
    tmp.blackKingsideRookMoved    = blackKingsideRookMoved;
    tmp.blackQueensideRookMoved   = blackQueensideRookMoved;
    tmp.currentSeason             = currentSeason;
    tmp.movesSinceSeasonChange    = movesSinceSeasonChange;
    tmp.movesPerSeasonChange      = movesPerSeasonChange;
    tmp.currentTurnNumber         = currentTurnNumber;
    tmp.bombThreshold             = bombThreshold;
    tmp.whiteQueenCanDoubleMove   = whiteQueenCanDoubleMove;
    tmp.blackQueenCanDoubleMove   = blackQueenCanDoubleMove;
    tmp.lastMoveTurn              = lastMoveTurn;
    tmp.pieceStationaryCounter    = pieceStationaryCounter;
    tmp.jokerTransformsRemaining  = jokerTransformsRemaining;
    tmp.jokerCurrentMimic         = jokerCurrentMimic;
    tmp.spyMoveCounter            = spyMoveCounter;
    tmp.spyRevealThreshold        = spyRevealThreshold;
    return tmp;
}

bool Board::isMoveLegal(const Move& move, Color color) const {
    Board tmp = makeTempCopy();
    Piece* target = tmp.getPieceAt(move.to);
    if (target) {

        tmp.removePieceAt(move.to);
    }
    auto movingPiece = tmp.removePieceAt(move.from);
    if (!movingPiece) return false;
    tmp.setPieceAt(move.to, std::move(movingPiece));
    Piece* kingMoved = tmp.getPieceAt(move.to);
    if (kingMoved && kingMoved->getType() == PieceType::KING &&
        std::abs(move.to.col - move.from.col) == 2) {
        int row = move.from.row;
        if (move.to.col == 6) { 
            auto rook = tmp.removePiece(row, 7);
            if (rook) tmp.setPiece(row, 5, std::move(rook));
        } else { 
            auto rook = tmp.removePiece(row, 0);
            if (rook) tmp.setPiece(row, 3, std::move(rook));
        }
    }

    return !tmp.isInCheck(color);
}


bool Board::canCastleKingside(Color color) const {
    int row = (color == Color::WHITE) ? 7 : 0;
    if (color == Color::WHITE) { if (whiteKingMoved || whiteKingsideRookMoved) return false; }
    else                       { if (blackKingMoved || blackKingsideRookMoved) return false; }

    Piece* king = getPiece(row, 4);
    Piece* rook = getPiece(row, 7);
    if (!king || king->getType() != PieceType::KING) return false;
    if (!rook || rook->getType() != PieceType::ROOK) return false;
    if (getPiece(row, 5) || getPiece(row, 6)) return false;
    if (isInCheck(color)) return false;

    Color opp = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (isSquareAttacked(Position(row,5), opp)) return false;
    if (isSquareAttacked(Position(row,6), opp)) return false;
    return true;
}

bool Board::canCastleQueenside(Color color) const {
    int row = (color == Color::WHITE) ? 7 : 0;
    if (color == Color::WHITE) { if (whiteKingMoved || whiteQueensideRookMoved) return false; }
    else                       { if (blackKingMoved || blackQueensideRookMoved) return false; }

    Piece* king = getPiece(row, 4);
    Piece* rook = getPiece(row, 0);
    if (!king || king->getType() != PieceType::KING) return false;
    if (!rook || rook->getType() != PieceType::ROOK) return false;
    if (getPiece(row,1) || getPiece(row,2) || getPiece(row,3)) return false;
    if (isInCheck(color)) return false;

    Color opp = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (isSquareAttacked(Position(row,2), opp)) return false;
    if (isSquareAttacked(Position(row,3), opp)) return false;
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
        case PieceType::QUEEN:  setPieceAt(pos, std::make_unique<Queen>(color, pos));  break;
        case PieceType::ROOK:   setPieceAt(pos, std::make_unique<Rook>(color, pos));   break;
        case PieceType::BISHOP: setPieceAt(pos, std::make_unique<Bishop>(color, pos)); break;
        case PieceType::KNIGHT: setPieceAt(pos, std::make_unique<Knight>(color, pos)); break;
        default:                setPieceAt(pos, std::make_unique<Queen>(color, pos));
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


void Board::setEnPassantSquare(Position pos) { enPassantSquare = pos; enPassantAvailable = true; }
void Board::clearEnPassant() { enPassantAvailable = false; enPassantSquare = Position(); }

void Board::undoLastMove() {
    if (moveHistory.empty()) return;
    Move lastMove = moveHistory.back();
    moveHistory.pop_back();
    auto piece = removePieceAt(lastMove.to);
    if (piece) setPieceAt(lastMove.from, std::move(piece));
    if (lastCapturedPiece && lastCapturedPosition.isValid()) {
        setPieceAt(lastCapturedPosition, std::move(lastCapturedPiece));
        lastCapturedPosition = Position();
    }
    if (movesSinceSeasonChange > 0) movesSinceSeasonChange--;
}

void Board::undoMultipleMoves(int n) {
    for (int i = 0; i < n && !moveHistory.empty(); ++i)
        undoLastMove();
}


void Board::enableQueenDoubleMove(Color color) {
    if (color == Color::WHITE) { whiteQueenCanDoubleMove = true; std::cout << "Massacre! White king can move twice.\n"; }
    else                       { blackQueenCanDoubleMove = true; std::cout << "Massacre! Black king can move twice.\n"; }
}
bool Board::canQueenDoubleMove(Color color) const {
    return (color == Color::WHITE) ? whiteQueenCanDoubleMove : blackQueenCanDoubleMove;
}
void Board::resetQueenDoubleMove(Color color) {
    if (color == Color::WHITE) whiteQueenCanDoubleMove = false;
    else blackQueenCanDoubleMove = false;
}


Season Board::getCurrentSeason() const { return currentSeason; }
void   Board::setCurrentSeason(Season s) { currentSeason = s; }

void Board::updateSeasonCycle() {
    Season seasons[] = {SPRING, SUMMER, AUTUMN, WINTER};
    for (int i = 0; i < 4; ++i) {
        if (seasons[i] == currentSeason) {
            currentSeason = seasons[(i+1)%4];
            movesSinceSeasonChange = 0;
            const char* names[] = {"Spring","Summer","Autumn","Winter"};
            std::cout << "Season changed to: " << names[(i+1)%4] << std::endl;
            switch (currentSeason) {
                case SPRING: std::cout << "Spring: Pawns can only move once every 2 turns.\n"; break;
                case SUMMER: std::cout << "Summer: Bishops can move up to 3 squares.\n"; break;
                case AUTUMN: std::cout << "Autumn: Knights can only move once every 2 turns.\n"; break;
                case WINTER: std::cout << "Winter: Rooks can move at most 4 squares.\n"; break;
            }
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
        return (currentTurnNumber - it->second) >= 2;
    }
    if (currentSeason == AUTUMN && type == PieceType::KNIGHT) {
        auto it = lastMoveTurn.find(pos);
        if (it == lastMoveTurn.end()) return true;
        return (currentTurnNumber - it->second) >= 2;
    }
    return true;
}

int Board::getMaxMovementInSeason(PieceType type) const {
    if (currentSeason == SUMMER && type == PieceType::BISHOP) return 3;
    if (currentSeason == WINTER && type == PieceType::ROOK)   return 4;
    return -1;
}


bool Board::transformJoker(Position pos, PieceType targetType) {
    Piece* piece = getPieceAt(pos);
    if (!piece || piece->getType() != PieceType::JOKER) return false;
    auto it = jokerTransformsRemaining.find(pos);
    int remaining = (it != jokerTransformsRemaining.end()) ? it->second : 2;
    if (remaining <= 0) return false;
    jokerCurrentMimic[pos] = targetType;
    jokerTransformsRemaining[pos] = remaining - 1;
    std::cout << "Joker transformed! Remaining: " << (remaining-1) << std::endl;
    return true;
}
void     Board::resetJokerTransform(Position pos)         { jokerCurrentMimic.erase(pos); }
int      Board::getJokerTransformsRemaining(Position pos) const {
    auto it = jokerTransformsRemaining.find(pos);
    return (it == jokerTransformsRemaining.end()) ? 2 : it->second;
}
bool     Board::isJokerTransformed(Position pos)          const { return jokerCurrentMimic.find(pos) != jokerCurrentMimic.end(); }
PieceType Board::getJokerCurrentMimic(Position pos)       const {
    auto it = jokerCurrentMimic.find(pos);
    return (it == jokerCurrentMimic.end()) ? PieceType::JOKER : it->second;
}


void Board::updateSpyCounters() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            Position pos(r, c);
            Piece* p = getPieceAt(pos);
            if (p && p->getType() == PieceType::SPY) {
                spyMoveCounter[pos]++;
                if (spyMoveCounter[pos] >= spyRevealThreshold)
                    std::cout << "Spy at " << (char)('a'+c) << (8-r) << " is about to be revealed!\n";
            }
        }
}
int Board::getSpyMovesUntilReveal(Position pos) const {
    auto it = spyMoveCounter.find(pos);
    int moves = (it != spyMoveCounter.end()) ? it->second : 0;
    return std::max(0, spyRevealThreshold - moves);
}


bool Board::tryCaptureArmoredQueen(Position pos) {
    Piece* piece = getPieceAt(pos);
    if (!piece || piece->getType() != PieceType::ARMORED_QUEEN) return true;
    ArmoredQueen* aq = dynamic_cast<ArmoredQueen*>(piece);
    if (!aq) return true;
    return aq->attemptCapture();
}

void Board::resetArmoredQueenAttackFlags(Color color) {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->getType() == PieceType::ARMORED_QUEEN && p->getColor() == color) {
                ArmoredQueen* aq = dynamic_cast<ArmoredQueen*>(p);
                if (aq) aq->resetAttackFlag();
            }
        }
}


void Board::printBoard() const {
    std::cout << "\n   a  b  c  d  e  f  g  h\n";
    for (int r = 0; r < 8; ++r) {
        std::cout << (8-r) << " ";
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p) std::cout << std::setw(2) << p->getSymbol() << " ";
            else   std::cout << " . ";
        }
        std::cout << (8-r) << "\n";
    }
    std::cout << "   a  b  c  d  e  f  g  h\n\n";
}


std::string Board::serialize() const {
    std::ostringstream ss;

    ss << (whiteKingMoved  ? 1:0) << ","
       << (blackKingMoved  ? 1:0) << ","
       << (whiteKingsideRookMoved  ? 1:0) << ","
       << (whiteQueensideRookMoved ? 1:0) << ","
       << (blackKingsideRookMoved  ? 1:0) << ","
       << (blackQueensideRookMoved ? 1:0) << ","
       << (enPassantAvailable ? 1:0) << ","
       << enPassantSquare.row << ","
       << enPassantSquare.col << ","
       << static_cast<int>(currentSeason) << ","
       << movesSinceSeasonChange << ","
       << currentTurnNumber << "|";

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = grid[r][c].get();
            if (!p) {
                ss << "-;";
                continue;
            }
            int t = static_cast<int>(p->getType());
            int col = static_cast<int>(p->getColor());
            int mc = p->getMoveCount();
            ss << t << "," << col << "," << mc;

            if (p->getType() == PieceType::ARMORED_QUEEN) {
                ArmoredQueen* aq = dynamic_cast<ArmoredQueen*>(p);
                if (aq) ss << "," << aq->getArmor();
            } else if (p->getType() == PieceType::JOKER) {
                Joker* j = dynamic_cast<Joker*>(p);
                if (j) ss << "," << j->getTransformsRemaining()
                          << "," << static_cast<int>(j->getCurrentMimic());
            } else if (p->getType() == PieceType::SPY) {
                Spy* spy = dynamic_cast<Spy*>(p);
                if (spy) {
                    ss << "," << (spy->isRevealed() ? 1:0)
                       << "," << static_cast<int>(spy->getRealOwner())
                       << "," << static_cast<int>(spy->getDisguisedAs());
                }
            }
            ss << ";";
        }
    }

    return ss.str();
}


void Board::deserialize(const std::string& data) {
    clear();
    if (data.empty()) return;

    auto pipePos = data.find('|');
    if (pipePos == std::string::npos) return;

    std::string flagPart = data.substr(0, pipePos);
    std::string gridPart = data.substr(pipePos + 1);

    try {
        std::istringstream fs(flagPart);
        std::string field;
        auto nextField = [&]() -> int {
            std::getline(fs, field, ',');
            return std::stoi(field);
        };
        whiteKingMoved            = nextField();
        blackKingMoved            = nextField();
        whiteKingsideRookMoved    = nextField();
        whiteQueensideRookMoved   = nextField();
        blackKingsideRookMoved    = nextField();
        blackQueensideRookMoved   = nextField();
        enPassantAvailable        = nextField();
        int epRow                 = nextField();
        int epCol                 = nextField();
        enPassantSquare           = Position(epRow, epCol);
        currentSeason             = static_cast<Season>(nextField());
        movesSinceSeasonChange    = nextField();
        currentTurnNumber         = nextField();
    } catch (...) {
        std::cerr << "Board::deserialize: error parsing flags\n";
        return;
    }


    std::istringstream gs(gridPart);
    std::string cellToken;
    int r = 0, c = 0;

    while (std::getline(gs, cellToken, ';')) {
        if (r >= 8) break;
        if (cellToken.empty()) {  }
        else if (cellToken == "-") {
            grid[r][c].reset();
        } else {
            try {
                std::istringstream cs(cellToken);
                std::string f;
                auto nf = [&]() -> int { std::getline(cs, f, ','); return std::stoi(f); };

                int type  = nf();
                int color = nf();
                int mc    = nf();
                PieceType pt = static_cast<PieceType>(type);
                Color     cl = static_cast<Color>(color);
                Position  pos(r, c);

                switch (pt) {
                    case PieceType::PAWN:
                        grid[r][c] = std::make_unique<Pawn>(cl, pos);
                        break;
                    case PieceType::SPECIAL_PAWN:
                        grid[r][c] = std::make_unique<SpecialPawn>(cl, pos);
                        break;
                    case PieceType::ROOK:
                        grid[r][c] = std::make_unique<Rook>(cl, pos);
                        break;
                    case PieceType::KNIGHT:
                        grid[r][c] = std::make_unique<Knight>(cl, pos);
                        break;
                    case PieceType::BISHOP:
                        grid[r][c] = std::make_unique<Bishop>(cl, pos);
                        break;
                    case PieceType::QUEEN:
                        grid[r][c] = std::make_unique<Queen>(cl, pos);
                        break;
                    case PieceType::KING:
                        grid[r][c] = std::make_unique<King>(cl, pos);
                        break;
                    case PieceType::ARMORED_QUEEN: {
                        auto aq = std::make_unique<ArmoredQueen>(cl, pos);
                        if (cs.peek() != EOF) { int armor = nf(); aq->setArmor(armor); }
                        grid[r][c] = std::move(aq);
                        break;
                    }
                    case PieceType::JOKER: {
                        auto jk = std::make_unique<Joker>(cl, pos);

                        grid[r][c] = std::move(jk);
                        break;
                    }
                    case PieceType::SPY: {
                        if (cs.peek() != EOF) {
                            int revealed  = nf();
                            int realOwner = nf();
                            int disguised = nf();
                            Color ro  = static_cast<Color>(realOwner);
                            PieceType dt = static_cast<PieceType>(disguised);
                            std::unique_ptr<Piece> disguisePiece;
                            switch (dt) {
                                case PieceType::PAWN:   disguisePiece = std::make_unique<Pawn>(cl, pos);   break;
                                case PieceType::ROOK:   disguisePiece = std::make_unique<Rook>(cl, pos);   break;
                                case PieceType::KNIGHT: disguisePiece = std::make_unique<Knight>(cl, pos); break;
                                case PieceType::BISHOP: disguisePiece = std::make_unique<Bishop>(cl, pos); break;
                                case PieceType::QUEEN:  disguisePiece = std::make_unique<Queen>(cl, pos);  break;
                                default:                disguisePiece = std::make_unique<Pawn>(cl, pos);
                            }
                            auto spy = std::make_unique<Spy>(ro, std::move(disguisePiece));
                            if (revealed) spy->reveal();
                            grid[r][c] = std::move(spy);
                        }
                        break;
                    }
                    default:
                        grid[r][c] = std::make_unique<Pawn>(cl, pos);
                        break;
                }

                if (grid[r][c]) {
                    for (int i = 0; i < mc; ++i)
                        grid[r][c]->incrementMoveCount();
                }

            } catch (...) {
                std::cerr << "Board::deserialize: error parsing cell [" << r << "][" << c << "]\n";
            }
        }

        ++c;
        if (c >= 8) { c = 0; ++r; }
    }
}
