#ifndef STANDARDMODE_HPP
#define STANDARDMODE_HPP

#include <iostream>

#include "GameMode.hpp"

class StandardMode : public GameMode
{
    public:

        StandardMode(GameEngine* eng, Board* brd, GameState* st) ;
        
        void initialize() override ;
        bool validateMove(const Move& move, Color playerColor) override ;
        void afterMove(const Move& move) override ;
        bool checkWinCondition() override ;
        std::string getModeDescription() const override ;
        GameModeType getType() const override ;
} ;

#endif
