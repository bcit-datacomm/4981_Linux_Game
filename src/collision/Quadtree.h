/*------------------------------------------------------------------------------
* Header: Quadtree.h
*
* Functions:
*
*
* Date:
*
* Revisions:
* Reviewed By : Justen DePourcq- Style guide
*
* Designer:
*
* Author:
*
* Notes:
*
------------------------------------------------------------------------------*/
#ifndef QUADTREE_H
#define QUADTREE_H
#include <SDL2/SDL.h>
#include <vector>
#include <array>
#include <memory>

#include "HitBox.h"
#include "../basic/Entity.h"

static constexpr unsigned int BRANCHSIZE = 4;
static constexpr unsigned int MAX_OBJECTS = 1000;
static constexpr unsigned int MAX_LEVELS = 50;

class Quadtree {
public:
    Quadtree(int pLevel, SDL_Rect pBounds);
    ~Quadtree() = default;

    Quadtree& operator=(const Quadtree& quad);

    void clear();
    void split();
    unsigned int getTreeSize() const;
    int getIndex(const HitBox *pRect) const;
    void insert(Entity *entity);
    std::vector<Entity *> retrieve(const Entity *entity) const;
    auto& getObjects() const {return objects;}

private:
    unsigned int objectCounter;
    unsigned int level;
    SDL_Rect bounds;
    std::array<std::shared_ptr<Quadtree>, BRANCHSIZE> nodes;
    std::vector<Entity *> objects;
};

#endif
