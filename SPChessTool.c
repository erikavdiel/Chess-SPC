/*
 * SPChessTool.c
 *
 *  Created on: 23 בספט 2017
 *      Author: ערי
 */
#include <stdbool.h>
#include <stdio.h>
#include "SPChessTool.h"



const char* spChessToolGetImageForTool(SP_TOOL type){
	static char* toolsImages[] = {"./images/tools/BlackPawn.bmp",
			"./images/tools/WhitePawn.bmp",
			"./images/tools/BlackBishop.bmp",
			"./images/tools/WhiteBishop.bmp",
			"./images/tools/BlackKnight.bmp",
			"./images/tools/WhiteKnight.bmp",
			"./images/tools/BlackRook.bmp",
			"./images/tools/WhiteRook.bmp",
			"./images/tools/BlackQueen.bmp",
			"./images/tools/WhiteQueen.bmp",
			"./images/tools/BlackKing.bmp",
			"./images/tools/WhiteKing.bmp"};
	switch(type){
	case SP_TOOL_PAWN_BLACK: return  toolsImages[0];
	case SP_TOOL_PAWN_WHITE: return toolsImages[1];
	case SP_TOOL_BISHOP_BLACK: return toolsImages[2];
	case SP_TOOL_BISHOP_WHITE: return toolsImages[3];
	case SP_TOOL_KNIGHT_BLACK: return toolsImages[4];
	case SP_TOOL_KNIGHT_WHITE: return toolsImages[5];
	case SP_TOOL_ROOK_BLACK: return toolsImages[6];
	case SP_TOOL_ROOK_WHITE: return toolsImages[7];
	case SP_TOOL_QUEEN_BLACK: return toolsImages[8];
	case SP_TOOL_QUEEN_WHITE: return toolsImages[9];
	case SP_TOOL_KING_BLACK: return toolsImages[10];
	case SP_TOOL_KING_WHITE: return toolsImages[11];
	default: return NULL;
	}
}



SPChessTool* spChessToolCreate(SDL_Renderer* windowRender, SDL_Rect* location, SP_TOOL type) {
	if (windowRender == NULL || location == NULL) {
		return NULL ;
	}
	const char* image = spChessToolGetImageForTool(type);
	SPChessTool* data = (SPChessTool*) malloc(sizeof(SPChessTool));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(windowRender,
			loadingSurface);
	int flag = 0;
	switch(type){
	case SP_TOOL_PAWN_WHITE: flag = 1; break;
	case SP_TOOL_ROOK_WHITE: flag = 1; break;
	case SP_TOOL_KNIGHT_WHITE: flag = 1; break;
	case SP_TOOL_BISHOP_WHITE: flag = 1; break;
	case SP_TOOL_QUEEN_WHITE: flag = 1; break;
	case SP_TOOL_KING_WHITE: flag = 1; break;
	default: flag =0;
	}
	if (flag){
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
	}
	else{
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_MOD);
	}
	if (data == NULL || loadingSurface == NULL || texture == NULL){
		free(data);
		SDL_FreeSurface(loadingSurface);
		SDL_DestroyTexture(texture);
		printf("Error in creating TOOL");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	data->duringdrag = false;
	data->texture = texture;
	data->location = spCopyRect(location);
	data->prevLocation = spCopyRect(location);
	data->windowRenderer = windowRender;
	data->type = type;
	return data;
}



int spChessToolGetToolColor(SPChessTool* src){
	if (src == NULL){
		return -1;
	}
	switch (src->type){
	case SP_TOOL_PAWN_WHITE: return 1;
	case SP_TOOL_ROOK_WHITE: return 1;
	case SP_TOOL_KNIGHT_WHITE: return 1;
	case SP_TOOL_BISHOP_WHITE: return 1;
	case SP_TOOL_QUEEN_WHITE: return 1;
	case SP_TOOL_KING_WHITE: return 1;
	default: return 0;
	}
}



char spChessToolConvertSPTOOLtoChar(SP_TOOL type){
	switch (type){
	case SP_TOOL_PAWN_WHITE: return 'm';
	case SP_TOOL_ROOK_WHITE: return 'r';
	case SP_TOOL_KNIGHT_WHITE: return 'n';
	case SP_TOOL_BISHOP_WHITE: return 'b';
	case SP_TOOL_QUEEN_WHITE: return 'q';
	case SP_TOOL_KING_WHITE: return 'k';
	case SP_TOOL_PAWN_BLACK: return 'M';
	case SP_TOOL_ROOK_BLACK: return 'R';
	case SP_TOOL_KNIGHT_BLACK: return 'N';
	case SP_TOOL_BISHOP_BLACK: return 'B';
	case SP_TOOL_QUEEN_BLACK: return 'Q';
	case SP_TOOL_KING_BLACK: return 'K';
	}
	return 'P';
}


SP_TOOL spChessToolConvertCharToSPTOOL(char c){
	switch(c){
	case 'm': return SP_TOOL_PAWN_WHITE;
	case 'M': return SP_TOOL_PAWN_BLACK;
	case 'r': return SP_TOOL_ROOK_WHITE;
	case 'R': return SP_TOOL_ROOK_BLACK;
	case 'n': return SP_TOOL_KNIGHT_WHITE;
	case 'N': return SP_TOOL_KNIGHT_BLACK;
	case 'b': return SP_TOOL_BISHOP_WHITE;
	case 'B': return SP_TOOL_BISHOP_BLACK;
	case 'q': return SP_TOOL_QUEEN_WHITE;
	case 'Q': return SP_TOOL_QUEEN_BLACK;
	case 'k': return SP_TOOL_KING_WHITE;
	case 'K': return SP_TOOL_KING_BLACK;
	default: return SP_TOOL_KING_BLACK;
}}


//You need this function in order to destroy all data Associate with a button:
void spChessToolDestroy(SPChessTool* src) {
	if (src == NULL ) {
		return;
	}
	if (src->location != NULL){
		free(src->location);
	}
	if (src->prevLocation != NULL){
		free(src->prevLocation);
	}
	if (src->texture != NULL){
		SDL_DestroyTexture(src->texture);
	}
	free(src);
}



void spChessToolDraw(SPChessTool* src) {
	if (src == NULL ) {
		return;
	}
	SDL_RenderCopy(src->windowRenderer, src->texture, NULL,
			src->location);
}




bool spChessToolHandleGameEvent(SPChessTool* src, SDL_Event* event) {
	if (src == NULL || event == NULL) {
		return false;
	}
	if (event->type == SDL_MOUSEBUTTONDOWN) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, src->location)){
			src->duringdrag = true;
			return true;
		}
	}
	return false;
}



