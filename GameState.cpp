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
      movesLeft(0),
      canMoveAgain(false) {}

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

bool GameState::getCanMoveAgain() const { return canMoveAgain; }
void GameState::setCanMoveAgain(bool value) { canMoveAgain = value; }

void GameState::updateSeason() {
    if (turnCount % seasonInterval == 0 && turnCount > 0) {
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
    ss << static_cast<int>(currentTurn) << ","
       << static_cast<int>(status) << ","
       << static_cast<int>(gameMode) << ","
       << static_cast<int>(currentSeason) << ","
       << turnCount << ","
       << whiteEnergy << ","
       << blackEnergy << ","
       << movesLeft << ","
       << (canMoveAgain ? 1 : 0) << ";";

    ss << board.serialize();
    return ss.str();
}

bool GameState::deserialize(const std::string& data, Board& board) {
    std::istringstream ss(data);
    std::string stateToken;
    std::getline(ss, stateToken, ';');

    std::istringstream stateSs(stateToken);
    std::string field;

    try {
        std::getline(stateSs, field, ',');
        currentTurn = static_cast<Color>(std::stoi(field));

        std::getline(stateSs, field, ',');
        status = static_cast<GameStatus>(std::stoi(field));

        std::getline(stateSs, field, ',');
        gameMode = static_cast<GameModeType>(std::stoi(field));

        std::getline(stateSs, field, ',');
        currentSeason = static_cast<Season>(std::stoi(field));

        std::getline(stateSs, field, ',');
        turnCount = std::stoi(field);

        std::getline(stateSs, field, ',');
        whiteEnergy = std::stoi(field);

        std::getline(stateSs, field, ',');
        blackEnergy = std::stoi(field);

        std::getline(stateSs, field, ',');
        movesLeft = std::stoi(field);
        
        std::getline(stateSs, field, ',');
        canMoveAgain = (std::stoi(field) == 1);
    } catch (const std::exception& e) {
        std::cerr << "Error deserializing game state: " << e.what() << std::endl;
        return false;
    }

    std::string boardData;
    std::string line;
    while (std::getline(ss, line)) {
        boardData += line;
    }
    if (boardData.empty()) {
        std::cerr << "Error: board data is empty in save file!" << std::endl;
        return false;
    }
    board.deserialize(boardData);
    board.setCurrentSeason(currentSeason);
    return true;
}
