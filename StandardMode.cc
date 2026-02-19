#include <iostream>

#include "StandardMode.hpp"

using namespace std;

StandardMode::StandardMode(GameEngine* eng, Board* brd, GameState* st) : GameMode(eng, brd, st){}

void StandardMode::initialize() {}

bool StandardMode::validateMove(const Move& move, Color playerColor)
{
    return board->isMoveLegal(move, playerColor) ;
}

void StandardMode::afterMove(const Move&) {}

bool StandardMode::checkWinCondition()
{
    GameStatus status = engine->getCurrentState() ;
    
    return status == CHECKMATE || status == STALEMATE || status == DRAW ;
}

string StandardMode::getModeDescription() const
{
    return "Standard Chess Rules" ;
}

GameModeType StandardMode::getType() const
{
    return STANDARD ;
}
