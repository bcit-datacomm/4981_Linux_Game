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
static constexpr unsigned int MAX_LEVELS = 3;

class Quadtree {
public:
    Quadtree(unsigned int pLevel, SDL_Rect pBounds);
    ~Quadtree() = default;

    void clear();
    unsigned int getTreeSize() const;
    void insert(Entity *entity);
    std::vector<Entity *> retrieve(const Entity *entity) const;

private:
    bool contains(const Quadtree& q, const Entity *entity) const;
    std::vector<Entity *> retrieve(const SDL_Rect& rect) const;

    unsigned int level;
    SDL_Rect bounds;
    std::array<std::unique_ptr<Quadtree>, BRANCHSIZE> nodes;
    std::vector<Entity *> objects;
};

#endif
