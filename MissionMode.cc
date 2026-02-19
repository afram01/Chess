#include <iostream>

#include "MissionMode.hpp"
using namespace std;

MissionMode::MissionMode(GameEngine* eng, Board* brd, GameState* st) : GameMode(eng, brd, st), initialMoves(0), description(""){}

void MissionMode::setMission(const std::string& desc, int moves)
{
    description = desc ;
    initialMoves = moves ;
}

void MissionMode::initialize()
{
    state->setMissionDescription(description) ;
    state->setMovesLeft(initialMoves) ;
}

bool MissionMode::validateMove(const Move& move, Color playerColor)
{
    return board->isMoveLegal(move, playerColor) ;
}

void MissionMode::afterMove(const Move&)
{
    state->decrementMovesLeft() ;
}

bool MissionMode::checkWinCondition()
{
    GameStatus status = engine->getCurrentState();

    if (status == CHECKMATE || status == STALEMATE || status == DRAW)
        return true;

    if (state->getMovesLeft() <= 0)
    {
        state->setStatus(DRAW);  
        return true;
    }

    return false;
}

std::string MissionMode::getModeDescription() const
{
    return description ;
}

GameModeType MissionMode::getType() const
{
    return MISSION ;
}
