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
#include <array>
#include <memory>
#include "Quadtree.h"
#include "../basic/Entity.h"

/**
 * Date: Feb. 8, 2017
 * Modified: -----
 * Author: Jacob McPhail
 * Function Interface: (int pLevel, SDL_Rect pBounds)
 *      pLevel : Levels of the quad tree
 *      pBounds : Bounds of the quadtree area
 *
 * Description:
 *      ctor for a quadtree.
 */
Quadtree::Quadtree(int pLevel, SDL_Rect pBounds) : objectCounter(0), level(pLevel), bounds(pBounds) {}

Quadtree& Quadtree::operator=(const Quadtree& quad) {
    objects = quad.objects;
    objectCounter = quad.objectCounter;
    level = quad.level;
    bounds = quad.bounds;
    nodes = quad.nodes;
    return *this;
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
    return objectCounter;
}

/**
 * Date: Feb. 8, 2017
 * Modified: -----
 * Author: Jacob McPhail
 * Function Interface: clear()
 * Description:
 *      Clears all objects in the tree.
 */
void Quadtree::clear() {
    objects.clear();
    objectCounter = 0;
    nodes.fill(nullptr);
}

/**
 * Date: Feb. 8, 2017
 * Modified: -----
 * Author: Jacob McPhail
 * Function Interface: split()
 * Description:
 *      Splits branch into set number of branches.
 */
void Quadtree::split() {
    const int subWidth = static_cast<int>(bounds.w / 2);
    const int subHeight = static_cast<int>(bounds.h / 2);
    const int x = static_cast<int>(bounds.x);
    const int y = static_cast<int>(bounds.y);

    nodes[0] = std::make_shared<Quadtree>(level + 1, SDL_Rect{x + subWidth, y, subWidth, subHeight});
    nodes[1] = std::make_shared<Quadtree>(level + 1, SDL_Rect{x, y, subWidth, subHeight});
    nodes[2] = std::make_shared<Quadtree>(level + 1, SDL_Rect{x, y + subHeight, subWidth, subHeight});
    nodes[3] = std::make_shared<Quadtree>(level + 1, SDL_Rect{x + subWidth, y + subHeight, subWidth, subHeight});
}

/**
 * Date: Feb. 8, 2017
 * Modified: -----
 * Author: Jacob McPhail
 * Function Interface: getIndex(const HitBox *pRect) 
 *      pRect : Hitbox to get index
 *
 * Description:
 *      Gets a tree index using a Hitbox.
 */
int Quadtree::getIndex(const HitBox *pRect) const {
    const double verticalMidpoint = bounds.x + (bounds.w / 2);
    const double horizontalMidpoint = bounds.y + (bounds.h / 2);

    const auto& hitRect = pRect->getRect();

    // Object can completely fit within the top quadrants
    const bool topQuadrant = (hitRect.y < horizontalMidpoint && hitRect.y + hitRect.h < horizontalMidpoint);
    // Object can completely fit within the bottom quadrants
    const bool bottomQuadrant = (hitRect.y > horizontalMidpoint);

    // Object can completely fit within the left quadrants
    if (hitRect.x < verticalMidpoint && hitRect.x + hitRect.w < verticalMidpoint) {
        if (topQuadrant) {
            return 1;
        }
        if (bottomQuadrant) {
            return 2;
        }
    } else if (hitRect.x > verticalMidpoint) {// Object can completely fit within the right quadrants
        if (topQuadrant) {
            return 0;
        }
        if (bottomQuadrant) {
            return 3;
        }
    }
    return -1;
}

/**
 * Date: Feb. 8, 2017
 * Modified: -----
 * Author: Jacob McPhail.
 * Function Interface: insert(Entity *entity)
 *      entity : Hitbox to insert
 *
 * Description:
 *      Insert a hitbox into the quadtree.
 */
void Quadtree::insert(Entity *entity) {
    ++objectCounter;
    if (nodes[0]) {
        const int index = getIndex(&entity->getMoveHitBox());
        if (index != -1) {
            nodes[index]->insert(entity);
            return;
        }
    }

    objects.push_back(entity);

    if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
        if (!nodes[0]) {
            split();
        }

        for (unsigned int i = 0; i < objects.size();) {
            const int index = getIndex(&objects[i]->getMoveHitBox());
            if (index != -1) {
                nodes[index]->insert(objects[i]);
                objects.erase(objects.begin() + i);
            } else {
                ++i;
            }
        }
    }
}

/**
 * Date: Feb. 8, 2017
 * Modified: -----
 * Author: Jacob McPhail.
 * Function Interface: retrieve(const Entity *entity)
 *      entity : Entity to check collisions
 *
 * Description:
 *      Retrieve a vector of hitboxes that are near the param hitbox.      
 */
std::vector<Entity *> Quadtree::retrieve(const Entity *entity) const {
    std::vector<Entity *> returnObjects;
    const int index = getIndex(&entity->getMoveHitBox());
    if (index != -1 && nodes[0]) {
        returnObjects = nodes[index]->retrieve(entity);
    }
    returnObjects.insert(std::end(returnObjects), std::begin(objects), std::end(objects));
    return returnObjects;
}

