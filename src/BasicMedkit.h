/*
    Created by Matt Goerwell 3/8/2017
*/
#ifndef BASICMEDKIT_H
#define BASICMEDKIT_H
#include "Consumable.h"
#include <string>

class BasicMedkit: public Consumable {
public:

    BasicMedkit();
    void OnConsume() override;
    ~BasicMedkit();
};

#endif