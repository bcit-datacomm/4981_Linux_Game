#include "StoreMenu.h"


StoreMenu::StoreMenu(const SDL_Rect s, GameHashMap<TEXTURES, int> i, int t): screen(s), Items(i), type(t){

}


void StoreMenu::setSizes(){
    switch(3){
        case 1:
            background = {screen.w / 2 + 50, screen.h /2 - 400, screen.w / 4, screen.h * .4};
        break;
        case 2:
            background = {screen.w / 2 + 50, screen.h /2 - 400, screen.w / 4, screen.h * .2};
        break;
        case 3:
            background = {screen.w / 2 + 50, screen.h /2 - 400, screen.w / 4, screen.h * .2};
        break;
    }

    slot[0] = {background.x + (background.w/16), background.y + (background.h/16), background.w / 4 , background.w / 4};
    slot[1] = {slot[0].x + slot[0].w + background.w/16, slot[0].y, slot[0].w , slot[0].h};
    slot[2] = {slot[1].x + slot[0].w + background.w/16, slot[0].y, slot[0].w , slot[0].h};

    slot[3] = {slot[0].x,                               slot[0].y + (slot[0].h * 1.2), slot[0].w , slot[0].w};
    slot[4] = {slot[3].x + slot[0].w + background.w/16, slot[0].y + (slot[0].h * 1.2) , slot[0].w, slot[0].w};
    slot[5] = {slot[4].x + slot[0].w + background.w/16, slot[0].y + (slot[0].h * 1.2) , slot[0].w, slot[0].w};

    slot[6] = {slot[0].x,                               slot[3].y + (slot[0].h * 1.2), slot[0].w , slot[0].w};
    slot[7] = {slot[6].x + slot[0].w + background.w/16, slot[3].y + (slot[0].h * 1.2), slot[0].w , slot[0].w};
}


void StoreMenu::renderBackground(){
    setSizes();

    Renderer::instance().render(background, TEXTURES::CONSUMABLE_SLOT);
    renderSlots(3);
}

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

int StoreMenu::getClicked(const float x, const float y){
    for(int i = 0; i < sizeof(slot); i++){
        if(checkSlot(slot[i], x, y)){
            //printf("slot x:%d y:%d  maxX:%d maxY:%d\n", slot[i].x, slot[i].y, slot[i].x + slot[i].w, slot[i].y + slot[i].h);
            //printf("mouse X:%f y:%f\n", x, y);
            return i;
        }
    }
    return -1;
}

int StoreMenu::checkSlot(SDL_Rect& s, float x, float y){
    return (x >= s.x && x <= s.x + s.w && y >= s.y && y <= s.y + s.h);
}

void StoreMenu::createWeaponStoreMenu(){
    TEXTURES tex;
    for(int i = 0; i < 9; i++){
        switch(i){
            case 0:
            tex = TEXTURES::RIFLE;
            break;
            case 1:
            tex = TEXTURES::SHOTGUN;
            break;
            case 2:
            tex = TEXTURES::CONCRETE;
            break;
            case 3:
            tex = TEXTURES::CONCRETE;
            break;
            case 4:
            tex = TEXTURES::CONCRETE;
            break;
            case 5:
            tex = TEXTURES::CONCRETE;
            break;
            case 6:
            tex = TEXTURES::CONCRETE;
            break;
            case 7:
            tex = TEXTURES::CONCRETE;
            break;
        }
        Renderer::instance().render(slot[i], tex);
    }
}

void StoreMenu::createTechStoreMenu(){
    TEXTURES tex;
    for(int i = 0; i < 2; i++){
        switch(i){
            case 0:
            tex = TEXTURES::CONCRETE;
            break;
            case 1:
            tex = TEXTURES::BASE;
            break;
        }
        Renderer::instance().render(slot[i], tex);
    }
}

void StoreMenu::createHealthStoreMenu(){
    Renderer::instance().render(slot[1], TEXTURES::HEALTHPACK);
}
