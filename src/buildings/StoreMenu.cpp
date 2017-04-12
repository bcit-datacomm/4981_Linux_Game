#include "StoreMenu.h"


StoreMenu::StoreMenu(const SDL_Rect s, GameHashMap<TEXTURES, int> i, int t): screen(s), Items(i), type(t){

    font = Renderer::instance().loadFont("assets/fonts/SEGOEUISL.ttf", 10);

}

/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Function void StoreMenu::setSizes()
 * Description:
 *     Sets the size of UI based on what store type
 */
void StoreMenu::setSizes(){
    switch(type){
        case 1:
            background = {screen.w / 2 + BACKGROUND_H_PADDING, screen.h /2 - BACKGROUND_V_PADDING,
                    screen.w / BACKGROUND_WIDTH, static_cast<int>(screen.h * WEAPONS_HEIGHT)};
        break;
        case 2:
            background = {screen.w / 2 + BACKGROUND_H_PADDING, screen.h /2 - BACKGROUND_V_PADDING,
                    screen.w / BACKGROUND_WIDTH, static_cast<int>(screen.h * BACKGROUND_HEIGHT)};
        break;
        case 3:
            background = {screen.w / 2 + BACKGROUND_H_PADDING, screen.h /2 - BACKGROUND_V_PADDING,
                    screen.w / BACKGROUND_WIDTH, static_cast<int>(screen.h * BACKGROUND_HEIGHT)};
        break;
    }

    slot[0] = {background.x + (background.w / SLOT_X), background.y + (background.h / SLOT_X), background.w / SLOT_SIZE , background.w / SLOT_SIZE};
    slot[1] = {slot[0].x + slot[0].w + background.w / SLOT_X, slot[0].y, slot[0].w , slot[0].h};
    slot[2] = {slot[1].x + slot[0].w + background.w / SLOT_X, slot[0].y, slot[0].w , slot[0].h};

    slot[3] = {slot[0].x, slot[0].y + (slot[0].h * V_SPACE), slot[0].w , slot[0].w};
    slot[4] = {slot[3].x + slot[0].w + background.w / SLOT_X, slot[0].y + slot[0].h * V_SPACE ,
            slot[0].w, slot[0].w};
    slot[5] = {slot[4].x + slot[0].w + background.w / SLOT_X, slot[0].y + slot[0].h * V_SPACE ,
            slot[0].w, slot[0].w};

    slot[6] = {slot[0].x, slot[3].y + slot[0].h * V_SPACE, slot[0].w , slot[0].w};
    slot[7] = {slot[6].x + slot[0].w + background.w / SLOT_X, slot[3].y + slot[0].h * V_SPACE, slot[0].w , slot[0].w};
}

/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Function void StoreMenu::renderBackground()
 * Description:
 *     Creates Background for UI
 */
void StoreMenu::renderBackground(){
    setSizes();

    Renderer::instance().render(background, TEXTURES::CONSUMABLE_SLOT);
    renderSlots(type);
    renderText();
}

/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Function void StoreMenu::renderSlots(const int num)
 * Description:
 *     Selects which slots to create base on store type
 */
void StoreMenu::renderSlots(const int num){
    switch(num){
        case 1:
            createWeaponStoreMenu();
            break;
        case 2:
            createTechStoreMenu();
            break;
        case 3:
            createHealthStoreMenu();
            break;
    }
}

/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Function int StoreMenu::getClicked(const float x, const float y)
 * Description:
 *    figures out which slot was clicked base on x and y coordinates
 */
int StoreMenu::getClicked(const float x, const float y){
    for(size_t i = 0; i < slot.size(); i++){
        if(checkSlot(slot[i], x, y)){
            return i;
        }
    }
    return -1;
}

/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Function bool StoreMenu::checkSlot(SDL_Rect& s, float x, float y)
 * Description:
 *    Checks if x and y coordinates land in slot
 */
bool StoreMenu::checkSlot(const SDL_Rect& s, const float x, const float y){
    SDL_Point mousePoint = {static_cast<int>(x), static_cast<int>(y)};
    return SDL_PointInRect(&mousePoint, &s);
}

/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Function void StoreMenu::createWeaponStoreMenu()
 * Description:
 *    creates slots for weapon store menu
 */
void StoreMenu::createWeaponStoreMenu(){
    TEXTURES tex;
    for(int i = 0; i < TOTAL_SLOTS; i++){
        switch(i){
            case 0:
            tex = TEXTURES::RIFLE;
            break;
            case 1:
            tex = TEXTURES::SHOTGUN;
            break;
            case 2:
            tex = TEXTURES::PLASMA_DEAGLE;
            break;
            case 3:
            tex = TEXTURES::RAILGUN;
            break;
            case 4:
            tex = TEXTURES::TYPE_75K;
            break;
            case 5:
            tex = TEXTURES::KATANA;
            break;
            case 6:
            tex = TEXTURES::PHASE_REPEATER;
            break;
            case 7:
            //tex = TEXTURES::CONCRETE;
            break;
        }
        Renderer::instance().render(slot[i], tex);
    }
}

/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Functionvoid StoreMenu::createTechStoreMenu()
 * Description:
 *    creates slots for  tech store menu
 */
void StoreMenu::createTechStoreMenu(){
    TEXTURES tex;
    for(int i = 0; i < TECH_SLOTS; i++){
        switch(i){
            case 0:
            tex = TEXTURES::TURRET;
            Renderer::instance().render(slot[i], tex);
            break;
            case 1:
            tex = TEXTURES::MAP_OBJECTS;
            Renderer::instance().render(slot[i], tex, {B_SRC_X, B_SRC_Y, B_SRC_W, B_SRC_H});
            break;
        }
    }
}

/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Functionvoid void StoreMenu::createHealthStoreMenu()
 * Description:
 *    creates slots for health store menu
 */
void StoreMenu::createHealthStoreMenu(){
    Renderer::instance().render(slot[1], TEXTURES::HEALTHPACK);
}

void StoreMenu::renderText(){

    switch(type){
        case 1:
            //creatWeaponStoreText();
            break;
        case 2:
            break;
        case 3:
            break;
    }
}

/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Functionvoid void StoreMenu::creatWeaponStoreText()
 * Description:
 *    creates text for weapon store menu
 */
void StoreMenu::creatWeaponStoreText(){
    SDL_Rect titleRect {background.x + background.w / 4, background.y, 500, 500};
    //SDL_Rect price {slot[0].x, slot[0].y, slot[0].w, 50};
    Textomagic title(titleRect, font, 0, "Weapon Store");
    title.render();
}

/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Functionvoid StoreMenu::creatTechStoreText()
 * Description:
 *    creates text for tech store menu
 */
void StoreMenu::creatTechStoreText(){
    SDL_Rect titleRect {background.x + background.w / 4, background.y, 500, 500};
    //SDL_Rect price {slot[0].x, slot[0].y, slot[0].w, 50};
    Textomagic title(titleRect, font, 0, "Weapon Store");
    title.render();
}

/**
 * Date: April. 8, 2017
 * Modified: ----
 * Author: Maitiu Morton
 * Functionvoid void StoreMenu::creatHealthStoreText()
 * Description:
 *    creates text for health store menu
 */
void StoreMenu::creatHealthStoreText(){
    SDL_Rect titleRect {background.x + background.w / 4, background.y, 500, 500};
    //SDL_Rect price {slot[0].x, slot[0].y, slot[0].w, 50};
    Textomagic title(titleRect, font, 0, "Weapon Store");
    title.render();
}
