#include "GameHud.h"


GameHud::GameHud(): inventorySlotOpacity(0){}

GameHud::~GameHud() {

}


void GameHud::setRectPosition(SDL_Rect *rect, size_t x, size_t y) {
    rect->x = x;
    rect->y = y;
}

void GameHud::setRectSize(SDL_Rect rect, size_t w, size_t h) {
    rect.w = w;
    rect.h = h;
}

SDL_Rect* GameHud::getHealthBarBackground() {
    return &healthBarBackground;
}

SDL_Rect* GameHud::getHealthBarForeground() {
    return &healthBarForeground;
}

SDL_Rect* GameHud::getConsumableSlot() {
    return &consumableSlot;
}

SDL_Rect* GameHud::getEquippedSlot() {
    return &equippedSlot;
}

SDL_Rect* GameHud::getAmmoClipBackground() {
    return &ammoClipBackground;
}

size_t GameHud::getHealthRgbElement(int index) {
    return healthRGB[index];
}

int GameHud::getOpacity() {
    return inventorySlotOpacity;
}

void GameHud::decrementOpacity(Uint8 amount) {
    inventorySlotOpacity -= amount;
}

void GameHud::setOpacity(Uint8 newOpacity) {
    inventorySlotOpacity = newOpacity;
}

void GameHud::setHealthBarColor(float currentHP) {
    float rDiff = 255;
    float gDiff = -255;
    float bDiff = 0;
    float healthPercent;

    if (currentHP > 100) {
        currentHP = 100;
    } else if (currentHP < 0) {
        currentHP = 0;
    }

    healthPercent = currentHP / 100;

    healthRGB[0] = RED_RGB[0] - static_cast<size_t>(healthPercent * rDiff);
    healthRGB[1] = RED_RGB[1] - static_cast<size_t>(healthPercent * gDiff);
    healthRGB[2] = RED_RGB[2] - static_cast<size_t>(healthPercent * bDiff);
}

void GameHud::renderStaticItems(SDL_Rect screenRect, Player p, Camera c) {

    //**************************************************DISPLAY THE HEALTHBAR********************************************//
    healthBarBackground.w = MARINE_WIDTH * 2;
    healthBarBackground.h = screenRect.h * .05;

    setRectPosition(&healthBarBackground,
        p.marine->getX() - c.getX() - healthBarBackground.w / 2 + MARINE_WIDTH / 2,
        p.marine->getY() - c.getY() - healthBarBackground.h - screenRect.h * .01);


    healthBarForeground.h = healthBarBackground.h * 0.8;
    healthBarForeground.x = healthBarBackground.x + healthBarBackground.h * 0.1;
    healthBarForeground.y = healthBarBackground.y + healthBarBackground.h * 0.1;

    //**************************************************DISPLAY THE AMMO CLIP********************************************//
    ammoClipBackground.w = screenRect.w * 0.06;
    ammoClipBackground.h = screenRect.w * 0.20;
    ammoClipBackground.x = screenRect.w - screenRect.w * 0.02 - ammoClipBackground.w;
    ammoClipBackground.y = screenRect.h - screenRect.w * 0.02 - ammoClipBackground.h;

    int ammoClipWidth;
    int ammoClipHeight;
    SDL_QueryTexture(Renderer::instance().getTexture(static_cast<int>(TEXTURES::WEAPON_CLIP_EMPTY)), NULL, NULL, &ammoClipWidth, &ammoClipHeight);


    ammoClipForeground.w = ammoClipBackground.w;
    ammoClipForeground.h = ammoClipBackground.h * 0.75;
    ammoClipForeground.x = ammoClipBackground.x;
    ammoClipForeground.y = ammoClipBackground.y + ammoClipBackground.h * 0.25;

    clipper.w = ammoClipWidth;
    clipper.h = ammoClipHeight;
    clipper.x = 0;
    clipper.y = ammoClipHeight * 0.25;

    Renderer::instance().render(healthBarBackground, TEXTURES::HEALTHBAR);
    Renderer::instance().render(ammoClipBackground, TEXTURES::WEAPON_CLIP_EMPTY);
    Renderer::instance().render(ammoClipForeground, TEXTURES::WEAPON_CLIP_FULL, clipper);
}



