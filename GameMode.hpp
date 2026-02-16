#ifndef GAMEMODE_HPP
#define GAMEMODE_HPP

#include <iostream>
#include <string>

#include "GameEngine.hpp"
#include "Board.hpp"
#include "GameState.hpp"
#include "Types.hpp"
#include "Move.hpp"

class GameMode
{
    protected:

        GameEngine* engine ;
        Board* board ;
        GameState* state ;
        
    public:

        GameMode(GameEngine* eng, Board* brd, GameState* st) : engine(eng), board(brd), state(st){}
        virtual ~GameMode() = default ;
        
        virtual void initialize() = 0 ;
        virtual bool validateMove(const Move& move, Color playerColor) = 0 ;
        virtual void afterMove(const Move& move) = 0 ;
        virtual bool checkWinCondition() = 0 ;
        virtual std::string getModeDescription() const = 0 ;
        virtual GameModeType getType() const = 0 ;
} ;

#endif
