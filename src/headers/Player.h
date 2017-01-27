#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
    void create(); //function displays character

    void setX(); //set x coordinate

    void setY(); //set y coordinate

    void setDX(); //set delta x coordinate

    void setDY(); //set delta y coordinate

    float getX(); // get x coordinate

    float getY(); // get y coordinate

    float getDX(); // get delta x coordinate

    float getDY(); //get delta y coordinate

private:
    float x; //x coordinate
    float y; //y coordinate
    float dx; // delta x coordinat
    float dy; //delta ycoordinate
    int health;
    int state; //used to select sprite to display

};

#endif
