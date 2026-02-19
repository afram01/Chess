#ifndef ENERGYMODE_HPP
#define ENERGYMODE_HPP

#include <iostream>

#include "GameMode.hpp"

class EnergyMode : public GameMode
{
    private:

        int initialEnergy ;
        int captureEnergyGain ;
        int moveEnergyCost ;
        
    public:

        EnergyMode(GameEngine* eng, Board* brd, GameState* st) ;
        
        void initialize() override ;
        bool validateMove(const Move& move, Color playerColor) override ;
        void afterMove(const Move& move) override ;
        bool checkWinCondition() override ;
        std::string getModeDescription() const override ;
        GameModeType getType() const override ;
        
        void setInitialEnergy(int energy) ;
        void setCaptureEnergyGain(int gain) ;
        void setMoveEnergyCost(int cost) ;
} ;

#endif
