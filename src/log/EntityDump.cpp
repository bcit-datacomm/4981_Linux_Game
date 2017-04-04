#include "EntityDump.h"
#include <iostream>
#include "../buildings/DropPoint.h"


/*Created by Maitiu Morton 4/3/2017
 *Loops through each manager and writes there information to
 *the EntityDumpLog.txt
 */
void dumpEntityPosistions(const Player* p){
    #ifndef NDEBUG
    std::ofstream entityDump;
    std::pair<float, float> coord;
    GameManager *gm = GameManager::instance();


    entityDump.open("EntityDumpLog.txt");

    entityDump << "ENTITY POSITION DUMP\n\n";

    coord  = getCoordinates(dynamic_cast<Entity*>(p->getMarine()));
    entityDump << "PLAYER MARINE'S POSITION X:" << coord.first << " Y:" << coord.second
     << std::endl << std::endl;

    entityDump << "All MARINE POSITIONS:\n";
    //print Marine Positions
    for (const auto& m : gm->getMarineManager()) {
        printEntityPositions("Marine", m.first, m.second, entityDump);
    }
    entityDump << "\nAll TURRET POSITIONS:\n";
    //print Turret Positions
    for (const auto& t : gm->getTurretManager()) {
        printEntityPositions("Turret", t.first, t.second, entityDump);
    }

    entityDump << "\nAll ZOMBIE POSITIONS:\n";
    //print Zombie Positions
    for (const auto& z : gm->getZombieManager()) {
        printEntityPositions("Zombie", z.first, z.second, entityDump);
    }

    entityDump << "\nAll WEAPON DROP POSITIONS:\n";
    //print WeaponDrop Positions
    for (const auto& wd : gm->getWeaponDropManager()) {
        printEntityPositions("WeaponDrop", wd.first, wd.second, entityDump);
    }

    entityDump << "\nAll BARRICADE POSITIONS:\n";
    //print WeaponDrop Positions
    for (const auto& b : gm->getBarricadeManager()) {
        printEntityPositions("Barricade", b.first, b.second, entityDump);
    }

    entityDump << "\nAll WALL POSITIONS:\n";
    //print WeaponDrop Positions
    for (const auto& w : gm->getWallManager()) {
        printEntityPositions("Wall", w.first, w.second, entityDump);
    }

    entityDump << "\nAll STORE POSITIONS:\n";
    //print Store Positions to File
    for (const auto& s : gm->getStoreManager()) {
        std::pair<float, float> coord = getCoordinates(s.second.get());
        entityDump << "Store id:" << s.first << " Position: " << "X:" << coord.first
            << " Y:" << coord.second << std::endl;
    }

    entityDump << "\nAll Drop POINT POSITIONS:\n";
    //print Store Positions to File
    for (const auto& dp : gm->getDropPointManager()) {
        std::pair<float, float> coord = dp.second.getCoord();
        entityDump << "Drop Point id:" << dp.first << " Position: " << "X:" << coord.first
            << " Y:" << coord.second << std::endl;
    }

    entityDump.close();

    #endif
}

/*created by Maitiu Morton 4/3/2017
 * Gets the Coordinates of an Entity and returns them as a pair
 */
std::pair<float, float> getCoordinates(const Entity* e){
    std::pair<float, float> coord;
    coord.first = e->getX();
    coord.second = e->getY();
    return coord;
}

/*Created by Maitiu Morton 4/3/1017
 * Writes the Entity's name, id and coordinates to EntityDumpLog.txt using
 * an Entity's reference
 */
void printEntityPositions(std::string entityName, int32_t id, const Entity &e, std::ofstream& entityDump){
        std::pair<float, float> coord = getCoordinates(&e);
        entityDump << entityName << " id:" << id << " Position: " << "X:" << coord.first
            << " Y:" << coord.second << std::endl;
}
