#include "StoreMenu.h"


StoreMenu::StoreMenu(const SDL_Rect s, GameHashMap<TEXTURES, int> i, int t): screen(s), Items(i), type(t){

    font = Renderer::instance().loadFont("assets/fonts/SEGOEUISL.ttf", 10);

}


void StoreMenu::setSizes(){
    switch(type){
        case 1:
            background = {screen.w / 2 + 50, screen.h /2 - 400, screen.w / 4, static_cast<int>(screen.h * .4)};
        break;
        case 2:
            background = {screen.w / 2 + 50, screen.h /2 - 400, screen.w / 4, static_cast<int>(screen.h * .2)};
        break;
        case 3:
            background = {screen.w / 2 + 50, screen.h /2 - 400, screen.w / 4, static_cast<int>(screen.h * .2)};
        break;
    }

    slot[0] = {background.x + (background.w/16), background.y + (background.h/16), background.w / 4 , background.w / 4};
    slot[1] = {slot[0].x + slot[0].w + background.w/16, slot[0].y, slot[0].w , slot[0].h};
    slot[2] = {slot[1].x + slot[0].w + background.w/16, slot[0].y, slot[0].w , slot[0].h};

    slot[3] = {slot[0].x,                               slot[0].y + (slot[0].h * 1.2), slot[0].w , slot[0].w};
    slot[4] = {slot[3].x + slot[0].w + background.w/16, slot[0].y + slot[0].h * 1.2 , slot[0].w, slot[0].w};
    slot[5] = {slot[4].x + slot[0].w + background.w/16, slot[0].y + slot[0].h * 1.2 , slot[0].w, slot[0].w};

    slot[6] = {slot[0].x,                               slot[3].y + slot[0].h * 1.2, slot[0].w , slot[0].w};
    slot[7] = {slot[6].x + slot[0].w + background.w/16, slot[3].y + slot[0].h * 1.2, slot[0].w , slot[0].w};
}


void StoreMenu::renderBackground(){
    setSizes();

    Renderer::instance().render(background, TEXTURES::CONSUMABLE_SLOT);
    renderSlots(type);
    renderText();
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
    printf("size of slot:%zu\n", slot.size());
    for(size_t i = 0; i < slot.size(); i++){
        printf("\nChecking: %zu\n", i);
        if(checkSlot(slot[i], x, y)){
            printf("Clicked: %zu\n", i);
            return i;
        }
    }
    printf("No item clicked\n");
    return -1;
}


bool StoreMenu::checkSlot(SDL_Rect& s, float x, float y){
    SDL_Point mousePoint = {static_cast<int>(x), static_cast<int>(y)};
    printf("Mouse X:%d Y:%d\n", static_cast<int>(x), static_cast<int>(x));
    printf("Rect X:%d Y:%d\n", s.x, s.y);
    printf("Is it in Rect: %d\n", SDL_PointInRect(&mousePoint, &s)); // prints 1
    return SDL_PointInRect(&mousePoint, &s);
}

void StoreMenu::createWeaponStoreMenu(){
    TEXTURES tex;
    for(int i = 0; i < 9; i++){
        switch(i){
            case 0:
            tex = RIFLE_TEX;
            break;
            case 1:
            tex = SHOTGUN_TEX;
            break;
            case 2:
            //tex = DEAGLE_TEX;
            break;
            case 3:
            //tex = RAILGUN_TEX;
            break;
            case 4:
            //tex = T75_TEX;
            break;
            case 5:
            //tex = KATANA_TEX;
            break;
            case 6:
            //tex = PHASE_TEX;
            break;
            case 7:
            //tex = AMMO_TEX;
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
            tex = TEXTURES::TURRET;
            break;
            case 1:
            tex = TEXTURES::CONCRETE;
            break;
        }
        Renderer::instance().render(slot[i], tex);
    }
}

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

void StoreMenu::creatWeaponStoreText(){
    SDL_Rect titleRect {background.x + background.w / 4, background.y, 500, 500};
    //SDL_Rect price {slot[0].x, slot[0].y, slot[0].w, 50};
    Textomagic title(titleRect, font, 0, "Weapon Store");
    title.render();
}

void StoreMenu::creatTechStoreText(){
    SDL_Rect titleRect {background.x + background.w / 4, background.y, 500, 500};
    //SDL_Rect price {slot[0].x, slot[0].y, slot[0].w, 50};
    Textomagic title(titleRect, font, 0, "Weapon Store");
    title.render();
}

void StoreMenu::creatHealthStoreText(){
    SDL_Rect titleRect {background.x + background.w / 4, background.y, 500, 500};
    //SDL_Rect price {slot[0].x, slot[0].y, slot[0].w, 50};
    Textomagic title(titleRect, font, 0, "Weapon Store");
    title.render();
}
