/*
 * SPChessLoadWindow.h
 *
 *  Created on: 22 בספט 2017
 *      Author: ערי
 */

#ifndef SPCHESSLOADWINDOW_H_
#define SPCHESSLOADWINDOW_H_



#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include "SPWindow.h"
#include "SPChessButton.h"


/*
 * A structure that represents a load-menu window.
 */
typedef struct sp_loadwindow_t  SPChessLoadWindow;
struct sp_loadwindow_t {
	SDL_Window* window; //the window of the load-menu
	SDL_Texture* backgroundTexture; //the texture of the window
	SDL_Renderer* windowRenderer; //the renderer of the window
	SPChessButton** buttons; //a list of all the buttons availble in the load menu (Game Slot 1-5,
	                         // back and load)
	int numOfWidgets; // = 7
	SP_WINDOW type; // = SP_WINDOW_LOAD
};


/*
 * Creates all the buttons needed for the load menu window (slots, load, back) and
 * returns a list of them.
 *
 * @param:
 * rend - the renderer of the load window.
 *
 */
SPChessButton** spMenuCreateLoadWidgets(SDL_Renderer* rend);


/*
 * Creates a SPWindow* of the SPChessLoadWindow* and all the widgets associated with it, and
 * return it.
 *
 */
SPWindow* spChessLoadWindowCreate();


/*
 * Handles the event of one of the slots being chosen. It switches the active field of the chosen slot
 * and also switches the load button to be in an active state.
 *
 * @param:
 * src - the source load menu window.
 * i - indicates which slot was pressed.
 *
 */
void spChessLoadHandleChoose(SPChessLoadWindow* src, int i);


/*
 * Handles any event of button pressed on the load menu window.
 *
 * @param:
 * src - the source load menu window.
 * event - the last event occured.
 *
 * @return:
 * SP_MEVENT_NONE - if src==NULL or event==NULL or if no button was pressed.
 * SP_MEVENT_CHANGESETTINGS_1-5 - if one of the slots was pressed.
 * SP_MEVENT_BACK - if the 'back' button was pressed.
 *
 */
SP_WINDOW_EVENT spChessLoadWindowHandleEvent(SPWindow* src, SDL_Event* event);


/*
 * Destroys src and frees all memory associated with it.
 *
 * @param:
 * src - the source load menu window.
 *
 */
void spChessLoadWindowDestroy(SPWindow* src);


/*
 * Draws the load menu window and its widgets to its renderer.
 *
 * @param:
 * src - the source window.
 *
 */
void spChessLoadWindowDraw(SPWindow* src);


/*
 * Returns the Renderer of window.
 *
 * @param:
 * src - the source window.
 *
 */
SDL_Renderer* spChessLoadGetRenderer (SPWindow* window);


/*
 * Returns the type of the load menu window.
 *
 * @return:
 * SP_WINDOW_LOAD.
 *
 */
SP_WINDOW spChessLoadGetType();


#endif /* SPCHESSLOADWINDOW_H_ */
