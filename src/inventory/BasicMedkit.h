/*------------------------------------------------------------------------------
* Source: BasicMedkit.h        
*
* Functions:
*
* Date: 
*
* Revisions: 
* Edited By : Tim Makimov on 2017/APR/05
*
* Designer: 
*
* Author: 
*
* Notes:
------------------------------------------------------------------------------*/
/*
    Created by Matthew Goerwell 3/8/2017
*/
#ifndef BASICMEDKIT_H
#define BASICMEDKIT_H
#include <string>

#include "Consumable.h"

/*
    This class represents a simple consumable in order to test basic consumable functionality.
    For the moment it does nothing, but in future builds it will add health to the player.
*/
class BasicMedkit: public Consumable {
public:
    //standard ctor
    BasicMedkit();
    //standard dtor
    ~BasicMedkit();
    //Consumable specific OnConsume Method
    void OnConsume() override;
};

#endif
