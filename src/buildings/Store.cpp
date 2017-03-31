#include "Store.h"
#include "../log/log.h"
#include <array>
#include <algorithm>


Store::Store(int32_t id, const SDL_Rect dest, const SDL_Rect pickupSize):
        Object(id, dest, dest, pickupSize, 400, 400, 0){
        logv("Created Store\n");
}

//maitiu March 16
Store::~Store(){
    logv("Destoryed Store\n");
}

//int num what the player wants to purchase
//Maitiu March 16
int32_t Store::purchase(int num){
    logv("Purchased from Store");
    return -1;
}

//maitiu March 16
int Store::selectItem(){
    return 0;
}

//maitiu March 16
std::pair<int,int> Store::dropLocation(){
    return {0,0};
}
