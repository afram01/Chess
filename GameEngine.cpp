#include "GameEngine.hpp"
#include <iostream>
#include <sstream>

GameEngine::GameEngine() : state() {}

void GameEngine::newGame() { newGame(GameModeType::STANDARD); }

void GameEngine::newGame(GameModeType mode)
{
    if (mode == GameModeType::STANDARD)
        board.setupBoard();
    else
        board.setupBoardWithSpecialPieces();

    state.setGameMode(mode);
    switch (mode)
    {
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

void GameEngine::initializeGame()
{
    moveHistory.clear();
    state.setCurrentTurn(Color::WHITE);
    state.setStatus(GameStatus::PLAYING);
    std::cout << "The game has begun. White's turn." << std::endl;
}

bool GameEngine::processMove(const std::string &input)
{
    std::istringstream iss(input);
    std::string fromStr, toStr;
    iss >> fromStr >> toStr;

    Position from = parsePosition(fromStr);
    Position to = parsePosition(toStr);

    if (!from.isValid() || !to.isValid())
    {
        std::cout << "Invalid position!" << std::endl;
        return false;
    }

    Move move(from, to);

    Piece *p = board.getPiece(from.row, from.col);
    if (!p || p->getColor() != state.getCurrentTurn())
    {
        std::cout << "Invalid piece or wrong turn!" << std::endl;
        return false;
    }

    PieceType movedPieceType = p->getType();
    Color currentPlayer = state.getCurrentTurn();

    if (!board.canPieceMoveInSeason(from))
    {
        const char *seasonNames[] = {"Spring", "Summer", "Autumn", "Winter"};
        std::cout << "This piece cannot move this season ("
                  << seasonNames[static_cast<int>(board.getCurrentSeason())] << ").\n";
        return false;
    }

    bool targetWasQueen = false;
    Piece *targetPiece = board.getPiece(to.row, to.col);
    if (targetPiece && targetPiece->getColor() != currentPlayer)
    {
        if (targetPiece->getType() == PieceType::QUEEN ||
            targetPiece->getType() == PieceType::ARMORED_QUEEN)
            targetWasQueen = true;
    }

    bool moveOk = board.movePiece(move);
    if (!moveOk)
    {

        if (targetWasQueen)
        {
            std::cout << "The Armored Queen absorbed the hit! Attack it again to destroy it.\n";
        }
        else
        {
            std::cout << "Illegal move!" << std::endl;
        }
        return false;
    }

    board.incrementTurnNumber();

    if (board.canQueenDoubleMove(currentPlayer))
    {
        board.resetQueenDoubleMove(currentPlayer);

        std::cout << "\n=== MASSACRE! ===" << std::endl;
        std::cout << "You killed the opponent's queen! You can make ONE more move.\n";
        std::cout << "Do you want to move again? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 'y' || choice == 'Y')
        {
            std::cout << "Enter second move: ";
            std::string secondInput;
            std::getline(std::cin, secondInput);

            std::istringstream iss2(secondInput);
            std::string fs2, ts2;
            iss2 >> fs2 >> ts2;
            Position from2 = parsePosition(fs2);
            Position to2 = parsePosition(ts2);

            if (from2.isValid() && to2.isValid())
            {
                Piece *p2 = board.getPiece(from2.row, from2.col);
                if (p2 && p2->getColor() == currentPlayer)
                {
                    if (board.movePiece(Move(from2, to2)))
                    {
                        board.incrementTurnNumber();
                        std::cout << "Second move executed!" << std::endl;
                    }
                    else
                    {
                        std::cout << "Invalid second move — skipped." << std::endl;
                    }
                }
            }
        }
    }


    if (state.getGameMode() == GameModeType::ENERGY)
    {
        int cost = calculateMoveCost(movedPieceType);
        state.reduceEnergy(currentPlayer, cost);
        if (state.getEnergy(currentPlayer) <= 0)
        {
            std::cout << "Energy exhausted! "
                      << (currentPlayer == Color::WHITE ? "White" : "Black")
                      << " loses.\n";
            state.setStatus(GameStatus::DRAW);
            return true;
        }
    }

    if (state.getGameMode() == GameModeType::MISSION)
    {
        state.decrementMovesLeft();
        if (state.getMovesLeft() <= 0)
        {
            std::cout << "Moves exhausted! Mission failed.\n";
            state.setStatus(GameStatus::DRAW);
            return true;
        }
    }

    moveHistory.push_back(createMoveRecord(move));
    state.incrementTurn();
    state.updateSeason();
    switchPlayerTurn();
    updateGameState();
    return true;
}

void GameEngine::switchPlayerTurn()
{
    Color cur = state.getCurrentTurn();
    state.setCurrentTurn((cur == Color::WHITE) ? Color::BLACK : Color::WHITE);
}

GameStatus GameEngine::getCurrentState() const { return state.getStatus(); }
Color GameEngine::getCurrentTurn() const { return state.getCurrentTurn(); }
bool GameEngine::isGameOver() const { return state.getStatus() != GameStatus::PLAYING; }

std::vector<Position> GameEngine::getValidMoves(Position pos)
{
    std::vector<Move> moves = board.getLegalMovesForPiece(pos);
    std::vector<Position> positions;
    for (const auto &m : moves)
        positions.push_back(m.to);
    return positions;
}

std::vector<Move> GameEngine::getAllValidMoves()
{
    return board.getAllLegalMoves(state.getCurrentTurn());
}

void GameEngine::undoMove()
{
    if (moveHistory.empty())
    {
        std::cout << "No moves to undo!\n";
        return;
    }
    board.undoLastMove();
    moveHistory.pop_back();
    switchPlayerTurn();
    std::cout << "Last move undone.\n";
}

void GameEngine::saveGame(const std::string &filename)
{
    state.saveToFile(filename, board);
    std::cout << "Game saved: " << filename << std::endl;
}

bool GameEngine::loadGame(const std::string &filename)
{
    if (state.loadFromFile(filename, board))
    {
        std::cout << "Game loaded: " << filename << std::endl;
        return true;
    }
    std::cout << "Error loading file!\n";
    return false;
}

Board *GameEngine::getBoard() { return &board; }
GameState *GameEngine::getGameState() { return &state; }

void GameEngine::displayBoard()
{
    board.printBoard();
    std::cout << getTurnInfo() << std::endl;
    const char *seasonNames[] = {"Spring", "Summer", "Autumn", "Winter"};
    std::cout << "Season: " << seasonNames[static_cast<int>(board.getCurrentSeason())] << std::endl;
}

std::string GameEngine::getTurnInfo() const
{
    std::string info = (state.getCurrentTurn() == Color::WHITE) ? "White" : "Black";
    info += "'s turn";
    if (state.getStatus() == GameStatus::CHECK)
        info += " (Check!)";
    if (state.getStatus() == GameStatus::CHECKMATE)
        info += " (Checkmate!)";
    if (state.getGameMode() == GameModeType::ENERGY)
        info += " | Energy: " + std::to_string(state.getEnergy(state.getCurrentTurn()));
    if (state.getGameMode() == GameModeType::MISSION)
        info += " | Moves left: " + std::to_string(state.getMovesLeft());
    info += " | Turn: " + std::to_string(state.getTurnCount());
    return info;
}

void GameEngine::updateGameState()
{
    Color current = state.getCurrentTurn();

    if (board.isCheckmate(current))
    {
        state.setStatus(GameStatus::CHECKMATE);
        Color winner = (current == Color::WHITE) ? Color::BLACK : Color::WHITE;
        std::cout << "\nCheckmate! "
                  << (winner == Color::WHITE ? "White" : "Black") << " wins!\n";
    }
    else if (board.isStalemate(current))
    {
        state.setStatus(GameStatus::STALEMATE);
        std::cout << "\nStalemate! Draw.\n";
    }
    else if (board.isInCheck(current))
    {
        state.setStatus(GameStatus::CHECK);
        std::cout << "\nCheck!\n";
    }
    else
    {
        state.setStatus(GameStatus::PLAYING);
    }
}

Position GameEngine::parsePosition(const std::string &input) const
{
    if (input.length() != 2)
        return Position();
    char colChar = input[0];
    char rowChar = input[1];
    if (colChar < 'a' || colChar > 'h')
        return Position();
    if (rowChar < '1' || rowChar > '8')
        return Position();
    int col = colChar - 'a';
    int row = 8 - (rowChar - '0');
    return Position(row, col);
}

std::string GameEngine::positionToString(Position pos) const
{
    char colChar = 'a' + pos.col;
    char rowChar = '0' + (8 - pos.row);
    return std::string(1, colChar) + rowChar;
}

MoveRecord GameEngine::createMoveRecord(const Move &move)
{
    MoveRecord rec;
    rec.move = move;
    Piece *movedPiece = board.getPiece(move.to.row, move.to.col);
    if (movedPiece)
        rec.movedPiece = movedPiece->getType();
    rec.playerColor = state.getCurrentTurn();
    rec.notation = positionToString(move.from) + "-" + positionToString(move.to);
    rec.capturedPiece = PieceType::PAWN;
    rec.wasCastling = false;
    rec.wasEnPassant = false;
    rec.wasPromotion = board.canPromote(move.to);
    return rec;
}

int GameEngine::calculateMoveCost(PieceType type) const
{
    switch (type)
    {
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
