#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include "Types.hpp"
#include "Board.hpp"
#include "GameState.hpp"
#include "Move.hpp"
#include <string>
#include <vector>

class GameEngine
{
private:
    Board board;
    GameState state;
    std::vector<MoveRecord> moveHistory;

    Position parsePosition(const std::string &input) const;
    std::string positionToString(Position pos) const;
    MoveRecord createMoveRecord(const Move &move);

    int calculateMoveCost(PieceType type) const;

public:
    GameEngine();

    void newGame();
    void newGame(GameModeType mode);

    bool processMove(const std::string &input);
    void switchPlayerTurn();

    GameStatus getCurrentState() const;
    Color getCurrentTurn() const;

    bool isGameOver() const;

    std::vector<Position> getValidMoves(Position pos);
    std::vector<Move> getAllValidMoves();

    void undoMove();
    bool revealSpy(const std::string &posInput);

    std::vector<Position> getRevealableSpies() const;

    void saveGame(const std::string &filename);
    bool loadGame(const std::string &filename);

    Board *getBoard();
    GameState *getGameState();

    void displayBoard();

    std::string getTurnInfo() const;

    void initializeGame();
    void updateGameState();

    bool transformJoker(const std::string &posStr, PieceType targetType);
};

#endif