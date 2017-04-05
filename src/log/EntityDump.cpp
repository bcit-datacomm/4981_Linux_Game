/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: EntityDump.cpp - Gets all the Entitys in the game and writes their coordinates
--                               to a EntityDumpLog.txt

--
-- FUNCTIONS:
--void dumpEntityPositions(const Player* p);
--std::pair<float, float> getCoordinates(const Entity* e);
--void printEntityPositions(std::string entityName, int32_t id, const Entity &e, std::ofstream &entitydump);
--
-- DATE: March 16, 2008
--
-- REVISIONS: 4/3/2017
--
-- DESIGNER: Maitiu Morton
--
-- PROGRAMMER: Maitiu Morton
--
-- NOTES:
--
----------------------------------------------------------------------------------------------------------------------*/
#include "EntityDump.h"
#include <iostream>
#include "../buildings/DropPoint.h"


/*Created by Maitiu Morton 4/3/2017
 *Loops through each manager and writes there information to
 *the EntityDumpLog.txt
 */
void dumpEntityPositions(const Player* p){
#ifndef NDEBUG
    std::ofstream entityDump("EntityDumpLog.txt", std::ofstream::out);
    GameManager *gm = GameManager::instance();
    const std::pair<float, float> coord = getDestCoordinates(dynamic_cast<Entity*>(p->getMarine()));
    entityDump << "ENTITY POSITION DUMP\n\n";

    entityDump << "PLAYER MARINE'S POSITION X:" << coord.first << " Y:" << coord.second
     << "\n\n";

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
        const std::pair<float, float> coord = getDestCoordinates(s.second.get());
        const std::pair<float, float> mCoord = getMoveCoordinates(s.second.get());
        entityDump << "Store id:" << s.first << "\n\tDest Position: " << "X:" << coord.first
            << " Y:" << coord.second << "\n\tMove Position: " << "X:" << mCoord.first
                << " Y:" << mCoord.second << "\n";
    }

    entityDump << "\nAll Drop POINT POSITIONS:\n";
    //print Store Positions to File
    for (const auto& dp : gm->getDropPointManager()) {
        std::pair<float, float> coord = dp.second.getCoord();
        entityDump << "Drop Point id:" << dp.first << " Position: " << "X:" << coord.first
            << " Y:" << coord.second << "\n";
    }
#endif
}

/*created by Maitiu Morton 4/3/2017
 * Gets the Coordinates of an Entity and returns them as a pair
 */
std::pair<float, float> getDestCoordinates(const Entity* e){
    return e->getDestCoord();
}

/*Create BY Maitiu 4/3/2017
 */
std::pair<float, float> getSrcCoordinates(const Entity* e){
    return e->getSrcCoord();
}

/*Create BY Maitiu 4/3/2017
 */
std::pair<float, float> getMoveCoordinates(const Entity* e){
    return e->getMoveCoord();
}

/*Created by Maitiu Morton 4/3/1017
 * Writes the Entity's name, id and coordinates to EntityDumpLog.txt using
 * an Entity's reference
 */
void printEntityPositions(std::string entityName, int32_t id, const Entity &e, std::ofstream& entityDump){
        const std::pair<float, float> destCoord = getDestCoordinates(&e);
        const std::pair<float, float> srcCoord = getSrcCoordinates(&e);
        const std::pair<float, float> moveCoord = getMoveCoordinates(&e);
        entityDump << entityName << " id:" << id << "\n\t DestRect: " << "X:" << destCoord.first
            << " Y:" << destCoord.second << "\n\t SrcRect: " << "X:" << srcCoord.first
                << " Y:" << srcCoord.second<< "\n\t MoveRect: " << "X:" << moveCoord.first
                    << " Y:" << moveCoord.second << "\n";
}
