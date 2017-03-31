#include "Store.h"
#include "../log/log.h"
#include <array>
#include <algorithm>


Store::Store(int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize):
        Object(id, dest, dest, pickupSize, 400, 400, 0){
        logv("Created Store\n");
}

//maitiu March 30
Store::~Store(){
    logv("Destoryed Store\n");
}

//int num what the player wants to purchase
//Maitiu March 30
int32_t Store::purchase(int num){
    logv("Purchased from Store");
    return -1;
}

//maitiu March 30
int Store::selectItem(){
    return 0;
}

//maitiu March 30
std::pair<int,int> Store::dropLocation(){
    return {0,0};
}
