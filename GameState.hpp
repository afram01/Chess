#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>
#include <fstream>
#include "Board.hpp"

class GameState {
public:
    void saveToFile(std::string filename, Board* board) {
        std::ofstream file(filename);
        file << serialize(board);
        file.close();
    }

    void loadFromFile(std::string filename, Board* board) {
        std::ifstream file(filename);
        std::string data;
        file >> data;
        deserialize(data, board);
        file.close();
    }

    std::string serialize(Board* board) { return "serialized_data"; }
    void deserialize(std::string data, Board* board) {  }
};

#endif
