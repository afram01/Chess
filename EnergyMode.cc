#include <iostream>

#include "EnergyMode.hpp"

using namespace std;

EnergyMode::EnergyMode(GameEngine* eng, Board* brd, GameState* st)
    : GameMode(eng, brd, st), initialEnergy(100), captureEnergyGain(10), moveEnergyCost(1){}

void EnergyMode::initialize()
{
    state->setEnergy(Color::WHITE, initialEnergy) ;
    state->setEnergy(Color::BLACK, initialEnergy) ;
}

bool EnergyMode::validateMove(const Move& move, Color playerColor)
{
    if (!board->isMoveLegal(move, playerColor))
    {
        return false ;
    }
    
    int currentEnergy = state->getEnergy(playerColor) ;
    
    if (currentEnergy < moveEnergyCost)
    {
        return false ;
    }
    
    return true ;
}

void EnergyMode::afterMove(const Move& move)
{
    Color currentPlayer = engine->getCurrentTurn() ;
    
    state->reduceEnergy(currentPlayer, moveEnergyCost) ;
    
    Piece* targetPiece = board->getPieceAt(move.to) ;
    
    if (targetPiece != nullptr)
    {
        int newEnergy = state->getEnergy(currentPlayer) + captureEnergyGain ;
        state->setEnergy(currentPlayer, newEnergy) ;
    }
}

bool EnergyMode::checkWinCondition()
{
    GameStatus status = engine->getCurrentState() ;
    
    if (status == CHECKMATE)
    {
        return true ;
    }
    
    int whiteEnergy = state->getEnergy(Color::WHITE) ;
    int blackEnergy = state->getEnergy(Color::BLACK) ;
    
    if (whiteEnergy <= 0 || blackEnergy <= 0)
    {
        return true ;
    }
    
    return false ;
}

string EnergyMode::getModeDescription() const
{
    return "Energy Mode: Manage energy to make moves" ;
}

GameModeType EnergyMode::getType() const
{
    return ENERGY ;
}

void EnergyMode::setInitialEnergy(int energy)
{
    initialEnergy = energy ;
}

void EnergyMode::setCaptureEnergyGain(int gain)
{
    captureEnergyGain = gain ;
}

void EnergyMode::setMoveEnergyCost(int cost)
{
    moveEnergyCost = cost ;
}
