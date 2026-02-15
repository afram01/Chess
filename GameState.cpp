#include "GameState.hpp"
#include "Types.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

GameState::GameState()
    : currentTurn(Color::WHITE),
      status(PLAYING),
      gameMode(STANDARD),
      currentSeason(SPRING),
      turnCount(0),
      seasonInterval(10),
      whiteEnergy(100),
      blackEnergy(100),
      movesLeft(0),
      canMoveAgain(false) {}

Color        GameState::getCurrentTurn()  const { return currentTurn; }
void         GameState::setCurrentTurn(Color t)  { currentTurn = t; }

GameStatus   GameState::getStatus()       const { return status; }
void         GameState::setStatus(GameStatus s)  { status = s; }

GameModeType GameState::getGameMode()     const { return gameMode; }
void         GameState::setGameMode(GameModeType m) { gameMode = m; }

Season       GameState::getCurrentSeason() const { return currentSeason; }
void         GameState::setCurrentSeason(Season s) { currentSeason = s; }

int  GameState::getTurnCount()  const { return turnCount; }
void GameState::incrementTurn()       { turnCount++; }

int  GameState::getEnergy(Color player) const {
    return (player == Color::WHITE) ? whiteEnergy : blackEnergy;
}
void GameState::setEnergy(Color player, int amount) {
    if (player == Color::WHITE) whiteEnergy = amount;
    else                        blackEnergy = amount;
}
void GameState::reduceEnergy(Color player, int amount) {
    if (player == Color::WHITE) whiteEnergy -= amount;
    else                        blackEnergy -= amount;
}

std::string GameState::getMissionDescription() const { return missionDescription; }
void        GameState::setMissionDescription(const std::string& d) { missionDescription = d; }
int  GameState::getMovesLeft()   const { return movesLeft; }
void GameState::setMovesLeft(int m)    { movesLeft = m; }
void GameState::decrementMovesLeft()   { if (movesLeft > 0) movesLeft--; }

bool GameState::getCanMoveAgain() const { return canMoveAgain; }
void GameState::setCanMoveAgain(bool v)  { canMoveAgain = v; }

void GameState::updateSeason() {
    if (turnCount % seasonInterval == 0 && turnCount > 0)
        currentSeason = static_cast<Season>((static_cast<int>(currentSeason) + 1) % 4);
}

void GameState::saveToFile(const std::string& filename, const Board& board) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << serialize(board);
        file.close();
    } else {
        std::cerr << "GameState::saveToFile: cannot open '" << filename << "'\n";
    }
}

bool GameState::loadFromFile(const std::string& filename, Board& board) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "GameState::loadFromFile: cannot open '" << filename << "'\n";
        return false;
    }
    std::string data((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
    file.close();
    return deserialize(data, board);
}

std::string GameState::serialize(const Board& board) const {
    std::ostringstream ss;

    ss << static_cast<int>(currentTurn)   << ","
       << static_cast<int>(status)        << ","
       << static_cast<int>(gameMode)      << ","
       << static_cast<int>(currentSeason) << ","
       << turnCount                       << ","
       << whiteEnergy                     << ","
       << blackEnergy                     << ","
       << movesLeft                       << ","
       << (canMoveAgain ? 1 : 0)          << ","
       << missionDescription              << "\n";

    ss << board.serialize();

    return ss.str();
}


bool GameState::deserialize(const std::string& data, Board& board) {
    auto nl = data.find('\n');
    if (nl == std::string::npos) {
        std::cerr << "GameState::deserialize: malformed save (no newline)\n";
        return false;
    }

    std::string stateLine = data.substr(0, nl);
    std::string boardData = data.substr(nl + 1);

    try {
        std::istringstream ss(stateLine);
        std::string field;

        auto nextField = [&]() -> std::string {
            std::getline(ss, field, ',');
            return field;
        };

        currentTurn   = static_cast<Color>(std::stoi(nextField()));
        status        = static_cast<GameStatus>(std::stoi(nextField()));
        gameMode      = static_cast<GameModeType>(std::stoi(nextField()));
        currentSeason = static_cast<Season>(std::stoi(nextField()));
        turnCount     = std::stoi(nextField());
        whiteEnergy   = std::stoi(nextField());
        blackEnergy   = std::stoi(nextField());
        movesLeft     = std::stoi(nextField());
        canMoveAgain  = (std::stoi(nextField()) == 1);
        std::getline(ss, missionDescription);
        if (!missionDescription.empty() && missionDescription[0] == ',')
            missionDescription = missionDescription.substr(1);

    } catch (const std::exception& e) {
        std::cerr << "GameState::deserialize: error parsing state — " << e.what() << "\n";
        return false;
    }

    board.deserialize(boardData);
    return true;
}
