#include <cmath>

#include "Player.h"
#include "../game/GameManager.h"
#include "../log/EntityDump.h"

/**
* Date: Jan. 28, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: Player()
* Description: 
*   ctor for a player.
*/
Player::Player() : tempBarricadeID(-1), tempTurretID(-1), holdingTurret(false), 
        pickupTick(0), pickupDelay(200), respawnTick(0), marine(nullptr) {
    moveAction.id = static_cast<int32_t>(UDPHeaders::WALK);
    attackAction.id = static_cast<int32_t>(UDPHeaders::ATTACKACTIONH);
}

/**
* Date: Jan. 28, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: setControl(Marine* newControl)
*       newControl : Marine to control
*
* Description: 
*   Set what marine the player controls.
*/
void Player::setControl(Marine* newControl) {
    marine = newControl;
}

/**------------------------------------------------------------------------------
Method: hasChangedAngle

Date: April 4, 2017

Designer: Brody McCrone

Programmer: Brody McCrone

Interface: bool hasChangedAngle()

Returns:
true: The players angle has changed since the last frame.
false: The players angle has NOT changed since the last frame.

Notes:
Checks if the players angle has changed since the last frame.
-------------------------------------------------------------------------------*/
bool Player::hasChangedAngle() const {
    return fabs(moveAction.data.ma.direction - marine->getAngle()) > DOUBLE_COMPARISON_PRECISION;
}

/**------------------------------------------------------------------------------
Method: hasChangedCourse

Date: April 4, 2017

Designer: Brody McCrone

Programmer: Brody McCrone

Interface: bool hasChangedCourse()

Returns:
true: The player's course has changed since the last frame.
false: The player's course has NOT changed since the last frame.

Notes:
Checks if the player's course has changed since the last frame.
-------------------------------------------------------------------------------*/
bool Player::hasChangedCourse() const {
    return moveAction.data.ma.xdel - marine->getDX()
            || moveAction.data.ma.ydel - marine->getDY();
}

/**------------------------------------------------------------------------------
Method: sendServMoveAction

Date: April 4, 2017

Designer: Brody McCrone

Programmer: Brody McCrone

Interface: bool sendServMoveAction

Returns:
void

Notes:
Updates the player's moveAction struct and send it to the server via UDP.
-------------------------------------------------------------------------------*/
void Player::sendServMoveAction() {
    moveAction.data.ma.id = id;
    moveAction.data.ma.xpos = marine->getX();
    moveAction.data.ma.ypos = marine->getY();
    moveAction.data.ma.xdel = marine->getDX();
    moveAction.data.ma.ydel = marine->getDY();
    moveAction.data.ma.vel = marine->getVelocity();
    moveAction.data.ma.direction = marine->getAngle();
    NetworkManager::instance().writeUDPSocket((char *)&moveAction, sizeof(ClientMessage));
}

/**------------------------------------------------------------------------------
Method: sendServAttackAction

Date: April 4, 2017

Designer: Brody McCrone

Programmer: Brody McCrone

Interface: bool sendServMoveAction

Returns:
void

Notes:
Updates the player's attack action and send it to the server via UDP.
-------------------------------------------------------------------------------*/
void Player::sendServAttackAction() {
    attackAction.data.aa.playerid = id;
    attackAction.data.aa.actionid = static_cast<int32_t>(UDPHeaders::SHOOT);
    attackAction.data.aa.weaponid = marine->inventory.getCurrent()->getID();
    attackAction.data.aa.xpos = marine->getX();
    attackAction.data.aa.ypos = marine->getY();
    attackAction.data.aa.direction = marine->getAngle();

    NetworkManager::instance().writeUDPSocket((char *)&attackAction, sizeof(ClientMessage));
}

/**
* Date: Feb. 6, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: handleMouseUpdate(const int winWidth, const int winHeight, 
*                const float camX, const float camY) 
*       winWidth : Window width
*       winHeight : Window height
*       camX : Camera x position
*       camY : Camera y position
*
* Description: 
*   Handle user mouse input.
*/
void Player::handleMouseUpdate(const int winWidth, const int winHeight, const float camX, const float camY) {
    int mouseX;
    int mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    const int mouseDeltaX = winWidth / 2 - mouseX;
    const int mouseDeltaY = winHeight / 2 - mouseY;

    marine->setAngle(((atan2(mouseDeltaX, mouseDeltaY)* 180.0)/M_PI) * - 1);

    if (tempBarricadeID > -1) {
        Barricade& tempBarricade = GameManager::instance()->getBarricade(tempBarricadeID);
        tempBarricade.move(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY,
            GameManager::instance()->getCollisionHandler());
    }

    if (tempTurretID > -1) {
        Turret& tempTurret = GameManager::instance()->getTurret(tempTurretID);
        tempTurret.move(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY,
            GameManager::instance()->getCollisionHandler());

        if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
            if (tempTurret.collisionCheckTurret(marine->getX(), marine->getY(), mouseX + camX, mouseY + camY,
                    GameManager::instance()->getCollisionHandler())) {
                tempTurret.placeTurret();
                tempTurretID = -1;
                holdingTurret = false;
            }
        }
    }
/*
    //fire weapon on left mouse click
    if (SDL_GetMouseState(nullptr, nullptr)  &SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if(marine->inventory.getCurrent() != nullptr){
            marine->fireWeapon();
            if (networked) {
                sendServAttackAction();
            }
        }
    }
*/
}

/**------------------------------------------------------------------------------
Method: fireWeapon

Date: April 4, 2017

Designer: Deric Mccadden

Programmer: Deric Mccadden, Brody McCrone

Interface: void fireWeapon()

Returns:
void

Notes:
Fires player's marine's weapon amd sends the Attack Action to the server.
-------------------------------------------------------------------------------*/
void Player::fireWeapon() {
    if (marine->inventory.getCurrent() && marine->fireWeapon() && networked) {
        sendServAttackAction();
    }
}

void Player::handleMouseWheelInput(const SDL_Event *e){
    marine->inventory.scrollCurrent(e->wheel.y);
}

// function to handle mouse-click events
void Player::handlePlacementClick(SDL_Renderer *renderer) {
    if (tempBarricadeID > -1) {
        Barricade& tempBarricade = GameManager::instance()->getBarricade(tempBarricadeID);
        if (tempBarricade.isPlaceable()) {
            tempBarricade.placeBarricade();
            tempBarricadeID = -1;
        }
    }
}

/**
* Date: Feb. 6, 2017
* Author: Jacob McPhail
* Modified: ---
* Function Interface: handleKeyboardInput(const Uint8 *state)
*         state : Keyboard state
*
* Description: 
*   Handle user key input.
*/
void Player::handleKeyboardInput(const Uint8 *state) {
    float x = 0;
    float y = 0;
    float velocity = marine->getVelocity();

    // Check for move inputs
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) {
        y -= velocity;
    }
    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) {
        y += velocity;
    }
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
        x -= velocity;
    }
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
        x += velocity;
    }

    //Inventory inputs
    if (state[SDL_SCANCODE_1]){
        marine->inventory.switchCurrent(0);
    } else if (state[SDL_SCANCODE_2]){
        marine->inventory.switchCurrent(1);
    } else if (state[SDL_SCANCODE_3]){
        marine->inventory.switchCurrent(2);
    }

    //Weapon input
    if(state[SDL_SCANCODE_R]){
        Weapon *w = marine->inventory.getCurrent();
        if(w){
            w->reloadClip();
        }
    }
    //pickup button
    if(state[SDL_SCANCODE_E]){
        const int currentTime = SDL_GetTicks();

        if(currentTime > (pickupTick + pickupDelay)) {
            pickupTick = currentTime;

            const int checkTurret = marine->checkForPickUp();
            if (checkTurret > -1 && holdingTurret == false)
            {
                tempTurretID = checkTurret;
                GameManager::instance()->getTurret(tempTurretID).pickUpTurret();
                holdingTurret = true;
            }
        }
    }
    //Drop button
    if(state[SDL_SCANCODE_F]){
        marine->inventory.dropWeapon(marine->getX(), marine->getY());
    }
    //use Inventory
    if(state[SDL_SCANCODE_I]) {
        marine->inventory.useItem();
    }

    //added by Maitiu Debug print 4/3/2017
    if(state[SDL_SCANCODE_PERIOD]){
        dumpEntityPositions(this);
    }
    marine->setDY(y);
    marine->setDX(x);
}

void Player::handleTempBarricade(SDL_Renderer *renderer) {
    if(tempBarricadeID < 0) {
        if (!marine) {
            return;
        }
        const double angle = marine->getAngle();
        tempBarricadeID = GameManager::instance()->createBarricade(
            marine->getX() + PLAYER_PLACE_DISTANCE * cos(angle),
            marine->getY() + PLAYER_PLACE_DISTANCE * sin(angle));
    } else {
        GameManager::instance()->deleteBarricade(tempBarricadeID);
        tempBarricadeID = -1;
    }
}

void Player::handleTempTurret(SDL_Renderer *renderer) {
   if(tempTurretID < 0) {
       const double angle = marine->getAngle();
       tempTurretID = GameManager::instance()->createTurret(
           marine->getX() + PLAYER_PLACE_DISTANCE * cos(angle),
           marine->getY() + PLAYER_PLACE_DISTANCE * sin(angle));
   } else {
       GameManager::instance()->deleteTurret(tempTurretID);
       tempTurretID = -1;
   }
}

/**
 * Date: Apl. 5, 2017
 * Author: Jacob McPhail
 * Function Interface: checkMarineState()
 * Description:
 *      Check marine health if 0 kill marine, also check if player respawns.
 */
bool Player::checkMarineState() {
    if (marine && marine->getHealth() <= 0){
        GameManager::instance()->deleteMarine(marine->getId());
        setControl(nullptr);
        respawnTick = SDL_GetTicks();
    } else if (!marine) {
        if (static_cast<int>(SDL_GetTicks()) < (respawnTick + RESPAWN_DELAY)) {
                return false;
        }
        return true;
    }
    return false;
}

/**
 * Date: Apl. 5, 2017
 * Author: Jacob McPhail
 * Function Interface: respawn(Point newPoint)
 *      newPoint : Player respawn point
 *
 * Description:
 *      Respawn player with a new marine.
 */
void Player::respawn(const Point& newPoint) {
    const int32_t playerMarineID = GameManager::instance()->createMarine();
    //gives the player control of the marine
    setControl(&GameManager::instance()->getMarine(playerMarineID).first);
    getMarine()->setPosition(newPoint.first, newPoint.second);
    getMarine()->setSrcRect(SPRITE_FRONT, SPRITE_FRONT, SPRITE_SIZE_X, SPRITE_SIZE_Y);  
}



