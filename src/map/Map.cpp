/*------------------------------------------------------------------------------
* Source: Map.cpp
*
* Functions:
*       Map(string file);
*       int loadFileData();
*       void genWalls(const struct MapPoint wstart[MAX_WALLS]);
*       string getFile();
*       void setFile(string f);
*       void mapLoadToGame();
*       void printData();
*
* Date: March 23, 2017
*
* Revisions:
* Edited By : Name 2016/JAN/25 - Description
*
* Designer: Jordan Lin
*
* Author: Jordan Lin
*
* Notes:
* Map Class functions used for reading in a map file and loading the map into the game.
------------------------------------------------------------------------------*/
#include "Map.h"

using namespace std;

/**
 * Date: Mar. 23, 2017
 * Author: Jordan Lin
 * Function Interface: Map(string file)
 * Parameters:
 *      file - File name, must be a CSV File
 * Description:
 * Map Object Constructor, creates a map object using the map selected file name.
 */
Map::Map(string file):fname(file) {
    for(int i = 0; i < M_HEIGHT; i++) {
        for(int j = 0; j < M_WIDTH; j++) {
            AIMap[i][j] = 0;
            mapdata[i][j] = 0;
        }
    }
}

/**
 * Date: Mar. 23, 2017
 * Author: Jordan Lin
 * Function Interface: int loadFileData()
 * Description:
 * Read's the Map CSV file map data with the appropriate tile IDs
 */
int Map::loadFileData() {
    // Open file stream for selected map file
    ifstream file;
    file.open(fname.c_str());

    if (file.fail()) {
        printf("failed to open file\n");
        return 0;
    }

    // Array of wallstart positions
    struct MapPoint wallStart[MAX_WALLS];
    // Indices
    int j = 0;
    int i = 0;

    // Character Variable
    char ch;
    while (file >> noskipws >> ch) {
        switch (ch) {
            case ',':
                continue;
            case '\n':
                j = 0;
                i++;
                continue;
            case WALL:              //Wall piece found, update AI Map
                AIMap[i][j] = 1;
                break;
            case WALL_START:        //Label start of wall rectangle
                wallStart[ws].x = j;
                wallStart[ws].y = i;
                ws++;
                AIMap[i][j] = 1;
                break;
            // case CONCRETE_START:    //Start of concrete area
            case ZOMBIE_SPAWN:      //Zombie spawn Points
                zombieSpawn[zs].x = j;
                zombieSpawn[zs].y = i;
                zs++;
                break;
            case SHOP_SPOT:         // Shop spot points
                shops[s].x = j;
                shops[s].y = i;
                s++;
                break;
            case BASE_START:        //Start of base area
                base.x = j;
                base.y = i;
                break;
        }
        mapdata[i][j] = ch;               //Store map file value
        j++;
    }
    file.close();

    genWalls(wallStart);

    // printData();
    return 1;
}

/**
 * Date: Mar. 23, 2017
 * Author: Jordan Lin
 * Function Interface: void genWalls(const struct MapPoint wstart[MAX_WALLS])
 * Parameters:
 *      wstart - Array of wall start MapPoints.
 * Description:
 * Read's the Map CSV file map data with the appropriate tile IDs
 */
void Map::genWalls(const struct MapPoint wstart[MAX_WALLS]) {
    for (int q = 0; q < ws; q++) {
        int startx = wstart[q].x;
        int starty = wstart[q].y;
        int nextx = startx + 1;
        int nexty = starty + 1;
        int endx;
        int endy;
        while (mapdata[starty][nextx] == WALL) {
            nextx ++;
        }
        endx = nextx;
        while (mapdata[nexty][startx] == WALL) {
            nexty++;
        }
        endy = nexty;
        MapWall temp;
        temp.x = startx * T_SIZE;
        temp.y = starty * T_SIZE;
        temp.width = (endx - startx) * T_SIZE;
        temp.height = (endy - starty) * T_SIZE;
        walls.push_back(temp);
    }
}

/**
 * Date: Mar. 23, 2017
 * Author: Jordan Lin
 * Function Interface: void mapLoadToGame()
 * Description:
 * Creates the wall structures using the GameManager createWall function.
 */
void Map::mapLoadToGame() {
    for(auto w : walls) {
        logv("Creating wall");
        GameManager::instance()->createWall(w.x, w.y, w.width, w.height);
    }
}

/**
 * Date: Mar. 23, 2017
 * Author: Jordan Lin
 * Function Interface: string getFile()
 * Parameters:
 * Description:
 * Get the current map file name
 */
string Map::getFile() {
    return fname;
}

/**
 * Date: Mar. 23, 2017
 * Author: Jordan Lin
 * Function Interface: string getFile()
 * Parameters:
 * Description:
 * Set the current map file name
 */
void Map::setFile(string f) {
    fname = f;
}

/**
 * Date: Mar. 23, 2017
 * Author: Jordan Lin
 * Function Interface: string getFile()
 * Parameters:
 * Description:
 * Print Map Data loaded includeing:
 *  - Base Position
 *  - Zombie spawn points
 *  - Shop Positions
 *  - Wall Position/Dimensions
 */
void Map::printData() {
    cout << "MAP DATA" << endl;
    for (int q = 0; q < M_HEIGHT; q++) {
        for (int w = 0; w < M_WIDTH; w++) {
            cout << mapdata[q][w];
        }
        std::cout << std::endl;
    }
    cout << endl;

    cout << "BASE POSITIONS" << endl;
    printf("Position: %d, %d\n", base.x, base.y);
    cout << endl;

    cout << "SPAWN POINTS" << endl;
    for(int i = 0; i < zs; i++) {
        printf("Spawn Point: %d; Position: %d, %d\n", i, zombieSpawn[i].x, zombieSpawn[i].y);
    }
    cout << endl;

    cout << "SHOP POSITIONS" << endl;
    for(int i = 0; i < s; i++) {
        printf("Shop: %d; Position: %d, %d\n", i, shops[i].x, shops[i].y);
    }

    cout << "AI MAP" << endl;
    for (int q = 0; q < M_HEIGHT; q++) {
        for (int w = 0; w < M_WIDTH; w++) {
            cout << AIMap[q][w];
        }
        std::cout << std::endl;
    }
    cout << endl;

    cout << "WALL DATA" << endl;
    for (int q = 0; q < ws; q++) {
    printf("Wall: %d;\t Position: %d, %d;\t\t width = %d, height = %d\n", q, walls[q].x / 250,
            walls[q].y / 250, walls[q].width / T_SIZE, walls[q].height / T_SIZE);
    }
}
