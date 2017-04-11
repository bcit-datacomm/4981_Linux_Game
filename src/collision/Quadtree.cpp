/*------------------------------------------------------------------------------
* Header: Quadtree.cpp
*
* Functions:
*
*
* Date:
*
* Revisions:
* Edited By : Justen DePourcq- Style guide
*
* Designer:
*
* Author:
*
* Notes:
*
------------------------------------------------------------------------------*/
#include <SDL2/SDL.h>
#include <array>
#include <memory>
#include <algorithm>
#include "Quadtree.h"
#include "../basic/Entity.h"
#include "../buildings/Base.h"

/**
 * Date: April. 9, 2017
 * Modified: -----
 * Author: John Agapeyev
 * Function Interface: (unsigned int pLevel, SDL_Rect pBounds)
 *      pLevel : Levels of the quad tree
 *      pBounds : Bounds of the quadtree area
 *
 * Description:
 *      ctor for a quadtree.
 */
Quadtree::Quadtree(unsigned int pLevel, SDL_Rect pBounds) : level(pLevel), bounds(pBounds) {
    if (level == MAX_LEVELS) {
        return;
    }
    nodes[0] = std::make_unique<Quadtree>(level + 1, SDL_Rect{bounds.x, bounds.y, bounds.w / 2, bounds.h / 2});
    nodes[1] = std::make_unique<Quadtree>(level + 1, SDL_Rect{bounds.x + bounds.w / 2, bounds.y, bounds.w / 2, 
            bounds.h / 2});
    nodes[2] = std::make_unique<Quadtree>(level + 1, SDL_Rect{bounds.x, bounds.y + bounds.h / 2, bounds.w / 2, 
            bounds.h / 2});
    nodes[3] = std::make_unique<Quadtree>(level + 1, SDL_Rect{bounds.x + bounds.w / 2, bounds.y + bounds.h / 2, 
            bounds.w / 2, bounds.h / 2});
}

/**
 * Date: Feb. 8, 2017
 * Modified: -----
 * Author: Jacob McPhail
 * Function Interface: getTreeSize()
 * Description:
 *      Get the number of objects in the tree.
 */
unsigned int Quadtree::getTreeSize() const {
    return objects.size();
}

/**
 * Date: April. 9, 2017
 * Author: John Agapeyev
 * Function Interface: clear()
 * Description:
 *      Clears all objects in the tree.
 */
void Quadtree::clear() {
    if (level == MAX_LEVELS) { 
        objects.clear();
        return;
    }
    if (nodes[0]) {
        nodes[0]->clear();
    }
    if (nodes[1]) {
        nodes[1]->clear();
    }
    if (nodes[2]) {
        nodes[2]->clear();
    }
    if (nodes[3]) {
        nodes[3]->clear();
    }
    objects.clear();
}

/**
 * Date: April. 9, 2017
 * Designer: Isaac Morneau
 * Programmer: John Agapeyev
 * Function Interface: bool contains(const Quadtree& q, const Entity *entity) const
 * Description:
 *      Checks if a given entity exists in the given quadtree
 */
bool Quadtree::contains(const Quadtree& q, const Entity *entity) const {
    if (q.level == MAX_LEVELS) {
        return (entity && rectContains(q.bounds, entity->getDestRect()));
    }
    return ((entity && rectContains(q.bounds, entity->getDestRect()))
            || (q.nodes[0] && contains(*q.nodes[0], entity)) 
            || (q.nodes[3] && contains(*q.nodes[3], entity))
            || (q.nodes[1] && contains(*q.nodes[1], entity))
            || (q.nodes[2] && contains(*q.nodes[2], entity)));
}

/**
 * Date: April. 9, 2017
 * Author: John Agapeyev
 * Function Interface: insert(Entity *entity)
 *      entity : Hitbox to insert
 *
 * Description:
 *      Insert a hitbox into the quadtree.
 */
void Quadtree::insert(Entity *entity) {
    if (level == MAX_LEVELS) {
        objects.push_back(entity);
        return;
    }
    if (nodes[0] && contains(*nodes[0], entity)) {
        nodes[0]->insert(entity);
        return;
    }
    if (nodes[3] && contains(*nodes[3], entity)) {
        nodes[3]->insert(entity);
        return;
    }
    if (nodes[1] && contains(*nodes[1], entity)) {
        nodes[1]->insert(entity);
        return;
    }
    if (nodes[2] && contains(*nodes[2], entity)) {
        nodes[2]->insert(entity);
        return;
    }
    objects.push_back(entity);
}

/**
 * Date: April. 9, 2017
 * Modified: -----
 * Author: John Agapeyev
 * Function Interface: retrieve(const Entity *entity)
 *      entity : Entity to check collisions
 *
 * Description:
 *      Retrieve a vector of hitboxes that are near the param hitbox.      
 */
std::vector<Entity *> Quadtree::retrieve(const Entity *entity) const {
    if (!entity) {
        return {};
    }
    std::vector<Entity *> retrieved;
    retrieved.reserve(100);
    retrieve(retrieved, entity->getDestRect());
    return retrieved;
}

void Quadtree::retrieve(std::vector<Entity *>& retrieveList, const SDL_Rect& rect) const {
    if (level == MAX_LEVELS) {
        retrieveList.insert(retrieveList.end(), objects.begin(), objects.end());
        return;
    }
    if (!objects.empty()) {
        retrieveList.insert(retrieveList.end(), objects.begin(), objects.end());
    }
    if (nodes[0] && SDL_HasIntersection(&nodes[0]->bounds, &rect)) {
        nodes[0]->retrieve(retrieveList, rect);
    }
    if (nodes[3] && SDL_HasIntersection(&nodes[3]->bounds, &rect)) {
        nodes[3]->retrieve(retrieveList, rect);
    }
    if (nodes[1] && SDL_HasIntersection(&nodes[1]->bounds, &rect)) {
        nodes[1]->retrieve(retrieveList, rect);
    }
    if (nodes[2] && SDL_HasIntersection(&nodes[2]->bounds, &rect)) {
        nodes[2]->retrieve(retrieveList, rect);
    }
}

std::vector<Entity *> Quadtree::retrieve(const Point& start, const Point& end) const {
    if (level == MAX_LEVELS) {
        return objects;
    }
    std::vector<Entity *> rtn;
    if (!objects.empty()) {
        rtn = objects;
    }
    if (nodes[0] && lineRectIntersect({start, end}, nodes[0]->bounds)) {
        const auto& childrtn = nodes[0]->retrieve(start, end);
        rtn.insert(rtn.end(), childrtn.begin(), childrtn.end());
    }
    if (nodes[3] && lineRectIntersect({start, end}, nodes[3]->bounds)) {
        const auto& childrtn = nodes[3]->retrieve(start, end);
        rtn.insert(rtn.end(), childrtn.begin(), childrtn.end());
    }
    if (nodes[1] && lineRectIntersect({start, end}, nodes[1]->bounds)) {
        const auto& childrtn = nodes[1]->retrieve(start, end);
        rtn.insert(rtn.end(), childrtn.begin(), childrtn.end());
    }
    if (nodes[2] && lineRectIntersect({start, end}, nodes[2]->bounds)) {
        const auto& childrtn = nodes[2]->retrieve(start, end);
        rtn.insert(rtn.end(), childrtn.begin(), childrtn.end());
    }
    if (level == 0) {
        std::sort(rtn.begin(), rtn.end());
        rtn.erase(std::unique(rtn.begin(), rtn.end()), rtn.end());
    }
    return rtn;
}

/**
 * https://stackoverflow.com/questions/4977491/determining-if-two-line-segments-intersect/4977569#4977569
 */
inline bool constexpr Quadtree::lineIntersect(const std::pair<Point, Point>& start, const std::pair<Point, Point>& end) {
    const float x00 = start.first.first;
    const float y00 = start.first.second;
    const float x10 = start.second.first;
    const float y10 = start.second.second;

    const float x01 = end.first.first;
    const float y01 = end.first.second;
    const float x11 = end.second.first;
    const float y11 = end.second.second;

    const float d = (x11 * y01) - (x01 * y11);

    if (!d) {
        return false;
    }

    const float s = (1 / d) *  ((x00 - x10) * y01 - (y00 - y10) * x01);
    const float t = (1 / d) * -(-(x00 - x10) * y11 + (y00 - y10) * x11);

    return (s >= 0 && s <= 1) && (t >=0 && t <= 1);
}

inline bool constexpr Quadtree::pointInRect(const Point& point, const SDL_Rect& rect) {
    return (rect.x <= point.first) && (point.first <= rect.x + rect.w) 
        && (rect.y <= point.second) && (point.second <= rect.y + rect.h);
}

inline bool constexpr Quadtree::rectContains(const SDL_Rect& outer, const SDL_Rect& inner) {
    return pointInRect({inner.x, inner.y}, outer) && pointInRect({inner.x + inner.w, inner.y + inner.h}, outer);
}

/**
 * Checks if the line intersects any of the 4 boundaries of the rectangle
 * If it doesn't intersect, checks if the line lies entirely inside the rectangle
 */
inline bool constexpr Quadtree::lineRectIntersect(const std::pair<Point, Point>& line, const SDL_Rect& rect) {
    return lineIntersect(line, {{rect.x, rect.y}, {rect.x + rect.w, rect.y}}) //Top
            && lineIntersect(line, {{rect.x, rect.y}, {rect.x, rect.y + rect.h}}) //Left
            && lineIntersect(line, {{rect.x + rect.w, rect.y}, {rect.x + rect.w, rect.y + rect.h}}) //Right
            && lineIntersect(line, {{rect.x, rect.y + rect.h}, {rect.x + rect.w, rect.y + rect.h}}) //Bottom
            && pointInRect(line.first, rect) && pointInRect(line.second, rect);
}
