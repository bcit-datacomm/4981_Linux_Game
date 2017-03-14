/*--------------------------------------------------------------------------------------------
-- HEADER FILE: Common.h
--
-- FUNCTIONS:

--
-- DATE:        March 12, 2017
--
-- DESIGNER:    Fred Yang, Robert Arendac
--
-- PROGRAMMER:  Fred Yang, Robert Arendac
--
-- NOTES:
-- This header file includes the common macro definitions and function 
-- prototypes for the project.
---------------------------------------------------------------------------------------------*/

#ifndef COMMON_H
#define COMMON_H
#include <iostream>

// pi
#define PI 3.14159265

// font size
#define FONT_SIZE   28

// map width/height
#define MAP_WIDTH   2000
#define MAP_HEIGHT  2000

// The size of a tile/node
#define TILE_SIZE   50

// base width/height
#define BASE_WIDTH  100
#define BASE_HEIGHT 100

// zombie spawn points
#define SPAWN_X1    600
#define SPAWN_Y1    900
#define SPAWN_X2    500
#define SPAWN_Y2    1500
 
// 8 possible directions
#define DIR_CAP     8
#define DIR_R       0
#define DIR_RD      1
#define DIR_D       2
#define DIR_LD      3
#define DIR_L       4
#define DIR_LU      5
#define DIR_U       6
#define DIR_RU      7

// Cardinal directions for setting angles
#define NORTH 0
#define NORTHEAST 45
#define EAST 90
#define SOUTHEAST 135
#define SOUTH 180
#define SOUTHWEST 225
#define WEST 270
#define NORTHWEST 315

// ticks (ms) in 1 second
const float TICK_SEC = 1000.f;

// overlapped
const float OVERLAP = 0.1f;

// zombie state
typedef enum {
    ZOMBIE_IDLE,
    ZOMBIE_MOVE,
    ZOMBIE_ATTACK,
    ZOMBIE_DIE
} ZOMBIE_STATE;

#endif
