#ifndef POSITION_H
#define POSITION_H

struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

enum class Color {
    WHITE,
    BLACK,
    NONE
};

#endif
