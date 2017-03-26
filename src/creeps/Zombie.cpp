#include <math.h>
#include <random>
#include <cassert>
#include <utility>
#include "Zombie.h"
#include "Node.h"
#include "../game/GameManager.h"
#include "../log/log.h"
using namespace std;

Zombie::Zombie(int32_t id, const SDL_Rect &dest, const SDL_Rect &movementSize, const SDL_Rect &projectileSize,
        const SDL_Rect &damageSize, int health, ZombieState state, int step, ZombieDirection dir, int frame)
        : Entity(id, dest, movementSize, projectileSize, damageSize),
        Movable(id, dest, movementSize, projectileSize, damageSize, ZOMBIE_VELOCITY),
        health(health), state(state), step(step), dir(dir), frame(frame) {
    logv("Create Zombie\n");
}

Zombie::~Zombie() {
    logv("Destroy Zombie\n");
}

/**
 * Get move direction
 * Fred Yang
 * February 14
 */
ZombieDirection Zombie::getMoveDir() const {
    if (frame > 0) {
        return dir;
    }

    const int sp = getStep();
    const string pth = getPath();
    /*
    cout << "path: " << pth << endl;
    cout << sp << '-' << pth.length() << endl;
    */

    return static_cast<ZombieDirection>(sp < static_cast<int>(pth.length()) ? stoi(pth.substr(sp,1)) : -1);
}

void Zombie::onCollision() {
    // Do nothing for now
}

void Zombie::collidingProjectile(int damage) {
    health -= damage;
}

/*
 * Returns if the zombie is moving
 * Robert Arendac
 * March 7
*/
bool Zombie::isMoving() const {
    return (state == ZombieState::ZOMBIE_MOVE);
}

/*
 * Does a check to see if the zombie already arrived at the target.
 * In theory, zombies will only have a movement collision with a target
 * as their pathfinding should walk around obstacles.
 * Robert Arendac
 * March 18
*/
bool Zombie::checkTarget() const {
    auto ch = GameManager::instance()->getCollisionHandler();
    return (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeMarine, this), this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeObj, this), this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeTurret, this), this));
}

/**
 * Get the direction of the zombie and take a step in the appropriate direction
 * Rob, Fred
 * March 13
*/
void Zombie::generateMove() {
    const ZombieDirection direction = getMoveDir();   //Direction zombie is moving
    //cout << "move dir: " << d << " state: " << state << " Frame: " << frame << endl;

    // Path is empty, shouldn't move
    if (direction == ZombieDirection::DIR_INVALID || checkTarget()) {
        if (frame > 0) {
            --frame;
        }

        // Changed to attack state once attack code is ready
        setState(ZombieState::ZOMBIE_IDLE);

        return;
    }

    // Each case will set direction and angle based on the next step in the path
    switch(direction) {
        case ZombieDirection::DIR_R:
            setDX(ZOMBIE_VELOCITY);
            setDY(0);
            setAngle(static_cast<double>(ZombieAngles::EAST));
            break;
        case ZombieDirection::DIR_RD:
            setDX(ZOMBIE_VELOCITY);
            setDY(ZOMBIE_VELOCITY);
            setAngle(static_cast<double>(ZombieAngles::SOUTHEAST));
            break;
        case ZombieDirection::DIR_D:
            setDX(0);
            setDY(ZOMBIE_VELOCITY);
            setAngle(static_cast<double>(ZombieAngles::SOUTH));
            break;
        case ZombieDirection::DIR_LD:
            setDX(-ZOMBIE_VELOCITY);
            setDY(ZOMBIE_VELOCITY);
            setAngle(static_cast<double>(ZombieAngles::SOUTHWEST));
            break;
        case ZombieDirection::DIR_L:
            setDX(-ZOMBIE_VELOCITY);
            setDY(0);
            setAngle(static_cast<double>(ZombieAngles::WEST));
            break;
        case ZombieDirection::DIR_LU:
            setDX(-ZOMBIE_VELOCITY);
            setDY(-ZOMBIE_VELOCITY);
            setAngle(static_cast<double>(ZombieAngles::NORTHWEST));
            break;
        case ZombieDirection::DIR_U:
            setDX(0);
            setDY(-ZOMBIE_VELOCITY);
            setAngle(static_cast<double>(ZombieAngles::NORTH));
            break;
        case ZombieDirection::DIR_RU:
            setDX(ZOMBIE_VELOCITY);
            setDY(-ZOMBIE_VELOCITY);
            setAngle(static_cast<double>(ZombieAngles::NORTHEAST));
            break;
        case ZombieDirection::DIR_INVALID:  // Shouldn't ever happens, gets rid of warning
            break;
    }

    // Frames are used to make sure the zombie doesn't move through the path too quickly/slowly
    if (frame > 0) {
        --frame;
    } else {
        setCurFrame(ZOMBIE_FRAMES);
        ++step;
    }

    setCurDir(direction);
    setState(ZombieState::ZOMBIE_MOVE);
}

/**
 * A* algo generates a string of direction digits.
 * Fred Yang
 * Feb 14
 */
string Zombie::generatePath(const float xStart, const float yStart,
        const float xDest, const float yDest) {
    // priority queue index
    int index = 0;

    // temp index
    int i;
    int j;

    // row / column index
    int x;
    int y;
    int xdx;
    int ydy;

    // path to be generated
    string path;

    // priority queue
    static array<priority_queue<Node>, 2> pq;

    // reset the node maps
    memset(closedNodes, 0, sizeof(closedNodes[0][0]) * ROW * COL);
    memset(openNodes, 0, sizeof(openNodes[0][0]) * ROW * COL);

    const int xNodeStart = static_cast<int>(xStart / TILE_SIZE);
    const int yNodeStart = static_cast<int>(yStart / TILE_SIZE);
    const int xNodeDest = static_cast<int>(xDest / TILE_SIZE);
    const int yNodeDest = static_cast<int>(yDest / TILE_SIZE);

    // create the start node and push the start node into open list
    Node curNode(xNodeStart, yNodeStart);
    curNode.updatePriority(xNodeDest, yNodeDest);
    pq[index].push(curNode);

    // A* path finding
    while (!pq[index].empty()) {
        // get the current node with the highest priority from open list
        curNode = pq[index].top();

        x = curNode.getXPos();
        y = curNode.getYPos();

        // remove the node from the open list
        pq[index].pop();
        openNodes[x][y] = 0;

        // mark it on the closed nodes map
        closedNodes[x][y] = 1;

        // quit searching when the destination is reached
        if (x == xNodeDest && y == yNodeDest) {
            // generate the path from destination to start
            // by following the directions
            path = "";
            while (!(x == xNodeStart && y == yNodeStart)) {
                j = dirMap[x][y];
                path = static_cast<char>('0' + (j + DIR_CAP / 2) % DIR_CAP) + path;
                x += MX[j];
                y += MY[j];
            }

            // empty the leftover nodes
            pq[index] = priority_queue<Node>();

            setPath(path);
            return path;
        }

        // traverse neighbors
        for (i = 0; i < DIR_CAP;i++) {
            // neighbor coordinates
            xdx = x + MX[i];
            ydy = y + MY[i];

            // not evaluated & not outside (bound checking)
            if (!(xdx < 0 || xdx > COL - 1 || ydy < 0 || ydy > ROW - 1
                    || gameMap[xdx][ydy] == 1 || closedNodes[xdx][ydy] == 1)) {

                // generate a child node
                Node childNode(xdx, ydy, curNode.getLevel(), curNode.getPriority());
                childNode.nextLevel(i);
                childNode.updatePriority(xNodeDest, yNodeDest);

                // if it is not in the open list then add into that
                if (openNodes[xdx][ydy] == 0) {
                    openNodes[xdx][ydy] = childNode.getPriority();
                    pq[index].push(childNode);
                    // update the parent direction info
                    dirMap[xdx][ydy] = (i + DIR_CAP / 2) % DIR_CAP;
                } else if (openNodes[xdx][ydy] > childNode.getPriority()) {
                    // update the priority info
                    openNodes[xdx][ydy]= childNode.getPriority();
                    // update the parent direction info
                    dirMap[xdx][ydy] = (i + DIR_CAP / 2) % DIR_CAP;

                    // use a queue and a backup queue to put the best node (with highest priority)
                    // on the top of the queue, which can be chosen later on to build the path.
                    while (!(pq[index].top().getXPos() == xdx &&
                           pq[index].top().getYPos() == ydy)) {
                        pq[1-index].push(pq[index].top());
                        pq[index].pop();
                    }

                    pq[index].pop();

                    // switch to pq with smaller size
                    if (pq[index].size() > pq[1 - index].size()) {
                        index = 1 - index;
                    }

                    while (!pq[index].empty()) {
                        pq[1 - index].push(pq[index].top());
                        pq[index].pop();
                    }
                    index = 1 - index;
                    pq[index].push(childNode);
                }
            }
        }
    }

    return ""; // no route found
}
