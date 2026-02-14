#include "Board.hpp"
#include <sstream>
#include <algorithm>


Board::Board() : 
    enPassantAvailable(false),
    whiteKingMoved(false), blackKingMoved(false),
    whiteKingsideRookMoved(false), whiteQueensideRookMoved(false),
    blackKingsideRookMoved(false), blackQueensideRookMoved(false),
    bombThreshold(5),  
    whiteQueenCanDoubleMove(false), blackQueenCanDoubleMove(false),
    currentSeason(Season::NONE),
    movesSinceSeasonChange(0),
    movesPerSeasonChange(20) 
{
    clear();
}

Board::Board(const Board& other) : 
    enPassantAvailable(other.enPassantAvailable),
    enPassantSquare(other.enPassantSquare),
    whiteKingMoved(other.whiteKingMoved),
    blackKingMoved(other.blackKingMoved),
    whiteKingsideRookMoved(other.whiteKingsideRookMoved),
    whiteQueensideRookMoved(other.whiteQueensideRookMoved),
    blackKingsideRookMoved(other.blackKingsideRookMoved),
    blackQueensideRookMoved(other.blackQueensideRookMoved),
    bombThreshold(other.bombThreshold),
    pieceStationaryCounter(other.pieceStationaryCounter),
    whiteQueenCanDoubleMove(other.whiteQueenCanDoubleMove),
    blackQueenCanDoubleMove(other.blackQueenCanDoubleMove),
    currentSeason(other.currentSeason),
    movesSinceSeasonChange(other.movesSinceSeasonChange),
    movesPerSeasonChange(other.movesPerSeasonChange),
    armoredQueenHits(other.armoredQueenHits),
    jokerTransformsUsed(other.jokerTransformsUsed)
{
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (other.grid[r][c]) {
                grid[r][c] = other.grid[r][c]->clone();
            }
        }
    }
    
    moveHistory = other.moveHistory;
    
    if (other.lastCapturedPiece) {
        lastCapturedPiece = other.lastCapturedPiece->clone();
    }
    lastCapturedPosition = other.lastCapturedPosition;
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        clear();
        
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (other.grid[r][c]) {
                    grid[r][c] = other.grid[r][c]->clone();
                }
            }
        }
        
        moveHistory = other.moveHistory;
        enPassantSquare = other.enPassantSquare;
        enPassantAvailable = other.enPassantAvailable;
        
        whiteKingMoved = other.whiteKingMoved;
        blackKingMoved = other.blackKingMoved;
        whiteKingsideRookMoved = other.whiteKingsideRookMoved;
        whiteQueensideRookMoved = other.whiteQueensideRookMoved;
        blackKingsideRookMoved = other.blackKingsideRookMoved;
        blackQueensideRookMoved = other.blackQueensideRookMoved;
        
        bombThreshold = other.bombThreshold;
        pieceStationaryCounter = other.pieceStationaryCounter;
        
        whiteQueenCanDoubleMove = other.whiteQueenCanDoubleMove;
        blackQueenCanDoubleMove = other.blackQueenCanDoubleMove;
        
        currentSeason = other.currentSeason;
        movesSinceSeasonChange = other.movesSinceSeasonChange;
        movesPerSeasonChange = other.movesPerSeasonChange;
        
        armoredQueenHits = other.armoredQueenHits;
        jokerTransformsUsed = other.jokerTransformsUsed;
        
        if (other.lastCapturedPiece) {
            lastCapturedPiece = other.lastCapturedPiece->clone();
        }
        lastCapturedPosition = other.lastCapturedPosition;
    }
    return *this;
}


void Board::initialize() {
    clear();
    
    for (int c = 0; c < 8; c++) {
        grid[1][c] = std::make_unique<Pawn>(Color::WHITE, Position(1, c));
    }
    
    for (int c = 0; c < 8; c++) {
        grid[6][c] = std::make_unique<Pawn>(Color::BLACK, Position(6, c));
    }
    
    grid[0][0] = std::make_unique<Rook>(Color::WHITE, Position(0, 0));
    grid[0][7] = std::make_unique<Rook>(Color::WHITE, Position(0, 7));
    grid[7][0] = std::make_unique<Rook>(Color::BLACK, Position(7, 0));
    grid[7][7] = std::make_unique<Rook>(Color::BLACK, Position(7, 7));
    
    grid[0][1] = std::make_unique<Knight>(Color::WHITE, Position(0, 1));
    grid[0][6] = std::make_unique<Knight>(Color::WHITE, Position(0, 6));
    grid[7][1] = std::make_unique<Knight>(Color::BLACK, Position(7, 1));
    grid[7][6] = std::make_unique<Knight>(Color::BLACK, Position(7, 6));
    
    grid[0][2] = std::make_unique<Bishop>(Color::WHITE, Position(0, 2));
    grid[0][5] = std::make_unique<Bishop>(Color::WHITE, Position(0, 5));
    grid[7][2] = std::make_unique<Bishop>(Color::BLACK, Position(7, 2));
    grid[7][5] = std::make_unique<Bishop>(Color::BLACK, Position(7, 5));
    
    grid[0][3] = std::make_unique<Queen>(Color::WHITE, Position(0, 3));
    grid[7][3] = std::make_unique<Queen>(Color::BLACK, Position(7, 3));
    
    grid[0][4] = std::make_unique<King>(Color::WHITE, Position(0, 4));
    grid[7][4] = std::make_unique<King>(Color::BLACK, Position(7, 4));
    
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteKingsideRookMoved = false;
    whiteQueensideRookMoved = false;
    blackKingsideRookMoved = false;
    blackQueensideRookMoved = false;
}

void Board::initializeWithSpecialPieces() {
    clear();
    
    grid[1][0] = std::make_unique<SoldierPlus>(Color::WHITE, Position(1, 0));
    grid[1][1] = std::make_unique<Pawn>(Color::WHITE, Position(1, 1));
    grid[1][2] = std::make_unique<Pawn>(Color::WHITE, Position(1, 2));
    grid[1][3] = std::make_unique<Joker>(Color::WHITE, Position(1, 3));
    grid[1][4] = std::make_unique<Pawn>(Color::WHITE, Position(1, 4));
    grid[1][5] = std::make_unique<Pawn>(Color::WHITE, Position(1, 5));
    grid[1][6] = std::make_unique<Pawn>(Color::WHITE, Position(1, 6));
    grid[1][7] = std::make_unique<Pawn>(Color::WHITE, Position(1, 7));
    
    grid[6][0] = std::make_unique<Pawn>(Color::BLACK, Position(6, 0));
    grid[6][1] = std::make_unique<Pawn>(Color::BLACK, Position(6, 1));
    grid[6][2] = std::make_unique<Pawn>(Color::BLACK, Position(6, 2));
    grid[6][3] = std::make_unique<Joker>(Color::BLACK, Position(6, 3));
    grid[6][4] = std::make_unique<SoldierPlus>(Color::BLACK, Position(6, 4));
    grid[6][5] = std::make_unique<Pawn>(Color::BLACK, Position(6, 5));
    grid[6][6] = std::make_unique<Pawn>(Color::BLACK, Position(6, 6));
    grid[6][7] = std::make_unique<Pawn>(Color::BLACK, Position(6, 7));
    
    grid[0][0] = std::make_unique<Rook>(Color::WHITE, Position(0, 0));
    grid[0][7] = std::make_unique<Rook>(Color::WHITE, Position(0, 7));
    grid[7][0] = std::make_unique<Rook>(Color::BLACK, Position(7, 0));
    grid[7][7] = std::make_unique<Rook>(Color::BLACK, Position(7, 7));
    
    grid[0][1] = std::make_unique<Knight>(Color::WHITE, Position(0, 1));
    grid[0][6] = std::make_unique<Knight>(Color::WHITE, Position(0, 6));
    grid[7][1] = std::make_unique<Knight>(Color::BLACK, Position(7, 1));
    grid[7][6] = std::make_unique<Knight>(Color::BLACK, Position(7, 6));
    
    grid[0][2] = std::make_unique<Bishop>(Color::WHITE, Position(0, 2));
    grid[0][5] = std::make_unique<Bishop>(Color::WHITE, Position(0, 5));
    grid[7][2] = std::make_unique<Bishop>(Color::BLACK, Position(7, 2));
    grid[7][5] = std::make_unique<Bishop>(Color::BLACK, Position(7, 5));
    
    grid[0][3] = std::make_unique<ArmoredQueen>(Color::WHITE, Position(0, 3));
    grid[7][3] = std::make_unique<ArmoredQueen>(Color::BLACK, Position(7, 3));
    
    grid[0][4] = std::make_unique<King>(Color::WHITE, Position(0, 4));
    grid[7][4] = std::make_unique<King>(Color::BLACK, Position(7, 4));
    
    grid[2][4] = std::make_unique<Spy>(Color::WHITE, Color::BLACK, Position(2, 4));
    grid[5][3] = std::make_unique<Spy>(Color::BLACK, Color::WHITE, Position(5, 3));
    
    armoredQueenHits[Position(0, 3)] = 0;
    armoredQueenHits[Position(7, 3)] = 0;
    jokerTransformsUsed[Position(1, 3)] = 0;
    jokerTransformsUsed[Position(6, 3)] = 0;
    
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteKingsideRookMoved = false;
    whiteQueensideRookMoved = false;
    blackKingsideRookMoved = false;
    blackQueensideRookMoved = false;
}

void Board::clear() {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            grid[r][c].reset();
        }
    }
    moveHistory.clear();
    enPassantAvailable = false;
    enPassantSquare = Position();
    pieceStationaryCounter.clear();
    armoredQueenHits.clear();
    jokerTransformsUsed.clear();
    lastCapturedPiece.reset();
    
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteKingsideRookMoved = false;
    whiteQueensideRookMoved = false;
    blackKingsideRookMoved = false;
    blackQueensideRookMoved = false;
    
    whiteQueenCanDoubleMove = false;
    blackQueenCanDoubleMove = false;
    
    currentSeason = Season::NONE;
    movesSinceSeasonChange = 0;
}
Piece* Board::getPieceAt(Position pos) const {
    if (!pos.isValid()) return nullptr;
    return grid[pos.row][pos.col].get();
}

Piece* Board::getPieceAt(int row, int col) const {
    return getPieceAt(Position(row, col));
}

void Board::setPieceAt(Position pos, std::unique_ptr<Piece> piece) {
    if (pos.isValid()) {
        grid[pos.row][pos.col] = std::move(piece);
        if (grid[pos.row][pos.col]) {
            grid[pos.row][pos.col]->setPosition(pos);
        }
    }
}

std::unique_ptr<Piece> Board::removePieceAt(Position pos) {
    if (!pos.isValid()) return nullptr;
    
    pieceStationaryCounter.erase(pos);
    armoredQueenHits.erase(pos);
    jokerTransformsUsed.erase(pos);
    
    return std::move(grid[pos.row][pos.col]);
}




void Board::movePiece(const Move& move) {
    if (!move.from.isValid() || !move.to.isValid()) return;
    
    Piece* piece = getPieceAt(move.from);
    if (!piece) return;
    
    Piece* captured = getPieceAt(move.to);
    if (captured) {
        lastCapturedPiece = removePieceAt(move.to);
        lastCapturedPosition = move.to;
        
        if (captured->getType() == PieceType::QUEEN || 
            captured->getType() == PieceType::ARMORED_QUEEN) {
            enableQueenDoubleMove(piece->getColor());
        }
    }
    
    auto movingPiece = removePieceAt(move.from);
    movingPiece->setPosition(move.to);
    movingPiece->setHasMoved(true);
    setPieceAt(move.to, std::move(movingPiece));
    
    updateCastlingRights(move);
    
    updateStationaryCounters(move);
    
    movesSinceSeasonChange++;
    if (movesSinceSeasonChange >= movesPerSeasonChange) {
        advanceSeasonCycle();
    }
    
    addMoveToHistory(move);
    
    checkAndExplodeBombs();
}



void Board::updateCastlingRights(const Move& move) {
    Piece* piece = getPieceAt(move.to);
    if (!piece) return;
    
    if (piece->getType() == PieceType::KING) {
        if (piece->getColor() == Color::WHITE) {
            whiteKingMoved = true;
        } else {
            blackKingMoved = true;
        }
    }
    
    if (piece->getType() == PieceType::ROOK) {
        if (piece->getColor() == Color::WHITE) {
            if (move.from.row == 0 && move.from.col == 0) {
                whiteQueensideRookMoved = true;
            } else if (move.from.row == 0 && move.from.col == 7) {
                whiteKingsideRookMoved = true;
            }
        } else {
            if (move.from.row == 7 && move.from.col == 0) {
                blackQueensideRookMoved = true;
            } else if (move.from.row == 7 && move.from.col == 7) {
                blackKingsideRookMoved = true;
            }
        }
    }
}

void Board::setCastlingRight(Color color, bool kingside, bool value) {
    if (color == Color::WHITE) {
        if (kingside) {
            whiteKingsideRookMoved = !value;
        } else {
            whiteQueensideRookMoved = !value;
        }
    } else {
        if (kingside) {
            blackKingsideRookMoved = !value;
        } else {
            blackQueensideRookMoved = !value;
        }
    }
}



bool Board::isSquareEmpty(Position pos) const {
    return getPieceAt(pos) == nullptr;
}

bool Board::isSquareAttacked(Position pos, Color byColor) const {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece* piece = getPieceAt(r, c);
            if (piece && piece->getColor() == byColor) {
                auto moves = piece->getPossibleMoves(*this);
                for (const auto& move : moves) {
                    if (move.to == pos) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Board::isInCheck(Color color) const {
    Position kingPos = findKing(color);
    if (!kingPos.isValid()) return false;
    
    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return isSquareAttacked(kingPos, opponent);
}

bool Board::isCheckmate(Color color) const {
    if (!isInCheck(color)) return false;
    return getAllLegalMoves(color).empty();
}

bool Board::isStalemate(Color color) const {
    if (isInCheck(color)) return false;
    return getAllLegalMoves(color).empty();
}

Position Board::findKing(Color color) const {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece* piece = getPieceAt(r, c);
            if (piece && piece->getType() == PieceType::KING && piece->getColor() == color) {
                return Position(r, c);
            }
        }
    }
    return Position(-1, -1);
}




std::vector<Move> Board::getAllLegalMoves(Color color) const {
    std::vector<Move> legalMoves;
    
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece* piece = getPieceAt(r, c);
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
    
    auto possibleMoves = piece->getPossibleMoves(*this);
    
    for (const auto& move : possibleMoves) {
        if (isMoveLegal(move, piece->getColor())) {
            legalMoves.push_back(move);
        }
    }
    
    return legalMoves;
}

bool Board::isMoveLegal(const Move& move, Color color) const {
    Board tempBoard(*this);
    
    Piece* piece = tempBoard.getPieceAt(move.from);
    if (!piece) return false;
    
    auto movingPiece = tempBoard.removePieceAt(move.from);
    movingPiece->setPosition(move.to);
    tempBoard.setPieceAt(move.to, std::move(movingPiece));
    
    if (piece->getType() == PieceType::PAWN && move.to == enPassantSquare && enPassantAvailable) {
        int captureRow = (color == Color::WHITE) ? move.to.row - 1 : move.to.row + 1;
        tempBoard.removePieceAt(Position(captureRow, move.to.col));
    }
    
    return !tempBoard.isInCheck(color);
}


bool Board::canCastleKingside(Color color) const {
    int row = (color == Color::WHITE) ? 0 : 7;
    
    if (color == Color::WHITE) {
        if (whiteKingMoved || whiteKingsideRookMoved) return false;
    } else {
        if (blackKingMoved || blackKingsideRookMoved) return false;
    }
    
    Piece* king = getPieceAt(row, 4);
    Piece* rook = getPieceAt(row, 7);
    
    if (!king || king->getType() != PieceType::KING) return false;
    if (!rook || rook->getType() != PieceType::ROOK) return false;
    
    if (!isSquareEmpty(Position(row, 5)) || !isSquareEmpty(Position(row, 6))) return false;
    
    if (isInCheck(color)) return false;
    
    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (isSquareAttacked(Position(row, 5), opponent)) return false;
    if (isSquareAttacked(Position(row, 6), opponent)) return false;
    
    return true;
}

bool Board::canCastleQueenside(Color color) const {
    int row = (color == Color::WHITE) ? 0 : 7;
    
    if (color == Color::WHITE) {
        if (whiteKingMoved || whiteQueensideRookMoved) return false;
    } else {
        if (blackKingMoved || blackQueensideRookMoved) return false;
    }
    
    Piece* king = getPieceAt(row, 4);
    Piece* rook = getPieceAt(row, 0);
    
    if (!king || king->getType() != PieceType::KING) return false;
    if (!rook || rook->getType() != PieceType::ROOK) return false;
    
    if (!isSquareEmpty(Position(row, 1)) || !isSquareEmpty(Position(row, 2)) || 
        !isSquareEmpty(Position(row, 3))) return false;
    
    if (isInCheck(color)) return false;
    
    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (isSquareAttacked(Position(row, 2), opponent)) return false;
    if (isSquareAttacked(Position(row, 3), opponent)) return false;
    
    return true;
}

void Board::performCastling(const Move& move) {
    int row = move.from.row;
    
    auto king = removePieceAt(move.from);
    king->setPosition(move.to);
    king->setHasMoved(true);
    setPieceAt(move.to, std::move(king));
    
    if (move.to.col == 6) {
        auto rook = removePieceAt(Position(row, 7));
        rook->setPosition(Position(row, 5));
        rook->setHasMoved(true);
        setPieceAt(Position(row, 5), std::move(rook));
    } else {  
        auto rook = removePieceAt(Position(row, 0));
        rook->setPosition(Position(row, 3));
        rook->setHasMoved(true);
        setPieceAt(Position(row, 3), std::move(rook));
    }
    
    updateCastlingRights(move);
    
    addMoveToHistory(move);
}