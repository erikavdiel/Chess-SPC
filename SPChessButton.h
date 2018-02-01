/*
 * SPChessButton.h
 *
 *  Created on: 18 בספט 2017
 *      Author: ערי
 */

#ifndef SPCHESSBUTTON_H_
#define SPCHESSBUTTON_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>


/*
 * An enum to represent a button event. It informs how to handle the event that occured,
 * and which button was pressed.
 */
typedef enum{
	SP_BEVENT_NONE,
	SP_BEVENT_NEXT,
	SP_BEVENT_START,
	SP_BEVENT_CHOOSE,
	SP_BEVENT_GOTOLOAD,
	SP_BEVENT_EXIT,
	SP_BEVENT_BACK,
	SP_BEVENT_UNDO,
	SP_BEVENT_RESTART,
	SP_BEVENT_SAVE,
}SP_BUTTON_EVENT;



typedef enum spbuttontype_t{
	SP_BUTTON_NEWGAME, //new game button (accessible on main menu)
	SP_BUTTON_GOTOLOAD, //go to the 'load games' menu (with the slots) button (accessible on main menu)
	SP_BUTTON_LOAD, //load (certain slot) button (accessible on load menu)
	SP_BUTTON_EXIT, //exit (the program) button (accessible on main menu and gameboard window)
	SP_BUTTON_BACK, // 'go to the previouse window' button (accessible on settings windows and load menu)
	SP_BUTTON_GAMESLOT, //load game button from a certain slot (there are 5 slots) button (accessible on load window)
	SP_BUTTON_PLAYERMODE, //set the game mode (one player or two players) button (accessible on settings window)
	SP_BUTTON_NEXT, //'go to the next window' button (accessible on settings windows)
	SP_BUTTON_START, //start the game button (accessible on settings windows)
	SP_BUTTON_DIFFICULTY, //set the difficulty level of the game button (accessible on settings window)
	SP_BUTTON_COLOR, //set the user color button (accessible on settings window)
	SP_BUTTON_UNDO, //undo previouse move button (accessible on gameboard window)
	SP_BUTTON_SAVE, // save the current game button (accessible on gameboard window)
	SP_BUTTON_RESTART, // restart the game button (accessible on gameboard window)
	SP_BUTTON_MAINMANU, // go to main menu button (accessible on gameboard window)
}SP_BUTTON;


/*
 * A structure that represents a button. Every SPChessButton holds the following
 * fields:
 *
 * 	buttonActiveTexture - the texture of the button when it's active.
 *	buttonNonActiveTexture - the texture of the button when it's not active.
 *	windowRenderer - the renderer which the button drawed in.
 *	location - contains the coordinates of the button in the window.
 *	bool active - true if the button is presseble, false otherwise.
 *	SP_BUTTON type - the type of the button.
 *
 */
typedef struct spchessbutton_t SPChessButton;
struct spchessbutton_t{
	SDL_Texture* buttonActiveTexture;
	SDL_Texture* buttonNonActiveTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	bool active;
	SP_BUTTON type;
};


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
SDL_Rect* spCopyRect(SDL_Rect* src);



/*
 * Creates a new button for a certain window in the program:
 *
 * @param:
 * All the fields of a SPChessButton structure.
 *
 * @return:
 * NULL - if one of the parameters is NULL or if an allocation failed.
 * SPChessButton* - the new button
 *
 */
SPChessButton* spChessButtonCreate(SDL_Renderer* windowRender, SDL_Rect* location, bool active,
		const char* activeImage, const char* nonActiveImage, SP_BUTTON type);



/*
 * Destroys a button and frees all memory associated with it.
 *
 * @param:
 * src- the button to destroy.
 *
 */
void spChessButtonDestroy(SPChessButton* src);



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
SP_BUTTON_EVENT spChessButtonHandleButtonEvent(SPChessButton* src, SDL_Event* event);


/*
 * Changes the value of 'active' field of a certain button. If active==true then changes it to false
 * and vice versa.
 *
 * @param:
 * src - the source button.
 *
 */
void spChessButtonChangeActiveness(SPChessButton* src);


/*
 * Draws the button in the right render (window) with the right texture according to its 'activeness'.
 *
 * @param:
 * src - the source button.
 *
 */
void spChessButtonDraw(SPChessButton* src);



#endif /* SPCHESSBUTTON_H_ */
