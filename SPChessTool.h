/*
 * SPChessTool.h
 *
 *  Created on: 23 בספט 2017
 *      Author: ערי
 */

#ifndef SPCHESSTOOL_H_
#define SPCHESSTOOL_H_


#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>
#include "SPChessButton.h"



/*
 * An enum to represent an event of movement of a tool on the board.
 */
typedef enum{
	SP_TEVENT_MOVE,
	SP_TEVENT_NONE,
}SP_TOOL_EVENT;



/*
 * An enum to represent the kind of a certain game tool.
 */
typedef enum sptooltype_t{
	SP_TOOL_PAWN_BLACK,
	SP_TOOL_PAWN_WHITE,
	SP_TOOL_ROOK_BLACK,
	SP_TOOL_ROOK_WHITE,
	SP_TOOL_KNIGHT_BLACK,
	SP_TOOL_KNIGHT_WHITE,
	SP_TOOL_BISHOP_BLACK,
	SP_TOOL_BISHOP_WHITE,
	SP_TOOL_QUEEN_BLACK,
	SP_TOOL_QUEEN_WHITE,
	SP_TOOL_KING_BLACK,
	SP_TOOL_KING_WHITE,
}SP_TOOL;



/*
 * A structure that represent a tool on the game board in GUI mode.
 */
typedef struct spchesstool_t SPChessTool;
struct spchesstool_t{
	SDL_Texture* texture; // the texture of the tool.
	SDL_Renderer* windowRenderer; // the renderer of the tool.
	SDL_Rect* location; // the coordinates of the tool's current position in the window.
	SDL_Rect* prevLocation; // the coordinates of the tool's previous position in the window (for undo move).
	int currRow; // the row number of the tool on the board.
	int currCol; // the column number of the tool on the board.
	SP_TOOL type; // the type of the tool.
	bool duringdrag; // true- if the tool is being drugged, false otherwise.
};




/*
 * Chooses and returns the right image adress for the tools type.
 *
 */
const char* spChessToolGetImageForTool(SP_TOOL type);


/*
 * Creates and returns the right tool for the given renderer (windowRenderer), location and type.
 *
 */
SPChessTool* spChessToolCreate(SDL_Renderer* windowRender, SDL_Rect* location,
		SP_TOOL type);


/*
 * Checks the color of a tool.
 *
 * @param:
 * src - the tool to check.
 *
 * @return:
 * 0 - if it is a black tool.
 * 1- if it is a white tool.
 *
 */
int spChessToolGetToolColor(SPChessTool* src);


/*
 * Converts a SP_TOOL type that represents the tool in GUI mode to a char that represent a tool in console mode.
 *
 * @param:
 * type - a type of a tool.
 *
 * @return:
 * A char that represents the 'type' tool.
 *
 */
char spChessToolConvertSPTOOLtoChar(SP_TOOL type);


/*
 * Converts a char that represent a chess tool in console mode to fitting SP_TOOL in GUI mode.
 *
 * @param:
 * c - a char that represents a chess tool.
 *
 * @return:
 * The correct SP_TOOL.
 *
 */
SP_TOOL spChessToolConvertCharToSPTOOL(char c);


/*
 * Destroys a SPChessTool instant and frees all memory associated with it.
 *
 * @param:
 * src - the source tool to destroy.
 *
 */
void spChessToolDestroy(SPChessTool* src);


/*
 * Draws the  source tool in the right place on the board.
 *
 * @param:
 * src - the source tool to draw.
 *
 */
void spChessToolDraw(SPChessTool* src);


/*
 * Checks if a certian pressing event is associated with a specific tool on the board.
 *
 * @param:
 * src - the tool to check if it is dragged.
 * event - the last event occured.
 *
 * @return:
 * true - if the event type is SDL_MOUSEBUTTONDOWN and also its
 * coordinates are inside the src (tool) loaction boundries.
 * false - otherwise.
 *
 */
bool spChessToolHandleGameEvent(SPChessTool* src, SDL_Event* event);


#endif /* SPCHESSTOOL_H_ */
