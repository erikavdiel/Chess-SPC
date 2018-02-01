/*
 * SPChessGameWindow.c
 *
 *  Created on: 22 בספט 2017
 *      Author: ערי
 */

#include "SPChessGameWindow.h"



SPChessButton** spChessGameCreateButtons(SDL_Renderer* rend){
	if (rend == NULL ) {
		return NULL ;
	}
	SPChessButton** buttons = malloc(sizeof(SPChessButton*)*6);
	if (buttons == NULL ) {
		return NULL ;
	}
	SDL_Rect restart = { .x = 20, .y = 10, .h = 72, .w = 180 };
	SDL_Rect load = { .x = 20, .y = 100, .h = 72, .w = 180 };
	SDL_Rect save = { .x = 20, .y = 190, .h = 72, .w = 180 };
	SDL_Rect undo = { .x = 20, .y = 280, .h = 72, .w = 180 };
	SDL_Rect mainMenu = { .x = 20, .y = 440, .h = 72, .w = 180 };
	SDL_Rect exit = { .x = 20, .y = 520, .h = 72, .w = 180 };
	buttons[0] = spChessButtonCreate(rend, &restart, true, "./images/buttons/Restart.bmp",
			"./images/buttons/Restart.bmp", SP_BUTTON_RESTART);
	buttons[1] = spChessButtonCreate(rend, &load, true, "./images/buttons/GameLoad.bmp",
			"./images/buttons/GameLoad.bmp", SP_BUTTON_GOTOLOAD);
	buttons[2] = spChessButtonCreate(rend, &save, true, "./images/buttons/Save.bmp",
			"./images/buttons/Save.bmp", SP_BUTTON_SAVE);
	buttons[3] = spChessButtonCreate(rend, &undo, true, "./images/buttons/Undo.bmp",
			"./images/buttons/UndoNA.bmp", SP_BUTTON_UNDO);
	buttons[4] = spChessButtonCreate(rend, &mainMenu, true, "./images/buttons/MainMenu.bmp",
			"./images/buttons/MainMenu.bmp", SP_BUTTON_BACK);
	buttons[5] = spChessButtonCreate(rend, &exit, true, "./images/buttons/GameExit.bmp",
			"./images/buttons/GameExit.bmp", SP_BUTTON_EXIT);

	for (int j = 0; j<6; j++){
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



void spChessGameWindowCreateToolsByBoard(SPChessGameWindow* data){
	SPChessTool** tools = (SPChessTool**) malloc(sizeof(SPChessTool*)*32);
	int numOfTools = 0;
	for (int i = 0; i<8; i++){
		for (int j = 0; j<8; j++){
			char checker = data->currGame->gameBoard[i][j];
			if (checker != '_'){
				SPChessTool* currTool;
				SDL_Rect currRect = { .x = 200 + 75*j + 5, .y = 75*i + 5, .h = 64, .w = 64 };
				SP_TOOL type = spChessToolConvertCharToSPTOOL(checker);
				currTool = spChessToolCreate(data->windowRenderer, &currRect, type);
				if (currTool != NULL){
					currTool->currRow = i;
					currTool->currCol = j;
					tools[numOfTools] = currTool;
					numOfTools++;
				}
			}
		}
	}
	data->numOfTools = numOfTools;
	data->tools = tools;
}



SPWindow* spChessGameWindowCreate(SPChessGame* game, SPSetting* set){
	if (game == NULL){
		return NULL;
	}
	SPWindow* res = (SPWindow*) malloc(sizeof(SPWindow));
	SPChessGameWindow* data = (SPChessGameWindow*) malloc(sizeof(SPChessGameWindow));
	SDL_Window* window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	SDL_Surface* background = SDL_LoadBMP("./images/bg/GameBG.bmp");
	SPChessButton** buttons = spChessGameCreateButtons(renderer);
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
	if (res == NULL || data == NULL || window == NULL || renderer == NULL || background == NULL
			|| buttons == NULL || backgroundTexture == NULL) {
		free(res);
		free(data);
		free(buttons);
		SDL_FreeSurface(background);
		SDL_DestroyTexture(backgroundTexture);
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
	data->type = SP_WINDOW_GAME;
	data->currGame = game;
	data->settings = set;
	data->currGame->level = set->difficulty;
	data->madeMove = false;
	data->boardFreeze = false;
	data->saved = false;
	res->data = (void*) data;
	spChessGameWindowCreateToolsByBoard(data);
	if (data->tools == NULL){
		printf("ERROR IN TOOLS MAKING");
	}
	res->handleEventWindow = spChessGameWindowHandleEvent;
	res->destroyWindow = spChessGameWindowDestroy;
	res->drawWindow = spChessGameWindowDraw;
	res->getType = spChessGameGetType;
	return res;
}



bool spChessGameWindowIsOnBoard(SDL_Event* event){
	if (event->button.x < 200 || event->button.x > 800){
		return false;
	}
	if (event->button.y < 0 || event->button.y > 600){
		return false;
	}
	return true;
}



void spChessGameWindowDestroy(SPWindow* src){
	if (src == NULL ) {
		return;
	}
	SPChessGameWindow* data = (SPChessGameWindow*) src->data;
	for (int i=0; i<6; i++){
		spChessButtonDestroy(data->buttons[i]);
	}
	if (data->buttons != NULL){
		free(data->buttons);
	}
	spChessGameWindowDestroyToolList(data);
	spChessGameDestroy(data->currGame);
	SDL_DestroyTexture(data->backgroundTexture);
	SDL_DestroyRenderer(data->windowRenderer);
	SDL_DestroyWindow(data->window);
	free(data);
	free(src);
}



void spChessGameWindowUndoMove(SPChessGameWindow* src){
	if (src == NULL){
		return;
	}
	CHMove* lastMove = spArrayListGetLast(src->currGame->historyMoves);
	src->currGame->gameBoard[lastMove->prevRow][lastMove->prevCol] = lastMove->tool;
	for (int i = 0; i<src->numOfTools; i++){
		if (src->tools[i]->currCol == lastMove->currCol && src->tools[i]->currRow == lastMove->currRow){
			SDL_Rect rect = {.x = 200+(lastMove->prevCol)*75 +5, .y= ((lastMove->prevRow)*75)+5, .h=64, .w=64};
			free(src->tools[i]->location);
			src->tools[i]->location = spCopyRect(&rect);
			src->tools[i]->currCol = lastMove->prevCol;
			src->tools[i]->currRow = lastMove->prevRow;
			break;
		}
	}
	if (lastMove->didCapture && lastMove->captured != '_'){
		src->currGame->gameBoard[lastMove->currRow][lastMove->currCol] = lastMove->captured;
		SDL_Rect rect = {.x = 200+(lastMove->currCol)*75 +5, .y= ((lastMove->currRow)*75)+5, .h=64, .w=64};
		SPChessTool* capturedTool = spChessToolCreate(src->windowRenderer, &rect, spChessToolConvertCharToSPTOOL(lastMove->captured));
		capturedTool->currCol = lastMove->currCol;
		capturedTool->currRow = lastMove->currRow;
		src->tools[src->numOfTools] = capturedTool;
		src->numOfTools = src->numOfTools + 1;

	}
	else {
		src->currGame->gameBoard[lastMove->currRow][lastMove->currCol] = '_';
	}
	spArrayListRemoveLast(src->currGame->historyMoves);
	changePlayer(src->currGame);
}



void spChessGameWindowHandleUndo(SPChessGameWindow* src){
	if (src == NULL){
		return;
	}
	if (src->settings->gameMode == 2){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "UNDO",
				"Undo command not Available in 2 players mode", NULL);
	}
	else if (src->currGame->historyMoves->actualSize < 2){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Undo",
				"Empty history, move cannot be undone", NULL );
	}
	else{
		spChessGameWindowUndoMove(src);
		spChessGameWindowUndoMove(src);
	}
}



void spChessGameWindowHandleRestart(SPChessGameWindow* src){
	if (src == NULL){
		return;
	}
	spChessGameDestroy(src->currGame);
	spChessGameWindowDestroyToolList(src);
	src->currGame = spChessGameCreate(6);
	src->currGame->level = src->settings->difficulty;
	spChessGameWindowCreateToolsByBoard(src);
	src->toolToMove = NULL;
	src->madeMove = false;
	src->boardFreeze = false;
	src->saved = false;
}



SP_WINDOW_EVENT spChessGameWindowHandleGameMenuEvent(SPWindow* src, SDL_Event* event){
	if (src == NULL || event==NULL){
		return SP_MEVENT_NONE;
	}
	SPChessGameWindow* data = (SPChessGameWindow*)src->data;
	for(int i=0; i<6 ;i++){
		SP_BUTTON_EVENT buttonEvent = spChessButtonHandleButtonEvent(data->buttons[i], event);
		if (buttonEvent != SP_BEVENT_NONE){
			switch(buttonEvent){
			case SP_BEVENT_EXIT: if (!data->saved){
								return SP_MEVENT_EXIT;
								}
								return SP_MEVENT_QUIT;
			case SP_BEVENT_GOTOLOAD: return SP_MEVENT_GOTOLOAD;
			case SP_BEVENT_BACK: if (!data->saved){
									return SP_MEVENT_BACK;
			}
			return SP_MEVENT_MAINMENU;
			case SP_BEVENT_UNDO:
				spChessGameWindowHandleUndo(data);
				return SP_MEVENT_NONE;
			case SP_BEVENT_RESTART:
				spChessGameWindowHandleRestart(data);
				return SP_MEVENT_NONE;
			case SP_BEVENT_SAVE:
				data->saved = true;

				return SP_MEVENT_SAVE;
			default: return SP_MEVENT_NONE;
			}
		}
	}
	return SP_MEVENT_NONE;
}



int spChessGameWindowGetPositionOnGameBoard(int z, int off){
	if (z-off<0){
		return -1;
	}
	if (z-off<75){
		return 0;
	}
	if (z-off<150){
		return 1;
	}
	if (z-off<225){
		return 2;
	}
	if (z-off<300){
		return 3;
	}
	if (z-off<375){
		return 4;
	}
	if (z-off<450){
		return 5;
	}
	if (z-off<525){
		return 6;
	}
	if (z-off<600){
		return 7;
	}
	else{
		return -1;
	}
}




int spChessGameWindowGetPositionOnGUIBoard(int z, int off){
	if (z-off<0){
		return -1;
	}
	int i = 75;
	if (z-off<75){
		return (0*i)+off;
	}
	if (z-off<150){
		return (1*i)+off;
	}
	if (z-off<225){
		return (2*i)+off;
	}
	if (z-off<300){
		return (3*i)+off;
	}
	if (z-off<375){
		return (4*i)+off;
	}
	if (z-off<450){
		return (5*i)+off;
	}
	if (z-off<525){
		return (6*i)+off;
	}
	if (z-off<600){
		return (7*i)+off;
	}
	else{
		return -1;
	}
}




void spChessGameWindowHandleBoardEvent(SPWindow* src, SDL_Event* event){
	SPChessGameWindow* data = (SPChessGameWindow*) src->data;
	for(int i = 0; i<data->numOfTools; i++){
		SPChessTool* currTool = data->tools[i];
		if (spChessToolGetToolColor(currTool) == spChessGameGetCurrentPlayer(data->currGame)){
			switch(event->type){
			case SDL_MOUSEBUTTONDOWN:
				if (spChessToolHandleGameEvent(currTool, event)){
					free(currTool->prevLocation);
					currTool->prevLocation = spCopyRect(currTool->location);
					return;
				}break;
			case SDL_MOUSEMOTION:
				if (currTool->duringdrag){
					currTool->location->x = event->motion.x -15;
					currTool->location->y = event->motion.y -15;
					return;
				}break;
			case SDL_MOUSEBUTTONUP:
				if (currTool->duringdrag){
					currTool->duringdrag = false;
					SDL_Point point;
					point.x = event->button.x;
					point.y = event->button.y;
					currTool->location->x = spChessGameWindowGetPositionOnGUIBoard(point.x, 200)+5;
					currTool->location->y = spChessGameWindowGetPositionOnGUIBoard(point.y, 0)+5;
					data->toolToMove = currTool;
					data->madeMove = true;
					return;
				}break;
			default: break;
			}}
}}



CHMove spChessGameWindowConvertGUIMoveToCHMove(SPChessGameWindow* src){
	int prevCol = spChessGameWindowGetPositionOnGameBoard(src->toolToMove->prevLocation->x, 200);
	int prevRow = spChessGameWindowGetPositionOnGameBoard(src->toolToMove->prevLocation->y, 0);
	int currCol = spChessGameWindowGetPositionOnGameBoard(src->toolToMove->location->x, 200);
	int currRow = spChessGameWindowGetPositionOnGameBoard(src->toolToMove->location->y, 0);
	CHMove move = spChessCreateMove('c', currRow, currCol, prevRow, prevCol);
	move.tool = spChessToolConvertSPTOOLtoChar(src->toolToMove->type);
	return move;
}



void spChessGameWindowUpdateTools(SPChessGameWindow* src){
	if (src == NULL){
		return;
	}
	int i = 0;
	while (i<src->numOfTools){
		if (src->tools[i]->location->x == src->toolToMove->location->x && src->tools[i]->location->y == src->toolToMove->location->y){
			if (src->tools[i]->type != src->toolToMove->type){
				spChessToolDestroy(src->tools[i]);
				break;
			}
		}
		i++;
	}
	for (int j = i; j<src->numOfTools-1; j++){
		src->tools[j] = src->tools[j+1];
	}
	src->numOfTools = src->numOfTools -1;
}



bool spGameWindowHandlePostMove(SPChessGame* src){
	SP_CHESS_GAME_MESSAGE msg = spCheckForCheck(src);
	if (msg == SP_CHESS_GAME_BLACK_CHECK){
		if (spChessGameCheckForCheckMateOnBlack(src) == SP_CHESS_GAME_CHECKMATE){
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Checkmate",
					"Checkmate! white player wins the game", NULL );
			return true;
		}
		else{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Checkmate",
					"Check: black King is threatened!", NULL );
			return false;
		}
	}
	else if (msg == SP_CHESS_GAME_WHITE_CHECK){
		if (spChessGameCheckForCheckMateOnWhite(src) == SP_CHESS_GAME_CHECKMATE){
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Checkmate",
					"Checkmate! black player wins the game", NULL );
			return true;
		}
		else{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Checkmate",
					"Check: white King is threatened!", NULL );
			return false;
		}
	}
	msg = spChessCheckforTie(src);
	if (msg == SP_CHESS_GAME_TIE){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Tie",
					"The game is tied", NULL );
		return true;
	}
	return false;
}



SP_CHESS_GAME_MESSAGE spChessGameWindowHandleComputerMove(SPChessGameWindow* src){
	if (src == NULL){
		return SP_CHESS_GAME_FAILED;
	}
	CHMove* moveToDo = spMiniMaxSuggestMove(src->currGame);
	spChessGameSetMove(src->currGame, moveToDo);
	src->saved = false;
	for (int i = 0; i<src->numOfTools; i++){
		if(src->tools[i]->currRow == moveToDo->prevRow && src->tools[i]->currCol == moveToDo->prevCol){
			src->toolToMove = src->tools[i];
			src->tools[i]->currRow = moveToDo->currRow;
			src->tools[i]->currCol = moveToDo->currCol;
			SDL_Rect rect = {.x = 200+(moveToDo->currCol)*75 +5, .y= ((moveToDo->currRow)*75)+5, .h=64, .w=64};
			free(src->tools[i]->location);
			src->tools[i]->location = spCopyRect(&rect);
			break;
		}
	}
	if (moveToDo->didCapture && moveToDo->captured != '_'){
		spChessGameWindowUpdateTools(src);
	}
	spChessDestroyMove(moveToDo);
	src->toolToMove = NULL;
	if(spGameWindowHandlePostMove(src->currGame)){
		return SP_CHESS_GAME_TIE; ////freeze board
	}
	return SP_CHESS_GAME_SUCCESS;
}



SP_WINDOW_EVENT spChessGameWindowHandleEvent(SPWindow* src, SDL_Event* event){
	SPChessGameWindow* data = (SPChessGameWindow*) src->data;
	if (data->settings->gameMode == 1 && data->settings->userColor != data->currGame->currentPlayer){
		if (spChessGameWindowHandleComputerMove(data) == SP_CHESS_GAME_TIE){
			data->boardFreeze = false;
		}
	}
	if (src == NULL || event==NULL){
		return SP_MEVENT_NONE;
	}
	if (!spChessGameWindowIsOnBoard(event)){
		return spChessGameWindowHandleGameMenuEvent(src, event);
	}
	else if (!data->boardFreeze){
		spChessGameWindowHandleBoardEvent(src, event);
		if (data->madeMove){
			data->madeMove = false;
			CHMove move = spChessGameWindowConvertGUIMoveToCHMove(data);
			if(!spChessGameCheckByTool(data->currGame, &move, data->currGame->currentPlayer) || spChessCheckIfSelfCheck(data->currGame, &move)){
				free(data->toolToMove->location);
				data->toolToMove->location = spCopyRect(data->toolToMove->prevLocation);
				data->toolToMove = NULL;
				return SP_MEVENT_NONE;
			}
			spChessGameSetMove(data->currGame, &move);
			data->saved = false;
			data->toolToMove->currRow = move.currRow;
			data->toolToMove->currCol = move.currCol;
			if(move.didCapture){
				spChessGameWindowUpdateTools(data);
			}
			if (spGameWindowHandlePostMove(data->currGame)){
				if (data->settings->gameMode == 2){
					return SP_MEVENT_QUIT;
				}
			}
			data->toolToMove = NULL;
			return SP_MEVENT_NONE;
		}
		else{
			return SP_MEVENT_NONE;
	}}
	return SP_MEVENT_NONE;
}




void spChessGameWindowDestroyToolList(SPChessGameWindow* data){
	if (data == NULL){
		return;
	}
	for (int i=0; i<data->numOfTools; i++){
		spChessToolDestroy(data->tools[i]);
	}
	free(data->tools);
	data->numOfTools = 0;
}






void spChessGameWindowDraw(SPWindow* src){
	if (src == NULL ) {
		return;
	}
	SPChessGameWindow* data = (SPChessGameWindow*) src->data;
	SDL_RenderClear(data->windowRenderer);
	SDL_RenderCopy(data->windowRenderer, data->backgroundTexture, NULL, NULL);
	for (int i=0 ;i < 6; i++) {
		spChessButtonDraw(data->buttons[i]);
	}
	for (int i=0; i<data->numOfTools; i++){
		spChessToolDraw(data->tools[i]);
	}
	SDL_RenderPresent(data->windowRenderer);
	}




SDL_Renderer* spChessGameGetRenderer (SPWindow* window){
	SPChessGameWindow* win = (SPChessGameWindow*) window->data;
	return win->windowRenderer;
}


SP_WINDOW spChessGameGetType(){
	return SP_WINDOW_GAME;
}




