#include "Node.h"
#include "Zombie.h"
using namespace std;

Zombie::Zombie()  {
	zombieSpriteClips[0].x = 0;
	zombieSpriteClips[0].y = 0;
	zombieSpriteClips[0].w = 100;
	zombieSpriteClips[0].h = 100;
	this->movementHitBox.setRect(zombieSpriteClips[0]);
	this->movementHitBox.move(this->getX(), this->getY());
	this->movementHitBox.attached = this;
	this->projectileHitBox.setRect(zombieSpriteClips[0]);
	this->projectileHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.attached = this;
	this->damageHitBox.setRect(zombieSpriteClips[0]);
	this->damageHitBox.move(this->getX(), this->getY());
	this->damageHitBox.attached = this;

	this->setAngle(this->getRandomAngle());
	printf("Create Zombie\n");
}

Zombie::~Zombie() {
	printf("Destory Zombie\n");
}

// Move Zombie by x and y amount
void Zombie::move(float moveX, float moveY, CollisionHandler* ch) {
	//Move the Zombie left or right
	this->setX(this->getX()+moveX);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());

	if (ch->detectMovementCollision(&this->movementHitBox)) {
		this->setX(this->getX()-moveX);
		this->movementHitBox.move(this->getX(), this->getY());
		this->projectileHitBox.move(this->getX(), this->getY());
	}

	//Move the Zombie up or down
	this->setY(this->getY()+moveY);
	this->movementHitBox.move(this->getX(), this->getY());
	this->projectileHitBox.move(this->getX(), this->getY());

	if (ch->detectMovementCollision(&this->movementHitBox)) {
		this->setY(this->getY()-moveY);
		this->movementHitBox.move(this->getX(), this->getY());
		this->projectileHitBox.move(this->getX(), this->getY());
	}

}

// Set delta x coordinate
void Zombie::setDX(float px) {
	dx = px;
}

// Set delta y coordinate
void Zombie::setDY(float py) {
	dy = py;
}

// set velocity of Zombie movement
void Zombie::setVelocity(int pvel) {
	velocity = pvel;
}

// Get delta x coordinate
float Zombie::getDX() {
	return dx;
}

// Get delta y coordinate
float Zombie::getDY() {
	return dy;
}

// Get step
int Zombie::getStep() {
    return step;
}

// Set step
void Zombie::setStep(int sp) {
    step = sp;
}

// Get move direction
int Zombie::getDir() {
    int sp = this->getStep();
    string pth = this->getPath();
    //char c = pth.at(sp);
    return (sp < (int) pth.length() ? stoi(pth.substr(sp,1)) : -1);
}

// Set path
void Zombie::setPath(string pth) {
    path = pth;
}

// Get path
string Zombie::getPath() {
    return path;
}

// Get velocity of Zombie movement
int Zombie::getVelocity() {
	return velocity;
}

void Zombie::onCollision() {
	// Do nothing for now
}

void Zombie::collidingProjectile(int damage) {
	this->health = health - damage;
}

//sets the angle of zombie's sprite
void Zombie::setAngle(double a){
    angle = a;
}

//returns the angle of the player's marine sprite
double Zombie::getAngle(){
    return angle;
}

int Zombie::getRandomAngle(){
	//random number generator 
	std::random_device rd;
	std::mt19937 eng(rd());

	//range 0 to 360 degrees for zombie's angle
	std::uniform_int_distribution<> distr(0,360);

	return distr(eng);
}

void Zombie::generateRandomMove(){
	int tx = this->getX()+50;
	int ty = this->getY()+50;
	if(tx<=0 || tx>=2000 || ty<=0 || ty>=2000){
		this->setAngle(this->getRandomAngle());
	}
		
	double cosVal = cos(this->getAngle()*PI/180.0);
	double sinVal = sin(this->getAngle()*PI/180.0);

	int x = this->getVelocity();
	int y = this->getVelocity();
	
	this->setDX(x*cosVal);
	this->setDY(y*sinVal);
}

void Zombie::generateMove() {
    int d = this->getDir();
    if (d < 0) return;
    
    float x = this->getDX();
    float y = this->getDY();
    
    switch(d) {
        case DIR_R:
            if (checkBound(x + STEP_SPAN, y)) {
                this->setDX(x + STEP_SPAN);
                this->step++;
            }
            break;
        case DIR_RD:
            if (checkBound(x + STEP_SPAN, y + STEP_SPAN)) {
                this->setDX(x + STEP_SPAN);
                this->setDY(y + STEP_SPAN);
                this->step++;
            }
            break;
        case DIR_D:
            if (checkBound(x, y + STEP_SPAN)) {
                this->setDY(y + STEP_SPAN);
                this->step++;
            }
            break;
        case DIR_LD:
            if (checkBound(x - STEP_SPAN, y + STEP_SPAN)) {
                this->setDX(x - STEP_SPAN);
                this->setDY(y + STEP_SPAN);
                this->step++;
            }
            break;
        case DIR_L:
            if (checkBound(x - STEP_SPAN, y)) {
                this->setDX(x - STEP_SPAN);
                this->step++;
            }
            break;
        case DIR_LU:
            if (checkBound(x - STEP_SPAN, y - STEP_SPAN)) {
                this->setDX(x - STEP_SPAN);
                this->setDY(y - STEP_SPAN);
                this->step++;
            }
            break;
        case DIR_U:
            if (checkBound(x, y - STEP_SPAN)) {
                this->setDY(y - STEP_SPAN);
                this->step++;
            }
            break;
        case DIR_RU:
            if (checkBound(x + STEP_SPAN, y - STEP_SPAN)) {
                this->setDX(x + STEP_SPAN);
                this->setDY(y - STEP_SPAN);
                this->step++;
            }
            break;
        default:
            break;
    }
}

// A* algo generates a string of direction digits.
string Zombie::generatePath(const int& xStart, const int& yStart, 
                            const int& xDest, const int& yDest) {
    // priority queue index
    int index = 0;
    
    // temp index
    int i, j;
    
    // row / column index
    int x, y, xdx, ydy;
    
    // temp char
    char c;
    
    // path
    string path;
    
    // current node & child node
    static Node* curNode;
    static Node* childNode;
    
    // priority queue
    static priority_queue<Node> pq[2];

    // reset the node maps
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
        {
            closedNodes[i][j] = 0;
            openNodes[i][j] = 0;
        }
    }

    // create the start node and push into open list
    curNode = new Node(xStart, yStart, 0, 0);
    curNode->updatePriority(xDest, yDest);
    pq[index].push(*curNode);

    // A* path finding
    while(!pq[index].empty())
    {
        // get the current node with the highest priority
        // from open list
        curNode = new Node(pq[index].top().getXPos(), pq[index].top().getYPos(), 
                           pq[index].top().getLevel(), pq[index].top().getPriority());

        x = curNode->getXPos(); 
        y = curNode->getYPos();

        // remove the node from the open list
        pq[index].pop(); 
        openNodes[x][y] = 0;
        
        // mark it on the closed nodes map
        closedNodes[x][y] = 1;

        // quit searching when the destination is reached
        if(x == xDest && y == yDest) 
        {
            // generate the path from destination to start
            // by following the directions
            path = "";
            while(!(x == xStart && y == yStart))
            {
                j = dirMap[x][y];
                c = '0' + (j + DIR_CAP/2)%DIR_CAP;
                path = c + path;
                x += mx[j];
                y += my[j];
            }
            // garbage collection
            delete curNode;
            
            // empty the leftover nodes
            while(!pq[index].empty()) pq[index].pop();
            setPath(path);
            return path;
        }

        // traverse neighbors
        for(i = 0; i < DIR_CAP;i++)
        {
            // neighbor coordinates
            xdx = x + mx[i]; 
            ydy = y + my[i];

            // not evaluated & not outside (bound checking)
            if(!(xdx < 0 || xdx > col -1 || ydy < 0 || ydy > row - 1 
                || map[xdx][ydy] == 1 || closedNodes[xdx][ydy] == 1))
            {
                // generate a child node
                childNode = new Node(xdx, ydy, curNode->getLevel(), curNode->getPriority());
                childNode->nextLevel(i);
                childNode->updatePriority(xDest, yDest);
                
                // if it is not in the open list then add into that
                if(openNodes[xdx][ydy] == 0)
                {
                    openNodes[xdx][ydy] = childNode->getPriority();
                    pq[index].push(*childNode);
                    // update the parent direction info
                    dirMap[xdx][ydy] = (i + DIR_CAP/2)%DIR_CAP;
                }
                else if(openNodes[xdx][ydy] > childNode->getPriority())
                {
                    // update the priority info
                    openNodes[xdx][ydy]= childNode->getPriority();
                    // update the parent direction info
                    dirMap[xdx][ydy] = (i + DIR_CAP/2)%DIR_CAP;

                    // use a queue and a backup queue to put the best node (with highest priority)
                    // on the top of the queue, which can be chosen later on to build the path.
                    while(!(pq[index].top().getXPos() == xdx && 
                           pq[index].top().getYPos() == ydy))
                    {                
                        pq[1-index].push(pq[index].top());
                        pq[index].pop();       
                    }

                    pq[index].pop();
                    
                    // switch to pq with smaller size
                    if(pq[index].size() > pq[1-index].size()) 
                    {
                        index = 1 - index;
                    }

                    while(!pq[index].empty())
                    {                
                        pq[1-index].push(pq[index].top());
                        pq[index].pop();       
                    }
                    index = 1 - index;
                    pq[index].push(*childNode);
                }
                else delete childNode;
            }
        }
        delete curNode;
    }
    
    return ""; // no route found
}

bool Zombie::checkBound(float x, float y) {
    if (x < 0 || x > SCREEN_W || y < 0 || y > SCREEN_H) 
        return false;
    return true;
}
