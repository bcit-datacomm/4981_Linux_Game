#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>
#include "LTexture.hpp"

class Player
{
public:
    void create(); //function displays character

    void setX(float px){x=px;} //set x coordinate

    void setY(float py){y=py;} //set y coordinate

    void setDX(float px){dx=px;} //set delta x coordinate

    void setDY(float py){dy=py;} //set delta y coordinate

    float getX(){return x;}; // get x coordinate

    float getY(){return y;}; // get y coordinate

    float getDX(){return dx;} // get delta x coordinate

    float getDY(){return dy;} //get delta y coordinate

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
