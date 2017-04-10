/*
 *  Created by Matthew Goerwell 3/8/2017
 *  This is meant to serve as our basic consumable, to provide an example of how others would be 
 *  implemented. It represents a simple medkit that will heal the player to full health when used.
 */
#include "BasicMedkit.h"
#include "../log/log.h"
#include "../player/Marine.h"

/**
 * Date:        March. 8, 2017
 * Modified:    -
 * Author:      Matthew Goerwell 
 * Function Interface: BasicMedkit()
 *
 * Description:
 *      Standard constructor for a basic medkit.
 */
BasicMedkit::BasicMedkit() : Consumable() {
    logv("BasicMedkit Created\n");
}

/**
 * Date:        March. 8, 2017
 * Modified:    -
 * Author:      Matthew Goerwell 
 * Function Interface: ~BasicMedkit()
 *
 * Description:
 *      Standard destructor for a basic medkit.
 */
BasicMedkit::~BasicMedkit() {
    logv("BasicMedkit Destroyed\n");
}

/**
 * Date:        March. 8, 2017
 * Modified:    April. 5, 2017
 * Author:      Matthew Goerwell 
 * Function Interface: OnConsume(Marine& marine)
 *      Marine &x: A reference to the marine using the medkit.
 *
 * Description:
 *      This is the implementation of the consume method for medkits. It sets the health of the marine
 *      using it to full health (100). 
 */
void BasicMedkit::OnConsume(Marine& marine) {
	marine.setHealth(MARINE_MAX_HEALTH);
    logv("BasicMedkit Consumed\n");
}
