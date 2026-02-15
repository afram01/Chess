#ifndef MOVE_HPP
#define MOVE_HPP

#include "Position.hpp"
#include <optional>

enum class PieceType;

struct Move
{
    Position from;
    Position to;
    std::optional<PieceType> promotionType;

    Move()  = default ;

    Move(Position f, Position t)
        : from(f), to(t)
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