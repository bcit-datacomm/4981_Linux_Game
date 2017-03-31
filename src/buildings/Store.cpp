#include "Store.h"
#include "../log/log.h"
#include <array>
#include <algorithm>


Store::Store(int32_t id, const SDL_Rect dest): Object(id, dest), dropCols(4), dropPoints(5){

}

//maitiu March 16
Store::~Store(){
    logv("Destoryed Store");
}

//int num what the player wants to purchase
//Maitiu March 16
int32_t Store::purchase(int num){
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
