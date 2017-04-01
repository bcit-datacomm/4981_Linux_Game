#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../game/GameManager.h"

using namespace std;

#define T_SIZE 150
#define BASE_SIZE 7
#define MAX_WALLS 60
#define MAX_SPAWNPOINTS 16
#define M_WIDTH 100
#define M_HEIGHT 100

#define CONCRETE_START  '0'
#define CONCRETE_M      '1'
#define WALL            '2'
#define ZOMBIE_SPAWN    '3'
#define SHOP_SPOT       '4'
#define BASE_START      '5'
#define BASE            '6'
#define FLOOR           '7'
#define WALL_START      '8'

struct MapPoint {
public:
    int x;
    int y;
};

struct MapWall {
public:
    int x;
    int y;
    int width;
    int height;
};

class Map {
public:
    Map(string file);
    int loadFileData();
    void genWalls(const struct MapPoint wstart[MAX_WALLS]);
    string getFile();
    void setFile(string f);
    void mapLoadToGame();
    void printData();

    // Data needed from map file
    char mapdata[M_HEIGHT][M_WIDTH];
    //zombie spawn point count
    int zs;
    // Wall start point count
    int ws;
    // Shop count;
    int s;
    // Base Start Point
    struct MapPoint base;
    // Array of zombieSpawn points
    struct MapPoint zombieSpawn[MAX_SPAWNPOINTS];
    // Array of shop points
    struct MapPoint shops[6];
    // Boolean Array for AI Nodes
    bool AIMap[M_HEIGHT][M_WIDTH];
    // Vector of walls with pixel position and dimensions.
    vector<MapWall> walls;

private:
    string fname;
};


#endif
