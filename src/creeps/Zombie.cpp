#include "Node.h"
#include "Zombie.h"
#include <utility>
using namespace std;

Zombie::Zombie(int health, ZombieState state, int step, int dir, int frame)
      : Movable(ZOMBIE_VELOCITY), health(health), state(state), step(step),
        dir(dir), frame(frame) {
    printf("Create Zombie\n");
}

Zombie::~Zombie() {
    //printf("Destory Zombie\n");
}

/**
 * Get steps taken
 * Fred Yang
 * Feb 14
 */
int Zombie::getStep() const {
    return step;
}

/**
 * Set steps taken
 * Fred Yang
 * Feb 14
 */
void Zombie::setStep(const int sp) {
    step = sp;
}

/**
 * Get state
 * Fred Yang
 * March 14
 */
ZombieState Zombie::getState() const {
    return state;
}

/**
 * Set state
 * Fred Yang
 * March 14
 */
void Zombie::setState(const ZombieState state_) {
    state = state_;
}

/**
 * Get current frame
 * Fred Yang
 * March 14
 */
int Zombie::getCurFrame() const {
    return frame;
}

/**
 * Set frame
 * Fred Yang
 * March 14
 */
void Zombie::setCurFrame(const int frm) {
    frame = frm;
}

/**
 * Get current direction
 * Fred Yang
 * March 14
 */
int Zombie::getCurDir() const {
    return dir;
}

/**
 * Set direction
 * Fred Yang
 * March 14
 */
void Zombie::setCurDir(const int d) {
    dir = d;
}

/**
 * Get move direction
 * Fred Yang
 * February 14
 */
int Zombie::getMoveDir() {
    if (frame > 0) {
        return dir;
    }

    int sp = getStep();
    string pth = getPath();
    /*
    cout << "path: " << pth << endl;
    cout << sp << '-' << pth.length() << endl;
    */

    return (sp < (int) pth.length() ? stoi(pth.substr(sp,1)) : -1);
}

/**
 * Set A* path
 * Fred Yang
 * Feb 14
 */
void Zombie::setPath(const string pth) {
    path = pth;
}

/**
 * Get A* path
 * Fred Yang
 * Feb 14
 */
string Zombie::getPath() const {
    return path;
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
bool Zombie::isMoving() {
    return (state == ZOMBIE_MOVE);
}

/*
 * Does a check to see if the zombie already arrived at the base.
 * Robert Arendac, Fred Yang
 * March 7
*/
bool Zombie::checkBase() {
    float curX = getX();
    float curY = getY();
    float baseX = MAP_WIDTH / 2 - BASE_WIDTH;
    float baseY = MAP_HEIGHT / 2 - BASE_HEIGHT;

    return overlapped(curX, curY, ZOMBIE_WIDTH, ZOMBIE_HEIGHT,
                      baseX, baseY, BASE_WIDTH, BASE_HEIGHT, OVERLAP);
}

/**
 * Get the direction of the zombie and take a step in the appropriate direction
 * Rob, Fred
 * March 13
*/
void Zombie::generateMove() {
    int d = getMoveDir();   //Direction zombie is moving
    //cout << "move dir: " << d << " state: " << state << " Frame: " << frame << endl;
    float startX = getX();
    float startY = getY();

    // Path is empty, shouldn't move
    if (d < 0 || checkBase()) {
        if (frame > 0) {
            frame--;
        }

        if (state != ZOMBIE_IDLE) {
            setState(ZOMBIE_IDLE);
        }

        return;
    }

    // Each case will check if the zombie is within bounds before moving
    switch(d) {
        case DIR_R:
            if (checkBounds(startX + ZOMBIE_VELOCITY, startY)) {
                setDX(ZOMBIE_VELOCITY);
                setDY(0);
                setAngle(EAST);
            }
            break;
        case DIR_RD:
            if (checkBounds(startX + ZOMBIE_VELOCITY, startY + ZOMBIE_VELOCITY)) {
                setDX(ZOMBIE_VELOCITY);
                setDY(ZOMBIE_VELOCITY);
                setAngle(SOUTHEAST);
            }
            break;
        case DIR_D:
            if (checkBounds(startX, startY + ZOMBIE_VELOCITY)) {
                setDX(0);
                setDY(ZOMBIE_VELOCITY);
                setAngle(SOUTH);
            }
            break;
        case DIR_LD:
            if (checkBounds(startX - ZOMBIE_VELOCITY, startY + ZOMBIE_VELOCITY)) {
                setDX(-ZOMBIE_VELOCITY);
                setDY(ZOMBIE_VELOCITY);
                setAngle(SOUTHWEST);
            }
            break;
        case DIR_L:
            if (checkBounds(startX - ZOMBIE_VELOCITY, startY)) {
                setDX(-ZOMBIE_VELOCITY);
                setDY(0);
                setAngle(WEST);
            }
            break;
        case DIR_LU:
            if (checkBounds(startX - ZOMBIE_VELOCITY, startY - ZOMBIE_VELOCITY)) {
                setDX(-ZOMBIE_VELOCITY);
                setDY(-ZOMBIE_VELOCITY);
                setAngle(NORTHWEST);
            }
            break;
        case DIR_U:
            if (checkBounds(startX, startY - ZOMBIE_VELOCITY)) {
                setDX(0);
                setDY(-ZOMBIE_VELOCITY);
                setAngle(NORTH);
            }
            break;
        case DIR_RU:
            if (checkBounds(startX + ZOMBIE_VELOCITY, startY - ZOMBIE_VELOCITY)) {
                setDX(ZOMBIE_VELOCITY);
                setDY(-ZOMBIE_VELOCITY);
                setAngle(NORTHEAST);
            }
            break;
    }

    if (frame > 0) {
        frame--;
    } else {
        setCurFrame(ZOMBIE_FRAMES);
        step++;
    }

    setCurDir(d);
    if (state != ZOMBIE_MOVE) {
        setState(ZOMBIE_MOVE);
    }
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
    int i, j;

    // row / column index
    int x, y, xdx, ydy;

    // temp char
    char c;

    // path to be generated
    string path;

    // current node & child node
    static Node curNode;
    static Node childNode;

    // priority queue
    static priority_queue<Node> pq[2];

    // reset the node maps
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            closedNodes[i][j] = 0;
            openNodes[i][j] = 0;
        }
    }

    int xNodeStart = (int) xStart / TILE_SIZE;
    int yNodeStart = (int) yStart / TILE_SIZE;
    int xNodeDest = (int) xDest / TILE_SIZE;
    int yNodeDest = (int) yDest / TILE_SIZE;

    // create the start node and push into open list
    curNode = Node(xNodeStart, yNodeStart, 0, 0);
    curNode.updatePriority(xNodeDest, yNodeDest);
    pq[index].push(curNode);

    // A* path finding
    while (!pq[index].empty()) {
        // get the current node with the highest priority from open list
        curNode = Node(pq[index].top().getXPos(), pq[index].top().getYPos(),
                           pq[index].top().getLevel(), pq[index].top().getPriority());

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
                c = '0' + (j + DIR_CAP/2)%DIR_CAP;
                path = c + path;
                x += mx[j];
                y += my[j];
            }

            // empty the leftover nodes
            while (!pq[index].empty()) {
                 pq[index].pop();
             }

            setPath(path);
            return path;
        }

        // traverse neighbors
        for (i = 0; i < DIR_CAP;i++) {
            // neighbor coordinates
            xdx = x + mx[i];
            ydy = y + my[i];

            // not evaluated & not outside (bound checking)
            if (!(xdx < 0 || xdx > col -1 || ydy < 0 || ydy > row - 1
                || map[xdx][ydy] == 1 || closedNodes[xdx][ydy] == 1)) {

                // generate a child node
                childNode = Node(xdx, ydy, curNode.getLevel(), curNode.getPriority());
                childNode.nextLevel(i);
                childNode.updatePriority(xNodeDest, yNodeDest);

                // if it is not in the open list then add into that
                if (openNodes[xdx][ydy] == 0) {
                    openNodes[xdx][ydy] = childNode.getPriority();
                    pq[index].push(childNode);
                    // update the parent direction info
                    dirMap[xdx][ydy] = (i + DIR_CAP/2)%DIR_CAP;
                } else if (openNodes[xdx][ydy] > childNode.getPriority()) {
                    // update the priority info
                    openNodes[xdx][ydy]= childNode.getPriority();
                    // update the parent direction info
                    dirMap[xdx][ydy] = (i + DIR_CAP/2)%DIR_CAP;

                    // use a queue and a backup queue to put the best node (with highest priority)
                    // on the top of the queue, which can be chosen later on to build the path.
                    while (!(pq[index].top().getXPos() == xdx &&
                           pq[index].top().getYPos() == ydy)) {
                        pq[1-index].push(pq[index].top());
                        pq[index].pop();
                    }

                    pq[index].pop();

                    // switch to pq with smaller size
                    if (pq[index].size() > pq[1-index].size()) {
                        index = 1 - index;
                    }

                    while (!pq[index].empty()) {
                        pq[1-index].push(pq[index].top());
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

/**
 * Check to see if the zombie is still within the screen bounds before moving
 * Fred Yang
 * Feb 14
 */
bool Zombie::checkBounds(const float x, const float y) const {
    return (!(x < 0 || x > MAP_WIDTH || y < 0 || y > MAP_HEIGHT));
}

/**
 * Check to see if two squares overlapped
 */
bool Zombie::overlapped(const float x1, const float y1,
                        const int w1, const int h1,
                        const float x2, const float y2,
                        const int w2, const int h2,
                        const float overlap) {
    int xb = 0; // big x
    int yb = 0; // big y
    int xs = 0; // small x
    int ys = 0; // small y
    int width = 0;
    int height = 0;

    if (x1 >= x2) {
        xb = x1;
        xs = x2;
        width = w2;
    } else {
        xb = x2;
        xs = x1;
        width = w1;
    }

    if (y1 >= y2) {
        yb = y1;
        ys = y2;
        height = h2;
    } else {
        yb = y2;
        ys = y1;
        height = h1;
    }

    if (xb >= xs && xb <= xs+width-1 && yb >= ys && yb <= ys+height-1) {
        float dWidth = width-xb+xs;
        float dHeight = height-yb+ys;

        if (dWidth * dHeight / (w2 * h2) >= OVERLAP) {
            return true;
        }
    }

    return false;
}
