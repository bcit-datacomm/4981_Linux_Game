#ifndef DROPPOINT_H
#define DROPPOINT_H

#include <utility>
#include <SDL2/SDL.h>
#include <memory>

class DropPoint {
public:
    DropPoint(int32_t did, float x, float y);
    ~DropPoint() = default;

    std::pair<float, float> getCoord()const{return {xCoord, yCoord};};/*maitiu March 16*/
    void occupy(const int32_t wdid);
    void free();
    bool isOccupied() const { return occupied;};/*maitiu March 16*/
    int32_t getWeaponDropId() const { return dropId;};/*maitiu March 16*/
    int32_t getID() const {return id;};//maitiu March 21

private:
    int32_t id;
    int32_t dropId;
    float xCoord;
    float yCoord;
    bool occupied;
};

#endif
