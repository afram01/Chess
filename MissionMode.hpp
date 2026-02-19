#ifndef MISSIONMODE_HPP
#define MISSIONMODE_HPP

#include "GameMode.hpp"

class MissionMode : public GameMode
{
    private:

        int initialMoves ;
        std::string description ;
        
    public:

        MissionMode(GameEngine* eng , Board* brd , GameState* st) ;
        
        void initialize() override ;
        bool validateMove(const Move& move , Color playerColor) override ;
        void afterMove(const Move& move) override ;
        bool checkWinCondition() override ;
        std::string getModeDescription() const override ;
        GameModeType getType() const override ;
        
        void setMission(const std::string& desc, int moves) ;
} ;

#endif
