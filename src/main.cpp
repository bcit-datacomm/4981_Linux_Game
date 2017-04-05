/*------------------------------------------------------------------------------
* Source: main.cpp     
*
* Functions:
*    
*
* Date: 
*
* Revisions: 
* Edited By : Yiaoping Shu- Style guide
*
* Designer: 
*
* Author: 
*
* Notes:
*  
------------------------------------------------------------------------------*/
#include <stdio.h>
#include <iostream>
#include <string>
#include <getopt.h>

#include "game/Game.h"
#include "log/log.h"

int main(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "evio")) != -1) {
        switch (opt) {
            case 'i'://verbose
                log_verbose = 3;
                break;
            case 'v'://verbose
                log_verbose = 2;
                break;
            case 'e'://error
                log_verbose = 1;
                break;
            case 'o':
                log_verbose = atoi(optarg);
                break;
            case '?':
                printf("-v verbose\n-e error\nverbose enables error as well.\n");
                break;
        }
    }
    Game game;

    logv("Loading...\n");

    //Start up SDL and create window
    if (game.init() && game.loadMedia()) {
        logv("Running...\n");
        game.run();
    } else {
        logv("Failed to start!\n");
    }

    logv("Exit\n");
    return 0;
}
