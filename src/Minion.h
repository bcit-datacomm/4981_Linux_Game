/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE: Minion.h
--
-- PROGRAM:     Astar
--
-- FUNCTIONS:

--
-- DATE:        February 1, 2017
--
-- DESIGNER:    Fred Yang, Robert Arendac, Jeonghun Lee
--
-- PROGRAMMER:  Fred Yang, Robert Arendac, Jeonghun Lee
--
-- NOTES:
--
----------------------------------------------------------------------------------------------------------------------*/
#ifndef MINION_H
#define MINION_H

#include "Point.h"

class Minion {
public:
    explicit Minion(const Point loc, const int hp = 100,
                    const int pwr = 100, const int amr = 50,
                    const int agil = 50, const int mode = 0)
    : loc_(loc), hp_(hp), pwr_(pwr), amr_(amr),
      agil_(agil), mode_(mode) {}
    virtual ~Minion() {}

    // methods
    virtual void move() {}
    virtual void attack() {}

    virtual void hit()
    {
        if (--hp <= 0)
        {
            die();
        }
    }

    virtual void die() {}

private:
    Point loc_;     // minion location
    int hp_;        // health points
    int pwr_;       // power
    int amr_;       // armor
    int agil_;      // agility
    int mode_;      // 0-idle, 1-move, 2-attack
};

#endif
