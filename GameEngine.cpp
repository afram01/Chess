#include "GameEngine.hpp"
#include <iostream>
#include <sstream>

GameEngine::GameEngine() : state() {}

void GameEngine::newGame() {
    newGame(GameModeType::STANDARD);
}

void GameEngine::newGame(GameModeType mode) {
    if (mode == GameModeType::STANDARD) {
        board.setupBoard();
    } else {
        board.setupBoardWithSpecialPieces();  
    }
    
    state.setGameMode(mode);

    switch (mode) {
        case GameModeType::ENERGY:
            state.setEnergy(Color::WHITE, 100);
            state.setEnergy(Color::BLACK, 100);
            break;
        case GameModeType::MISSION:
            state.setMissionDescription("Capture the enemy queen in 20 moves!");
            state.setMovesLeft(20);
            break;
        default:
            break;
    }
    
    initializeGame();
}

void GameEngine::initializeGame() {
    state.setCurrentTurn(Color::WHITE);
    state.setStatus(GameStatus::PLAYING);
    updateGameState();
}


bool GameEngine::processMove(const std::string& input) {
    std::istringstream iss(input);
    std::string fromStr, toStr;
    iss >> fromStr >> toStr;

    Position from = parsePosition(fromStr);
    Position to = parsePosition(toStr);

    if (!from.isValid() || !to.isValid()) {
        std::cout << "Invalid position!" << std::endl;
        return false;
    }

    Move move(from, to);

    Piece* p = board.getPiece(from.row, from.col);
    if (!p || p->getColor() != state.getCurrentTurn()) {
        std::cout << "Invalid piece or wrong turn!" << std::endl;
        return false;
    }

    Color currentPlayer = state.getCurrentTurn();
    if (board.isInCheck(currentPlayer)) {
        if (!board.isMoveLegal(move, currentPlayer)) {
            std::cout << "⚠️  Your king is in CHECK! You must escape check." << std::endl;
            return false;
        }
    }

    PieceType movedPieceType = p->getType();

    if (!board.canPieceMoveInSeason(from)) {
        const char* seasonNames[] = {"Spring", "Summer", "Autumn", "Winter"};
        std::cout << "This piece cannot move this season ("
                  << seasonNames[static_cast<int>(board.getCurrentSeason())]
                  << ")." << std::endl;
        return false;
    }

    if (!board.movePiece(move)) {
        std::cout << "Illegal move!" << std::endl;
        return false;
    }

    board.incrementTurnNumber();

    if (state.getGameMode() == GameModeType::ENERGY) {
        int cost = calculateMoveCost(movedPieceType);
        state.reduceEnergy(currentPlayer, cost);

        if (state.getEnergy(currentPlayer) <= 0) {
            std::cout << "Energy exhausted! "
                      << (currentPlayer == Color::WHITE ? "White" : "Black")
                      << " loses." << std::endl;
            state.setStatus(GameStatus::DRAW);
            return true;
        }
    }

    if (state.getGameMode() == GameModeType::MISSION) {
        state.decrementMovesLeft();

        if (state.getMovesLeft() <= 0) {
            std::cout << "Moves exhausted! Mission failed." << std::endl;
            state.setStatus(GameStatus::DRAW);
            return true;
        }
    }

    moveHistory.push_back(createMoveRecord(move));
    state.incrementTurn();
    state.updateSeason();

    if (board.canQueenDoubleMove(currentPlayer)) {
        std::cout << "\n MASSACRE! "
                  << (currentPlayer == Color::WHITE ? "White" : "Black")
                  << "'s QUEEN can move a second time (or skip)." << std::endl;
        std::cout << "Do you want to move your QUEEN again? (y/n): ";

        char choice;
        std::cin >> choice;
        std::cin.ignore();

        board.resetQueenDoubleMove(currentPlayer);

        if (choice == 'y' || choice == 'Y') {
            std::cout << "Enter your QUEEN's second move: ";
            std::string secondInput;
            std::getline(std::cin, secondInput);

            std::istringstream iss2(secondInput);
            std::string fromStr2, toStr2;
            iss2 >> fromStr2 >> toStr2;
            Position from2 = parsePosition(fromStr2);
            Position to2   = parsePosition(toStr2);

            if (from2.isValid() && to2.isValid()) {
                Piece* p2 = board.getPiece(from2.row, from2.col);
                if (p2 && p2->getColor() == currentPlayer &&
                    (p2->getType() == PieceType::QUEEN || p2->getType() == PieceType::ARMORED_QUEEN)) {
                    board.movePiece(Move(from2, to2));
                    board.incrementTurnNumber();
                } else {
                    std::cout << " You must move your QUEEN!" << std::endl;
                }
            }
        }
    }

    switchPlayerTurn();
    updateGameState();

    return true;
}


void GameEngine::switchPlayerTurn() {
    Color current = state.getCurrentTurn();
    state.setCurrentTurn((current == Color::WHITE) ? Color::BLACK : Color::WHITE);
}

GameStatus GameEngine::getCurrentState() const { 
    return state.getStatus(); 
}

Color GameEngine::getCurrentTurn() const { 
    return state.getCurrentTurn(); 
}

bool GameEngine::isGameOver() const { 
    GameStatus status = state.getStatus();
    return status == GameStatus::CHECKMATE || 
           status == GameStatus::STALEMATE || 
           status == GameStatus::DRAW;
}

std::vector<Position> GameEngine::getValidMoves(Position pos) {
    std::vector<Move> moves = board.getLegalMovesForPiece(pos);
    std::vector<Position> positions;
    
    for (const auto& move : moves) {
        positions.push_back(move.to);
    }
    
    return positions;
}

std::vector<Move> GameEngine::getAllValidMoves() {
    return board.getAllLegalMoves(state.getCurrentTurn());
}

void GameEngine::undoMove() {
    if (moveHistory.empty()) {
        std::cout << "No moves to undo!" << std::endl;
        return;
    }

    board.undoLastMove();
    moveHistory.pop_back();
    switchPlayerTurn();
    
    std::cout << "Last move undone." << std::endl;
}

void GameEngine::saveGame(const std::string& filename) {
    state.saveToFile(filename, board);
    std::cout << "Game saved: " << filename << std::endl;
}

bool GameEngine::loadGame(const std::string& filename) {
    if (state.loadFromFile(filename, board)) {
        updateGameState();
        std::cout << "Game loaded: " << filename << std::endl;
        return true;
    }
    std::cout << "Error loading file!" << std::endl;
    return false;
}

Board* GameEngine::getBoard() { 
    return &board; 
}

GameState* GameEngine::getGameState() { 
    return &state; 
}

void GameEngine::displayBoard() {
    board.printBoard();
    
    std::cout << getTurnInfo() << std::endl;
    
    const char* seasonNames[] = {"Spring", "Summer", "Autumn", "Winter"};
    std::cout << "Season: " << seasonNames[static_cast<int>(board.getCurrentSeason())] << std::endl;
}

std::string GameEngine::getTurnInfo() const {
    std::string info = (state.getCurrentTurn() == Color::WHITE) ? "White" : "Black";
    info += "'s turn";
    
    if (state.getStatus() == GameStatus::CHECK)     info += " (Check!)";
    if (state.getStatus() == GameStatus::CHECKMATE) info += " (Checkmate!)";
    
    if (state.getGameMode() == GameModeType::ENERGY) {
        info += " | Energy: " + std::to_string(state.getEnergy(state.getCurrentTurn()));
    }
    
    if (state.getGameMode() == GameModeType::MISSION) {
        info += " | Moves left: " + std::to_string(state.getMovesLeft());
    }
    
    info += " | Turn: " + std::to_string(state.getTurnCount());
    
    return info;
}

void GameEngine::updateGameState() {
    Color current = state.getCurrentTurn();
    
    if (board.isCheckmate(current)) {
        state.setStatus(GameStatus::CHECKMATE);
        Color winner = (current == Color::WHITE) ? Color::BLACK : Color::WHITE;
        std::cout << "\nCheckmate! "
                  << (winner == Color::WHITE ? "White" : "Black") 
                  << " wins!" << std::endl;
    }
    else if (board.isStalemate(current)) {
        state.setStatus(GameStatus::STALEMATE);
        std::cout << "\nStalemate! Draw." << std::endl;
    }
    else if (board.isInCheck(current)) {
        state.setStatus(GameStatus::CHECK);
        std::cout << "\nCheck!" << std::endl;
    }
    else {
        state.setStatus(GameStatus::PLAYING);
    }
}

Position GameEngine::parsePosition(const std::string& input) const {
    if (input.length() != 2) return Position();
    
    char colChar = input[0];
    char rowChar = input[1];
    
    if (colChar < 'a' || colChar > 'h') return Position();
    if (rowChar < '1' || rowChar > '8') return Position();
    
    int col = colChar - 'a';
    int row = 8 - (rowChar - '0');
    
    return Position(row, col);
}

std::string GameEngine::positionToString(Position pos) const {
    char colChar = 'a' + pos.col;
    char rowChar = '0' + (8 - pos.row); 
    return std::string(1, colChar) + rowChar;
}

MoveRecord GameEngine::createMoveRecord(const Move& move) {
    MoveRecord rec;
    rec.move = move;
    
    Piece* movedPiece = board.getPiece(move.to.row, move.to.col);
    if (movedPiece) {
        rec.movedPiece = movedPiece->getType();
    }
    
    rec.playerColor = state.getCurrentTurn();
    rec.notation = positionToString(move.from) + "-" + positionToString(move.to);
    rec.capturedPiece = PieceType::PAWN;
    rec.wasCastling = false;
    rec.wasEnPassant = false;
    rec.wasPromotion = board.canPromote(move.to);
    
    return rec;
}

int GameEngine::calculateMoveCost(PieceType type) const {
    switch (type) {
        case PieceType::PAWN:
        case PieceType::SPECIAL_PAWN:
            return 1;
        case PieceType::KNIGHT:
        case PieceType::BISHOP:
            return 3;
        case PieceType::ROOK:
            return 5;
        case PieceType::QUEEN:
        case PieceType::ARMORED_QUEEN:
            return 9;
        case PieceType::KING:
            return 0;
        case PieceType::JOKER:
            return 4;
        case PieceType::SPY:
            return 2;
        default:
            return 2;
    }
}