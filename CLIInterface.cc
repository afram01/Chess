#include <iostream>

#include "CLIInterface.hpp"
#include "Joker.h" 

using namespace std;

CLIInterface::CLIInterface(GameEngine *eng) : engine(eng) {}

void CLIInterface::printWelcomeBanner()
{
    cout << "\n" ;
    cout << "========================================\n" ;
    cout << "          WELCOME TO CHESS++              \n" ;
    cout << "========================================\n" ;
    cout << endl ;
}

void CLIInterface::printGameInfo()
{
    GameState *state = engine->getGameState() ;

    cout << "\n--- Game Information ---\n" ;

    const char *modeNames[] = {"Standard", "Energy", "Mission"} ;
    cout << "Mode: " << modeNames[static_cast<int>(state->getGameMode())] << endl ;

    const char *seasonNames[] = {"Spring", "Summer", "Autumn", "Winter"} ;
    cout << "Season: " << seasonNames[static_cast<int>(state->getCurrentSeason())] << endl ;

    cout << "Turn: " << state->getTurnCount() << endl ;

    Board* board = engine->getBoard();
    int bombThreshold = board->getBombThreshold();
    int minMovesUntilBomb = bombThreshold;
    
    std::vector<std::pair<Position, int>> criticalPieces;
    
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Position pos(r, c);
            Piece* piece = board->getPieceAt(pos);
            if (piece) {
                int stationaryCount = board->getStationaryCount(pos);
                int movesLeft = bombThreshold - stationaryCount;
                
                if (movesLeft < minMovesUntilBomb) {
                    minMovesUntilBomb = movesLeft;
                }
                
                if (movesLeft < 5 && movesLeft > 0) {
                    criticalPieces.push_back({pos, movesLeft});
                }
            }
        }
    }
    
    cout << "Next Bomb in: " << minMovesUntilBomb << " moves" << endl;
    
    if (!criticalPieces.empty()) {
        cout << "Critical Pieces:" << endl;
        for (const auto& pair : criticalPieces) {
            Piece* p = board->getPieceAt(pair.first);
            cout << "   " << p->getSymbol() 
                 << " at " << (char)('a' + pair.first.col) << (8 - pair.first.row)
                 << " (" << pair.second << " moves)" << endl;
        }
    }

    if (state->getGameMode() == ENERGY)
    {
        cout << "White Energy: " << state->getEnergy(Color::WHITE) << endl ;
        cout << "Black Energy: " << state->getEnergy(Color::BLACK) << endl ;
    }

    if (state->getGameMode() == MISSION)
    {
        cout << "Mission: " << state->getMissionDescription() << endl ;
        cout << "Moves Left: " << state->getMovesLeft() << endl ;
    }
}

void CLIInterface::printTurnInfo()
{
    Color currentTurn = engine->getCurrentTurn() ;
    string turnColor = (currentTurn == Color::WHITE) ? "WHITE" : "BLACK" ;

    cout << "\n>>> " << turnColor << "'s Turn <<<\n" ;

    GameStatus status = engine->getCurrentState() ;
    if (status == CHECK)
    {
        printCheckWarning() ;
    }
}

void CLIInterface::showRevealableSpies()
{
    std::vector<Position> spies = engine->getRevealableSpies();
    Color currentPlayer = engine->getCurrentTurn();
    std::string playerName = (currentPlayer == Color::WHITE) ? "White" : "Black";

    cout << "\n=== Your Revealable Spies (" << playerName << ") ===" << endl;

    if (spies.empty())
    {
        cout << "You have no hidden spies available to reveal." << endl;
        cout << "Spies can only be revealed by their real owner." << endl;
    }
    else
    {
        cout << "The following spies belong to you and can be revealed:" << endl;
        for (const auto& pos : spies)
        {
            Piece* p = engine->getBoard()->getPieceAt(pos);
            char col = 'a' + pos.col;
            char row = '0' + (8 - pos.row);
            cout << "  >> " << p->getSymbol()
                 << " at " << col << row
                 << "  (disguised as enemy piece)" << endl;
        }
        cout << "\nTo reveal a spy, type:  reveal <position>" << endl;
        cout << "Example:  reveal e4" << endl;
        cout << "Revealing a spy uses your turn!" << endl;
    }
    cout << "============================================" << endl;
}

void CLIInterface::showAllPossibleMoves()
{
    Board* board = engine->getBoard() ;
    Color currentPlayer = engine->getCurrentTurn() ;

    cout << "\n=== All Possible Moves ===" << endl ;

    bool foundMoves = false ;

    for (int r = 0 ; r < 8 ; r++)
    {
        for (int c = 0 ; c < 8 ; c++)
        {
            Piece* piece = board->getPiece(r, c) ;
            if (piece && piece->getColor() == currentPlayer)
            {
                Position pos(r, c) ;
                vector<Position> moves = engine->getValidMoves(pos) ;

                if (!moves.empty())
                {
                    foundMoves = true ;
                    char col = 'a' + c ;
                    char row = '0' + (8 - r) ;

                    cout << "\n" << piece -> getSymbol() << " at " << col << row << " can move to: " ;

                    for (size_t i = 0 ; i < moves.size() ; i++)
                    {
                        char toCol = 'a' + moves[i].col ;
                        char toRow = '0' + (8 - moves[i].row) ;
                        cout << toCol << toRow ;

                        if (i < moves.size() - 1)
                            cout << ", " ;
                    }
                    cout << endl ;
                }
            }
        }
    }

    if (!foundMoves)
    {
        cout << "\nNo legal moves available !!!" << endl ;
    }

    cout << "=========================" << endl ;
}

bool CLIInterface::handleInGameMenu()
{
    while (true)
    {
        cout << "\n=== Game Menu ===\n\n" ;
        cout << "1. Save Game" << endl ;
        cout << "2. Load Game" << endl ;
        cout << "3. Undo Move" << endl ;
        cout << "4. Show Hints" << endl ;
        cout << "5. Reveal Spy" << endl ;
        cout << "6. Return to Game" << endl ;
        cout << "7. Quit" << endl ;

        cout << "Choice: " ;

        string choice ;
        getline(cin, choice) ;

        if (choice == "1")
        {
            cout << "Filename: " ;

            string filename ;
            getline(cin, filename) ;
            engine -> saveGame(filename) ;
            waitForEnter() ;

            return false ;
        }

        else if (choice == "2")
        {
            cout << "Filename: " ;

            string filename ;
            getline(cin, filename) ;
            engine -> loadGame(filename) ;
            waitForEnter() ;

            return false ;
        }

        else if (choice == "3")
        {
            engine -> undoMove() ;
            waitForEnter() ;

            return false ;
        }

        else if (choice == "4")
        {
            showAllPossibleMoves() ;
            waitForEnter() ;

            continue;
        }

        else if (choice == "5")
        {
            showRevealableSpies();

            cout << "\nEnter spy position to reveal (or press Enter to cancel): ";
            string posInput;
            getline(cin, posInput);

            if (!posInput.empty())
            {
                bool ok = engine->revealSpy(posInput);
                if (ok)
                {
                    cout << "Spy successfully revealed!" << endl;
                    waitForEnter();
                    return false; 
                }
                else
                {
                    cout << "Failed to reveal spy." << endl;
                    waitForEnter();
                }
            }
            continue;
        }

        else if (choice == "6")
        {
            return false ;
        }

        else if (choice == "7")
        {
            cout << "Do You Want To Quit The Game ? (y/n): " ;
            string confirm ;
            getline(cin, confirm) ;

            if (confirm == "y" || confirm == "Y")
            {
                return true ;
            }
        }
    }
}

void CLIInterface::printMovePrompt()
{
    cout << "\nEnter move (e.g., e2 e4), 'joker', 'hint', 'help', or 'menu': " ;
}

void CLIInterface::printInvalidMove()
{
    cout << "\nInvalid move !!!\n" ;
}

void CLIInterface::printCheckWarning()
{
    cout << "\n!!! CHECK !!! Your king is under attack !\n" ;
}

void CLIInterface::printGameOver()
{
    cout << "\n" ;
    cout << "========================================\n" ;
    cout << "           GAME OVER                    \n" ;
    cout << "========================================\n" ;

    GameStatus status = engine->getCurrentState() ;
    Color winner = (engine->getCurrentTurn() == Color::WHITE) ? Color::BLACK : Color::WHITE ;

    if (status == CHECKMATE)
    {
        string winnerColor = (winner == Color::WHITE) ? "WHITE" : "BLACK" ;
        cout << "Checkmate! " << winnerColor << " wins!" << endl ;
    }

    else if (status == STALEMATE)
    {
        cout << "Stalemate! The game is a draw." << endl ;
    }

    else if (status == DRAW)
    {
        cout << "Draw!" << endl ;
    }

    else if (status == MISSION_WIN)
    {
        cout << "\n MISSION ACCOMPLISHED !!!" << endl ;
        cout << "  You completed the mission successfully !!!" << endl ;

        GameState *st = engine->getGameState() ;
        cout << "  Mission: " << st->getMissionDescription() << endl ;
    }

    else if (status == MISSION_FAIL)
    {
        cout << "\nMISSION FAILED !!!" << endl ;
        cout << "  You ran out of moves without completing the mission." << endl ;

        GameState *st = engine->getGameState() ;
        cout << "  Mission was: " << st->getMissionDescription() << endl ;
    }

    cout << "========================================\n" ;
}

void CLIInterface::clearScreen()
{
#ifdef _WIN32
    system("cls") ;
#else
    system("clear") ;
#endif
}

void CLIInterface::waitForEnter()
{
    cout << "Press Enter to continue..." ;
    cin.get() ;
}

void CLIInterface::displayBoard()
{
    engine->displayBoard() ;
}

void CLIInterface::displayGameState()
{
    clearScreen() ;
    printWelcomeBanner() ;
    displayBoard() ;
    printGameInfo() ;
    cout << endl ;
    printTurnInfo() ;
}

void CLIInterface::displayMoveHistory()
{
    cout << "Move history not implemented yet" << endl ;
}

string CLIInterface::getPlayerInput()
{
    string input ;
    printMovePrompt() ;
    getline(cin, input) ;
    return input ;
}

void CLIInterface::showMessage(const string &msg)
{
    cout << msg << endl ;
}

void CLIInterface::showError(const string &error)
{
    cout << "ERROR: " << error << endl ;
}
void CLIInterface::printHelp() {
    cout << "\n=== Commands ===\n";
    cout << "move <from> <to> : Move a piece (e.g., e2 e4)\n";
    cout << "joker            : Transform a Joker piece\n";
    cout << "hint             : Show possible moves\n";
    cout << "undo             : Undo last move\n";
    cout << "save             : Save game\n";
    cout << "menu             : Open game menu\n";
    cout << "quit             : Exit game\n";
    cout << "\n=== Joker Guide ===\n";
    cout << "Select 'joker' command to transform.\n";
    cout << "You can choose the piece to mimic.\n";
    cout << "Transformation lasts for ONE turn only.\n";
    cout << "You can transform up to 2 times per game.\n";
    waitForEnter();
}

void CLIInterface::handleJokerCommand() {
    cout << "Enter the position of your Joker (e.g., d4): ";
    string posStr;
    getline(cin, posStr);
    
    cout << "Choose piece to mimic (Q=Queen, R=Rook, B=Bishop, N=Knight, P=Pawn, K=King): ";
    string typeStr;
    getline(cin, typeStr);
    
    PieceType targetType = PieceType::PAWN;
    if (typeStr == "Q" || typeStr == "q") targetType = PieceType::QUEEN;
    else if (typeStr == "R" || typeStr == "r") targetType = PieceType::ROOK;
    else if (typeStr == "B" || typeStr == "b") targetType = PieceType::BISHOP;
    else if (typeStr == "N" || typeStr == "n") targetType = PieceType::KNIGHT;
    else if (typeStr == "P" || typeStr == "p") targetType = PieceType::PAWN;
    else if (typeStr == "K" || typeStr == "k") targetType = PieceType::KING;
    else {
        cout << "Invalid piece type! Transformation cancelled." << endl;
        waitForEnter();
        return;
    }
    
    if (engine->transformJoker(posStr, targetType)) {
        cout << "Joker transformed successfully!" << endl;
    } else {
        cout << "Joker transformation failed. Check position or remaining transforms." << endl;
    }
    waitForEnter();
}

void CLIInterface::runGameLoop()
{
    while (!engine -> isGameOver())
    {
        displayGameState() ;

        GameState *state = engine -> getGameState() ;
        if (state -> getGameMode() == ENERGY)
        {
            if (state -> getEnergy(Color::WHITE) <= 0 || state->getEnergy(Color::BLACK) <= 0)
            {
                cout << "Energy exhausted! Game Over." << endl ;
                break;
            }
        }

        string input = getPlayerInput() ;

        if (input == "quit" || input == "exit")
        {
            cout << "Exiting game..." << endl ;
            break;
        }

        if (input == "hint")
        {
            showAllPossibleMoves() ;
            waitForEnter() ;

            continue;
        }
        
        if (input == "help") {
            printHelp();
            continue;
        }

        if (input == "joker") {
            handleJokerCommand();
            continue;
        }

        if (input == "menu")
        {
            bool shouldQuit = handleInGameMenu() ;
            if (shouldQuit)
            {
                break;
            }

            continue;
        }

        if (input == "save")
        {
            cout << "Enter filename: " ;
            string filename ;
            getline(cin, filename) ;
            engine->saveGame(filename) ;

            cout << "Game saved! Press Enter." << endl ;
            waitForEnter() ;

            continue;
        }

        if (input == "undo")
        {
            engine->undoMove() ;
            cout << "Move undone! Press Enter." << endl ;
            waitForEnter() ;

            continue;
        }

        bool success = engine->processMove(input) ;

        if (!success)
        {
            printInvalidMove() ;
            waitForEnter() ;
        }
    }

    displayGameState() ;
    printGameOver() ;
    waitForEnter() ;
}
