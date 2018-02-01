/*
 * SPChessGameWindow.h
 *
 *  Created on: 22 בספט 2017
 *      Author: ערי
 */

#ifndef SPCHESSGAMEWINDOW_H_
#define SPCHESSGAMEWINDOW_H_




#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include "SPWindow.h"
#include "SPChessButton.h"
#include "SPChessMainAux.h"
#include "SPChessGame.h"
#include "SPChessTool.h"

/*
 * Structure that represents the gameboard window of the program.
 * The structure has the following fields:
 * window - the SDL_Window*.
 * backgroundTexture - the texture of the backround of the gameboard window.
 * windowRenderer - the SDL_Renderer* of the window.
 * buttons - A list of SPChessButton* of the window.
 * currGame - the current SPChessGame that is played in the window.
 * settings - the settings of the current game.
 * tools - a list of SPChessTool*, all the tools on the gameboard.
 * numOfTools - size of 'tools'.
 * type - the type of SP_WINDOW - SP_WINDOW_GAME.
 * toolToMove - a SPChessTool*, the next tool that should move on the board.
 * madeMove - set to true if a certain move has been made, false otherwise.
 * boardFreeze - true if it is impossible to press the buttons of the game window or move tools, false otherwise.
 * saved - true if the game saved after the last move done.
 *
 */
typedef struct sp_gamewindow_t  SPChessGameWindow;
struct sp_gamewindow_t {
	SDL_Window* window;
	SDL_Texture* backgroundTexture;
	SDL_Renderer* windowRenderer;
	SPChessButton** buttons;
	SPChessGame* currGame;
	SPSetting* settings;
	SPChessTool** tools;
	int numOfTools;
	SP_WINDOW type;
	SPChessTool* toolToMove;
	bool madeMove;
	bool boardFreeze;
	bool saved;
};


/*
 * Creates a list of all the buttons needed for the game window (load, save, undo, restart, main menu and exit).
 *
 * @param:
 * rend - the SDL_Rederer of the game window.
 *
 * @return:
 * A list of all the buttons.
 *
 */
SPChessButton** spChessGameCreateButtons(SDL_Renderer* rend);


/*
 * Creates all the tools needed for a specific game state. It is done by going over on the game board of
 * SPChessGame. Updates the tools list of and its size of the game window.
 *
 * @param:
 * data - the source game window to update.
 *
 */
void spChessGameWindowCreateToolsByBoard(SPChessGameWindow* data);


/* Allocates memory to create a new structure of window.
 *
 * @param:
 * game - the source SPChessGame.
 * set - the settings of the game.
 *
 * @return:
 * The new window.
 *
 */
SPWindow* spChessGameWindowCreate(SPChessGame* game, SPSetting* set);


/* Checks if the coordinates of the  last event occured are on the game board part of the window
 * or on  the menu part of the game window.
 *
 * @param:
 * event - the event to check.
 *
 * @return:
 * true - if the event occured on the game board (movement of a tool).
 * false - if the event occured on the button menu (undo, restart...)
 *
 */
bool spChessGameWindowIsOnBoard(SDL_Event* event);


/*
 * Destroys the game window and frees all memory assocciated with it.
 * It happenes when the user wants to exit the game or go back to the main menu.
 *
 * @param:
 * src - the source window to destroy.
 *
 */
void spChessGameWindowDestroy(SPWindow* src);


/*
 * Invokes an undo move event on the gameboard of the game window.
 *
 * @param:
 * src - the source game window to update.
 *
 */
void spChessGameWindowUndoMove(SPChessGameWindow* src);


/*
 * Checks if an undo move event is possible (if the size of historyMoves is < 1 ,it is not allowed to undo a move).
 * If the undo move is not allowed then a fitted massagebox appears,
 * otherwise, calls
 * spChessGameWindowUndoMove to invoke the undo move.
 *
 * @param:
 * src - the source game window.
 *
 */
void spChessGameWindowHandleUndo(SPChessGameWindow* src);


/*
 * Restart the game with its current settings.
 *
 * @param:
 * src - the source game window to restart.
 *
 */
void spChessGameWindowHandleRestart(SPChessGameWindow* src);


/* This function is called if the last event was on the menu bar of the game window,
 * and checks which button was pressed.
 *
 * @param:
 * src - the source game window.
 * event - the last event occured.
 *
 * @return:
 * SP_MEVENT_NONE -  if it is an in-game command (undo, restart).
 * The right SP_WIDOW_EVENT - if it related to the GameWindow or GUIManager.
 *
 */
SP_WINDOW_EVENT spChessGameWindowHandleGameMenuEvent(SPWindow* src, SDL_Event* event);


/*
 * Gets the x or y coordinate on the board (x: offset == 200, y: offset == 0) and
 * returns the right column or row (respectivly) on the SPChessGame gameBoard.
 *
 */
int spChessGameWindowGetPositionOnGameBoard(int z, int off);


/*
 * Gets the x or y coordinate on the board (x: offset == 200, y: offset == 0) and
 * returns the correct checker coordinate (the right square on the game board).
 *
 */
int spChessGameWindowGetPositionOnGUIBoard(int z, int off);


/*
 * Handles the event of a movement of a tool on the board.
 * It check which SPChessTool on the board was dragged or dropped and updates the window accordinglly.
 *
 * @param:
 * src - the source game window.
 * event - the last event on the window.
 *
 */
void spChessGameWindowHandleBoardEvent(SPWindow* src, SDL_Event* event);


/*
 * Translates the drag and drop coordinates and returns which move is should be tried to play
 * on the SPChessGame gameBoard.
 *
 * @param:
 * src - the source game window.
 *
 * @return:
 * CHMove - the move that is tried to do on the board.
 *
 */
CHMove spChessGameWindowConvertGUIMoveToCHMove(SPChessGameWindow* src);


/*
 * This function is called if a certain tool was captured. It destroys the matching SPChessTool from the tools list
 * and updates the numOfTools field.
 *
 * src - the source game window.
 */
void spChessGameWindowUpdateTools(SPChessGameWindow* src);


/*
 * Reacts to a legal move that was made by one of the players.
 * It opens a massege box if the move lead to check, checkmate or tie.
 *
 * @param:
 * src - the source game window.
 *
 * @return:
 * true - if the game ended (checkmate or tie).
 * false - otherwise.
 *
 */
bool spGameWindowHandlePostMove(SPChessGame* src);


/*
 * Simulates the computer's move by calling the minimax function and invoking the returned move by it.
 *
 * @param:
 * src - the source game window.
 *
 * @return:
 * SP_CHESS_GAME_TIE - if the move that was done lead to tie or checkmate (end of the game)
 * and now the user can only press the menu buttons.
 * SP_CHESS_GAME_SUCCESS - if the game continues.
 *
 */
SP_CHESS_GAME_MESSAGE spChessGameWindowHandleComputerMove(SPChessGameWindow* src);


/*
 * Checks the coordinates of a certain event in a window and handles it accordingly.
 *
 * @param:
 * src - the source game window.
 * event - the event to check.
 *
 * @return:
 * SP_WINDOW_EVENT - the fitting event.
 *
 */
SP_WINDOW_EVENT spChessGameWindowHandleEvent(SPWindow* src, SDL_Event* event);


/*
 * Destroys the field tools of data (the tools list).
 *
 * @param:
 * data - the source game window.
 *
 */
void spChessGameWindowDestroyToolList(SPChessGameWindow* data);


/*
 * Draws the game window and all of its associated buttons and tools.
 *
 * @param:
 * src - the source window.
 *
 */
void spChessGameWindowDraw(SPWindow* src);


/*
 * Returns the renderer of the specific window.
 *
 * @param:
 * src - the source window.
 *
 */
SDL_Renderer* spChessGameGetRenderer (SPWindow* window);


/*
 * Returns the type of a window (for the gui manager).
 *
 */
SP_WINDOW spChessGameGetType();





#endif /* SPCHESSGAMEWINDOW_H_ */
