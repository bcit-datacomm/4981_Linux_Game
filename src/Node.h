/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE: Node.h
--
-- FUNCTIONS:

--
-- DATE:        February 1, 2017
--
-- DESIGNER:    Fred Yang, Robert Arendac
--
-- PROGRAMMER:  Fred Yang, Robert Arendac
--
-- NOTES:
-- Used for the A* algorithm in navigating the map
----------------------------------------------------------------------------------------------------------------------*/

#ifndef NODE_H
#define NODE_H
#include <math.h>
#include <queue>
#include "Common.h"

// horizontal/vertical & diagonal cost
#define BASE_COST   10
#define EXTEND_COST 14

// map row & column
const int row = TILE_SIZE;
const int col = TILE_SIZE;
static int map[row][col];

static int closedNodes[row][col]; // array of closed nodes (evaluated)
static int openNodes[row][col];   // array of open nodes (to be evaluated)
static int dirMap[row][col];      // array of directions

/**
 * 8 possible movements
 * 0 - right, 1 - right down, 2 - down, 3 - left down
 * 4 - left, 5 - left up, 6 - up, 7 - right up
 */
static int mx[DIR_CAP]={1, 1, 0, -1, -1, -1, 0, 1};
static int my[DIR_CAP]={0, 1, 1, 1, 0, -1, -1, -1};

class Node {
public:
    explicit Node(const int xPos = 0, const int yPos = 0, const int lv = 0,
                  const int pri = 0)
    : xPos_(xPos), yPos_(yPos), lv_(lv), pri_(pri) {}

    virtual ~Node() {}

    // X coordinate of current node
    int getXPos() const {
        return xPos_;
    }

    // Y coordinate of current node
    int getYPos() const {
        return yPos_;
    }

    // Get distance travelled so far
    int getLevel() const {
        return lv_;
    }

    // Get priority of current node
    int getPriority() const {
        return pri_;
    }

    // current level plus remaining cost
    void updatePriority(const int& xDest, const int &yDest) {
         pri_ = lv_ + estimate(xDest, yDest) * BASE_COST;
    }

    // calculate next level based on direction
    void nextLevel(const int& dir) {
         lv_ += (dir%2 ==0 ? BASE_COST : EXTEND_COST);
    }

    // calculate cost per the remaining distance to the destination
    const int& estimate(const int& xDest, const int& yDest) const {
        static int xDist, yDist, dist;
        xDist = xDest - xPos_;
        yDist = yDest - yPos_;

        // Euclidian Distance
        dist = static_cast<int>(sqrt(xDist * xDist + yDist * yDist));

        // Manhattan distance
        //dist = abs(xDist) + abs(yDist);

        return dist;
    }

private:
    // current position
    int xPos_;
    int yPos_;

    // level = total distance already travelled to reach the node
    int lv_;

    // priority = level+remaining distance estimated
    // smaller one with higher priority
    int pri_;
};

// determine priority in priority queue
inline bool operator<(const Node& node1, const Node& node2) {
    return node1.getPriority() > node2.getPriority();
}

#endif
