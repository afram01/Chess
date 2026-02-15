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
    moveHistory.clear();
    state.setCurrentTurn(Color::WHITE);
    state.setStatus(GameStatus::PLAYING);
    std::cout << "The game has begun. White's turn." << std::endl;
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
    
    if (!board.canPieceMoveInSeason(from)) {
        const char* seasonNames[] = {"Spring", "Summer", "Autumn", "Winter"};
        std::cout << "This piece cannot be used in the season." 
                  << seasonNames[static_cast<int>(board.getCurrentSeason())]
                  << " Move!" << std::endl;
        return false;
    }

    if (!board.movePiece(move)) {
        std::cout << "Illegal move!" << std::endl;
        return false;
    }

    state.incrementTurn();
    state.updateSeason();
    
    if (state.getGameMode() == GameModeType::ENERGY) {
        int cost = calculateMoveCost(p->getType());
        state.reduceEnergy(state.getCurrentTurn(), cost);
        
        if (state.getEnergy(state.getCurrentTurn()) <= 0) {
            std::cout << "Energy is out! Game over." << std::endl;
            state.setStatus(GameStatus::DRAW);
            return true;
        }
    }
    
    if (state.getGameMode() == GameModeType::MISSION) {
        state.decrementMovesLeft();
        
        if (state.getMovesLeft() <= 0) {
            std::cout << "Moves are over! Mission failed." << std::endl;
            state.setStatus(GameStatus::DRAW);
            return true;
        }
    }

    moveHistory.push_back(createMoveRecord(move));
    
    board.incrementTurnNumber();
    
    Color currentPlayer = state.getCurrentTurn();
    if (board.canQueenDoubleMove(currentPlayer)) {
        std::cout << "\nMassacre! Minister" 
                  << (currentPlayer == Color::WHITE ? "White" : "Black")
                  << " It can move twice." << std::endl;
        std::cout << "Do you want to move again? (y/n):";
        
        char choice;
        std::cin >> choice;
        
        if (choice == 'y' || choice == 'Y') {
            std::cout << "Enter the second move:";
            board.resetQueenDoubleMove(currentPlayer);
            return true; 
        } else {
            board.resetQueenDoubleMove(currentPlayer);
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
    return state.getStatus() != GameStatus::PLAYING; 
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
        std::cout << "There is no turning back!" << std::endl;
        return;
    }

    board.undoLastMove();
    
    moveHistory.pop_back();
    
    switchPlayerTurn();
    
    if (state.getTurnCount() > 0) {

    }
    
    std::cout << "The previous move was reversed." << std::endl;
}

void GameEngine::saveGame(const std::string& filename) {
    state.saveToFile(filename, board);
    std::cout << "Game saved: " << filename << std::endl;
}

bool GameEngine::loadGame(const std::string& filename) {
    if (state.loadFromFile(filename, board)) {
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
    std::cout << "Season:" << seasonNames[static_cast<int>(board.getCurrentSeason())] << std::endl;
}

std::string GameEngine::getTurnInfo() const {
    std::string info = (state.getCurrentTurn() == Color::WHITE) ? "White" : "Black";
    info += " - Turn";
    
    if (state.getStatus() == GameStatus::CHECK) info += "(Church!)";
    if (state.getStatus() == GameStatus::CHECKMATE) info += "(Checkmate!)";
    
    if (state.getGameMode() == GameModeType::ENERGY) {
        info += "| Energy:" + std::to_string(state.getEnergy(state.getCurrentTurn()));
    }
    
    if (state.getGameMode() == GameModeType::MISSION) {
        info += "| Remaining move:" + std::to_string(state.getMovesLeft());
    }
    
    info += " | Turn:" + std::to_string(state.getTurnCount());
    
    return info;
}

void GameEngine::updateGameState() {
    Color current = state.getCurrentTurn();
    
    if (board.isCheckmate(current)) {
        state.setStatus(GameStatus::CHECKMATE);
        Color winner = (current == Color::WHITE) ? Color::BLACK : Color::WHITE;
        std::cout << "\nGame over!" 
                  << (winner == Color::WHITE ? "White" : "Black") 
                  << "Won!" << std::endl;
    }
    else if (board.isStalemate(current)) {
        state.setStatus(GameStatus::STALEMATE);
        std::cout << "\nDraw (putt)!" << std::endl;
    }
    else if (board.isInCheck(current)) {
        state.setStatus(GameStatus::CHECK);
        std::cout << "\nKish!" << std::endl;
    }
    else {
        state.setStatus(GameStatus::PLAYING);
    }
}

Position GameEngine::parsePosition(const std::string& input) const {
    if (input.length() != 2) return Position();
    
    int col = input[0] - 'a';
    int row = 8 - (input[1] - '0');
    
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
