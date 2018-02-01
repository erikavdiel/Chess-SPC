/*
 * SPChessButton.c
 *
 *  Created on: 18 בספט 2017
 *      Author: ערי
 */
#include <stdbool.h>
#include <stdio.h>
#include "SPChessButton.h"


/*
 * Allocates memory to a new SDL_Rect and copies the source SDL_Rect to the
 * allocated memory.
 *
 * @param:
 * src - the source SDL_Rect
 *
 * @return:
 * NULL - if src==NULL or if the allocation failed.
 * SDL_Rect* - a copy of src
 *
 */
SDL_Rect* spCopyRect(SDL_Rect* src){
	if(src == NULL){
		return NULL;
	}
	SDL_Rect* res = malloc(sizeof(SDL_Rect));
	if(res==NULL){
		return NULL;
	}
	res->h = src->h;
	res->w = src->w;
	res->x = src->x;
	res->y = src->y;
	return res;
}



SPChessButton* spChessButtonCreate(SDL_Renderer* windowRender, SDL_Rect* location, bool active,
		const char* activeImage, const char* nonActiveImage, SP_BUTTON type) {
	if (windowRender == NULL || location == NULL || activeImage == NULL || nonActiveImage == NULL ) {
		return NULL ;
	}
	SPChessButton* data = (SPChessButton*) malloc(sizeof(SPChessButton));
	SDL_Surface* loadingActiveSurface = SDL_LoadBMP(activeImage);
	SDL_Surface* loadingNonActiveSurface = SDL_LoadBMP(nonActiveImage);
	SDL_Texture* buttonActiveTexture = SDL_CreateTextureFromSurface(windowRender,
			loadingActiveSurface);
	SDL_Texture* buttonNonActiveTexture = SDL_CreateTextureFromSurface(windowRender, loadingNonActiveSurface);
	if (data == NULL || loadingActiveSurface == NULL || buttonNonActiveTexture == NULL
			|| buttonActiveTexture == NULL || buttonNonActiveTexture == NULL) {
		free(data);
		SDL_FreeSurface(loadingActiveSurface);
		SDL_FreeSurface(loadingNonActiveSurface);
		SDL_DestroyTexture(buttonActiveTexture);
		SDL_DestroyTexture(buttonNonActiveTexture);
		printf("Error in creating button");
		return NULL ;
	}
	SDL_FreeSurface(loadingActiveSurface);
	SDL_FreeSurface(loadingNonActiveSurface);
	data->buttonActiveTexture = buttonActiveTexture;
	data->buttonNonActiveTexture = buttonNonActiveTexture;
	data->location = spCopyRect(location);
	data->windowRenderer = windowRender;
	data->active = active;
	data->type = type;
	return data;
}



void spChessButtonDestroy(SPChessButton* src) {
	if (src == NULL ) {
		return;
	}
	free(src->location);
	SDL_DestroyTexture(src->buttonActiveTexture);
	SDL_DestroyTexture(src->buttonNonActiveTexture);
	free(src);
}



/*
 * Checks if an event in the window is related to a specific button (what button is pressed).
 *
 * @param:
 * src - the button to check.
 * event - the event occured in the window.
 *
 * @return:
 * If src is related to the event then the fitting SP_BUTTON_EVENT returned.
 * If event and src are not related so SP_BEVENT_NONE returned.
 *
 */
SP_BUTTON_EVENT spChessButtonHandleButtonEvent(SPChessButton* src, SDL_Event* event) {
	if (src == NULL || event == NULL) {
		return SP_BEVENT_NONE;
	}
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, src->location) && src->active) {
			switch(src->type){
			case SP_BUTTON_NEWGAME: return SP_BEVENT_NEXT;
			case SP_BUTTON_GOTOLOAD: return SP_BEVENT_GOTOLOAD;
			case SP_BUTTON_EXIT: return SP_BEVENT_EXIT;
			case SP_BUTTON_DIFFICULTY: return SP_BEVENT_CHOOSE;
			case SP_BUTTON_NEXT: return SP_BEVENT_NEXT;
			case SP_BUTTON_PLAYERMODE: return SP_BEVENT_CHOOSE;
			case SP_BUTTON_BACK: return SP_BEVENT_BACK;
			case SP_BUTTON_COLOR: return SP_BEVENT_CHOOSE;
			case SP_BUTTON_GAMESLOT: return SP_BEVENT_CHOOSE;
			case SP_BUTTON_RESTART: return SP_BEVENT_RESTART;
			case SP_BUTTON_UNDO: return SP_BEVENT_UNDO;
			case SP_BUTTON_SAVE: return SP_BEVENT_SAVE;
			default: return SP_BEVENT_NONE;
		}
	}else if (SDL_PointInRect(&point, src->location) && !(src->active) && src->type== SP_BUTTON_START){
		return SP_BEVENT_START;
	}
		else {
		return SP_BEVENT_NONE;
	}
}
	return SP_BEVENT_NONE;
}



void spChessButtonChangeActiveness(SPChessButton* src){
	if (src == NULL){
		return;
	}
	src->active = !(src->active);
}




void spChessButtonDraw(SPChessButton* src) {
	if (src == NULL ) {
		return;
	}
	if (src->active){
		SDL_RenderCopy(src->windowRenderer, src->buttonActiveTexture, NULL,
				src->location);
	}
	else{
		SDL_RenderCopy(src->windowRenderer, src->buttonNonActiveTexture, NULL,
				src->location);
	}
}


