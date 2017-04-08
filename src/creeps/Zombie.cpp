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
#include <cmath>
#include <random>
#include <cassert>
#include <utility>
#include "Zombie.h"
#include "../game/GameManager.h"
#include "../log/log.h"
#include "../sprites/VisualEffect.h"
#include <cstdlib>
using namespace std;

/**
 * Author: Robert Arendac
 *
 * Date: April 6, 2017
 */
Zombie::Zombie(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& projectileSize,
        const SDL_Rect& damageSize, const int health) : Entity(id, dest, movementSize, projectileSize,
        damageSize), Movable(id, dest, movementSize, projectileSize, damageSize, ZOMBIE_VELOCITY), health(health),
        frameCount(0), ignore(0){
    logv("Create Zombie\n");
    inventory.initZombie();
}

/**
 * Author: Robert Arendac
 *
 * Date: April 6, 2017
 */
Zombie::~Zombie() {
    logv("Destroy Zombie\n");
}
/**
 * Author: Robert Arendac
 *
 * Date: April 6, 2017
 */
void Zombie::update(){
    ++frameCount;
    //middle of me
    const int midMeX = getX() + (getW() / 2);
    const int midMeY = getY() + (getH() / 2);
    const Entity visSection(0, {midMeX - ZOMBIE_SIGHT, midMeY - ZOMBIE_SIGHT,
        2 * ZOMBIE_SIGHT, 2 * ZOMBIE_SIGHT});

    if (!(frameCount % ANGLE_UPDATE_RATE)) {
        //Movement updates
        GameManager *gm = GameManager::instance();
        const auto& base = gm->getBase();
        auto& collision = gm->getCollisionHandler();
        const auto& marines = collision.getQuadTreeEntities(collision.getMarineTree(), &visSection);
        const auto& turrrets = collision.getQuadTreeEntities(collision.getTurretTree(), &visSection);

        //the difference in zombie to target distance
        float movX;
        float movY;
        
        //middle of the base
        const int midBaseX = base.getX() + (base.getW() / 2);
        const int midBaseY = base.getY() + (base.getH() / 2);

        //temp x and y for calculating the hypot
        int hypX;
        int hypY;

        //hypo variables
        float hyp = ZOMBIE_SIGHT;
        float temp;

        //who is closest?
        for (const auto m : marines){
            hypX = m->getX() + (m->getW() / 2);
            hypY = m->getY() + (m->getH() / 2);

            //we only want the closest one
            if((temp = hypot(hypX - midMeX, hypY - midMeY)) < hyp){
                hyp = temp;
                movX = hypX - midMeX;
                movY = hypY - midMeY;
            }
        }
        for (const auto t : turrrets){
            hypX = t->getX() + (t->getW() / 2);
            hypY = t->getY() + (t->getH() / 2);

            //we only want the closest one
            if((temp = hypot(hypX - midMeX, hypY - midMeY)) < hyp){
                hyp = temp;
                movX = hypX - midMeX;
                movY = hypY - midMeY;
            }
        }

        //only change the angle a max of 4 times a second and its not being ignored
        if (!ignore) {
            //if no one was close lets go get the base!
            if (hyp >= ZOMBIE_SIGHT) {
                movX = midBaseX - midMeX;
                movY = midBaseY - midMeY;
            }
            //-1 converts from cartisian to screen coords
            setRadianAngle(fmod(atan2(movX, movY) + 2 * M_PI, 2 * M_PI));
        } else if (ignore > 0){
            --ignore;
        }
    }
    //get the distance of 
    setDX(ZOMBIE_VELOCITY * sin(getRadianAngle()));
    setDY(ZOMBIE_VELOCITY * cos(getRadianAngle()));

    //useful for figuring out where the zombies are tracking, it paints a line on where they are currently headed.
    //for example, right at you.
#ifndef NDEBUG
    VisualEffect::instance().addPreLine(2, midMeX, midMeY, midMeX + ZOMBIE_SIGHT * sin(getRadianAngle()),
        midMeY + ZOMBIE_SIGHT * cos(getRadianAngle()), 0, 0, 0);
#endif

    //Attack updates
    if (!(frameCount % CHECK_RATE)){
        zAttack();
    }
}

/**
 * Author: Jamie Lee
 *
 * Date: April 6, 2017
 */
void Zombie::move(const float moveX, const float moveY, CollisionHandler& ch) {
    static constexpr int IGNORE_TIME = 10;
    static constexpr int RANDOM_DEGREE = 5;
    //Move the Movable left or right
    setX(getX() + moveX);

    //if there is a collision with anything with a movement hitbox, move it back
    if (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getZombieMovementTree(),this),this)) {
        setX(getX() - moveX);
        setAngle(getAngle() + RANDOM_DEGREE * (1 - rand()));
        ignore = IGNORE_TIME;
    }

    //Move the Movable up or down
    setY(getY()+moveY);

    //if there is a collision with anything with a movement hitbox, move it back
    if (ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getZombieMovementTree(),this),this)) {
        setY(getY() - moveY);
        setAngle(getAngle() + RANDOM_DEGREE * (1 - rand()));
        ignore = IGNORE_TIME;
    }
}

/**
 * Author: Jamie Lee
 *
 * Date: April 6, 2017
 */
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


/**
* Date: April 6, 2017
*
* Designer: Trista Huang
*
* Programmer: Isaac Morneau
*
* Function Interface: void Marine::updateImageWalk(double frameCount)
*       double frameCount - counted frames while walking
*
* Description:
*       This function repeatedly updates the image of the marine in order to animate
*       walking. It is called from GameManager::updateMarines every frame.
*/
void Zombie::updateImageWalk() {
    const float dy = getDY();
    const float dx = getDX();
    if (dy > 0) {
        //stops lag when taking first step
        if (getSrcRect().x == SPRITE_FRONT) {
            setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
        } else if (frameCount % FRAME_COUNT_WALK == 0) {
            //cycle throught the walking images
            if (getSrcRect().x < SPRITE_NEXT_STEP) {
                setSrcRect(getSrcRect().x + SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            } else {
                setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            }
        }
    } else if (dy < 0) {
        if (getSrcRect().x == SPRITE_FRONT) {
            setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
        } else if (frameCount % FRAME_COUNT_WALK == 0) {
            if (getSrcRect().x < SPRITE_NEXT_STEP) {
                setSrcRect(getSrcRect().x + SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            } else {
                setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            }
        }
    } else if (dx < 0) {
        if (getSrcRect().x == SPRITE_FRONT) {
            setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
        } else if (frameCount % FRAME_COUNT_WALK == 0) {
            if (getSrcRect().x < SPRITE_NEXT_STEP) {
                setSrcRect(getSrcRect().x + SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            } else {
                setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            }
        }
    } else if (dx > 0) {
        if (getSrcRect().x == SPRITE_FRONT) {
            setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
        } else if (frameCount % FRAME_COUNT_WALK == 0) {
            if (getSrcRect().x < SPRITE_NEXT_STEP) {
                setSrcRect(getSrcRect().x +SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            } else {
                setSrcRect(SPRITE_SIZE_X, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
            }
        }
    } else {
        setSrcRect(SPRITE_FRONT, getSrcRect().y, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    }
}

/**
* Date: Mar 27
*
* Designer: Trista Huang
*
* Programmer: Fred Yang
*
* Interface: void Marine::updateImageDirection()
*
* Description:
*       This function changes the direction that the character is facing.
*       It is called from GameStateMatch::handle after every frame and
*       updates the direction of the sprite according to the angle of the
*       mouse from the center of the screen.
*/
void Zombie::updateImageDirection() {
    const double radians = getRadianAngle();

    //order: start from ~0 rad, counter clockwise
    if (radians > ZOMBIE_SPRITE_ANGLE1 && radians < ZOMBIE_SPRITE_ANGLE2) {
        setSrcRect(getSrcRect().x, SPRITE_FRONT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > ZOMBIE_SPRITE_ANGLE2 && radians < ZOMBIE_SPRITE_ANGLE3) {
        setSrcRect(getSrcRect().x, SPRITE_FRONT_RIGHT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > ZOMBIE_SPRITE_ANGLE3 && radians < ZOMBIE_SPRITE_ANGLE4) {
        setSrcRect(getSrcRect().x, SPRITE_RIGHT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > ZOMBIE_SPRITE_ANGLE4 && radians < ZOMBIE_SPRITE_ANGLE5) {
        setSrcRect(getSrcRect().x, SPRITE_BACK_RIGHT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > ZOMBIE_SPRITE_ANGLE5 && radians < ZOMBIE_SPRITE_ANGLE6) {
        setSrcRect(getSrcRect().x, SPRITE_BACK, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > ZOMBIE_SPRITE_ANGLE6 && radians < ZOMBIE_SPRITE_ANGLE7) {
        setSrcRect(getSrcRect().x, SPRITE_BACK_LEFT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > ZOMBIE_SPRITE_ANGLE7 && radians < ZOMBIE_SPRITE_ANGLE9) {
        setSrcRect(getSrcRect().x, SPRITE_LEFT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    } else if (radians > ZOMBIE_SPRITE_ANGLE8 && radians < ZOMBIE_SPRITE_ANGLE9) {
        setSrcRect(getSrcRect().x, SPRITE_FRONT_LEFT, SPRITE_SIZE_X, SPRITE_SIZE_Y);
    }
}
