#include <stdio.h>
#include <iostream>
#include <string>
#include "game/Game.h"
#include "log/log.h"
#include <getopt.h>
#include "client/NetworkManager.h"

int main(int argc, char *argv[]) {
    int opt;
    while((opt = getopt(argc, argv, "ni:p:evo:")) != -1){
        switch(opt){
            case 'n':
                networked = true;
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
                printf("-v verbose\n-e error\nverbose enables error as well.");
                break;
        }
    }
    Game game;

    logv("Loading...\n");

    //Start up SDL and create window
    if(game.init() && game.loadMedia())
    {
        logv( "Running...\n");

        game.run();
    } else {
        logv( "Failed to start!\n" );
    }

    game.close();

    logv( "Exit\n" );
    return 0;
}
