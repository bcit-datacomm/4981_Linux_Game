#ifndef WEAPONDROP_H
#define WEAPONDROP_H
#include <string>
#include "LTexture.h"
#include "HitBox.h"
#include "Entity.h"
#include "CollisionHandler.h"
#include "Weapon.h"
#include <SDL2/SDL.h>

class WeaponDrop: public Entity{
public:
    WeaponDrop(Weapon &w);
    ~WeaponDrop();
    void setPosition(int x, int y);
    void onCollision();

    void collidingProjectile(int damage);
	
    int getX() const;
    int getY() const;
    Weapon& getWeapon();


private:
    int xCoord;
    int yCoord;
    Weapon &weapon;
};


#endif
