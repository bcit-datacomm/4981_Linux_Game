#include <array>
#include <memory>
#include "Quadtree.h"

Quadtree::Quadtree(int pLevel, SDL_Rect pBounds) {
    level = pLevel;
    bounds = pBounds;
    objectCounter = 0;
}

Quadtree& Quadtree::operator=(const Quadtree& quad) {
    objects = quad.objects;
    objectCounter = quad.objectCounter;
    level = quad.level;
    bounds = quad.bounds;
    nodes = quad.nodes;
    return *this;
}

unsigned int Quadtree::getTreeSize() const{
    return objectCounter;
}

void Quadtree::clear() {
    objects.clear();
    objectCounter = 0;
    for (unsigned int i = 0; i < BRANCHSIZE; ++i) {
        nodes[i] = nullptr;
    }
}

void Quadtree::split() {
    int subWidth = static_cast<int>(bounds.w / 2);
    int subHeight = static_cast<int>(bounds.h / 2);
    int x = static_cast<int>(bounds.x);
    int y = static_cast<int>(bounds.y);

    nodes[0] = std::make_shared<Quadtree>(level+1, SDL_Rect{x + subWidth, y, subWidth, subHeight});
    nodes[1] = std::make_shared<Quadtree>(level+1, SDL_Rect{x, y, subWidth, subHeight});
    nodes[2] = std::make_shared<Quadtree>(level+1, SDL_Rect{x, y + subHeight, subWidth, subHeight});
    nodes[3] = std::make_shared<Quadtree>(level+1, SDL_Rect{x + subWidth, y + subHeight, subWidth, subHeight});
}

int Quadtree::getIndex(const HitBox *pRect) const{
    int index = -1;
    double verticalMidpoint = bounds.x + (bounds.w / 2);
    double horizontalMidpoint = bounds.y + (bounds.h / 2);

    const auto& hitRect = pRect->getRect();

    // Object can completely fit within the top quadrants
    bool topQuadrant = (hitRect.y < horizontalMidpoint && hitRect.y + hitRect.h < horizontalMidpoint);
    // Object can completely fit within the bottom quadrants
    bool bottomQuadrant = (hitRect.y > horizontalMidpoint);

    // Object can completely fit within the left quadrants
    if (hitRect.x < verticalMidpoint && hitRect.x + hitRect.w < verticalMidpoint) {
        if (topQuadrant) {
            index = 1;
        } else if (bottomQuadrant) {
            index = 2;
        }
    }
    // Object can completely fit within the right quadrants
    else if (hitRect.x > verticalMidpoint) {
        if (topQuadrant) {
            index = 0;
        } else if (bottomQuadrant) {
            index = 3;
        }
    }
    return index;
}

void Quadtree::insert(Entity *entity) {
    objectCounter++;
    if (nodes[0] != nullptr) {
        int index = getIndex(entity->movementHitBox.get());
        if (index != -1) {
            nodes[index]->insert(entity);
            return;
        }
    }

    objects.push_back(entity);

    if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
        if (nodes[0] == nullptr) {
            split();
        }

        unsigned int i = 0;
        while (i < objects.size()) {
            int index = getIndex(objects.at(i)->movementHitBox.get());
            if (index != -1) {
                nodes[index]->insert(objects.at(i));
                objects.erase(objects.begin()+i);
            } else {
                i++;
            }
        }
    }
}


std::vector<Entity *> Quadtree::retrieve(const Entity *entity) {
    std::vector<Entity *> returnObjects;
    int index = getIndex(entity->movementHitBox.get());
    if (index != -1 && nodes[0] != nullptr) {
        returnObjects = nodes[index]->retrieve(entity);
    }
    returnObjects.insert(std::end(returnObjects), std::begin(objects), std::end(objects));
    return returnObjects;
}
