/*------------------------------------------------------------------------------
* Header: HitBox.cpp
*
* Functions:
*
*
* Date:
*
* Revisions:
* Edited By : Justen DePourcq- Style guide
*
* Designer:
*
* Author:
*
* Notes:
*
------------------------------------------------------------------------------*/
#include "HitBox.h"

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: HitBox(SDL_Rect nrect)
 *      nrect : Hit box rect
 *
 * Description:
 *      ctor for a hitbox     
 */
HitBox::HitBox(SDL_Rect nrect) : rect(nrect) {

}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: getRect()
 * Description:
 *     Getter for the hit box rect.
 */
const SDL_Rect& HitBox::getRect() const {
    return rect;
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: move(const float x, const float y)
 *      x : x position
 *      y : y position
 *
 * Description:
 *     Move the hitbox to a position.
 */
void HitBox::move(const float x, const float y) {
    rect.x = x;
    rect.y = y;
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: setRect(const SDL_Rect& rerect)
 *      rerect : New hit box rect
 *
 * Description:
 *     Sets the hit box rect.
 */
void HitBox::setRect(const SDL_Rect& rerect) {
    rect = rerect;
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: setFriendly(const bool isFriendly) 
 *      isFriendly : Sets friendly
 *
 * Description:
 *     Sets if hitbox is friendly to players
 */
void HitBox::setFriendly(const bool isFriendly) {
    playerFriendly = isFriendly;
}

/**
 * Date: Feb. 4, 2017
 * Modified: ----
 * Author: Jacob McPhail
 * Function Interface: isPlayerFriendly()
 * Description:
 *     Check if hitbox is friendly.
 */
bool HitBox::isPlayerFriendly() const {
    return playerFriendly;
}

bool operator!=(const HitBox& first, const HitBox& second) {
    return first.rect != second.rect && first.playerFriendly != second.playerFriendly;
}

bool operator!=(const SDL_Rect& first, const SDL_Rect& second) {
    return first.x != second.x && first.y != second.y && first.w != second.h && first.h != second.h;
}
