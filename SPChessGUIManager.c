/*
 * SPChessGUIManager.c
 *
 *  Created on: 19 בספט 2017
 *      Author: ערי
 */


#include "SPChessGUIManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_video.h>



SPChessGuiManager* spChessGuiManagerCreate() {
	SPChessGuiManager* res = (SPChessGuiManager*) malloc(sizeof(SPChessGuiManager));
	if (res == NULL ) {
		return NULL ;
	}
	res->menuWin = spChessMenuWindowCreate();
	res->loadWin = NULL;
	res->gameWin = NULL;
	res->settings = spSettingCreate();
	if (res->menuWin == NULL || res->settings == NULL) {
		destroyWindow(res->menuWin);
		if (res->settings != NULL){
			free(res->settings);
		}
		return NULL ;
	}
	res->activeWin = SP_WINDOW_MAINMENU;
	res->lastActiveWin = SP_WINDOW_MAINMENU;
	return res;
}



void spChessGuiManagerDestroy(SPChessGuiManager* src) {
	if (!src) {
		return;
	}
	if (src->activeWin == SP_WINDOW_GAME){
		src->gameWin->destroyWindow(src->gameWin);
	}
	else{
		src->menuWin->destroyWindow(src->menuWin);
	}
	if (src->settings != NULL){
		free(src->settings);
	}
	free(src);
	return;
}



void spChessGuiManagerDraw(SPChessGuiManager* src) {
	if (!src) {
		return;
	}
	switch (src->activeWin){
	case SP_WINDOW_MAINMENU: src->menuWin->drawWindow(src->menuWin);return;
	case SP_WINDOW_LOAD: src->loadWin->drawWindow(src->loadWin); return;
	case SP_WINDOW_GAMEMODE: src->menuWin->drawWindow(src->menuWin); return;
	case SP_WINDOW_COLOR: src->menuWin->drawWindow(src->menuWin); return;
	case SP_WINDOW_DIFFICULTY: src->menuWin->drawWindow(src->menuWin); return;
	case SP_WINDOW_GAME: src->gameWin->drawWindow(src->gameWin); return;
}
}



SPWindow* spChessGuiManagerGetWindow (SPChessGuiManager* src, SP_WINDOW type){
	if (src == NULL){
		return NULL;
	}
	switch (type){
	case SP_WINDOW_MAINMENU: return src->menuWin;
	case SP_WINDOW_LOAD: return src->loadWin;
	case SP_WINDOW_GAMEMODE: return src->menuWin;
	case SP_WINDOW_COLOR: return src->menuWin;
	case SP_WINDOW_DIFFICULTY: return src->menuWin;
	case SP_WINDOW_GAME: return src->gameWin;
	default: return NULL;
	}
	return NULL;
}



void spChessGuiManagerStartGame (SPChessGuiManager* src){
	if (src == NULL){
		return;
	}
	SPChessGame*  game = spChessGameCreate(6);
	game->level = src->settings->difficulty;
	src->activeWin = SP_WINDOW_GAME;
	src->gameWin = spChessGameWindowCreate(game, src->settings);
	src->menuWin->destroyWindow(src->menuWin);
}




SP_MANAGER_EVENT spChessGuiManagerHandleNext(SPChessGuiManager* src){
	if (src == NULL){
		return SP_MANAGER_NONE;
	}
	switch(src->activeWin){
	case SP_WINDOW_MAINMENU:
		spChessMenuChangeMenu(src->menuWin, SP_WINDOW_GAMEMODE);
		src->activeWin = SP_WINDOW_GAMEMODE;
		return SP_MANAGER_NONE;
	case SP_WINDOW_LOAD: return SP_MANAGER_NONE;
	case SP_WINDOW_GAMEMODE:
		spChessMenuChangeMenu(src->menuWin, SP_WINDOW_DIFFICULTY);
		src->activeWin = SP_WINDOW_DIFFICULTY;
		return SP_MANAGER_NONE;
	case SP_WINDOW_DIFFICULTY:
		spChessMenuChangeMenu(src->menuWin, SP_WINDOW_COLOR);
		src->activeWin = SP_WINDOW_COLOR;
		return SP_MANAGER_NONE;
	case SP_WINDOW_COLOR:
		spChessGuiManagerStartGame(src);
		src->activeWin = SP_WINDOW_GAME;
		return SP_MANAGER_NONE;
	case SP_WINDOW_GAME: return SP_MANAGER_NONE;
	default: return SP_MANAGER_NONE;
	}
}




SP_MANAGER_EVENT spChessGuiManagerHandleBack(SPChessGuiManager* src){
	if (src == NULL){
		return SP_MANAGER_NONE;
	}
	switch(src->activeWin){
	case SP_WINDOW_MAINMENU:
		return SP_MANAGER_NONE;
	case SP_WINDOW_GAMEMODE:
		spChessMenuChangeMenu(src->menuWin, SP_WINDOW_MAINMENU);
		src->activeWin = SP_WINDOW_MAINMENU;
		return SP_MANAGER_NONE;
	case SP_WINDOW_DIFFICULTY:
		spChessMenuChangeMenu(src->menuWin, SP_WINDOW_GAMEMODE);
		src->activeWin = SP_WINDOW_GAMEMODE;
		return SP_MANAGER_NONE;
	case SP_WINDOW_COLOR:
		spChessMenuChangeMenu(src->menuWin, SP_WINDOW_DIFFICULTY);
		src->activeWin = SP_WINDOW_DIFFICULTY;
		return SP_MANAGER_NONE;
	case SP_WINDOW_GAME: return SP_MANAGER_NONE;
	default: return SP_MANAGER_NONE;
}
}



SPChessGame* spChessGuiManagerGetGameSlot(SPChessGuiManager* src, char* path){
	SPChessGame* loadedGame = spChessGameCreate(6);
	SPSetting* set = spSettingCreate();
	SP_COMMAND com = spFileHandleLoad(path, loadedGame, set);
	if (com == SP_INVALID_LINE){
		spChessGameDestroy(loadedGame);
		return NULL;
	}
	else{
		free(src->settings);
		src->settings = set;
		return loadedGame;
	}
}



void spChessGuiManagerHandleLoad(SPChessGuiManager* src, int i){
	if ( src == NULL || src->activeWin != SP_WINDOW_LOAD){
		printf("called Load with non-possible param");
		return;
	}
	SPChessGame* loaded;
	switch(i){
	case 1: loaded = spChessGuiManagerGetGameSlot(src, "./Slots/slot1.xml"); break;
	case 2: loaded = spChessGuiManagerGetGameSlot(src, "./Slots/slot2.xml"); break;
	case 3: loaded = spChessGuiManagerGetGameSlot(src, "./Slots/slot3.xml"); break;
	case 4: loaded = spChessGuiManagerGetGameSlot(src, "./Slots/slot4.xml"); break;
	case 5: loaded = spChessGuiManagerGetGameSlot(src, "./Slots/slot5.xml"); break;
	default: loaded = NULL; break;
	}
	if (loaded == NULL){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Nothing Loaded",
				"The file on the desired slot does not exist or corrupted", NULL );
	}
	else{
		if (src->lastActiveWin == SP_WINDOW_GAME){
			src->gameWin->destroyWindow(src->gameWin);
		}
		else{
			src->menuWin->destroyWindow(src->menuWin);
		}
		src->gameWin = spChessGameWindowCreate(loaded, src->settings);
		src->activeWin = SP_WINDOW_GAME;
		src->loadWin->destroyWindow(src->loadWin);
	}
}



void spChessGuiManagerSet (SPChessGuiManager* src, int i){
	switch (src->activeWin){
	case SP_WINDOW_GAMEMODE: src->settings->gameMode = i; break;
	case SP_WINDOW_DIFFICULTY: src->settings->difficulty = i; break;
	case SP_WINDOW_COLOR: src->settings->userColor = i-1; break;
	case SP_WINDOW_LOAD: spChessGuiManagerHandleLoad(src, i); break;
	default: break;
	}
}



SP_MANAGER_EVENT spChessGuiManagerHandleMenuEvent(SPChessGuiManager* src,
		SP_WINDOW_EVENT event) {
	if (src == NULL ) {
		return SP_MANAGER_NONE;
	}
	switch(event){
	case SP_MEVENT_EXIT: return SP_MANAGER_QUIT;
	case SP_MEVENT_BACK: return spChessGuiManagerHandleBack(src);
	case SP_MEVENT_NEXT: return spChessGuiManagerHandleNext(src);
	case SP_MEVENT_START: 	spChessGuiManagerStartGame(src);
							src->activeWin = SP_WINDOW_GAME;
							return SP_MANAGER_NONE;
	case SP_MEVENT_CHANGESETTINGS_1: spChessGuiManagerSet(src, 1); return SP_MANAGER_NONE;
	case SP_MEVENT_CHANGESETTINGS_2: spChessGuiManagerSet(src, 2); return SP_MANAGER_NONE;
	case SP_MEVENT_CHANGESETTINGS_3: spChessGuiManagerSet(src, 3); return SP_MANAGER_NONE;
	case SP_MEVENT_CHANGESETTINGS_4: spChessGuiManagerSet(src, 4); return SP_MANAGER_NONE;
	case SP_MEVENT_CHANGESETTINGS_5: spChessGuiManagerSet(src, 5); return SP_MANAGER_NONE;
	default: return SP_MANAGER_NONE;
}
}




void spChessGuiManagerHandleSave(SPChessGuiManager* src){
	SPChessGameWindow* castData = (SPChessGameWindow*) src->gameWin->data;
	SP_CHESS_GAME_MESSAGE com = spFileHandleSaveInSDL(castData->currGame, src->settings);
	if (com == SP_CHESS_GAME_FAILED){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Nothing Saved",
				"The game can not be saved", NULL );
	}
	else{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Saved",
				"Saved Successfully in slot 1", NULL );
	}
}



void spChessGuiManagerGoToMainMenu(SPChessGuiManager* src){
	if (src== NULL){
		return;
	}
	src->menuWin = spChessMenuWindowCreate();
	src->activeWin = SP_WINDOW_MAINMENU;
	src->gameWin->destroyWindow(src->gameWin);
	free(src->settings);
	src->settings = spSettingCreate();
}


int spChessGameMakeSaveBox(){
    const SDL_MessageBoxButtonData buttons[] = {
    	{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel" },
        {  0, 0, "no" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" },
    };
    const SDL_MessageBoxColorScheme colorScheme = {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { 255,   0,   0 },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            {   0, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            { 255, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            {   0,   0, 255 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "Confirm Exit", /* .title */
        "Do you want to save the game before leaving?", /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
        return SP_MEVENT_NONE;
    }
    return buttonid;
}



void spChessGuiManagerGoToLoadWindow(SPChessGuiManager* src){
	if (src == NULL){
		return;
	}
	src->lastActiveWin = src->activeWin;
	src->activeWin = SP_WINDOW_LOAD;
	src->loadWin = spChessLoadWindowCreate();
}



SP_MANAGER_EVENT spChessGuiManagerBackFromLoadMenu(SPChessGuiManager* src){
	if (src==NULL){
		return SP_MANAGER_NONE;
	}
	src->loadWin->destroyWindow(src->loadWin);
	src->loadWin = NULL;
	src->activeWin = src->lastActiveWin;
	return SP_MANAGER_NONE;
}


SP_MANAGER_EVENT spChessGuiManagerHandleEvent(SPChessGuiManager* src, SDL_Event* event) {
	if (src == NULL || event == NULL ) {
		return SP_MANAGER_NONE;
	}
	SPWindow* currWin = spChessGuiManagerGetWindow(src, src->activeWin);
	SP_WINDOW_EVENT mainEvent = currWin->handleEventWindow(currWin, event);
	if (src->activeWin == SP_WINDOW_LOAD){
		if (mainEvent == SP_MEVENT_BACK){
			return spChessGuiManagerBackFromLoadMenu(src);
		}
		else if (mainEvent == SP_MEVENT_NONE){
		return SP_MANAGER_NONE;
		}
		else{
			return spChessGuiManagerHandleMenuEvent(src, mainEvent);
		}
	}
	else if (src->activeWin != SP_WINDOW_GAME) {
		if (mainEvent == SP_MEVENT_GOTOLOAD){
			spChessGuiManagerGoToLoadWindow(src);
			return SP_MANAGER_NONE;
		}
		else if (mainEvent != SP_MEVENT_EXIT){
			return spChessGuiManagerHandleMenuEvent(src, mainEvent);
		}
		else{
			return SP_MANAGER_QUIT;
		}
	} else {
		if (mainEvent == SP_MEVENT_GOTOLOAD){
			spChessGuiManagerGoToLoadWindow(src);
		}
		if (mainEvent == SP_MEVENT_EXIT){
			switch (spChessGameMakeSaveBox()){
			case 1: spChessGuiManagerHandleSave(src); return SP_MANAGER_QUIT;
			case 0: return SP_MANAGER_QUIT;
			default: return SP_MANAGER_NONE;
		}}
		if (mainEvent == SP_MEVENT_BACK){
			switch(spChessGameMakeSaveBox()){
			case 1:	spChessGuiManagerHandleSave(src); spChessGuiManagerGoToMainMenu(src); break;
			case 0:spChessGuiManagerGoToMainMenu(src); break;
			default: break;
			return SP_MANAGER_NONE;
		}}
		if (mainEvent == SP_MEVENT_MAINMENU){
			spChessGuiManagerGoToMainMenu(src);
			return SP_MANAGER_NONE;
		}
		if (mainEvent == SP_MEVENT_SAVE){
			spChessGuiManagerHandleSave(src);
		}
		if (mainEvent == SP_MEVENT_QUIT){
			return SP_MANAGER_QUIT;
		}
	return SP_MANAGER_NONE;
}}



void spChessGuiMode() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return;
	}
	SPChessGuiManager* manager = spChessGuiManagerCreate();
	if (manager == NULL ){
		printf("Error in creating GUI manager");
		SDL_Quit();
		return;
	}
	SDL_Event event;
	while (1) {
		SDL_WaitEvent(&event);
		SP_MANAGER_EVENT mEvent = spChessGuiManagerHandleEvent(manager, &event);
		if (mEvent == SP_MANAGER_QUIT) {
			break;
		}
		spChessGuiManagerDraw(manager);
	}
	spChessGuiManagerDestroy(manager);
	SDL_Quit();
	return;
}


