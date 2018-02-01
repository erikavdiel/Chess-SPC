/*
 * SPChessMenuWindow.h
 *
 *  Created on: 18 בספט 2017
 *      Author: ערי
 */

#ifndef SPCHESSMENUWINDOW_H_
#define SPCHESSMENUWINDOW_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include "SPWindow.h"
#include "SPChessButton.h"




/*
 * A structure that represents a certain menu window (might be main menu, game mode, difficulty or choose color).
 */
typedef struct sp_simplewindow_t  SPChessMenuWindow;
struct sp_simplewindow_t {
	SDL_Window* window;
	SDL_Surface** backgrounds; // 4 backrounds (main menu, game mode, difficulty,  player color).
	SDL_Texture* backgroundTexture; // changes according to the right window. Can be one of the surfeces.
	SDL_Renderer* windowRenderer; // the renderer of the window.
	SPChessButton** currButtons; // a list of the buttonsavailble in the specific window.
	int currNumOfWidgets; // the size of currButtons.
	SP_WINDOW type; // the type of the window.
};


/*
 * Creates all the buttons needed for the main menu window
 * and returns a list of them.
 *
 * @param:
 * rend - the renderer of the source window.
 *
 */
SPChessButton** spMenuCreateMainMenuWidgets(SDL_Renderer* rend);


/*
 * Creates all the buttons needed for the 'choose user color' window
 * and returns a list of them.
 *
 * @param:
 * rend - the renderer of the source window.
 *
 */
SPChessButton** spMenuCreateColorWidgets(SDL_Renderer* rend);


/*
 * Creates all the buttons needed for the 'choose difficulty level' menu window
 * and returns a list of them.
 *
 * @param:
 * rend - the renderer of the source window.
 *
 */
SPChessButton** spMenuCreateDifficultyWidgets(SDL_Renderer* rend);

/*
 * Creates all the buttons needed for the 'choose game mode' menu window
 * and returns a list of them.
 *
 * @param:
 * rend - the renderer of the source window.
 *
 */
SPChessButton** spMenuCreateGameModeWidgets(SDL_Renderer* rend);


/*
 * Calls the right function that creates the buttons needed to a certain type window
 * and returns a list of the buttons.
 *
 * @param:
 * rend - the renderer of the source window.
 * type - the type of the source window.
 *
 */
SPChessButton** spMenuCreateWidgetsByType(SDL_Renderer* rend, SP_WINDOW type);


/*
 * Returns the number of buttons associated with the type window.
 *
 * @param:
 * type - the type of the source window.
 *
 */
int spChessMenuWindowNumofWidgets(SP_WINDOW type);


/*
 * Loads all the surfaces needed for SP_WINDOW_MAINMENU, SP_WINDOW_GAMEMODE, SP_WINDOW_DIFFICULTY and SP_WINDOW_COLOR.
 * Returns a pointer the the list of surfaces.
 *
 */
SDL_Surface** spChessMenuGetBackgroundSurfaces();


/*
 * Destroy the list of surfaces and frees all memory assocciated with it.
 *
 * @param:
 * backrounds - the list to destroy.
 *
 */
void spChessMenuDestroySurfaces(SDL_Surface** backgrounds);


/*
 * Creates a default SPWINDOW* instant with data- SPChessMenuWindow, type- SP_WINDOW_MAINMENU
 * (afterwards the window type can be changed) and returns it.
 *
 */
SPWindow* spChessMenuWindowCreate();


/*
 * Destroys the list of all the buttons of the window and frees all memory associated with it.
 *
 * @param:
 * window - the source menu window to destroy.
 *
 */
void spChessMenuDestroyButtons(SPChessMenuWindow* window);


/*
 * Destroys an instant of SPWindow and frees all memory associated with it.
 *
 * @param:
 * src - the source window to destroy.
 *
 */
void spChessMenuWindowDestroy(SPWindow* src);


/*
 * Changes the type of an instant of SPWindow* to 'type' and destroy all the buttons of the previous window.
 *
 * @param:
 * src - the source window to destroy.
 * type - the type of src.
 *
 */
void spChessMenuChangeMenu(SPWindow* src, SP_WINDOW type);


/*
 * Handels an event of one of the buttons being pressed.
 *
 * @param:
 * src - the source menu window.
 * i  - represent a the index of the button chosen in 'src' buttons list.
 *
 */
void spChessMenuHandleChoose(SPChessMenuWindow* src, int i);


/*
 * Draws the src window to the renderer.
 *
 * @param:
 * src - the source window to draw.
 *
 */
void spChessMenuWindowDraw(SPWindow* src);


/*
 * Gets a bevent (button event) and returns a fitting mevent (menu event).
 *
 */
SP_WINDOW_EVENT spChessButtonTranslateEvent(SP_BUTTON_EVENT buttonEvent);


/*
 * Handles the given event according to its x,y coordinates on a SPWindow* src with data
 * SPChessMenuWindow* and according to it's type.
 *
 * @param:
 * src - the source window.
 * event - the last event occured.
 *
 * @return:
 * The fitting mevent.
 *
 */
SP_WINDOW_EVENT spChessMenuWindowHandleEvent(SPWindow* src, SDL_Event* event);


/*
 * Returns the current type of a certain window.
 *
 * @param:
 * src - the source window.
 *
 */
SP_WINDOW spChessMenuGetType(SPWindow* src);


/*
 * Returns the Renderer of a certain window.
 *
 * @param:
 * src - the source window.
 *
 */
SDL_Renderer* spChessMenuGetRenderer (SPWindow* window);



#endif /* SPCHESSMENUWINDOW_H_ */
