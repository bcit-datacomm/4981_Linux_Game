#include <stdio.h>
#include <iostream>
#include <string>
#include "game/Game.h"
#include "log/log.h"
#include <getopt.h>


int main(int argc, char *argv[]) {
    int opt;
    while((opt = getopt(argc, argv, "ev")) != -1){
        switch(opt){
            case 'v'://verbose
                log_verbose = 2;
                break;
            case 'e'://error
                log_verbose = 1;
                break;
            case '?':
                printf("-v verbose\n-e error\nverbose enables error as well.");
                break;
        }
    }
    Game game;

    logv( "Loading...\n");

    //Start up SDL and create window
    if(game.init() && game.loadMedia())
    {
        logv( "Running...\n");

        game.run();

    } else {
        logv( "Failed to start!\n" );
    }

    //Free resources and close SDL
    game.close();

    logv( "Exit\n" );

    return 0;
}
