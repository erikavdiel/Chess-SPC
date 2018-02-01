/*
 * SPChessLoadWindow.c
 *
 *  Created on: 22 בספט 2017
 *      Author: ערי
 */

#include "SPChessLoadWindow.h"



SPChessButton** spMenuCreateLoadWidgets(SDL_Renderer* rend){
	if (rend == NULL ) {
		return NULL ;
	}
	SPChessButton** buttons = malloc(sizeof(SPChessButton*)*7);
	if (buttons == NULL ) {
		return NULL ;
	}
	SDL_Rect slot1 = { .x = 100, .y = 20, .h = 72, .w = 180 };
	SDL_Rect slot2 = { .x = 100, .y = 110, .h = 72, .w = 180 };
	SDL_Rect slot3 = { .x = 100, .y = 200, .h = 72, .w = 180 };
	SDL_Rect slot4 = { .x = 100, .y = 290, .h = 72, .w = 180 };
	SDL_Rect slot5 = { .x = 100, .y = 380, .h = 72, .w = 180 };
	SDL_Rect load = { .x = 520, .y = 150, .h = 72, .w = 180 };
	SDL_Rect back = { .x = 520, .y = 320, .h = 72, .w = 180 };
	buttons[0] = spChessButtonCreate(rend, &slot1, true, "./images/buttons/GameSlot1.bmp",
			"./images/buttons/GameSlot1NA.bmp", SP_BUTTON_GAMESLOT);
	buttons[1] = spChessButtonCreate(rend, &slot2, true, "./images/buttons/GameSlot2.bmp",
			"./images/buttons/GameSlot2NA.bmp", SP_BUTTON_GAMESLOT);
	buttons[2] = spChessButtonCreate(rend, &slot3, true, "./images/buttons/GameSlot3.bmp",
			"./images/buttons/GameSlot3NA.bmp", SP_BUTTON_GAMESLOT);
	buttons[3] = spChessButtonCreate(rend, &slot4, true, "./images/buttons/GameSlot4.bmp",
			"./images/buttons/GameSlot4NA.bmp", SP_BUTTON_GAMESLOT);
	buttons[4] = spChessButtonCreate(rend, &slot5, true, "./images/buttons/GameSlot5.bmp",
			"./images/buttons/GameSlot5NA.bmp", SP_BUTTON_GAMESLOT);
	buttons[5] = spChessButtonCreate(rend, &load, false, "./images/buttons/LoadActive.bmp",
			"./images/buttons/LoadNonActive.bmp", SP_BUTTON_NEXT);
	buttons[6] = spChessButtonCreate(rend, &back, true, "./images/buttons/Back.bmp",
			"./images/buttons/BackNA.bmp", SP_BUTTON_BACK);
	for (int j = 0; j<7; j++){
		if (buttons[j] == NULL){
			for (int i = 0; i<5; i++){
				spChessButtonDestroy(buttons[i]);
			}
		free(buttons);
		return NULL ;
		j=7;
		}
	}
	return buttons;
}



SPWindow* spChessLoadWindowCreate(){
	SPWindow* res = (SPWindow*) malloc(sizeof(SPWindow));
	SPChessLoadWindow* data = (SPChessLoadWindow*) malloc(sizeof(SPChessLoadWindow));
	SDL_Window* window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	SDL_Surface* background = SDL_LoadBMP("./images/bg/LoadBG.bmp");
	SPChessButton** buttons = spMenuCreateLoadWidgets(renderer);
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
	if (res == NULL || data == NULL || window == NULL || renderer == NULL || background == NULL
			|| buttons == NULL || backgroundTexture == NULL) {
		free(res);
		free(data);
		free(buttons);
		SDL_FreeSurface(background);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		printf("Error in creating window");
		return NULL;
	}
	SDL_FreeSurface(background);
	data->buttons = buttons;
	data->window = window;
	data->windowRenderer = renderer;
	data->backgroundTexture = backgroundTexture;
	data->numOfWidgets = 7;
	data->type = SP_WINDOW_LOAD;
	res->data = (void*) data;
	res->handleEventWindow = spChessLoadWindowHandleEvent;
	res->destroyWindow = spChessLoadWindowDestroy;
	res->drawWindow = spChessLoadWindowDraw;
	res->getType = spChessLoadGetType;
	return res;
}


void spChessLoadHandleChoose(SPChessLoadWindow* src, int i){
	if (src == NULL){
		return;
	}
	for (int j = 0; j<6; j++){
		if (src->buttons[j]->active == false){
			spChessButtonChangeActiveness(src->buttons[j]);
		}
	}
	spChessButtonChangeActiveness(src->buttons[i]);
}




SP_WINDOW_EVENT spChessLoadWindowHandleEvent(SPWindow* src, SDL_Event* event){
	if(src == NULL || event==NULL){
		return SP_MEVENT_NONE;
	}
	SPChessLoadWindow* data = (SPChessLoadWindow*)src->data;
	for(int i=0; i<data->numOfWidgets ;i++){
		SP_BUTTON_EVENT buttonEvent = spChessButtonHandleButtonEvent(data->buttons[i], event);
		if (buttonEvent == SP_BEVENT_CHOOSE){
			spChessLoadHandleChoose(data, i);
		}
		if (buttonEvent == SP_BEVENT_NEXT){
			for (int j=0; j<5; j++){
				if (!(data->buttons[j]->active)){
					switch(j){
					case 0: return SP_MEVENT_CHANGESETTINGS_1;
					case 1: return SP_MEVENT_CHANGESETTINGS_2;
					case 2: return SP_MEVENT_CHANGESETTINGS_3;
					case 3: return SP_MEVENT_CHANGESETTINGS_4;
					case 4: return SP_MEVENT_CHANGESETTINGS_5;
					}
				}
			}
		}
		if (buttonEvent == SP_BEVENT_BACK){
			return SP_MEVENT_BACK;
		}
		if (buttonEvent == SP_BEVENT_SAVE){
			return SP_MEVENT_SAVE;
		}
	}
	return SP_MEVENT_NONE;
}




void spChessLoadWindowDestroy(SPWindow* src){
	if (src == NULL ) {
		return;
	}
	SPChessLoadWindow* data = (SPChessLoadWindow*) src->data;
	for (int i=0; i<data->numOfWidgets; i++){
		spChessButtonDestroy(data->buttons[i]);
	}
	free(data->buttons);
	SDL_DestroyTexture(data->backgroundTexture);
	SDL_DestroyRenderer(data->windowRenderer);
	SDL_DestroyWindow(data->window);
	free(data);
	free(src);
}



void spChessLoadWindowDraw(SPWindow* src){
	if (src == NULL ) {
		return;
	}
	SPChessLoadWindow* data = (SPChessLoadWindow*) src->data;
	SDL_RenderClear(data->windowRenderer);
	SDL_RenderCopy(data->windowRenderer, data->backgroundTexture, NULL, NULL);
	int i = 0;
	for (;i < data->numOfWidgets; i++) {
		spChessButtonDraw(data->buttons[i]);
	}
	SDL_RenderPresent(data->windowRenderer);
	}




SDL_Renderer* spChessLoadGetRenderer (SPWindow* window){
	SPChessLoadWindow* win = (SPChessLoadWindow*) window->data;
	return win->windowRenderer;
}


SP_WINDOW spChessLoadGetType(){
	return SP_WINDOW_LOAD;
}

