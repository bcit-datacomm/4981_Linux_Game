#include "GameHud.h"

/**
 * Function: GameHud ctor
 *
 * Date:
 * JF: March 25, 2017: added member initilizer for opacity
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 *
 * Interface: GameHud()
 *
 * Notes:
 * GameStateMenu ctor which initializes the inventory slot opacity to zero
 * this ensure that the weapon inventory slots are not rendered when starting gameStateMatch
*/
GameHud::GameHud(): inventorySlotOpacity(0){}

/**
 * Function: getHealthRgbElement
 *
 * Date:
 * JF: March 25, 2017:
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Interface: getHealthRgbElement(int index)
 *                  int index: the rgb index to be retrieved
 *                              0: red
 *                              1: green
 *                              2: blue
 *
 * Returns: the color value of the index specified
 *
 * Notes:
 * Obtains the R, G, or B color value dependant upon index specified
 */
size_t GameHud::getHealthRgbElement(const int index) {

    return (index >=0 && index <=2) ? healthRGB[index] : 0;
}

/**
 * Function: decrementOpacity
 *
 * Date:
 * JF: March 25, 2017:
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Interface: decrementOpacity(Uint8 amount)
 *                  Uint8 amount: How much to decrement the opacity (increases transparency)
 *
 * Returns: void
 *
 * Notes:
 * When function is called, it decreases the inventory slot opacity by the value passed in.
 * If this decreases the opacity value below zero, the opacity is set to zero
 */
void GameHud::decrementOpacity(const Uint8 amount) {

    if ( inventorySlotOpacity <= MIN_RGB_VALUE ) {
       inventorySlotOpacity = MIN_RGB_VALUE;
    } else {
        inventorySlotOpacity -= amount;
    }
}

/**
 * Function: setOpacity
 *
 * Date:
 * JF: March 25, 2017:
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Interface: setOpacity(Uint8 newOpacity)
 *                  Uint8 newOpacity: Sets the opacity of the inventory slots
 *
 * Returns: void
 *
 * Notes:
 * When function called, inventory slot opacity is set to the new value.
 * verifies that opacity value can only fall between the maximum and minimum RGB values
 */
void GameHud::setOpacity(const Uint8 newOpacity) {

    if ( newOpacity < MIN_RGB_VALUE ) {
       inventorySlotOpacity = MIN_RGB_VALUE;

    } else  if ( newOpacity > MAX_RGB_VALUE){
        inventorySlotOpacity = MAX_RGB_VALUE;
    }
    inventorySlotOpacity = newOpacity;
}

/**
 * Function: setHealthBarColor
 *
 * Date:
 * JF: March 26, 2017:
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Modified by:
 * Jacob Frank (March 28, 2017)
 *
 * Interface: setHealthBarColor(float currentHP)
 *                  float currentHP: The Marine's current health
 *
 * Returns: void
 *
 * Notes:
 * Functions takes the current health of the marine and calulcates a new RGB value
 * based on the percentof health left that the marine has.
 * Function fades the health bar color from green to red as the marine has less health
 *
 * Revisions:
 * JF Mar 28: Tweaked formula to use a different shade of red and green to
 * help distinguish it from the background
 */
void GameHud::setHealthBarColor(const float currentHP) {

    const float healthPercent = currentHP / MAX_HEALTH; //Percent of health left of the marine
    size_t colorAmount = static_cast<size_t>(healthPercent * MAX_RGB_VALUE); //results in a value between 0 and 255

    healthRGB[0] = MAX_RGB_VALUE - colorAmount; //set the Red value
    healthRGB[1] = colorAmount; //Set the Green value
    healthRGB[2] = MIN_RGB_VALUE; //Set the Blue value
}

/**
 * Function: renderEquippedWeaponSlot
 *
 * Date:
 * JF: March 27, 2017:
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 *
 * Interface: renderEquippedWeaponSlot(SDL_Rect screenRect)
 *                  SDL_Rect screenRect: The Current screen properties (height, width)
 *
 * Notes:
 * Function renders the equipped weapon slot to the screen.
 * slot is positions in bottom right of screen next to the ammo clip.
 */
void GameHud::renderEquippedWeaponSlot(const SDL_Rect& screenRect, const Player& p) {

    //Makes the texture properties a squeare regardless of screen size
    if (screenRect.w <= screenRect.h) {
        equippedSlot.w = screenRect.w * EQUIPPED_SLOT_RAT;
        equippedSlot.h = screenRect.w * EQUIPPED_SLOT_RAT;
    } else {
        equippedSlot.w = screenRect.h * EQUIPPED_SLOT_RAT;
        equippedSlot.h = screenRect.h * EQUIPPED_SLOT_RAT;
    }

    //Set position to the left of the ammo clip
    equippedSlot.x = ammoClipBackground.x - screenRect.w * PADDING_RAT - equippedSlot.w;

    //align bottom of slot with other hud elements
    equippedSlot.y = screenRect.h - screenRect.w * PADDING_RAT - equippedSlot.h;

    Renderer::instance().render(equippedSlot, TEXTURES::EQUIPPED_WEAPON_SLOT);
    renderEquippedWeapon(equippedSlot, p);
}

/**
 * Function: renderClip
 *
 * Date:
 * JF: March 28, 2017:
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Modified by:
 * Jacob Frank (April 1, 2017)
 *
 * Interface: renderClip(SDL_Rect screenRect, Player p)
 *                  SDL_Rect screenRect: The Current screen properties (height, width)
 *                  Player p: The player in the game. used to find current equipped weapon
 *
 * Notes:
 * Renders the weapon clip next to the currently equipped weapon.
 * Visually displays how much ammo is left in the clip via a "depleting" image as bullets are fired.
 * Ammo left in clip is calculated using the number of bullets in the clip and the max bullets the clip can hold.
 *
 * Revisions:
 * JF April 1: Fixed Segfault bug resulting from not checking if the current weapon is a nullptr
 */
void GameHud::renderClip(const SDL_Rect& screenRect, const Player& p) {

    float percentLeftinClip; //bullets in clip / max bullets of clip

    if (p.getMarine()->inventory.getCurrent() == nullptr) { //ensure that the current weapon is not null
        percentLeftinClip = 0; //if currently equipped weapon is null set clip to empty
    } else {
        const float ammoInClip = p.getMarine()->inventory.getCurrent()->getClip();
        const float maxClipAmmo = p.getMarine()->inventory.getCurrent()->getClipMax();
        percentLeftinClip = ammoInClip / maxClipAmmo;
    }


    int ammoClipWidth; //pixel width of the ammo clip image used
    int ammoClipHeight; //pixel height of the ammo clip image used
    //Find the pixel width and height of the image used for the ammo clip texture
    SDL_QueryTexture(Renderer::instance().getTexture(static_cast<int>(TEXTURES::WEAPON_CLIP_EMPTY)),
                        nullptr, nullptr, &ammoClipWidth, &ammoClipHeight);

    //position the ammo clip background in the bottom right of the screen
    ammoClipBackground.w = screenRect.w * AMMO_CLIP_BACKROUND_W_RAT;
    ammoClipBackground.h = screenRect.w * AMMO_CLIP_BACKROUND_H_RAT;
    ammoClipBackground.x = screenRect.w - screenRect.w * PADDING_RAT - ammoClipBackground.w;
    ammoClipBackground.y = screenRect.h - screenRect.w * PADDING_RAT - ammoClipBackground.h;

    //position the ammo clip foreground overtop of the background
    // adjusted by the amount of ammo left in the clip
    ammoClipForeground.w = ammoClipBackground.w;
    ammoClipForeground.h = ammoClipBackground.h * percentLeftinClip;
    ammoClipForeground.x = ammoClipBackground.x;
    ammoClipForeground.y = ammoClipBackground.y + ammoClipBackground.h * (1 - percentLeftinClip);

    //Determines the size of the rect to clip from
    //the ammo clip foreground to display the amount of ammo left in clip
    ammoClipClipper.w = ammoClipWidth;
    ammoClipClipper.h = ammoClipHeight;
    ammoClipClipper.x = 0;
    ammoClipClipper.y = ammoClipHeight * (1 - percentLeftinClip);

    Renderer::instance().render(ammoClipBackground, TEXTURES::WEAPON_CLIP_EMPTY);
    Renderer::instance().render(ammoClipForeground, TEXTURES::WEAPON_CLIP_FULL, ammoClipClipper);
}

/**
 * Function: renderHealthBar
 *
 * Date:
 * JF: April 2, 2017:
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Interface: renderHealthBar(SDL_Rect screenRect, Player p, Camera c)
 *                  SDL_Rect screenRect: The Current screen properties (height, width)
 *                  Player p: The player in the game. used to find current equipped weapon
 *                  Camera c: The camera position to view player area
 *
 * Notes:
 * Function used to display the healthbar above the marine
 * Displays the amount of health the marine has left including dynamic coloring from green to red
 *
 */
void GameHud::renderHealthBar(const SDL_Rect& screenRect, const Player& p, const Camera& c) {

    healthBarBackground.w = MARINE_WIDTH * 2;
    healthBarBackground.h = screenRect.h * HEALTHBAR_BACKROUND_H_RAT;

    healthBarBackground.x = p.getMarine()->getX() - c.getX() -
        healthBarBackground.w / 2 + MARINE_WIDTH / 2;
    healthBarBackground.y = p.getMarine()->getY() - c.getY() -
        healthBarBackground.h - screenRect.h * HEALTHBAR_BACKROUND_Y_RAT;

    healthBarForeground.h = healthBarBackground.h * HEALTHBAR_FOREGROUND_H_RAT;
    healthBarForeground.x = healthBarBackground.x + healthBarBackground.h * HEALTHBAR_FOREGROUND_XY_RAT;
    healthBarForeground.y = healthBarBackground.y + healthBarBackground.h * HEALTHBAR_FOREGROUND_XY_RAT;

    Renderer::instance().render(healthBarBackground, TEXTURES::HEALTHBAR);

    const float HP = p.getMarine()->getCurrentHealth();

    if (HP > MIN_HEALTH && HP <= MAX_HEALTH) {
        setHealthBarColor(HP); //Sets the RGB values of the healthbar from the marine's current HP
        healthBarForeground.w = static_cast<size_t>(HP / MAX_HEALTH * healthBarBackground.w) -
            healthBarBackground.h * HEALTHBAR_FOREGROUND_W_RAT;

        //Sets the renderers color based on calculated RGB value
        SDL_SetRenderDrawColor(Renderer::instance().getRenderer(), getHealthRgbElement(0),
            getHealthRgbElement(1), getHealthRgbElement(2), OPAQUE);

        //Renders the healthbar foreground
        SDL_RenderFillRect(Renderer::instance().getRenderer(), &healthBarForeground);
    }
}

/**
 * Function: renderConsumable
 *
 * Date:
 * JF: April 1, 2017:
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Interface: renderConsumable(SDL_Rect screenRect, Player p)
 *                  SDL_Rect screenRect: The Current screen properties (height, width)
 *                  Player p: The player in the game. used to find current equipped weapon
 *
 * Notes:
 * Function, when called renders the consumable item slot in the bottom left corner of the visible screen
 * Consumable slot is only visible when the player has a consumable item in their inventory.
 */
void GameHud::renderConsumable(const SDL_Rect& screenRect, const Player& p) {

    if (screenRect.w <= screenRect.h) {
        consumableSlot.w =  screenRect.w * CONSUMABLE_SIZE_RAT;
        consumableSlot.h =  screenRect.w * CONSUMABLE_SIZE_RAT;
    } else {
        consumableSlot.w = screenRect.h * CONSUMABLE_SIZE_RAT;
        consumableSlot.h = screenRect.h * CONSUMABLE_SIZE_RAT;
    }

    consumableSlot.x = screenRect.w * PADDING_RAT;
    consumableSlot.y = screenRect.h - screenRect.w * PADDING_RAT - consumableSlot.h;

    Renderer::instance().render(consumableSlot, TEXTURES::CONSUMABLE_SLOT);
    Renderer::instance().render(consumableSlot, TEXTURES::HEALTHPACK);

}

/**
 * Function: renderWeaponSlots
 *
 * Date:
 * JF: April 1, 2017:
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Modified By:
 * Jacob Frank (April 4, 2017)
 *
 * Interface: renderWeaponSlots(SDL_Rect screenRect, Player p)
 *                  SDL_Rect screenRect: The Current screen properties (height, width)
 *                  Player p: The player in the game. used to find current equipped weapon
 *
 * Notes:
 * Function, when called renders the Weapon inventory slots along the bottom center of the visible screen
 * Inventory slots are only visible when the player changes their currently equiped weapon.
 *
 * Revisions:
 * JF April 4: Now makes use of rendering alpha modulation wrapper functions created by Terry
 */
void GameHud::renderWeaponSlots(const SDL_Rect& screenRect, const Player& p) {
    const int weaponSlotWidth = screenRect.w * WEAPON_SLOT_WIDTH_RAT;
    const int weaponSlotHeight = screenRect.h * WEAPON_SLOT_HEIGHT_RAT;
    const int weaponSlotPosY = screenRect.h - screenRect.w * PADDING_RAT - weaponSlotHeight;


    inventorySlot[1].w = weaponSlotWidth;
    inventorySlot[1].h = weaponSlotHeight;
    inventorySlot[1].y = weaponSlotPosY;
    inventorySlot[1].x = screenRect.w * HALF - inventorySlot[0].w * HALF; //Positions slot 1 in the center

    //positions slot 0 to the left of slot 1
    inventorySlot[0].w = weaponSlotWidth;
    inventorySlot[0].h = weaponSlotHeight;
    inventorySlot[0].y = weaponSlotPosY;
    inventorySlot[0].x = inventorySlot[1].x - weaponSlotWidth - screenRect.w * PADDING_RAT;

    //positions slot 2 to the right of slot 1
    inventorySlot[2].w = weaponSlotWidth;
    inventorySlot[2].h = weaponSlotHeight;
    inventorySlot[2].y = weaponSlotPosY;
    inventorySlot[2].x = inventorySlot[1].x + weaponSlotWidth + screenRect.w * PADDING_RAT;

    //Decrease the opacity of the inventory item slots by 1
    //This gets called on every loop resulting in the inventory items fading away
    decrementOpacity(1);

    for (int i = 0; i < 3; ++i) {
        if (i == p.getMarine()->inventory.getCurrentSlot()) {
            Renderer::instance().setAlpha(TEXTURES::ACTIVE_SLOT, inventorySlotOpacity);
            Renderer::instance().render(inventorySlot[i], TEXTURES::ACTIVE_SLOT);
            renderInventoryWeapons(inventorySlot[i], p, i);
        } else {
            Renderer::instance().setAlpha(TEXTURES::PASSIVE_SLOT, inventorySlotOpacity);
            Renderer::instance().render(inventorySlot[i], TEXTURES::PASSIVE_SLOT);
            renderInventoryWeapons(inventorySlot[i], p, i);
        }
    }
}

/**
 * Function: renderWeapon
 *
 * Date:
 * JF: April 4, 2017:
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Interface: renderWeapon(SDL_Rect position, size_t weaponId)
 *                  SDL_Rect position: The position where to render the weapon
 *                  size_t weaponId: The ID of the weapon to render
 *
 * Notes:
 * The below two methods currently do nothing, but will be used to display the weapons in the
 * players inventory and equipped item slot.
 */
void GameHud::renderInventoryWeapons(SDL_Rect& position, const Player& p, int inventorySlotPosition) {
    if (p.getMarine()->inventory.getWeaponFromInventory(inventorySlotPosition) != nullptr) {
        std::string weaponType = p.getMarine()->inventory.getWeaponFromInventory(inventorySlotPosition)->getType();

        if (weaponType.compare("Handgun") == 0) {
            Renderer::instance().setAlpha(TEXTURES::HANDGUN_INVENTORY, inventorySlotOpacity);
            Renderer::instance().render(position, TEXTURES::HANDGUN_INVENTORY);
        } else if (weaponType.compare("Rifle") == 0){
            Renderer::instance().setAlpha(TEXTURES::RIFLE_INVENTORY, inventorySlotOpacity);
            Renderer::instance().render(position, TEXTURES::RIFLE_INVENTORY);
        } else if (weaponType.compare("Shotgun") == 0){
            Renderer::instance().setAlpha(TEXTURES::SHOTGUN_INVENTORY, inventorySlotOpacity);
            Renderer::instance().render(position, TEXTURES::SHOTGUN_INVENTORY);
        } else if (weaponType.compare("Plasma Deagle") == 0){
            Renderer::instance().setAlpha(TEXTURES::PLASMA_DEAGLE_INVENTORY, inventorySlotOpacity);
            Renderer::instance().render(position, TEXTURES::PLASMA_DEAGLE_INVENTORY);
        } else if (weaponType.compare("Railgun") == 0){
            Renderer::instance().setAlpha(TEXTURES::RAILGUN_INVENTORY, inventorySlotOpacity);
            Renderer::instance().render(position, TEXTURES::RAILGUN_INVENTORY);
        } else if (weaponType.compare("RPG") == 0){
            Renderer::instance().setAlpha(TEXTURES::RPG_INVENTORY, inventorySlotOpacity);
            Renderer::instance().render(position, TEXTURES::RPG_INVENTORY);
        } else if (weaponType.compare("Type75") == 0){
            Renderer::instance().setAlpha(TEXTURES::TYPE_75K_INVENTORY, inventorySlotOpacity);
            Renderer::instance().render(position, TEXTURES::TYPE_75K_INVENTORY);
        } else if (weaponType.compare("Katana") == 0){
            Renderer::instance().setAlpha(TEXTURES::KATANA_INVENTORY, inventorySlotOpacity);
            Renderer::instance().render(position, TEXTURES::KATANA_INVENTORY);
        } else if (weaponType.compare("Phase Repeater") == 0){
            Renderer::instance().setAlpha(TEXTURES::PHASE_REPEATER_INVENTORY, inventorySlotOpacity);
            Renderer::instance().render(position, TEXTURES::PHASE_REPEATER_INVENTORY);
        }
    }
}

/**
 * Function: renderEquippedWeapon
 *
 * Date:
 * JF: April 4, 2017:
 *
 * Designer:
 * Jacob Frank
 *
 * Programmer:
 * Jacob Frank
 *
 * Interface: renderEquippedWeapon(SDL_Rect& position, const Player& p)
 *                  SDL_Rect position: The position where to render the weapon
 *                  Player& p: The player holding the weapon
 *
 * Notes:
 * Function, when called renders the "Equipped" version of the weapon texture to
 * the desired location on screen.
 * Function is called from the render Equippedweapon slot method
 */
void GameHud::renderEquippedWeapon(SDL_Rect& position, const Player& p) {
    if (p.getMarine()->inventory.getCurrent() != nullptr) {
        std::string weaponType = p.getMarine()->inventory.getCurrent()->getType();

        if (weaponType.compare("Handgun") == 0) {
            Renderer::instance().render(position, TEXTURES::HANDGUN);
        } else if (weaponType.compare("Rifle") == 0){
            Renderer::instance().render(position, TEXTURES::RIFLE);
        } else if (weaponType.compare("Shotgun") == 0){
            Renderer::instance().render(position, TEXTURES::SHOTGUN);
        } else if (weaponType.compare("Plasma Deagle") == 0){
            Renderer::instance().render(position, TEXTURES::PLASMA_DEAGLE);
        } else if (weaponType.compare("Railgun") == 0){
            Renderer::instance().render(position, TEXTURES::RAILGUN);
        } else if (weaponType.compare("RPG") == 0){
            Renderer::instance().render(position, TEXTURES::RPG);
        } else if (weaponType.compare("Type75") == 0){
            Renderer::instance().render(position, TEXTURES::TYPE_75K);
        } else if (weaponType.compare("Katana") == 0){
            Renderer::instance().render(position, TEXTURES::KATANA);
        } else if (weaponType.compare("Phase Repeater") == 0){
            Renderer::instance().render(position, TEXTURES::PHASE_REPEATER);
        }
    }
}

