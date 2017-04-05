#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <array>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
// Tile Size
static constexpr int T_SIZE = 250;

// Map width/height
static constexpr int M_WIDTH = 50;
static constexpr int M_HEIGHT = 50;

// Base width/height
static constexpr int BASE_SIZE = 7;
static constexpr int DROPZONE_SIZE = 4;

// Max number of walls, spawn points and shops
static constexpr int MAX_WALLS = 60;
static constexpr int MAX_SPAWNPOINTS = 16;
static constexpr int MAX_SHOPS = 6;
static constexpr int NUM_DROP_ZONES = 2;

static constexpr char DROP_ZONE_START = '0';
static constexpr char CONCRETE_M = '1';
static constexpr char WALL = '2';
static constexpr char ZOMBIE_SPAWN = '3';
static constexpr char SHOP_SPOT = '4';
static constexpr char BASE_START = '5';
static constexpr char BASE = '6';
static constexpr char FLOOR = '7';
static constexpr char WALL_START = '8';


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
    Map(const std::string& file);
    int loadFileData();
    void genWalls(const std::vector<MapPoint>& wallStart);
    void mapLoadToGame();
    void printData();

    void setFile(const std::string& f) { fname = f; };
    const std::string getFile() const { return fname; };
    const std::vector<MapPoint> getZombieSpawn() const { return zombieSpawn; };
    const std::array<std::array<bool, M_WIDTH>, M_HEIGHT> getAIMap() const { return AIMap; };

private:
    std::string fname;
    // Vector of walls with pixel position and dimensions.
    std::vector<MapWall> walls;
    // Data needed from map file
    std::array<std::array<char, M_WIDTH>, M_HEIGHT> mapdata;
    //zombie spawn point count
    int zombieSpawnCount = 0;
    // Wall start point count
    int dropPointCount = 0;
    // Shop count;
    int shopCount = 0;
    // Base Start Point
    MapPoint base;
    // Array of shop points
    std::array<MapPoint,MAX_SHOPS> shops;
    // Boolean Array for AI Nodes
    std::array<std::array<bool, M_WIDTH>, M_HEIGHT> AIMap;
    // Array of zombieSpawn points
    std::vector<MapPoint> zombieSpawn;
    // Drop Zone Start Points
    std::array<MapPoint, NUM_DROP_ZONES> dropPoints;
};


#endif
