/*
 * SPChessMenuWindow.c
 *
 *  Created on: 18 בספט 2017
 *      Author: ערי
 */
#include "SPChessMenuWindow.h"


SPChessButton** spMenuCreateMainMenuWidgets(SDL_Renderer* rend){
	if (rend == NULL ) {
		return NULL ;
	}
	SPChessButton** buttons = malloc(sizeof(SPChessButton*)*3);
	if (buttons == NULL ) {
		return NULL ;
	}
	SDL_Rect newGame = { .x = 100, .y = 30, .h = 72, .w = 180 };
	SDL_Rect exit = { .x = 480, .y = 30, .h = 72, .w = 180 };
	SDL_Rect load = { .x = 100, .y = 115, .h = 72, .w = 180 };
	buttons[0] = spChessButtonCreate(rend, &newGame, true, "./images/buttons/NewGameActive.bmp",
			"./images/buttons/NewGameNotActive.bmp", SP_BUTTON_NEWGAME);
	buttons[1] = spChessButtonCreate(rend, &load, true, "./images/buttons/LoadActive.bmp",
			"./images/buttons/LoadNonActive.bmp", SP_BUTTON_GOTOLOAD);
	buttons[2] = spChessButtonCreate(rend, &exit, true, "./images/buttons/ExitActive.bmp",
			"./images/buttons/ExitNonActive.bmp", SP_BUTTON_EXIT);
	if (buttons[0] == NULL || buttons[1] == NULL || buttons[2] == NULL ) {
		spChessButtonDestroy(buttons[0]);
		spChessButtonDestroy(buttons[1]);
		spChessButtonDestroy(buttons[2]);
		free(buttons);
		return NULL ;
	}
	return buttons;
}





SPChessButton** spMenuCreateGameModeWidgets(SDL_Renderer* rend){
	if (rend == NULL ) {
		return NULL ;
	}
	SPChessButton** buttons = malloc(sizeof(SPChessButton*)*4);
	if (buttons == NULL ) {
		return NULL ;
	}
	SDL_Rect onePlayer = { .x = 120, .y = 50, .h = 72, .w = 180 };
	SDL_Rect twoPlayers = { .x = 120, .y = 140, .h = 72, .w = 180 };
	SDL_Rect startNext = { .x = 550, .y = 30, .h = 72, .w = 180 };
	SDL_Rect back = { .x = 550, .y = 200, .h = 72, .w = 180 };
	buttons[0] = spChessButtonCreate(rend, &onePlayer, false, "./images/buttons/OnePlayer.bmp",
			"./images/buttons/OnePlayerNA.bmp", SP_BUTTON_PLAYERMODE);
	buttons[1] = spChessButtonCreate(rend, &twoPlayers, true, "./images/buttons/TwoPlayers.bmp",
			"./images/buttons/TwoPlayersNA.bmp", SP_BUTTON_PLAYERMODE);
	buttons[2] = spChessButtonCreate(rend, &startNext, true, "./images/buttons/Next.bmp",
			"./images/buttons/Start.bmp", SP_BUTTON_NEXT);
	buttons[3] = spChessButtonCreate(rend, &back, true, "./images/buttons/Back.bmp",
			"./images/buttons/Back.bmp", SP_BUTTON_BACK);
	if (buttons[0] == NULL || buttons[1] == NULL || buttons[2] == NULL || buttons[3] == NULL ) {
		spChessButtonDestroy(buttons[0]);
		spChessButtonDestroy(buttons[1]);
		spChessButtonDestroy(buttons[2]);
		spChessButtonDestroy(buttons[3]);
		free(buttons);
		return NULL ;
	}
	return buttons;
}



SPChessButton** spMenuCreateDifficultyWidgets(SDL_Renderer* rend){
	if (rend == NULL ) {
		return NULL ;
	}
	SPChessButton** buttons = malloc(sizeof(SPChessButton*)*6);
	if (buttons == NULL ) {
		return NULL ;
	}
	SDL_Rect noob = { .x = 20, .y = 50, .h = 72, .w = 180 };
	SDL_Rect easy = { .x = 20, .y = 140, .h = 72, .w = 180 };
	SDL_Rect moderate = { .x = 220, .y = 50, .h = 72, .w = 180 };
	SDL_Rect expert = { .x = 220, .y = 140, .h = 72, .w = 180 };
	SDL_Rect next = { .x = 550, .y = 30, .h = 72, .w = 180 };
	SDL_Rect back = { .x = 550, .y = 200, .h = 72, .w = 180 };
	buttons[0] = spChessButtonCreate(rend, &noob, true, "./images/buttons/Noob.bmp",
			"./images/buttons/NoobNA.bmp", SP_BUTTON_DIFFICULTY);
	buttons[1] = spChessButtonCreate(rend, &easy, true, "./images/buttons/Easy.bmp",
			"./images/buttons/EasyNA.bmp", SP_BUTTON_DIFFICULTY);
	buttons[2] = spChessButtonCreate(rend, &moderate, true, "./images/buttons/Moderate.bmp",
			"./images/buttons/ModerateNA.bmp", SP_BUTTON_DIFFICULTY);
	buttons[3] = spChessButtonCreate(rend, &expert, true, "./images/buttons/Expert.bmp",
			"./images/buttons/ExpertNA.bmp", SP_BUTTON_DIFFICULTY);
	buttons[4] = spChessButtonCreate(rend, &next, false, "./images/buttons/Next.bmp",
			"./images/buttons/NextNA.bmp", SP_BUTTON_NEXT);
	buttons[5] = spChessButtonCreate(rend, &back, true, "./images/buttons/Back.bmp",
			"./images/buttons/Back.bmp", SP_BUTTON_BACK);
	for (int j = 0; j<6; j++){
		if (buttons[j] == NULL){
			for (int i = 0; i<5; i++){
				spChessButtonDestroy(buttons[i]);
			}
		free(buttons);
		return NULL ;
		j=6;
		}
	}
	return buttons;
}



SPChessButton** spMenuCreateColorWidgets(SDL_Renderer* rend){
	if (rend == NULL ) {
		return NULL ;
	}
	SPChessButton** buttons = malloc(sizeof(SPChessButton*)*4);
	if (buttons == NULL ) {
		return NULL ;
	}
	SDL_Rect blackPlayer = { .x = 120, .y = 50, .h = 72, .w = 53 };
	SDL_Rect whitePlayers = { .x = 120, .y = 140, .h = 72, .w = 53};
	SDL_Rect start = { .x = 550, .y = 30, .h = 72, .w = 180 };
	SDL_Rect back = { .x = 550, .y = 200, .h = 72, .w = 180 };
	buttons[0] = spChessButtonCreate(rend, &blackPlayer, true, "./images/buttons/BlackPlayer.bmp",
			"./images/buttons/BlackPlayerNA.bmp", SP_BUTTON_COLOR);
	buttons[1] = spChessButtonCreate(rend, &whitePlayers, false, "./images/buttons/WhitePlayer.bmp",
			"./images/buttons/WhitePlayerNA.bmp", SP_BUTTON_COLOR);
	buttons[2] = spChessButtonCreate(rend, &start, true, "./images/buttons/Start.bmp",
			"./images/buttons/Start.bmp", SP_BUTTON_NEXT);
	buttons[3] = spChessButtonCreate(rend, &back, true, "./images/buttons/Back.bmp",
			"./images/buttons/Back.bmp", SP_BUTTON_BACK);
	if (buttons[0] == NULL || buttons[1] == NULL || buttons[2] == NULL || buttons[3] == NULL ) {
		spChessButtonDestroy(buttons[0]);
		spChessButtonDestroy(buttons[1]);
		spChessButtonDestroy(buttons[2]);
		spChessButtonDestroy(buttons[3]);
		free(buttons);
		return NULL ;
	}
	return buttons;
}



SPChessButton** spMenuCreateWidgetsByType(SDL_Renderer* rend, SP_WINDOW type){
	if (rend == NULL){
		return NULL;
	}
	switch(type){
	case SP_WINDOW_MAINMENU: return spMenuCreateMainMenuWidgets(rend);
	case SP_WINDOW_GAMEMODE: return spMenuCreateGameModeWidgets(rend);
	case SP_WINDOW_DIFFICULTY: return spMenuCreateDifficultyWidgets(rend);
	case SP_WINDOW_COLOR: return spMenuCreateColorWidgets(rend);
	default: return NULL;
	}
}



int spChessMenuWindowNumofWidgets(SP_WINDOW type){
	switch(type){
	case SP_WINDOW_MAINMENU: return 3;
	case SP_WINDOW_GAMEMODE: return 4;
	case SP_WINDOW_DIFFICULTY: return 6;
	case SP_WINDOW_COLOR: return 4;
	default: return 0;
	}
}



SDL_Surface** spChessMenuGetBackgroundSurfaces(){
	SDL_Surface** backgrounds = malloc(sizeof(SDL_Surface*)*4);
	if (backgrounds == NULL){
		return NULL;
	}
	backgrounds[0] = SDL_LoadBMP("./images/bg/MainMenuBG.bmp");
	backgrounds[1] = SDL_LoadBMP("./images/bg/GameModeBG.bmp");
	backgrounds[2] = SDL_LoadBMP("./images/bg/DifficultyBG.bmp");
	backgrounds[3] = SDL_LoadBMP("./images/bg/ColorBG.bmp");
	return backgrounds;
}



void spChessMenuDestroySurfaces(SDL_Surface** backgrounds){
	if (backgrounds == NULL){
		return;
	}
	for (int i = 0; i < 4 ; i++){
		SDL_FreeSurface(backgrounds[i]);
	}
}



SPWindow* spChessMenuWindowCreate(){
	SPWindow* res = (SPWindow*) malloc(sizeof(SPWindow));
	SPChessMenuWindow* data = (SPChessMenuWindow*) malloc(sizeof(SPChessMenuWindow));
	SDL_Window* window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	SDL_Surface** backgrounds = spChessMenuGetBackgroundSurfaces();
	SPChessButton** currButtons = spMenuCreateWidgetsByType(renderer, SP_WINDOW_MAINMENU);
	if (res == NULL || data == NULL || window == NULL || renderer == NULL || backgrounds == NULL
			|| currButtons == NULL) {
		free(res);
		free(data);
		free(currButtons);
		spChessMenuDestroySurfaces(backgrounds);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		printf("Error in creating window");
		return NULL;
	}
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgrounds[0]);
	data->backgrounds = backgrounds;
	data->currButtons = currButtons;
	data->window = window;
	data->windowRenderer = renderer;
	data->backgroundTexture = backgroundTexture;
	data->currNumOfWidgets = 3;
	data->type = SP_WINDOW_MAINMENU;
	res->data = (void*) data;
	res->handleEventWindow = spChessMenuWindowHandleEvent;
	res->destroyWindow = spChessMenuWindowDestroy;
	res->drawWindow = spChessMenuWindowDraw;
	res->getType = spChessMenuGetType;
	return res;
}



void spChessMenuDestroyButtons(SPChessMenuWindow* window){
	if (window == NULL){
		return;
	}
	for (int i = 0; i < window->currNumOfWidgets; i++) {
		spChessButtonDestroy(window->currButtons[i]);//
	}
	free(window->currButtons);
}



void spChessMenuWindowDestroy(SPWindow* src){
	if (src == NULL ) {
		return;
	}
	SPChessMenuWindow* data = (SPChessMenuWindow*) src->data;
	spChessMenuDestroyButtons(data);
	spChessMenuDestroySurfaces(data->backgrounds);
	if (data->backgrounds != NULL){
		free(data->backgrounds);
	}
	SDL_DestroyTexture(data->backgroundTexture);
	SDL_DestroyRenderer(data->windowRenderer);
	SDL_DestroyWindow(data->window);
	free(data);
	free(src);
}



/*gets the current Menu and changes it to type Menu
 */


void spChessMenuChangeMenu(SPWindow* src, SP_WINDOW type){
	if (src == NULL){
		return;
	}
	SPChessMenuWindow* castData = (SPChessMenuWindow*) src->data;
	spChessMenuDestroyButtons(castData);
	SDL_RenderClear(castData->windowRenderer);
	int i = 0;
	SPChessButton** buttons = NULL;
	switch(type){
	case SP_WINDOW_MAINMENU:
		i=0;
		buttons = spMenuCreateMainMenuWidgets(castData->windowRenderer);
		break;
	case SP_WINDOW_GAMEMODE:
		i=1;
		buttons = spMenuCreateGameModeWidgets(castData->windowRenderer);
		break;
	case SP_WINDOW_DIFFICULTY:
		i=2;
		buttons = spMenuCreateDifficultyWidgets(castData->windowRenderer);
		break;
	case SP_WINDOW_COLOR:
		i=3;
		buttons = spMenuCreateColorWidgets(castData->windowRenderer);
		break;
	default: break;
	}
	SDL_Texture* nextBG = SDL_CreateTextureFromSurface(castData->windowRenderer, castData->backgrounds[i]);
	castData->type = type;
	castData->currButtons = buttons;
	castData->backgroundTexture = nextBG;
	castData->currNumOfWidgets = spChessMenuWindowNumofWidgets(type);
	return;
}



void spChessMenuHandleChoose(SPChessMenuWindow* src, int i){
	if (src == NULL){
		return;
	}
	switch(src->type){
	case SP_WINDOW_DIFFICULTY:
		for (int j = 0; j<5; j++){
			if (src->currButtons[j]->active == false){
				spChessButtonChangeActiveness(src->currButtons[j]);
			}
		}
		spChessButtonChangeActiveness(src->currButtons[i]);
		break;
	case SP_WINDOW_GAMEMODE:
		for (int j=0; j<2; j++){
			spChessButtonChangeActiveness(src->currButtons[j]);
		}
		if (i==1){
			src->currButtons[2]->type = SP_BUTTON_START;
			src->currButtons[2]->active = false;
		}
		else if (i==0){
			src->currButtons[2]->type = SP_BUTTON_NEXT;
			src->currButtons[2]->active = true;
		}
		break;
	case SP_WINDOW_COLOR:
		for (int j=0; j<2; j++){
			spChessButtonChangeActiveness(src->currButtons[j]);
		}
		break;
	default: break;
	}
}




void spChessMenuWindowDraw(SPWindow* src){
	if (src == NULL ) {
		return;
	}
	SPChessMenuWindow* data = (SPChessMenuWindow*) src->data;
	SDL_RenderClear(data->windowRenderer);
	SDL_RenderCopy(data->windowRenderer, data->backgroundTexture, NULL, NULL);
	int i = 0;
	for (;i < data->currNumOfWidgets; i++) {
		spChessButtonDraw(data->currButtons[i]);
	}
	SDL_RenderPresent(data->windowRenderer);
	}



SP_WINDOW_EVENT spChessButtonTranslateEvent(SP_BUTTON_EVENT buttonEvent){
	switch(buttonEvent){
	case SP_BEVENT_BACK: return SP_MEVENT_BACK;
	case SP_BEVENT_EXIT: return SP_MEVENT_EXIT;
	case SP_BEVENT_GOTOLOAD: return SP_MEVENT_GOTOLOAD;
	case SP_BEVENT_NEXT: return SP_MEVENT_NEXT;
	case SP_BEVENT_START: return SP_MEVENT_START;
	default: return SP_MEVENT_NONE;
}
}



SP_WINDOW_EVENT spChessMenuWindowHandleEvent(SPWindow* src, SDL_Event* event){
	if(src == NULL || event==NULL){
		return SP_MEVENT_NONE;
	}
	SPChessMenuWindow* data = (SPChessMenuWindow*)src->data;
	for(int i=0; i<data->currNumOfWidgets ;i++){
		SP_BUTTON_EVENT buttonEvent = spChessButtonHandleButtonEvent(data->currButtons[i], event);
		if (buttonEvent == SP_BEVENT_CHOOSE){
			spChessMenuHandleChoose(data, i);
			switch(i){
			case 0: return SP_MEVENT_CHANGESETTINGS_1;
			case 1: return SP_MEVENT_CHANGESETTINGS_2;
			case 2: return SP_MEVENT_CHANGESETTINGS_3;
			case 3: return SP_MEVENT_CHANGESETTINGS_4;
			case 4: return SP_MEVENT_CHANGESETTINGS_5;
			}
		}
		SP_WINDOW_EVENT menuEvent = spChessButtonTranslateEvent(buttonEvent);
		if (menuEvent != SP_MEVENT_NONE){
			return menuEvent;
		}
	}
	return SP_MEVENT_NONE;
}


SDL_Renderer* spChessMenuGetRenderer (SPWindow* window){
	SPChessMenuWindow* win = (SPChessMenuWindow*) window->data;
	return win->windowRenderer;
}


SP_WINDOW spChessMenuGetType(SPWindow* src){
	SPChessMenuWindow* data = (SPChessMenuWindow*) src->data;
	return data->type;
}


