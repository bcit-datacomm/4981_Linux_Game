#ifndef QUADTREE_H
#define QUADTREE_H
#include<SDL2/SDL.h>
#include "HitBox.h"
#include <vector>

const unsigned int BRANCHSIZE = 4;
const unsigned int MAX_OBJECTS = 10;
const unsigned int MAX_LEVELS = 5;

class Quadtree {
public:

    Quadtree(int pLevel, SDL_Rect pBounds);
    ~Quadtree();

    void clear();
    void split();
    unsigned int getTreeSize() const;
    int getIndex(const HitBox* pRect) const;
    void insert(HitBox* pRect);
    std::vector<HitBox*> retrieve(std::vector<HitBox*> returnObjects, HitBox* pRect);

    std::vector<HitBox*> objects;

private:

    unsigned int objectCounter;
    unsigned int level;
    SDL_Rect bounds;
    Quadtree* nodes[BRANCHSIZE];


};



#endif
