#ifndef DIRECTION_H
#define DIRECTION_H

enum class Direction { Up, Down, Left, Right, None };

inline bool areOpposite(Direction a, Direction b) {
    return (a == Direction::Up    && b == Direction::Down)  ||
           (a == Direction::Down  && b == Direction::Up)    ||
           (a == Direction::Left  && b == Direction::Right) ||
           (a == Direction::Right && b == Direction::Left);
}

#endif // DIRECTION_H
