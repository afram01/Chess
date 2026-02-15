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
    movesPerSeasonChange(20),
    currentTurnNumber(0),
    spyRevealThreshold(3)  
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
    currentTurnNumber(other.currentTurnNumber),
    lastMoveTurn(other.lastMoveTurn),
    armoredQueenArmor(other.armoredQueenArmor),
    jokerTransformsRemaining(other.jokerTransformsRemaining),
    jokerCurrentMimic(other.jokerCurrentMimic),
    spyMoveCounter(other.spyMoveCounter),
    spyRevealThreshold(other.spyRevealThreshold)
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
        currentTurnNumber = other.currentTurnNumber;
        lastMoveTurn = other.lastMoveTurn;
        
        armoredQueenArmor = other.armoredQueenArmor;
        jokerTransformsRemaining = other.jokerTransformsRemaining;
        jokerCurrentMimic = other.jokerCurrentMimic;
        spyMoveCounter = other.spyMoveCounter;
        spyRevealThreshold = other.spyRevealThreshold;
        
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
    

    armoredQueenArmor[Position(0, 3)] = 2; 
    armoredQueenArmor[Position(7, 3)] = 2;  
    
    jokerTransformsRemaining[Position(1, 3)] = 2; 
    jokerTransformsRemaining[Position(6, 3)] = 2;  
    
    spyMoveCounter[Position(2, 4)] = 0;  
    spyMoveCounter[Position(5, 3)] = 0;
    
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
    lastMoveTurn.clear();
    armoredQueenArmor.clear();
    jokerTransformsRemaining.clear();
    jokerCurrentMimic.clear();
    spyMoveCounter.clear();
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
    currentTurnNumber = 0;
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
    lastMoveTurn.erase(pos);
    armoredQueenArmor.erase(pos);
    jokerTransformsRemaining.erase(pos);
    jokerCurrentMimic.erase(pos);
    spyMoveCounter.erase(pos);
    
    return std::move(grid[pos.row][pos.col]);
}


void Board::movePiece(const Move& move) {
    if (!move.from.isValid() || !move.to.isValid()) return;
    
    Piece* piece = getPieceAt(move.from);
    if (!piece) return;
    

    Piece* targetPiece = getPieceAt(move.to);
    if (targetPiece) {
        if (targetPiece->getType() == PieceType::ARMORED_QUEEN) {

            if (!tryAttackArmoredQueen(move.to)) {

                return;
            }
        }
        
        lastCapturedPiece = removePieceAt(move.to);
        lastCapturedPosition = move.to;
        
        if (targetPiece->getType() == PieceType::QUEEN || 
            targetPiece->getType() == PieceType::ARMORED_QUEEN) {
            enableQueenDoubleMove(piece->getColor());
        }
    }

    auto movingPiece = removePieceAt(move.from);
    movingPiece->setPosition(move.to);
    movingPiece->setHasMoved(true);
    setPieceAt(move.to, std::move(movingPiece));

    lastMoveTurn[move.to] = currentTurnNumber;
    
    updateCastlingRights(move);
    
    updateStationaryCounters(move);
    

    updateSpyCounters();
    

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
    
    if (!canPieceMoveInSeason(pos)) {
        return legalMoves;  
    }
    
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


void Board::setEnPassantSquare(Position pos) {
    enPassantSquare = pos;
    enPassantAvailable = true;
}


void Board::addMoveToHistory(const Move& move) {
    moveHistory.push_back(move);
}

void Board::undoLastMove() {
    if (moveHistory.empty()) return;
    
    Move lastMove = moveHistory.back();
    moveHistory.pop_back();

    auto piece = removePieceAt(lastMove.to);
    if (piece) {
        piece->setPosition(lastMove.from);
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
    for (int i = 0; i < n && !moveHistory.empty(); i++) {
        undoLastMove();
    }
}


std::vector<Piece*> Board::getAllPieces(Color color) const {
    std::vector<Piece*> pieces;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece* piece = getPieceAt(r, c);
            if (piece && piece->getColor() == color) {
                pieces.push_back(piece);
            }
        }
    }
    return pieces;
}

int Board::countPieces(Color color) const {
    return getAllPieces(color).size();
}

int Board::getMaterialValue(Color color) const {
    int value = 0;
    std::map<PieceType, int> pieceValues = {
        {PieceType::PAWN, 1},
        {PieceType::KNIGHT, 3},
        {PieceType::BISHOP, 3},
        {PieceType::ROOK, 5},
        {PieceType::QUEEN, 9},
        {PieceType::KING, 0},
        {PieceType::SOLDIER_PLUS, 2},
        {PieceType::ARMORED_QUEEN, 10},
        {PieceType::SPY, 2},
        {PieceType::JOKER, 3}
    };
    
    for (const auto& piece : getAllPieces(color)) {
        value += pieceValues[piece->getType()];
    }
    
    return value;
}

std::map<PieceType, int> Board::getPieceCounts(Color color) const {
    std::map<PieceType, int> counts;
    
    for (const auto& piece : getAllPieces(color)) {
        counts[piece->getType()]++;
    }
    
    return counts;
}


void Board::promotePawn(Position pos, PieceType newType) {
    Piece* pawn = getPieceAt(pos);
    if (!pawn || (pawn->getType() != PieceType::PAWN && pawn->getType() != PieceType::SOLDIER_PLUS)) {
        return;
    }
    
    Color color = pawn->getColor();
    std::unique_ptr<Piece> newPiece;
    
    switch (newType) {
        case PieceType::QUEEN:
            newPiece = std::make_unique<Queen>(color, pos);
            break;
        case PieceType::ROOK:
            newPiece = std::make_unique<Rook>(color, pos);
            break;
        case PieceType::BISHOP:
            newPiece = std::make_unique<Bishop>(color, pos);
            break;
        case PieceType::KNIGHT:
            newPiece = std::make_unique<Knight>(color, pos);
            break;
        default:
            return;
    }
    
    newPiece->setHasMoved(true);
    removePieceAt(pos);
    setPieceAt(pos, std::move(newPiece));
}

bool Board::canPromote(Position pos) const {
    Piece* piece = getPieceAt(pos);
    if (!piece || (piece->getType() != PieceType::PAWN && piece->getType() != PieceType::SOLDIER_PLUS)) {
        return false;
    }
    
    if (piece->getColor() == Color::WHITE && pos.row == 7) return true;
    if (piece->getColor() == Color::BLACK && pos.row == 0) return true;
    
    return false;
}


std::vector<Position> Board::getExplosionTargets(Position center) const {
    std::vector<Position> targets;
    int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    
    for (auto& dir : directions) {
        Position pos(center.row + dir[0], center.col + dir[1]);
        if (pos.isValid() && getPieceAt(pos)) {
            targets.push_back(pos);
        }
    }
    return targets;
}

void Board::explodePieces(const std::vector<Position>& positions) {
    for (const auto& pos : positions) {
        removePieceAt(pos);
    }
}

void Board::updateStationaryCounters(const Move& move) {
    pieceStationaryCounter.erase(move.to);
    
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Position pos(r, c);
            if (getPieceAt(pos) && pos != move.to) {
                pieceStationaryCounter[pos]++;
            }
        }
    }
}

void Board::checkAndExplodeBombs() {
    std::vector<Position> bombPositions;
    
    for (const auto& entry : pieceStationaryCounter) {
        if (entry.second >= bombThreshold) {
            bombPositions.push_back(entry.first);
        }
    }
    
    for (const auto& bombPos : bombPositions) {
        auto targets = getExplosionTargets(bombPos);
        targets.push_back(bombPos);
        explodePieces(targets);
    }
}


void Board::enableQueenDoubleMove(Color color) {
    if (color == Color::WHITE) {
        whiteQueenCanDoubleMove = true;
    } else {
        blackQueenCanDoubleMove = true;
    }
}

bool Board::canQueenDoubleMove(Color color) const {
    if (color == Color::WHITE) {
        return whiteQueenCanDoubleMove;
    } else {
        return blackQueenCanDoubleMove;
    }
}

void Board::resetQueenDoubleMove(Color color) {
    if (color == Color::WHITE) {
        whiteQueenCanDoubleMove = false;
    } else {
        blackQueenCanDoubleMove = false;
    }
}


void Board::advanceSeasonCycle() {
    Season seasons[] = {Season::SPRING, Season::SUMMER, Season::AUTUMN, Season::WINTER};
    
    if (currentSeason == Season::NONE) {
        currentSeason = Season::SPRING;
    } else {
        for (int i = 0; i < 4; i++) {
            if (seasons[i] == currentSeason) {
                currentSeason = seasons[(i + 1) % 4];
                break;
            }
        }
    }
    
    movesSinceSeasonChange = 0;
    applySeason(currentSeason);
}

void Board::applySeason(Season season) {
    currentSeason = season;
}

bool Board::canPieceMoveInSeason(Position pos) const {
    Piece* piece = getPieceAt(pos);
    if (!piece) return false;
    
    PieceType type = piece->getType();
    
    if (currentSeason == Season::SPRING && 
        (type == PieceType::PAWN || type == PieceType::SOLDIER_PLUS)) {
        
        auto it = lastMoveTurn.find(pos);
        if (it == lastMoveTurn.end()) {
            return true;  
        }
        
        int turnsSinceLastMove = currentTurnNumber - it->second;
        if (turnsSinceLastMove < 2) {
            return false; 
        }
    }
    
    if (currentSeason == Season::AUTUMN && type == PieceType::KNIGHT) {
        auto it = lastMoveTurn.find(pos);
        if (it == lastMoveTurn.end()) {
            return true;
        }
        
        int turnsSinceLastMove = currentTurnNumber - it->second;
        if (turnsSinceLastMove < 2) {
            return false;
        }
    }
    
    return true;
}

int Board::getMaxMovementInSeason(PieceType type) const {

    if (currentSeason == Season::SUMMER && type == PieceType::BISHOP) {
        return 3;
    }
    

    if (currentSeason == Season::WINTER && type == PieceType::ROOK) {
        return 4;
    }
    
    return -1;  


bool Board::tryAttackArmoredQueen(Position pos) {
    Piece* piece = getPieceAt(pos);
    if (!piece || piece->getType() != PieceType::ARMORED_QUEEN) {
        return true;  
    }
    
    auto it = armoredQueenArmor.find(pos);
    int currentArmor = (it != armoredQueenArmor.end()) ? it->second : 2;
    
    if (currentArmor > 0) {

        armoredQueenArmor[pos] = currentArmor - 1;
        return false; 
    }

    return true;
}

int Board::getArmoredQueenArmor(Position pos) const {
    auto it = armoredQueenArmor.find(pos);
    if (it == armoredQueenArmor.end()) return 2;
    return it->second;
}

void Board::resetArmoredQueenArmor(Position pos, int armor) {
    armoredQueenArmor[pos] = armor;
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


void Board::revealSpy(Position pos) {
    Piece* piece = getPieceAt(pos);
    if (!piece || piece->getType() != PieceType::SPY) {
        return;
    }
    
    Spy* spy = dynamic_cast<Spy*>(piece);
    if (spy) {
        spy->reveal();
    }
}

void Board::activateSpy(Position pos) {
    Piece* piece = getPieceAt(pos);
    if (!piece || piece->getType() != PieceType::SPY) {
        return;
    }
    
    Spy* spy = dynamic_cast<Spy*>(piece);
    if (spy && spy->isRevealed()) {
        spy->activate();
    }
}

bool Board::isSpyRevealed(Position pos) const {
    Piece* piece = getPieceAt(pos);
    if (!piece || piece->getType() != PieceType::SPY) {
        return false;
    }
    
    const Spy* spy = dynamic_cast<const Spy*>(piece);
    return spy && spy->isRevealed();
}

void Board::updateSpyCounters() {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Position pos(r, c);
            Piece* piece = getPieceAt(pos);
            
            if (piece && piece->getType() == PieceType::SPY) {
                Spy* spy = dynamic_cast<Spy*>(piece);
                if (spy && !spy->isRevealed()) {
                    spyMoveCounter[pos]++;
                    
                    if (spyMoveCounter[pos] >= spyRevealThreshold) {
                        revealSpy(pos);
                    }
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


std::string Board::serialize() const {
    std::stringstream ss;
    
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (grid[r][c]) {
                ss << "1 " << grid[r][c]->serialize() << "\n";
            } else {
                ss << "0\n";
            }
        }
    }
    
    ss << enPassantAvailable << " " << enPassantSquare.row << " " << enPassantSquare.col << "\n";
    
    ss << whiteKingMoved << " " << blackKingMoved << " "
       << whiteKingsideRookMoved << " " << whiteQueensideRookMoved << " "
       << blackKingsideRookMoved << " " << blackQueensideRookMoved << "\n";
    
    ss << bombThreshold << "\n";
    ss << whiteQueenCanDoubleMove << " " << blackQueenCanDoubleMove << "\n";
    ss << static_cast<int>(currentSeason) << " " << movesSinceSeasonChange << " " 
       << movesPerSeasonChange << " " << currentTurnNumber << "\n";
    
    ss << spyRevealThreshold << "\n";
    
    ss << moveHistory.size() << "\n";
    for (const auto& move : moveHistory) {
        ss << move.from.row << " " << move.from.col << " " 
           << move.to.row << " " << move.to.col << "\n";
    }
    
    return ss.str();
}

void Board::deserialize(const std::string& data) {
    clear();
    std::istringstream iss(data);
    
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            int hasPiece;
            iss >> hasPiece;
            
            if (hasPiece) {
                int typeInt, colorInt, row, col, hasMoved;
                iss >> typeInt >> colorInt >> row >> col >> hasMoved;
                
                PieceType type = static_cast<PieceType>(typeInt);
                Color color = static_cast<Color>(colorInt);
                Position pos(row, col);
                
                std::unique_ptr<Piece> piece;
                switch (type) {
                    case PieceType::KING:
                        piece = std::make_unique<King>(color, pos);
                        break;
                    case PieceType::QUEEN:
                        piece = std::make_unique<Queen>(color, pos);
                        break;
                    case PieceType::ROOK:
                        piece = std::make_unique<Rook>(color, pos);
                        break;
                    case PieceType::BISHOP:
                        piece = std::make_unique<Bishop>(color, pos);
                        break;
                    case PieceType::KNIGHT:
                        piece = std::make_unique<Knight>(color, pos);
                        break;
                    case PieceType::PAWN:
                        piece = std::make_unique<Pawn>(color, pos);
                        break;
                    case PieceType::SOLDIER_PLUS:
                        piece = std::make_unique<SoldierPlus>(color, pos);
                        break;
                    case PieceType::ARMORED_QUEEN: {
                        int armor;
                        iss >> armor;
                        piece = std::make_unique<ArmoredQueen>(color, pos);
                        armoredQueenArmor[pos] = armor;
                        break;
                    }
                    case PieceType::SPY: {
                        int revealed, trueOwnerInt, moveCounter;
                        iss >> revealed >> trueOwnerInt >> moveCounter;
                        piece = std::make_unique<Spy>(color, static_cast<Color>(trueOwnerInt), pos);
                        spyMoveCounter[pos] = moveCounter;
                        if (revealed) {
                            dynamic_cast<Spy*>(piece.get())->reveal();
                        }
                        break;
                    }
                    case PieceType::JOKER: {
                        int remaining, mimicInt;
                        iss >> remaining >> mimicInt;
                        piece = std::make_unique<Joker>(color, pos);
                        jokerTransformsRemaining[pos] = remaining;
                        if (mimicInt != -1) {
                            jokerCurrentMimic[pos] = static_cast<PieceType>(mimicInt);
                        }
                        break;
                    }
                    default:
                        break;
                }
                
                if (piece) {
                    piece->setHasMoved(hasMoved);
                    grid[r][c] = std::move(piece);
                }
            }
        }
    }
    
    int epAvail, epRow, epCol;
    iss >> epAvail >> epRow >> epCol;
    enPassantAvailable = epAvail;
    enPassantSquare = Position(epRow, epCol);
    
    iss >> whiteKingMoved >> blackKingMoved
        >> whiteKingsideRookMoved >> whiteQueensideRookMoved
        >> blackKingsideRookMoved >> blackQueensideRookMoved;
    
    iss >> bombThreshold;
    iss >> whiteQueenCanDoubleMove >> blackQueenCanDoubleMove;
    
    int seasonInt;
    iss >> seasonInt >> movesSinceSeasonChange >> movesPerSeasonChange >> currentTurnNumber;
    currentSeason = static_cast<Season>(seasonInt);
    
    iss >> spyRevealThreshold;
    
    int historySize;
    iss >> historySize;
    for (int i = 0; i < historySize; i++) {
        int fr, fc, tr, tc;
        iss >> fr >> fc >> tr >> tc;
        moveHistory.push_back(Move(Position(fr, fc), Position(tr, tc)));
    }
}
