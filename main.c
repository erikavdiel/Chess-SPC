/*
 * main.c
 *
 *  Created on: 17 בספט 2017
 *      Author: ערי
 */


#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include <string.h>
#include "SPChessGUIManager.h"
#include "SPChessMainAux.h"

int main(int argc, char** argv) {
	if ( argc <= 1 || strcmp(argv[1], "-c") == 0){
		spChessConsoleMode();
	}
	else if (strcmp(argv[1], "-g") == 0){
		spChessGuiMode();
	}
	return 0;
}
