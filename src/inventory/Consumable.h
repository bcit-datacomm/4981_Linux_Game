
#ifndef Consumable_H
#define Consumable_H

#include <SDL2/SDL.h>
#include <string>

#include "../log/log.h"

class Marine;
/*
 *  Created by Matthew Goerwell 3/8/2017
 *  This is meant to represent a "pure" consumable, from which all actual consumables are descended.
 *  As an abstract class, it should never exist directly, and handling should occur within a specific
 *  consumable type for anything complex.
 */
class Consumable {
public:
    /**
     * Date:        March. 8, 2017
     * Modified:    -
     * Author:      Matthew Goerwell 
     * Function Interface: Consumable()
     *
     * Description:
     *      Standard constructor for a consumable
     */
    Consumable(){logv("Create Consumable\n");};

    /**
     * Date:        March. 8, 2017
     * Modified:    -
     * Author:      Matthew Goerwell 
     * Function Interface: Consumable()
     *      Consumable& c: A reference to an existing Consumable
     *
     * Description:
     *      Copy constructor for a consumable
     */
    Consumable(const Consumable& c);

    /**
     * Date:        March. 8, 2017
     * Modified:    -
     * Author:      Matthew Goerwell 
     * Function Interface: ~Consumable()
     *
     * Description:
     *      Standard destructor for a consumable
     */
    virtual ~Consumable(){logv("Destroy Consumable\n");};

    /**
     * Date:        March. 8, 2017
     * Modified:    April. 5, 2017
     * Author:      Matthew Goerwell 
     * Function Interface: OnConsume(Marine& marine)
     *      Marine &x: A reference to the marine using this consumable.
     *
     * Description:
     *      This is an abstract method meant to serve as an interface for the specific consume methods
     *      associated with each consumable type.
     */
    virtual void OnConsume(Marine& marine) = 0;
};

#endif
