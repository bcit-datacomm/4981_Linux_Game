/*------------------------------------------------------------------------------
* Source: BasicMedkit.cpp         
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
    Created by Matt Goerwell 3/8/2017
*/
#include "BasicMedkit.h"
#include "../log/log.h"

BasicMedkit::BasicMedkit() : Consumable() {
    logv("BasicMedkit Created\n");
}

BasicMedkit::~BasicMedkit() {
    logv("BasicMedkit Destroyed\n");
}

void BasicMedkit::OnConsume() {
    logv("BasicMedkit Consumed\n");
}
