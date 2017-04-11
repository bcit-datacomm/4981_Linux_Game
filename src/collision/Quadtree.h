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
#include <utility>

#include "HitBox.h"
#include "../basic/Entity.h"
#include "../buildings/Base.h"

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
    void retrieve(std::vector<Entity *>& retrieveList, const SDL_Rect& rect) const;
    std::vector<Entity *> retrieve(const Point& start, const Point& end) const;

private:
    bool contains(const Quadtree& q, const Entity *entity) const;
    std::vector<Entity *> retrieve(const SDL_Rect& rect) const;

    static inline bool constexpr lineIntersect(const std::pair<Point, Point>& start, const std::pair<Point, Point>& end);
    static inline bool constexpr lineRectIntersect(const std::pair<Point, Point>& line, const SDL_Rect& rect);
    static inline bool constexpr pointInRect(const Point& point, const SDL_Rect& rect);
    static inline bool constexpr rectContains(const SDL_Rect& outer, const SDL_Rect& inner);

    unsigned int level;
    SDL_Rect bounds;
    std::array<std::unique_ptr<Quadtree>, BRANCHSIZE> nodes;
    std::vector<Entity *> objects;
};

#endif
