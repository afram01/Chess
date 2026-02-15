#include "Move.hpp"
#include "piece.h"

Move::Move()
    : from(), to(), promotionType(PieceType::QUEEN)
{
}

Move::Move(Position f, Position t)
    : from(f), to(t), promotionType(PieceType::QUEEN)
{
}

Move::Move(Position f, Position t, PieceType promo)
    : from(f), to(t), promotionType(promo)
{
}

bool Move::operator==(const Move &other) const
{
    return from == other.from &&
           to == other.to &&
           promotionType == other.promotionType;
}
