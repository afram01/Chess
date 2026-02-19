#include "GameEngine.hpp"
#include "Spy.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>

GameEngine::GameEngine() : state() { srand(static_cast<unsigned int>(time(nullptr))); }

void GameEngine::newGame()
{
    newGame(GameModeType::STANDARD);
}

void GameEngine::newGame(GameModeType mode)
{
    moveHistory.clear();
    state.resetTurnCount();
    state.setCurrentSeason(SPRING);

    if (mode == GameModeType::STANDARD)
    {
        board.setupBoard();
    }
    else if (mode == GameModeType::ENERGY)
    {
        board.setupBoardWithSpecialPieces();
    }

    state.setGameMode(mode);

    switch (mode)
    {
    case GameModeType::ENERGY:
        state.setEnergy(Color::WHITE, 100);
        state.setEnergy(Color::BLACK, 100);
        break;
    case GameModeType::MISSION:
    {
        int pick = rand() % 3;
        if (pick == 0)
        {
            state.setMissionType(CAPTURE_QUEEN);
            state.setMissionDescription("Capture the enemy queen in 10 moves!");
            state.setMovesLeft(10);
            board.setupBoard();
        }
        else if (pick == 1)
        {
            state.setMissionType(REACH_SQUARE);
            state.setTargetPosition({4, 3});
            state.setMissionDescription("Move your King to d4 in 15 moves!");
            state.setMovesLeft(15);
            board.setupBoard();
        }
        else
        {
            state.setMissionType(ESCAPE_CHECK);
            state.setMissionDescription("Escape check within 5 moves!");
            state.setMovesLeft(5);
            board.setupCheckScenario();
        }
        break;
    }
    default:
        break;
    }

    initializeGame();
}

void GameEngine::initializeGame()
{
    state.setCurrentTurn(Color::WHITE);
    state.setStatus(GameStatus::PLAYING);
    updateGameState();
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

    Color currentPlayer = state.getCurrentTurn();
    if (board.isInCheck(currentPlayer))
    {
        if (!board.isMoveLegal(move, currentPlayer))
        {
            std::cout << "Your king is in CHECK! You must escape check." << std::endl;
            return false;
        }
    }

    PieceType movedPieceType = p->getType();

    if (!board.canPieceMoveInSeason(from))
    {
        const char *seasonNames[] = {"Spring", "Summer", "Autumn", "Winter"};
        std::cout << "This piece cannot move this season ("
                  << seasonNames[static_cast<int>(board.getCurrentSeason())]
                  << ")." << std::endl;
        return false;
    }
    Piece* targetPiece = board.getPiece(to.row, to.col) ;
    bool isCapture     = (targetPiece != nullptr) ;
    PieceType capturedType = isCapture ? targetPiece->getType() : PieceType::PAWN;

    int energyWhiteBefore = state.getEnergy(Color::WHITE) ;
    int energyBlackBefore = state.getEnergy(Color::BLACK) ;

    board.incrementTurnNumber();

    if (!board.movePiece(move))
    {
        board.decrementTurnNumber();
        std::cout << "Illegal move!" << std::endl;
        return false;
    }

    if (state.getGameMode() == GameModeType::ENERGY)
    {
        int cost = calculateMoveCost(movedPieceType) ;


        state.reduceEnergy(currentPlayer, cost) ;

        if (isCapture)
        {
            int refund = calculateMoveCost(capturedType) ;
            int newEnergy = state.getEnergy(currentPlayer) + refund ;
            if (newEnergy > 100) newEnergy = 100 ;
            state.setEnergy(currentPlayer, newEnergy) ;
        }


        if (state.getEnergy(currentPlayer) <= 0)
        {
            Color winner = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE ;
            std::cout << "Energy exhausted !!! "
                      << (currentPlayer == Color::WHITE ? "White" : "Black")
                      << " has run out of energy!" << std::endl ;
            std::cout << "\n" << (winner == Color::WHITE ? "WHITE" : "BLACK")
                      << " WINS!!!" << std::endl ;
            state.setStatus(GameStatus::DRAW) ;
            return true ;
        }
    }

    if (state.getGameMode() == GameModeType::MISSION)
    {
        state.decrementMovesLeft();

        bool missionComplete = false;
        std::string winMessage;

        if (state.getMissionType() == CAPTURE_QUEEN)
        {
            Color enemy = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
            bool enemyQueenExists = false;
            for (int r = 0; r < 8; r++)
            {
                for (int c = 0; c < 8; c++)
                {
                    Piece *pc = board.getPiece(r, c);
                    if (pc && pc->getColor() == enemy &&
                        (pc->getType() == PieceType::QUEEN || pc->getType() == PieceType::ARMORED_QUEEN))
                    {
                        enemyQueenExists = true;
                        break;
                    }
                }
            }
            if (!enemyQueenExists)
            {
                missionComplete = true;
                winMessage = (currentPlayer == Color::WHITE ? "White" : "Black");
                winMessage += " captured the enemy Queen! Mission complete!";
            }
        }
        else if (state.getMissionType() == REACH_SQUARE)
        {
            Position target = state.getTargetPosition();
            Piece *atTarget = board.getPiece(target.row, target.col);
            if (atTarget && atTarget->getType() == PieceType::KING &&
                atTarget->getColor() == Color::WHITE)
            {
                missionComplete = true;
                char col = 'a' + target.col;
                char row = '0' + (8 - target.row);
                winMessage = "White King reached ";
                winMessage += col;
                winMessage += row;
                winMessage += "! Mission complete!";
            }
        }
        else if (state.getMissionType() == ESCAPE_CHECK)
        {
            if (!board.isInCheck(currentPlayer))
            {
                missionComplete = true;
                winMessage = (currentPlayer == Color::WHITE ? "White" : "Black");
                winMessage += " escaped check! Mission complete!";
            }
        }

        if (missionComplete)
        {
            std::cout << "\n" << winMessage << std::endl;
            state.setStatus(GameStatus::MISSION_WIN);
            return true;
        }

        if (state.getMovesLeft() <= 0)
        {
            std::cout << "\nMoves exhausted! Mission failed." << std::endl;
            state.setStatus(GameStatus::MISSION_FAIL);
            return true;
        }
    }

    MoveRecord rec = createMoveRecord(move);

    rec.prevTurnCount    = state.getTurnCount() ;
    rec.prevWhiteEnergy  = energyWhiteBefore ;  
    rec.prevBlackEnergy  = energyBlackBefore ; 
    rec.prevMovesLeft    = state.getMovesLeft() ;
    rec.prevSeason       = state.getCurrentSeason() ;
    rec.prevCanMoveAgain = state.getCanMoveAgain() ;
    rec.prevStatus       = state.getStatus() ;
    rec.prevCurrentTurn  = state.getCurrentTurn() ;

    moveHistory.push_back(rec) ;
    state.incrementTurn() ;
    state.updateSeason() ;
    board.setCurrentSeason(state.getCurrentSeason()) ;

    if (state.getGameMode() == GameModeType::STANDARD &&
        board.canQueenDoubleMove(currentPlayer))
    {
        std::cout << "\nMASSACRE! "
                  << (currentPlayer == Color::WHITE ? "White" : "Black")
                  << "'s QUEEN can move a second time (or skip)." << std::endl ;
        std::cout << "Do you want to move your QUEEN again? (y/n): " ;

        char choice ;
        std::cin >> choice ;
        std::cin.ignore() ;

        board.resetQueenDoubleMove(currentPlayer) ;

        if (choice == 'y' || choice == 'Y')
        {
            std::cout << "Enter your QUEEN's second move: " ;
            std::string secondInput ;
            std::getline(std::cin, secondInput) ;

            std::istringstream iss2(secondInput) ;
            std::string fromStr2, toStr2 ;
            iss2 >> fromStr2 >> toStr2 ;
            Position from2 = parsePosition(fromStr2) ;
            Position to2   = parsePosition(toStr2) ;

            if (from2.isValid() && to2.isValid())
            {
                Piece *p2 = board.getPiece(from2.row, from2.col) ;
                if (p2 && p2->getColor() == currentPlayer &&
                    (p2->getType() == PieceType::QUEEN || p2->getType() == PieceType::ARMORED_QUEEN))
                {
                    board.movePiece(Move(from2, to2)) ;
                    board.incrementTurnNumber() ;
                }
                else
                {
                    std::cout << "Invalid piece for second move. Must be your Queen." << std::endl ;
                }
            }
        }
    }

    if (board.canQueenDoubleMove(currentPlayer))
    {
        Position queenPos = {-1, -1};
        for (int r = 0; r < 8 && !queenPos.isValid(); r++)
        {
            for (int c = 0; c < 8 && !queenPos.isValid(); c++)
            {
                Piece *pc = board.getPiece(r, c);
                if (pc && pc->getColor() == currentPlayer &&
                    (pc->getType() == PieceType::QUEEN || pc->getType() == PieceType::ARMORED_QUEEN))
                {
                    queenPos = {r, c};
                }
            }
        }

        board.resetQueenDoubleMove(currentPlayer);

        if (!queenPos.isValid())
        {
        }
        else
        {
            std::vector<Move> queenMoves = board.getLegalMovesForPiece(queenPos);

            if (queenMoves.empty())
            {
                std::cout << "\nMASSACRE! You captured the enemy Queen!" << std::endl;
                std::cout << "Your Queen has no available moves  bonus turn skipped." << std::endl;
            }
            else
            {
                std::cout << "\nMASSACRE! "
                          << (currentPlayer == Color::WHITE ? "White" : "Black")
                          << "'s QUEEN can move a second time!" << std::endl;
                std::cout << "Do you want to move your QUEEN again? (y/n): ";

                char choice;
                std::cin >> choice;
                std::cin.ignore();

                if (choice == 'y' || choice == 'Y')
                {
                    bool validQueenMove = false;

                    while (!validQueenMove)
                    {
                        std::cout << "Enter your QUEEN's second move (e.g. d1 d5): ";
                        std::string secondInput;
                        std::getline(std::cin, secondInput);

                        std::istringstream iss2(secondInput);
                        std::string fromStr2, toStr2;
                        iss2 >> fromStr2 >> toStr2;
                        Position from2 = parsePosition(fromStr2);
                        Position to2   = parsePosition(toStr2);

                        if (!from2.isValid() || !to2.isValid())
                        {
                            std::cout << "Invalid position format! Use format like 'd1 d5'." << std::endl;
                            std::cout << "   Try again or type 'skip' to cancel: ";
                            std::string skipCheck;
                            std::getline(std::cin, skipCheck);
                            if (skipCheck == "skip" || skipCheck == "s")
                                break;
                            continue;
                        }

                        Piece *p2 = board.getPiece(from2.row, from2.col);

                        if (!p2 || p2->getColor() != currentPlayer)
                        {
                            std::cout << "That is not your piece!" << std::endl;
                            std::cout << "You MUST move your QUEEN (";
                            std::cout << (char)('a' + queenPos.col) << (char)('0' + (8 - queenPos.row));
                            std::cout << "). Try again or type 'skip' to cancel: ";
                            std::string skipCheck;
                            std::getline(std::cin, skipCheck);
                            if (skipCheck == "skip" || skipCheck == "s")
                                break;
                            continue;
                        }

                        if (p2->getType() != PieceType::QUEEN && p2->getType() != PieceType::ARMORED_QUEEN)
                        {
                            std::cout << "You can only move your QUEEN in this bonus turn!" << std::endl;
                            std::cout << "Your Queen is at ";
                            std::cout << (char)('a' + queenPos.col) << (char)('0' + (8 - queenPos.row));
                            std::cout << ". Try again or type 'skip' to cancel: ";
                            std::string skipCheck;
                            std::getline(std::cin, skipCheck);
                            if (skipCheck == "skip" || skipCheck == "s")
                                break;
                            continue;
                        }

                        Move queenMove(from2, to2);
                        if (!board.isMoveLegal(queenMove, currentPlayer))
                        {
                            std::cout << "Illegal move for the Queen!" << std::endl;
                            std::cout << "Try again or type 'skip' to cancel: ";
                            std::string skipCheck;
                            std::getline(std::cin, skipCheck);
                            if (skipCheck == "skip" || skipCheck == "s")
                                break;
                            continue;
                        }

                        board.movePiece(queenMove);
                        board.incrementTurnNumber();
                        std::cout << "Queen moved successfully!" << std::endl;
                        validQueenMove = true;
                    }
                }
                else
                {
                    std::cout << "Bonus queen move skipped." << std::endl;
                }
            }
        }
    }

    switchPlayerTurn();
    updateGameState();

    return true;
}

void GameEngine::switchPlayerTurn()
{
    Color current = state.getCurrentTurn();
    state.setCurrentTurn((current == Color::WHITE) ? Color::BLACK : Color::WHITE);
}

GameStatus GameEngine::getCurrentState() const
{
    return state.getStatus();
}

Color GameEngine::getCurrentTurn() const
{
    return state.getCurrentTurn();
}

bool GameEngine::isGameOver() const
{
    GameStatus status = state.getStatus();

    if (status == GameStatus::CHECKMATE ||
        status == GameStatus::STALEMATE ||
        status == GameStatus::DRAW ||
        status == GameStatus::MISSION_WIN ||
        status == GameStatus::MISSION_FAIL)
        return true;

    if (state.getGameMode() == GameModeType::MISSION && state.getMovesLeft() <= 0)
        return true;

    return false;
}

std::vector<Position> GameEngine::getValidMoves(Position pos)
{
    std::vector<Move> moves = board.getLegalMovesForPiece(pos);
    std::vector<Position> positions;

    for (const auto &move : moves)
    {
        positions.push_back(move.to);
    }

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
        std::cout << "No moves to undo!" << std::endl;
        return;
    }

    MoveRecord last = moveHistory.back();
    moveHistory.pop_back();

    board.undoLastMove();

    state.setCurrentTurn(last.prevCurrentTurn);
    state.setStatus(last.prevStatus);
    state.setCurrentSeason(last.prevSeason);
    state.setCanMoveAgain(last.prevCanMoveAgain);
    state.setEnergy(Color::WHITE, last.prevWhiteEnergy);  
    state.setEnergy(Color::BLACK, last.prevBlackEnergy); 
    state.setMovesLeft(last.prevMovesLeft);
    state.setTurnCount(last.prevTurnCount);
    board.setCurrentSeason(last.prevSeason);

    updateGameState();
    std::cout << "Last move undone." << std::endl;
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
        updateGameState();
        std::cout << "Game loaded: " << filename << std::endl;
        return true;
    }
    std::cout << "Error loading file!" << std::endl;
    return false;
}

Board *GameEngine::getBoard()
{
    return &board;
}

GameState *GameEngine::getGameState()
{
    return &state;
}

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
    {
        info += " | Energy: " + std::to_string(state.getEnergy(state.getCurrentTurn()));
    }

    if (state.getGameMode() == GameModeType::MISSION)
    {
        info += " | Moves left: " + std::to_string(state.getMovesLeft());
    }

    info += " | Turn: " + std::to_string(state.getTurnCount());

    return info;
}

void GameEngine::updateGameState()
{
    Color current = state.getCurrentTurn();

    if (state.getGameMode() == GameModeType::MISSION)
    {
        Color opponent = (current == Color::WHITE) ? Color::BLACK : Color::WHITE;
        bool opponentQueenExists = false;

        for (int r = 0; r < 8; ++r)
        {
            for (int c = 0; c < 8; ++c)
            {
                Piece *p = board.getPiece(r, c);
                if (p && p->getColor() == opponent)
                {
                    if (p->getType() == PieceType::QUEEN ||
                        p->getType() == PieceType::ARMORED_QUEEN)
                    {
                        opponentQueenExists = true;
                        break;
                    }
                }
            }
            if (opponentQueenExists)
                break;
        }

        if (!opponentQueenExists)
        {
            state.setStatus(GameStatus::CHECKMATE);
            std::cout << "\nMission Accomplished! "
                      << (current == Color::WHITE ? "White" : "Black")
                      << " captured the enemy queen and WINS!" << std::endl;
            return;
        }
    }

    if (board.isCheckmate(current))
    {
        state.setStatus(GameStatus::CHECKMATE);
        Color winner = (current == Color::WHITE) ? Color::BLACK : Color::WHITE;
        std::cout << "\nCHECKMATE!" << std::endl;
        std::cout << (winner == Color::WHITE ? "White" : "Black")
                  << " wins!" << std::endl;

        std::cout << (current == Color::WHITE ? "White" : "Black")
                  << "'s king has been checkmated!" << std::endl;

        if (state.getGameMode() == GameModeType::MISSION)
        {
            std::cout << "\nMISSION COMPLETED!!!" << std::endl;
        }
    }
    else if (board.isStalemate(current))
    {
        state.setStatus(GameStatus::STALEMATE);
        std::cout << "\nStalemate! Draw." << std::endl;
    }
    else if (board.isInCheck(current))
    {
        state.setStatus(GameStatus::CHECK);
        std::cout << "\nCHECK!" << std::endl;

        std::cout << (current == Color::WHITE ? "White" : "Black")
                  << "'s king is under attack!" << std::endl;
        std::cout << "\nYou MUST escape check! Choose one:" << std::endl;
        std::cout << "  1. Move your king to a safe square" << std::endl;
        std::cout << "  2. Block the attack with another piece" << std::endl;
        std::cout << "  3. Capture the attacking piece" << std::endl;
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
    {
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

std::vector<Position> GameEngine::getRevealableSpies() const
{
    std::vector<Position> result;
    Color currentPlayer = state.getCurrentTurn();

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Piece* piece = board.getPiece(r, c);
            if (!piece) continue;

            if (piece->getType() == PieceType::SPY)
            {
                Spy* spy = dynamic_cast<Spy*>(piece);
                if (spy && spy->canBeRevealedBy(currentPlayer))
                {
                    result.push_back(Position(r, c));
                }
            }
        }
    }
    return result;
}

bool GameEngine::revealSpy(const std::string& posInput)
{
    Position pos = parsePosition(posInput);
    if (!pos.isValid())
    {
        std::cout << "Invalid position: " << posInput << std::endl;
        return false;
    }

    Piece* piece = board.getPiece(pos.row, pos.col);
    if (!piece)
    {
        std::cout << "No piece at " << posInput << "!" << std::endl;
        return false;
    }

    if (piece->getType() != PieceType::SPY)
    {
        std::cout << "The piece at " << posInput << " is not a spy!" << std::endl;
        return false;
    }

    Spy* spy = dynamic_cast<Spy*>(piece);
    if (!spy)
    {
        std::cout << "Error accessing spy piece!" << std::endl;
        return false;
    }

    Color currentPlayer = state.getCurrentTurn();

    bool success = spy->requestReveal(currentPlayer);

    if (success)
    {
        std::cout << "\nSpy revealed at " << posInput << "!" << std::endl;
        std::cout << "The spy now fights for " 
                  << (currentPlayer == Color::WHITE ? "White" : "Black") << "!" << std::endl;
        std::cout << "This action uses your turn." << std::endl;

        Move revealMove(pos, pos);
        MoveRecord rec = createMoveRecord(revealMove);
        rec.prevTurnCount    = state.getTurnCount();
        rec.prevWhiteEnergy  = state.getEnergy(Color::WHITE);
        rec.prevBlackEnergy  = state.getEnergy(Color::BLACK);
        rec.prevMovesLeft    = state.getMovesLeft();
        rec.prevSeason       = state.getCurrentSeason();
        rec.prevCanMoveAgain = state.getCanMoveAgain();
        rec.prevStatus       = state.getStatus();
        rec.prevCurrentTurn  = state.getCurrentTurn();
        moveHistory.push_back(rec);

        state.incrementTurn();
        state.updateSeason();
        board.setCurrentSeason(state.getCurrentSeason());
        board.incrementTurnNumber();

        if (state.getGameMode() == GameModeType::MISSION)
        {
            state.decrementMovesLeft();
            if (state.getMovesLeft() <= 0)
            {
                std::cout << "\nMoves exhausted! Mission failed." << std::endl;
                state.setStatus(GameStatus::MISSION_FAIL);
                return true;
            }
        }

        switchPlayerTurn();
        updateGameState();
    }

    return success;
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