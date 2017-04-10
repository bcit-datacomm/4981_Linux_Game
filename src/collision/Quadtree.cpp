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
        return (entity && SDL_HasIntersection(&q.bounds, &entity->getSrcRect()));
    }
    return ((entity && SDL_HasIntersection(&q.bounds, &entity->getSrcRect()))
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
    }
    if (nodes[1] && contains(*nodes[1], entity)) {
        nodes[1]->insert(entity);
    }
    if (nodes[2] && contains(*nodes[2], entity)) {
        nodes[2]->insert(entity);
    }
    if (nodes[3] && contains(*nodes[3], entity)) {
        nodes[3]->insert(entity);
    }
    if (contains(*this, entity)) {
        objects.push_back(entity);
    }
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
    auto retrieved = retrieve(entity->getSrcRect());
    std::sort(retrieved.begin(), retrieved.end());
    retrieved.erase(std::unique(retrieved.begin(), retrieved.end()), retrieved.end());
    return retrieved;
}

std::vector<Entity *> Quadtree::retrieve(const SDL_Rect& rect) const {
    if (level == MAX_LEVELS) {
        return objects;
    }
    std::vector<Entity *> rtn;
    if (!objects.empty()) {
        rtn = objects;
    }
    if (nodes[0] && SDL_HasIntersection(&nodes[0]->bounds, &rect)) {
        const auto& childrtn = nodes[0]->retrieve(rect);
        rtn.insert(rtn.end(), childrtn.begin(), childrtn.end());
    }
    if (nodes[1] && SDL_HasIntersection(&nodes[1]->bounds, &rect)) {
        const auto& childrtn = nodes[1]->retrieve(rect);
        rtn.insert(rtn.end(), childrtn.begin(), childrtn.end());
    }
    if (nodes[2] && SDL_HasIntersection(&nodes[2]->bounds, &rect)) {
        const auto& childrtn = nodes[2]->retrieve(rect);
        rtn.insert(rtn.end(), childrtn.begin(), childrtn.end());
    }
    if (nodes[3] && SDL_HasIntersection(&nodes[3]->bounds, &rect)) {
        const auto& childrtn = nodes[3]->retrieve(rect);
        rtn.insert(rtn.end(), childrtn.begin(), childrtn.end());
    }
    return rtn;
}

