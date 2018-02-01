/*
 * SPChessGUIManager.h
 *
 *  Created on: 19 בספט 2017
 *      Author: ערי
 */

#ifndef SPCHESSGUIMANAGER_H_
#define SPCHESSGUIMANAGER_H_
#include <SDL.h>
#include <SDL_video.h>
#include "SPWindow.h"
#include "SPChessMenuWindow.h"
#include "SPChessMainAux.h"
#include "SPChessGame.h"
#include "SPChessLoadWindow.h"
#include "SPChessGameWindow.h"

/*
 * An enum that informs the GUI manager if it needs to finish the program (quit) or not.
 */
typedef enum{
	SP_MANAGER_QUIT,
	SP_MANAGER_NONE,
}SP_MANAGER_EVENT;



/*
 * A structure that represents a GUI manager - that handles all the functions assocciated
 * with the GUI mode game. It has the following fields:
 * menuWin - a SPWindow* of the main menu.
 * gameWin - a SPWindow* of the game board window.
 * loadWin - a SPWindow* of the load window.
 * settings - the settings of the SPChessGame.
 * activeWin - the window that is active at the moment.
 * lastActiveWin - the window that was active before (for load window).
 *
 */
typedef struct {
	SPWindow* menuWin;
	SPWindow* gameWin;
	SPWindow* loadWin;
	SPSetting* settings;
	SP_WINDOW activeWin;
	SP_WINDOW lastActiveWin;
} SPChessGuiManager;


/*
 * Creates a new SPChessGuiManager and returns it. The active window is set to main menu, the settings
 * are set to default.
 *
 */
SPChessGuiManager* spChessGuiManagerCreate();


/*
 * Destroys and free the GUI Manager and all windows and fields assocciated with it.
 *
 * @param:
 * src - the manager to destroy.
 *
 */
void spChessGuiManagerDestroy(SPChessGuiManager* src);


/*
 * Checks which window is active in the gui manager and draws it.
 *
 * @param:
 * src - the source gui manager.
 *
 */
void spChessGuiManagerDraw(SPChessGuiManager* src);


/*
 * Returns the window of the right type from the gui manager.
 *
 * @param:
 * src - the source gui manager.
 * type - the type of the window needed.
 *
 */
SPWindow* spChessGuiManagerGetWindow (SPChessGuiManager* src, SP_WINDOW type);


/*
 * Opens the game window and begins the game according to the src settings.
 *
 * @param:
 * src - the source gui manager.
 */
void spChessGuiManagerStartGame (SPChessGuiManager* src);


/*
 * Handles SP_MEVENT_NEXT event. It changes the open window to the next screen
 * according to the current screen opened.
 *
 * @param:
 * src - the source gui manager.
 *
 * @return:
 * SP_MANAGER_NONE.
 *
 */
SP_MANAGER_EVENT spChessGuiManagerHandleNext(SPChessGuiManager* src);


/*
 * Handles SP_MEVENT_BACK event.
 * It switches to the previous screen according to the current screen.
 *
 * @param:
 * src - the source gui manager.
 *
 * @return:
 * SP_MANAGER_NONE.
 *
 */
SP_MANAGER_EVENT spChessGuiManagerHandleBack(SPChessGuiManager* src);


/*
 * Loads a certain game (according to the slot chosen) with its settings.
 *
 * @param:
 * src - the source gui manager.
 * path - the path to the game saved in the slot.
 *
 * @return:
 * SPChessGame* - the loaded game.
 *
 */
SPChessGame* spChessGuiManagerGetGameSlot(SPChessGuiManager* src, char* path);


/*
 * Handles a load event when needed to load a chosen game slot i (i=1,2,3,4,5).
 *
 * @param:
 * src - the source gui manager.
 * i - the slot number.
 *
 */
void spChessGuiManagerHandleLoad(SPChessGuiManager* src, int i);


/*
 * Changes game settings according to the buttons pressed in the current settings window.
 *
 * @param:
 * src - the source gui manager.
 * i - indicates which button in the window was pressed.
 *
 */
void spChessGuiManagerSet (SPChessGuiManager* src, int i);


/*
 * Handles the SP_WINDOW_EVENT event according to its type.
 *
 * @param:
 * src - the source gui manager.
 * event - the last event occured.
 *
 * @return:
 * SP_MANAGER_QUIT - if SP_MEVENT_EXIT event occured.
 * SP_MANAGER_NONE - otherwise.
 *
 */
SP_MANAGER_EVENT spChessGuiManagerHandleMenuEvent(SPChessGuiManager* src,
		SP_WINDOW_EVENT event);


/*
 * Saves the current game to game slot 1, shows a message that indicates if the saving operation
 * failed or succeeded.
 *
 * @param:
 * src - the source gui manager.
 *
 */
void spChessGuiManagerHandleSave(SPChessGuiManager* src);


/*
 * Switches the current window to main menu window and destroys the previous window.
 *
 * @param:
 * src - the source gui manager.
 *
 */
void spChessGuiManagerGoToMainMenu(SPChessGuiManager* src);


/*
 * Creates the message box that asks if to save the game, when the user quits the game.
 *
 * @return:
 * SP_MEVENT_NONE - if an error occured with displaying the message box.
 * buttonid - 0 (cancel), 1 (yes) or 2 (no).
 *
 */
int spChessGameMakeSaveBox();


/*
 * Creates the load menu window and switches the active window to load menu window.
 *
 * @param:
 * src - the source gui manager.
 *
 */
void spChessGuiManagerGoToLoadWindow(SPChessGuiManager* src);


/*
 * Switches from load menu window to the previous window that was active (main menu or game window).
 *
 * @param:
 * src - the source gui manager.
 *
 * @return:
 * SP_MANAGER_NONE.
 *
 */
SP_MANAGER_EVENT spChessGuiManagerBackFromLoadMenu(SPChessGuiManager* src);



/*
 * Handles an event according to the events location and the active window of the manager.
 *
 * @param:
 * src - the source gui manager.
 * event - the event to handle.
 *
 * @return:
 * SP_MANAGER_QUIT - if it is needed to close the program (the user wants to quit/close the program).
 * SP_MANAGER_NONE - otherwise.
 *
 */
SP_MANAGER_EVENT spChessGuiManagerHandleEvent(SPChessGuiManager* src, SDL_Event* event) ;


/*
 * Opens the GUI mode of the program, and begins the game in GUI mode.
 * This function is called only from main.
 *
 */
void spChessGuiMode();




#endif /* SPCHESSGUIMANAGER_H_ */
