#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>
#include "LTexture.hpp"

class Player
{
public:
    void create(); //function displays character

    void setX(); //set x coordinate

    void setY(); //set y coordinate

    void setDX(); //set delta x coordinate

    void setDY(); //set delta y coordinate

    float getX(){return x;}; // get x coordinate

    float getY(){return y;}; // get y coordinate

    float getDX(); // get delta x coordinate

    float getDY(); //get delta y coordinate

    std::string getSpritePath(){return spritePath;};//returns path of players sprite

	Player();
	~Player();
	
	LTexture playerTexture;
	SDL_Rect playerSpriteClips[1];
	
private:
    float x = 200; //x coordinate
    float y = 200; //y coordinate
    float dx; // delta x coordinat
    float dy; //delta ycoordinate
    int health;
    int state; //used to select sprite to display
    std::string spritePath = "assets/texture/arrow.png";
	

};

#endif
