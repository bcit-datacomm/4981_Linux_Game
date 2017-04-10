/**
 * Source: Turret.cpp
 *
 * Functions: void placementCheckTurret()
 *            bool collisionCheckTurret(const float , const float , const float , const float , CollisionHandler &)
 *            void activateTurret()
 *            void collidingProjectile(const int damage)
 *            void damageTurret(const int damage)
 *            void shootTurret()
 *            bool ammoCheckTurret()
 *            bool healthCheckTurret()
 *            bool targetScanTurret()
 *            void move(const float playerX, const float playerY,
 *                    const float moveX, const float moveY, CollisionHandler &ch)
 *            void placeTurret()
 *            bool isPlaced() {return placed;}
 *            void pickUpTurret()
 *
 * Date: Feb. 02, 2017
 *
 * Designer: Mark Chen, Jaime Lee, Terry Kang
 *
 * Author: Mark Chen, Michael Goll, Jacob McPhail, Isaac Morneau, Maitiu Morton, Mark Tattrie
 *
 * Revisions:
 * Edited By : Yiaoping Shu- Style guide
 *
 * Notes:
 * Source file for the turret class. This file defines all the functions associated with a turret.
 */
#include <cassert>

#include "Turret.h"
#include "../game/GameManager.h"
#include "../log/log.h"
//for the angle update rate
#include "../creeps/Zombie.h"

/**
 * Date: Feb. 02, 2017
 *
 * Designer: Mark Chen
 *
 * Programmer: Mark Chen, Michael Goll, Maitiu Morton
 *
 * Function Interface: Turret(int32_t id, const SDL_Rect dest, const SDL_Rect &movementSize,
 *       const SDL_Rect &projectileSize, const SDL_Rect &damageSize, const SDL_Rect &pickupSize,
 *       bool activated, int health, int ammo, bool placed)
 *
 * Description:
 * Constructor for a turret, creates a turret on the map.
 *
 * Revisions:
 * Mar. 15, 2017, Mark Chen : Changed parameters that the constructor takes.
 * Mar. 15, 2017, Matiu Morton : Changed parameteres that the constructor takes.
 * Mar. 15, 2017, Micheal Goll : Fixed rendereing issues with turret images.
 * Mar. 16, 2017, Mark Chen : Changed parameters that the constructor takes.
 * Mar. 30, 2017, Mark Chen : Turret now has an inventory.
 * Apr. 02, 2017, Mark Chen : Turret now has a dropzone value.
 * Apr. 07, 2017, Isaac Morneau : Turret checks more efficiently
 */

Turret::Turret(const int32_t id, const SDL_Rect& dest, const SDL_Rect& movementSize, const SDL_Rect& projectileSize,
        const SDL_Rect& damageSize, const SDL_Rect& pickupSize, const bool activated, const int health,
        const bool placed, const float range, const int32_t dropzone): Entity(id, dest, movementSize,
        projectileSize, damageSize, pickupSize), Movable(id, dest, movementSize, projectileSize, damageSize,
        pickupSize, MARINE_VELOCITY), activated(activated), placed(placed), range(range) {
    //movementHitBox.setFriendly(true); Uncomment to allow movement through other players
    //projectileHitBox.setFriendly(true); Uncomment for no friendly fire
    //damageHitBox.setFriendly(true); Uncomment for no friendly fire
    inventory.makeTurretInv();
    logv("Turret created\n");
}

/**
 * Date: Feb. 02, 2017
 *
 * Designer: Mark Chen
 *
 * Programmer: Mark Chen
 *
 * Function Interface: ~Turret()
 *
 * Description:
 * Destructor for the turret object, destroys a turret from the map.
 */

Turret::~Turret() {
    logv("Destroy Turret\n");
}

/**
 * Date: Feb. 02, 2017
 *
 * Designer: Mark Chen
 *
 * Programmer: Mark Chen
 *
 * Function Interface: bool placementCheckTurret()
 *
 * Description:
 * Checks if the turret is placed within the bounds of the map. The function will return true if the
 * turret is within bounds and false if it falls outside the bounds.
 */
bool Turret::placementCheckTurret(){
    return true;
}

/**
 * Date: Mar. 10, 2017
 *
 * Designer: Mark Chen, Terry Kang
 *
 * Programmer: Mark Chen, Mark Tattrie
 *
 * Function Interface: bool collisionCheckTurret(const float playerX, const float playerY, const float moveX,
 *       const float moveY, CollisionHandler &ch)
 *
 * Description:
 * Checks whether the area where the user selects to place the turret is valid. The following are the two
 * requirements:
 * 1. distance must be within 200 pixels of where player is currently located
 * 2. no existing objects with a hitbox must be on the location selected
 *
 * If both requirements are met, then the function returns true. If either or both the requirements are false,
 * then the function returns false.
 *
 * Revisions:
 * Mar. 15, 2017, Mark Chen : Changed turret collision to work similar to barricades.
 * Mar. 15, 2017, Mark Tattrie : Changed the checks for hitboxes.
 * Mar. 16, 2017, Mark Chen : General formatting changes.
 */
bool Turret::collisionCheckTurret(const float playerX, const float playerY, const float moveX,
        const float moveY, CollisionHandler& ch) {
    SDL_Rect checkBox;

    checkBox.h = TURRET_HEIGHT;
    checkBox.w = TURRET_WIDTH;
    checkBox.x = moveX;
    checkBox.y = moveY;
    HitBox hitBox(checkBox);
    const float distanceX = (playerX - moveX) * (playerX - moveX);
    const float distanceY = (playerY - moveY) * (playerY - moveY);
    const float distance = sqrt(abs(distanceX + distanceY));

    return (distance <= 200 && (!ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getMarineTree(),this), this)
        && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getZombieTree(),this), this)
        && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getBarricadeTree(),this), this)
        && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getWallTree(),this), this)
        && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getTurretTree(),this), this)
        && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getObjTree(),this), this)
        && !ch.detectMovementCollision(ch.getQuadTreeEntities(ch.getPickUpTree(),this), this)));
}

/**
 * Date: Feb. 02, 2017
 *
 * Designer: Mark Chen
 *
 * Programmer: Mark Chen, Mark Tattrie
 *
 * Function Interface: void collidingProjectile(const int damage)
 *
 * Description:
 * Damages the turret by the amount of 'damage' parameter passed in.
 *
 * Revisions:
 * Mar. 31, 2017, Mark Tattrie : Turret now takes damage and dies when health is <= 0.
 */
void Turret::collidingProjectile(const int damage) {
    health -= damage;
    //turret health is less then or equal to 0
    if(!healthCheckTurret()){
        GameManager::instance()->deleteTurret(getId());
    }
}

/**
 * Date: Feb. 02, 2017
 *
 * Designer: Mark Chen
 *
 * Programmer: Mark Chen
 *
 * Function Interface: void shootTurret()
 *
 * Description:
 * Makes the turret shoot in a direction.
 *
 * Revisions:
 * Mar. 30, 2017, Mark Chen : Made the function actually fire the turrets weapon.
 * Apr. 04, 2017, Mark Chen : Altered the funuction to check for null weapon.
 */
void Turret::shootTurret() {

    Weapon *w = inventory.getCurrent();
    if (w) {
        w->fire(*this);
    }
}

/**
 * Date: Mar. 10, 2017
 *
 * Designer: Mark Chen, Terry Kang
 *
 * Programmer: Mark Chen
 *
 * Function Interface: void move(const float playerX, const float playerY,
         const float moveX, const float moveY, CollisionHandler &ch)
         *
 * Description:
 * Changes the transparency of the turret object that replaces the mouse cursor when the player
 * is holding a turret. The turret is transparency is changed depending on whether the location
 * is a valid location.
 *
 * Revisions:
 * Mar. 15, 2017, Mark Chen: Function now takes in a collision handler.
 * Mar. 16, 2017, Mark Chen: Formatting issues resolved.
 */
void Turret::move(const float playerX, const float playerY,
        const float moveX, const float moveY, CollisionHandler& ch) {

    setPosition(moveX, moveY);

    if (collisionCheckTurret(playerX, playerY, moveX, moveY, ch)) {
        //change the texture rendered for the turret here
        //left blank for now
    } else {
        //change the texture rendered for the turret here
        //left blank for now
    }
}

/**
 * Date: Feb. 02, 2017
 *
 * Modified: Mar. 15, 2017 - Mark Chen
 * Designer: Mark Chen
 *
 * Programmer: Mark Chen
 *
 * Function Interface: void placeTurret()
 *
 * Description:
 * Sets the turret onto the map by changing the transparency of the object and changes the private
 * member 'placed' of the turret to 'true'.
 *
 * Revisions:
 * Mar. 15, 2017, Mark Chen: Made turret be in a 'placed' and activated state when placed on map.
 */
void Turret::placeTurret() {
    placed = true;
    activated = true;
}

/**
 * Date: Feb. 02, 2017
 *
 * Designer: Mark Chen
 *
 * Programmer: Mark Chen
 *
 * Function Interface: void pickUpTurret()
 *
 * Description:
 * Picks up the truret from the map, temporarily removing it from the map by changing its private
 * member 'placed' to false.
 *
 * Revisions:
 * Mar. 15, 2017, Mark Chen : Made turrets deactivated when picked up.
 */
void Turret::pickUpTurret() {
    placed = false;
    activated = false;
}

/**
 * Date: Mar. 01, 2017
 * Modified: Mar. 05, 2017 - Robert Arendac
 * Designer: Jamie Lee
 *
 * Programmer: Jamie Lee, Robert Arendac
 *
 * Function Interface: bool targetScanTurret()
 *
 * Description:
 * Checks if there are any enemies in the turret's coverage area.
 *
 * Revisions:
 * Mar. 05, 2017, Robert Arendac - General code clean up
 */
bool Turret::targetScanTurret() {
    ++frameCount;
    //middle of me
    const int midMeX = getX() + (getW() / 2);
    const int midMeY = getY() + (getH() / 2);
    const Entity visSection(0, {midMeX - ZOMBIE_SIGHT, static_cast<int>(midMeY - getRange()),
        static_cast<int>(2 * getRange()), static_cast<int>(2 * getRange())});

    if (!(frameCount % ANGLE_UPDATE_RATE)) {
        //Movement updates
        GameManager *gm = GameManager::instance();
        auto& collision = gm->getCollisionHandler();
        const auto& zombies = collision.getQuadTreeEntities(collision.getZombieTree(), &visSection);

        //the difference in zombie to target distance
        float movX;
        float movY;

        //temp x and y for calculating the hypot
        int hypX;
        int hypY;

        //hypo variables
        float hyp = getRange();
        float temp;

        //who is closest?
        for (const auto t : zombies){
            hypX = t->getX() + (t->getW() / 2);
            hypY = t->getY() + (t->getH() / 2);

            //we only want the closest one
            if((temp = hypot(hypX - midMeX, hypY - midMeY)) < hyp){
                hyp = temp;
                movX = hypX - midMeX;
                movY = hypY - midMeY;
            }
        }

        //invert the return of the arc tan
        if (hyp < getRange()) {
            setRadianAngle(fmod((-1 * atan2(movX, movY)) + M_PI, 2 * M_PI));
            return true;
        }
    }
    return false;
}
