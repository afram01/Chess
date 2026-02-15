#include "GameState.hpp"
#include "Types.hpp"
#include <iostream>
#include <sstream>

GameState::GameState()
    : currentTurn(Color::WHITE),
      status(PLAYING),
      gameMode(STANDARD),
      currentSeason(SPRING),
      turnCount(0),
      seasonInterval(10),
      whiteEnergy(100),
      blackEnergy(100),
      movesLeft(0) {}

Color GameState::getCurrentTurn() const { return currentTurn; }
void GameState::setCurrentTurn(Color turn) { currentTurn = turn; }

GameStatus GameState::getStatus() const { return status; }
void GameState::setStatus(GameStatus s) { status = s; }

GameModeType GameState::getGameMode() const { return gameMode; }
void GameState::setGameMode(GameModeType m) { gameMode = m; }

Season GameState::getCurrentSeason() const { return currentSeason; }
void GameState::setCurrentSeason(Season s) { currentSeason = s; }

int GameState::getTurnCount() const { return turnCount; }
void GameState::incrementTurn() { turnCount++; }

int GameState::getEnergy(Color player) const {
    return (player == Color::WHITE) ? whiteEnergy : blackEnergy;
}
void GameState::setEnergy(Color player, int amount) {
    if (player == Color::WHITE) whiteEnergy = amount;
    else blackEnergy = amount;
}
void GameState::reduceEnergy(Color player, int amount) {
    if (player == Color::WHITE) whiteEnergy -= amount;
    else blackEnergy -= amount;
}

std::string GameState::getMissionDescription() const { return missionDescription; }
void GameState::setMissionDescription(const std::string& desc) { missionDescription = desc; }
int GameState::getMovesLeft() const { return movesLeft; }
void GameState::setMovesLeft(int moves) { movesLeft = moves; }
void GameState::decrementMovesLeft() { if (movesLeft > 0) movesLeft--; }

void GameState::updateSeason() {
    if (turnCount % seasonInterval == 0) {
        currentSeason = static_cast<Season>((static_cast<int>(currentSeason) + 1) % 4);
    }
}

void GameState::saveToFile(const std::string& filename, const Board& board) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << serialize(board);
        file.close();
    }
}

bool GameState::loadFromFile(const std::string& filename, Board& board) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        return deserialize(data, board);
    }
    return false;
}

std::string GameState::serialize(const Board& board) const {
    std::ostringstream ss;
    ss << static_cast<int>(currentTurn) << "," << static_cast<int>(status) << "," 
       << static_cast<int>(gameMode) << "," << static_cast<int>(currentSeason) << "," << turnCount << ","
       << whiteEnergy << "," << blackEnergy << "," << movesLeft << ";";

    ss << board.serialize();

    return ss.str();
}

bool GameState::deserialize(const std::string& data, Board& board) {
    std::istringstream ss(data);
    std::string token;
    std::getline(ss, token, ';');
    std::istringstream stateSs(token);
    int val;
    stateSs >> val; currentTurn = static_cast<Color>(val);
    stateSs >> val; status = static_cast<GameStatus>(val);
    stateSs >> val; gameMode = static_cast<GameModeType>(val);
    stateSs >> val; currentSeason = static_cast<Season>(val);
    stateSs >> turnCount >> whiteEnergy >> blackEnergy >> movesLeft;

    std::string boardData;
    std::getline(ss, boardData);
    board.deserialize(boardData);
    return true;
}