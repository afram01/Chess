#ifndef MOVE_HPP
#define MOVE_HPP

#include "Position.hpp"
#include "Types.hpp"

struct Move
{
    Position from;
    Position to;
    PieceType promotionType = QUEEN;

    Move() = default;

    Move(Position f, Position t)
        : from(f), to(t), promotionType(QUEEN)
    {
    }

    Move(Position f, Position t, PieceType promo)
        : from(f), to(t), promotionType(promo)
    {
    }

    bool operator==(const Move &other) const
    {
        return from == other.from &&
               to == other.to &&
               promotionType == other.promotionType;
    }
};

#endif