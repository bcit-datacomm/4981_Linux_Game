#include <stdio.h>
#include <iostream>
#include <string>
#include "Game.h"


int main() {
    
    Game* game = new Game();
    
    printf( "Loading...\n");
    
    //Start up SDL and create window
    if(game->init() && game->loadMedia())
    {
        printf( "Running...\n");
    
        game->run();
        
    } else {
        printf( "Failed to start!\n" );
    }

    //Free resources and close SDL
    game->close();
    
    delete game;
    printf( "Exit\n" );
    return 0;
}
