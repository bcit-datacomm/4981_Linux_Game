 #include "Quadtree.h"



Quadtree::Quadtree(int pLevel, SDL_Rect pBounds) {
       level = pLevel;
       bounds = pBounds;
    objectCounter = 0;
    for (unsigned int i = 0; i < BRANCHSIZE; i++) {
        nodes[i] = nullptr;
       }
}

Quadtree::~Quadtree() {
    for (unsigned int i = 0; i < BRANCHSIZE; i++) {
        if (nodes[i] != nullptr) {
            nodes[i]->clear();
            delete nodes[i];
            nodes[i] = nullptr;
        }
   }
}

unsigned int Quadtree::getTreeSize() const{
    return objectCounter;
}

void Quadtree::clear() {
   objects.clear();

   for (unsigned int i = 0; i < BRANCHSIZE; i++) {
     if (nodes[i] != nullptr) {
       nodes[i]->clear();
        delete nodes[i];
         nodes[i] = nullptr;
     }
   }
    objectCounter = 0;
}

void Quadtree::split() {
   int subWidth = (int)(bounds.w / 2);
   int subHeight = (int)(bounds.h / 2);
   int x = (int)bounds.x;
   int y = (int)bounds.y;

   nodes[0] = new Quadtree(level+1, {x + subWidth, y, subWidth, subHeight});
   nodes[1] = new Quadtree(level+1, {x, y, subWidth, subHeight});
   nodes[2] = new Quadtree(level+1, {x, y + subHeight, subWidth, subHeight});
   nodes[3] = new Quadtree(level+1, {x + subWidth, y + subHeight, subWidth, subHeight});
}

int Quadtree::getIndex(const HitBox* pRect) const{
   int index = -1;
   double verticalMidpoint = bounds.x + (bounds.w / 2);
   double horizontalMidpoint = bounds.y + (bounds.h / 2);

   // Object can completely fit within the top quadrants
   bool topQuadrant = (pRect->getRect().y < horizontalMidpoint && pRect->getRect().y + pRect->getRect().h < horizontalMidpoint);
   // Object can completely fit within the bottom quadrants
   bool bottomQuadrant = (pRect->getRect().y > horizontalMidpoint);

   // Object can completely fit within the left quadrants
   if (pRect->getRect().x < verticalMidpoint && pRect->getRect().x + pRect->getRect().w < verticalMidpoint) {
      if (topQuadrant) {
        index = 1;
      }
      else if (bottomQuadrant) {
        index = 2;
      }
    }
    // Object can completely fit within the right quadrants
    else if (pRect->getRect().x > verticalMidpoint) {
     if (topQuadrant) {
       index = 0;
     }
     else if (bottomQuadrant) {
       index = 3;
     }
   }

   return index;
 }


void Quadtree::insert(HitBox* pRect) {
    objectCounter++;
   if (nodes[0] != nullptr) {
     int index = getIndex(pRect);

     if (index != -1) {
       nodes[index]->insert(pRect);

       return;
     }
   }

   objects.push_back(pRect);

   if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
      if (nodes[0] == nullptr) {
         split();
      }

     unsigned int i = 0;
     while (i < objects.size()) {
       int index = getIndex(objects.at(i));
       if (index != -1) {
         nodes[index]->insert(objects.at(i));
         objects.erase(objects.begin()+i);
       }
       else {
         i++;
       }
     }
   }
 }


std::vector<HitBox*> Quadtree::retrieve(std::vector<HitBox*> returnObjects, HitBox* pRect) {
   int index = getIndex(pRect);
   if (index != -1 && nodes[0] != nullptr) {
     returnObjects = nodes[index]->retrieve(returnObjects, pRect);
   }

   returnObjects.insert(std::end(returnObjects), std::begin(objects), std::end(objects));

   return returnObjects;
}
