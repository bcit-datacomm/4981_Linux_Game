#include "DropPoint.h"

/*
int did: drop id
int x: xCoord
int y: YCoord
maitiu March 16*/
DropPoint::DropPoint(int32_t id, float x, float y):id(id), dropId(-1), xCoord(x), yCoord(y), occupied(false){

}

/*wdid: id of weapondrop in droppoint
maitiu March 16*/
void DropPoint::occupy(const int32_t wdid){
    dropId = wdid;
    occupied = true;
}

/*maitiu March 16*/
void DropPoint::free(){
    dropId = -1;
    occupied = false;
}
