#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Types.hpp"
#include "Board.hpp"
#include <string>
#include <vector>
#include <fstream>

class GameState {
private:
    Color currentTurn;
    GameStatus status;
    GameModeType gameMode;
    Season currentSeason;
    int turnCount;
    int seasonInterval; 

    int whiteEnergy;
    int blackEnergy;

    std::string missionDescription;
    int movesLeft;

    bool canMoveAgain; 

public:
    GameState();

    Color getCurrentTurn() const;
    void setCurrentTurn(Color turn);

    GameStatus getStatus() const;
    void setStatus(GameStatus s);

    GameModeType getGameMode() const;
    void setGameMode(GameModeType m);

    Season getCurrentSeason() const;
    void setCurrentSeason(Season s);

    int getTurnCount() const;
    void incrementTurn();

    int getEnergy(Color player) const;
    void setEnergy(Color player, int amount);
    void reduceEnergy(Color player, int amount);

    std::string getMissionDescription() const;
    void setMissionDescription(const std::string& desc);
    int getMovesLeft() const;
    void setMovesLeft(int moves);
    void decrementMovesLeft();

    bool getCanMoveAgain() const;
    void setCanMoveAgain(bool value);

    void updateSeason();

    void saveToFile(const std::string& filename, const Board& board) const;
    bool loadFromFile(const std::string& filename, Board& board);

    std::string serialize(const Board& board) const;
    bool deserialize(const std::string& data, Board& board);
};

#endif 