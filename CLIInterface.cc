#include <iostream>

#include "CLIInterface.hpp"

using namespace std;

CLIInterface::CLIInterface(GameEngine *eng) : engine(eng) {}

void CLIInterface::printWelcomeBanner()
{
    cout << "\n";
    cout << "========================================\n";
    cout << "        WELCOME TO CHESS++              \n";
    cout << "        Advanced Chess Game             \n";
    cout << "========================================\n";
    cout << endl;
}

void CLIInterface::printGameInfo()
{
    GameState *state = engine->getGameState();

    cout << "\n--- Game Information ---\n";

    const char *modeNames[] = {"Standard", "Energy", "Mission"};
    cout << "Mode: " << modeNames[static_cast<int>(state->getGameMode())] << endl;

    const char *seasonNames[] = {"Spring", "Summer", "Autumn", "Winter"};
    cout << "Season: " << seasonNames[static_cast<int>(state->getCurrentSeason())] << endl;

    cout << "Turn: " << state->getTurnCount() << endl;

    if (state->getGameMode() == ENERGY)
    {
        cout << "White Energy: " << state->getEnergy(Color::WHITE) << endl;
        cout << "Black Energy: " << state->getEnergy(Color::BLACK) << endl;
    }

    if (state->getGameMode() == MISSION)
    {
        cout << "Mission: " << state->getMissionDescription() << endl;
        cout << "Moves Left: " << state->getMovesLeft() << endl;
    }
}

void CLIInterface::printTurnInfo()
{
    Color currentTurn = engine->getCurrentTurn();
    string turnColor = (currentTurn == Color::WHITE) ? "WHITE" : "BLACK";

    cout << "\n>>> " << turnColor << "'s Turn <<<\n";

    GameStatus status = engine->getCurrentState();
    if (status == CHECK)
    {
        printCheckWarning();
    }
}

void CLIInterface::printMovePrompt()
{
    cout << "\nEnter move (for example, e2 e4): ";
}

void CLIInterface::printInvalidMove()
{
    cout << "\n*** Invalid move! Please try again. ***\n";
}

void CLIInterface::printCheckWarning()
{
    cout << "\n!!! CHECK !!! Your king is under attack!\n";
}

void CLIInterface::printGameOver()
{
    cout << "\n";
    cout << "========================================\n";
    cout << "           GAME OVER                    \n";
    cout << "========================================\n";

    GameStatus status = engine->getCurrentState();
    Color winner = (engine->getCurrentTurn() == Color::WHITE) ? Color::BLACK : Color::WHITE;

    if (status == CHECKMATE)
    {
        string winnerColor = (winner == Color::WHITE) ? "WHITE" : "BLACK";
        cout << "Checkmate! " << winnerColor << " wins!" << endl;
    }
    else if (status == STALEMATE)
    {
        cout << "Stalemate! The game is a draw." << endl;
    }
    else if (status == DRAW)
    {
        cout << "Draw!" << endl;
    }
    else if (status == MISSION_WIN)
    {
        cout << "\n  🏆  MISSION ACCOMPLISHED!  🏆" << endl;
        cout << "  You completed the mission successfully!" << endl;
        GameState *st = engine->getGameState();
        cout << "  Mission: " << st->getMissionDescription() << endl;
    }
    else if (status == MISSION_FAIL)
    {
        cout << "\n  ❌  MISSION FAILED  ❌" << endl;
        cout << "  You ran out of moves without completing the mission." << endl;
        GameState *st = engine->getGameState();
        cout << "  Mission was: " << st->getMissionDescription() << endl;
    }

    cout << "========================================\n";
}

void CLIInterface::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void CLIInterface::waitForEnter()
{
    cout << "Press Enter to continue...";
    cin.get();
}

void CLIInterface::displayBoard()
{
    engine->displayBoard();
}

void CLIInterface::displayGameState()
{
    clearScreen();
    printWelcomeBanner();
    displayBoard();
    printGameInfo();
    cout << endl;
    printTurnInfo();
}

void CLIInterface::displayMoveHistory()
{
    cout << "Move history not implemented yet" << endl;
}

string CLIInterface::getPlayerInput()
{
    string input;
    printMovePrompt();
    getline(cin, input);
    return input;
}

void CLIInterface::showMessage(const string &msg)
{
    cout << msg << endl;
}

void CLIInterface::showError(const string &error)
{
    cout << "ERROR: " << error << endl;
}

void CLIInterface::runGameLoop()
{
    while (!engine->isGameOver())
    {
        displayGameState();

        GameState *state = engine->getGameState();
        if (state->getGameMode() == ENERGY)
        {
            if (state->getEnergy(Color::WHITE) <= 0 || state->getEnergy(Color::BLACK) <= 0)
            {
                cout << "Energy exhausted! Game Over." << endl;
                break;
            }
        }

        string input = getPlayerInput();

        if (input == "quit" || input == "exit")
        {
            cout << "Exiting game..." << endl;
            break;
        }

        if (input == "save")
        {
            cout << "Enter filename: ";
            string filename;
            getline(cin, filename);
            engine->saveGame(filename);
            cout << "Game saved! Press Enter." << endl;
            waitForEnter();
            continue;
        }

        if (input == "undo")
        {
            engine->undoMove();
            cout << "Move undone! Press Enter." << endl;
            waitForEnter();
            continue;
        }

        bool success = engine->processMove(input);

        if (!success)
        {
            printInvalidMove();
            waitForEnter();
        }
    }

    displayGameState();
    printGameOver();
    displayGameState();
    printGameOver();
    waitForEnter(); 
}
