# Chess++ - Advanced Chess Game

A modern and enhanced version of the classic chess game featuring special pieces, multiple game modes, and dynamic seasonal mechanics.

## Prerequisites

- C++17 compiler or higher
- CMake 3.10+
- Operating System: Windows / Linux / macOS

## Key Features

### Special Pieces
- **Special Pawn**: Can threaten the square directly in front, in addition to diagonal captures
- **Armored Queen**: Protected by two layers of armor against attacks
- **Spy**: An enemy piece in disguise that reveals itself after several moves
- **Joker**: Can mimic the movement of any other piece (twice per game)

### Game Modes
1. **Standard Mode**: Classic chess with "Massacre" feature (double move after capturing enemy queen)
2. **Energy Mode**: Energy management system - gain energy by capturing pieces
3. **Mission Mode**: Complete specific objectives within a limited number of moves

### Seasonal System
- **Spring**: Pawns can only move every other turn
- **Summer**: Bishops are limited to 3 squares maximum
- **Autumn**: Knights can only move every other turn
- **Winter**: Rooks are limited to 4 squares maximum

## Installation & Running

### 1. Build
```bash
mkdir build
cd build
cmake ..
make
```

### 2. Run
```bash
./chess++
```

## Chess Piece Display Fix

If chess pieces are not displaying correctly, follow these steps:

### Windows
1. Open `Control Panel`
2. Navigate to `Region`
3. Select the `Administrative` tab
4. Click `Change system locale...`
5. Enable `Beta: Use Unicode UTF-8 for worldwide language support`
6. Restart your computer

### Linux/macOS
Ensure your terminal supports UTF-8:
```bash
export LANG=en_US.UTF-8
```

## How to Play

### Input Format
```
e2 e4
```
- First: Source position (e2)
- Second: Destination position (e4)

### Special Commands
- `hint` - Show all possible moves
- `menu` - Access game menu
- `save` - Save current game
- `undo` - Undo last move
- `quit` - Exit game


### Energy Mechanics
- Each move costs energy based on the piece moved
- Capturing an enemy piece refunds energy equal to the captured piece's value
- Example: Moving a Rook (5) to capture a Knight (3) → -5 +3 = -2 net energy

## Project Structure

```
chess++/
├── main.cc              # Entry point
├── Board.cpp/.hpp       # Board and piece management
├── GameEngine.cpp/.hpp  # Core game engine
├── GameState.cpp/.hpp   # Game state management
├── piece.cc/.h          # Base piece class
├── Pawn/King/...        # Individual piece implementations
├── *Mode.cpp/.hpp       # Different game modes
└── CMakeLists.txt       # Build configuration
```

## Architecture

The project follows **MVC (Model-View-Controller)** design pattern:

- **Model**: `Board`, `GameState`, `Piece` classes
- **View**: `CLIInterface`
- **Controller**: `GameEngine`, `GameMode` classes

Key design principles:
- Object-oriented programming with inheritance and polymorphism
- Strategy pattern for game modes
- Template method pattern for piece movements

## Credits

- **Instructor**: Dr. Ali Javidani
- **Semester**: Winter 1404 (Bu-Ali Sina Uniersity)v


**Note**: This project is part of the Advanced Programming course at Bu-Ali Sina University.

## Known Issues & Troubleshooting

### Issue: Chess symbols not displaying
**Solution**: Enable UTF-8 Unicode support in system locale settings (see "Chess Piece Display Fix" section above)

### Issue: Undo doesn't restore energy
**Solution**: Fixed in latest version - energy is now correctly saved before each move

### Issue: Spy piece not revealing
**Solution**: Spy reveals after a set number of moves based on `spyRevealThreshold` setting
---
