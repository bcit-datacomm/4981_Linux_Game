#include "Store.h"
#include "../log/log.h"
#include <array>
#include <algorithm>
#include "DropPoint.h"
#include "../game/GameHashMap.h"
#include "Store.h"

Store::Store(const int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize, SDL_Rect screen, const GameHashMap<TEXTURES, int> i, int type):
        Object(id, dest, dest, pickupSize, dest.w, dest.h, 0), open(false), menu(screen, i, type){
        logv("Created Store\n");
}

//maitiu March 30
Store::~Store(){
    logv("Destoryed Store\n");
}

//int num what the player wants to purchase
//Maitiu March 30
int32_t Store::purchase(const int num, const int credits){
    logv("Purchased from Store");
    return -1;
}

//maitiu March 30
std::pair<int,int> Store::dropLocation(){
    return {0,0};
}
