/*
 * SPChessFile.h
 *
 *  Created on: Aug 30, 2017
 *      Author: Maria Klimkin
 */

#ifndef SPCHESSFILE_H_
#define SPCHESSFILE_H_

#include "SPChessGame.h"
#include "SPChessSettings.h"


/*
 * Returns the color of the current player of the game.
 *
 * @param:
 * src - the current game played.
 * @return:
 * A string:
 * "white" if its the white players turn in src,
 * "black" otherwise.
 */
const char* spFileCurrentPlayerColor(SPChessGame* src);


/*
 * Writes the current state of the game board to a file as writen in the submission guidlines.
 * @param:
 * src - the current game played.
 * fb - the file to write inside the boardgame of src.
 */
void spFilePrintBoardToFile(SPChessGame* src, FILE* fb);


/*
 * Saves a given chess game and its setting to xml file. The file will contain the following information:
 * current turn (white or black), game mode (1-player or 2-player), difficulty, user color (relevant in case of 1-player mode),
 * and the board state.
 * In case the saving operation hasn't succeeded, an error message will be printed.
 */
void spFileSaveGameToFile(SPChessGame* src, SPSetting* set, char* path);


/*
 * Calculates the length of a given file.
 * @param:
 * file - the file to calculate its length.
 * @return:
 * -1 - if an error acured while reading the file.
 * The length of the file otherwise.
 *
 */
long spFileCheckFilesLength(FILE* file);


/*
 * Parsers a string that holds information about a certain game and its setting and updates the given
 * SPChessGame and SPSetting accordingly.
 * @param:
 * str - the string to parse.
 * src - a SPChessGame instant to update.
 * set - a SPSetting instant to update.
 * @return:
 * SP_CHESS_GAME_FAILED - if an error occured.
 * SP_CHESS_GAME_SUCCESS - if the string interpreted correctly and src and set updated successfuly.
 */
SP_CHESS_GAME_MESSAGE spFileRestoreGameFromStr(char* str, SPChessGame* src, SPSetting* set);


/*
 * Loads a chess game and its setting from a xml file.
 * @param:
 * path - a path to the xml file.
 * src - a SPChessGame instant to update according to the game saved in 'path'.
 * set - a SPSetting instant to update according to the setting saved in 'path'.
 * @return:
 * SP_CHESS_GAME_FAILED - if an error occured while opening or reading the file, or if an allocation error
 * occured.
 * SP_CHESS_GAME_SUCCESS - otherwise.
 */
SP_CHESS_GAME_MESSAGE spFileLoadGameFromFile(char* path, SPChessGame* src, SPSetting* set);


/*
 * Handels a load from file operation. If a certain load from file operation hasn't succeded, an error message will
 * be printed.
 * @param:
 * path - a path to xml file that holds a chess game and its setting.
 * src - a SPChessGame instant to update according to the game saved in 'path'.
 * set - a SPSetting instant to update according to the setting saved in 'path'.
 * @return:
 * An SP_COMMAND
 * SP_INVALID_LINE - if the load operation failed.
 * SP_LOAD - otherwise.
 */
SP_COMMAND spFileHandleLoad(char* path, SPChessGame* src, SPSetting* set);


/*
 * Copies the content of one file (fp1) to another file (fp2).
 *
 */
void spFileCopyFileToOtherFile(FILE* fp1, FILE* fp2);


/*
 * Handles a save game event in GUI mode. The game is  automatially saved to game slot1, so this function
 * copies the previuos saved game in slot1 to slot2, the saved game in slot2 to slot3 and so on.
 * If there is a saved game in slot5 it is erased.
 *
 * @param:
 * src - the source game to save.
 * set - the setting of src.
 *
 * @return:
 * SP_CHESS_GAME_FAILED - if the saving of the game failed or if the copy of one of the slots failed.
 * SP_CHESS_GAME_SUCCESS - if the game saved.
 *
 */
SP_COMMAND spFileHandleSaveInSDL(SPChessGame* src, SPSetting* set);


#endif /* SPCHESSFILE_H_ */
