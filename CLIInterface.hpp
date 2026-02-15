#ifndef CLIINTERFACE_HPP
#define CLIINTERFACE_HPP

#include "GameEngine.hpp"

#include <string>

class CLIInterface
{
    private:
        GameEngine* engine ;
        
        void printWelcomeBanner() ;
        void printGameInfo() ;
        void printTurnInfo() ;
        void printMovePrompt() ;
        void printInvalidMove() ;
        void printCheckWarning() ;
        void printGameOver() ;
        
        void clearScreen() ;
        void waitForEnter() ;
        
    public:
        CLIInterface(GameEngine* eng) ;
        
        void displayBoard() ;
        void displayGameState() ;
        void displayMoveHistory() ;
        
        std::string getPlayerInput() ;
        void showMessage(const std::string& msg) ;
        void showError(const std::string& error) ;
        
        void runGameLoop() ;
} ;

#endif
