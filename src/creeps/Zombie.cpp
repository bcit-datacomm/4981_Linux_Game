/*------------------------------------------------------------------------------
* Source: Zombie.cpp
*
* Functions:
*
*
* Date:
*
* Revisions:
* Edited By : Yiaoping Shu- Style guide
*
* Designer:
*
* Author:
*
* Notes:
*
------------------------------------------------------------------------------*/
#include <math.h>
#include <random>
#include <cassert>
#include <utility>
#include <math.h>
#include "Zombie.h"
#include "../game/GameManager.h"
#include "../log/log.h"
#include "../sprites/VisualEffect.h"
#include <cstdlib>
using namespace std;

Zombie::Zombie(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& projectileSize,
        const SDL_Rect& damageSize, const int health) : Entity(id, dest, movementSize, projectileSize,
        damageSize), Movable(id, dest, movementSize, projectileSize, damageSize, ZOMBIE_VELOCITY), health(health),
        frameCount(0), moveAngle(0.0), ignore(0){
    logv("Create Zombie\n");
    inventory.initZombie();
}

Zombie::~Zombie() {
    logv("Destroy Zombie\n");
}

void Zombie::update(){
    //Movement updates
    GameManager *gm = GameManager::instance();
    const auto& base = gm->getBase();
    const auto& marines = gm->getMarineManager();

    //the difference in zombie to target distance
    float movX;
    float movY;
    
    //middle of me
    const int midMeX = getX() + (getW() / 2);
    const int midMeY = getY() + (getH() / 2);
    
    //middle of the base
    const int midBaseX = base.getX() + (base.getW() / 2);
    const int midBaseY = base.getY() + (base.getH() / 2);

    //temp x and y for calculating the hypot
    int hypX;
    int hypY;

    //hypo variables
    float hyp = 0.0;
    float temp;

    //who is closest?
    for (const auto& m : marines){
        hypX = m.second.getX() + (m.second.getW() / 2);
        hypY = m.second.getY() + (m.second.getH() / 2);

        //we only want the closest one
        if(!hyp || (temp = hypot(hypX - midMeX, hypY - midMeY)) < hyp){
            hyp = temp;
            movX = hypX - midMeX;
            movY = hypY - midMeY;
        }
    }

    //only change the angle a max of 4 times a second and its not being ignored
    if (!ignore) {
        if (hyp > ZOMBIE_SIGHT) {
            movX = midBaseX - midMeX;
            movY = midBaseY - midMeY;
        }
        moveAngle = atan2(movX, movY);
    } else if (ignore > 0){
        --ignore;
    }

    //get the distance of 
    setDX(ZOMBIE_VELOCITY * sin(moveAngle));
    setDY(ZOMBIE_VELOCITY * cos(moveAngle));

    VisualEffect::instance().addPreLine(2, midMeX, midMeY, midMeX + ZOMBIE_SIGHT * sin(moveAngle),
        midMeY + ZOMBIE_SIGHT * cos(moveAngle), 0, 0, 0);

    //Attack updates
    if (!(frameCount % CHECK_RATE)){
        zAttack();
    }
}

void Zombie::move(const float moveX, const float moveY, CollisionHandler& ch) {

    //Move the Movable left or right
    setX(getX() + moveX);

    //if there is a collision with anything with a movement hitbox, move it back
    if (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeMarine,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeZombie,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeWall,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeBarricade,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeTurret,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeObj,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeStore,this),this)) {
        setX(getX() - moveX);
        moveAngle += (M_PI / -8) * rand();
        ignore = 15;
    }

    //Move the Movable up or down
    setY(getY()+moveY);

    //if there is a collision with anything with a movement hitbox, move it back
    if (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeMarine,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeZombie,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeWall,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeBarricade,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeTurret,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeObj,this),this)
            || ch.detectMovementCollision(ch.getQuadTreeEntities(ch.quadtreeStore,this),this)) {
        setY(getY() - moveY);
        moveAngle += (M_PI / 8) * rand();
        ignore = 15;
    }
}

void Zombie::collidingProjectile(int damage) {
    health -= damage;
    if (health <= 0) {
        GameManager::instance()->deleteZombie(getId());
    }
}

/**
 * Date: Mar. 28, 2017
 * Author: Mark Tattrie
 * Function Interface: void Zombie::zAttack()
 * Description:
 * Calls the zombies current weapon "ZombieHands" to fire
 */
void Zombie::zAttack(){
    Weapon* w = inventory.getCurrent();
    if (w){
        w->fire(*this);
    } else {
        logv("Zombie Slot Empty\n");
    }
}

