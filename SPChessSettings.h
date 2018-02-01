/*
 * SPChessSettings.h
 *
 *  Created on: 27 באוג 2017
 *      Author: ערי
 */

#ifndef SPCHESSSETTINGS_H_
#define SPCHESSSETTINGS_H_



#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "SPChessParser.h"


/*
 * A structure that contains inside it the setting of a specific chess game.
 * difficulty - can be set to 1,2,3 or 4.
 * gameMode - set to 1 if the game is a one-player game, or 2 if the it's a two-players game.
 * userColor - defined only if gameMode==1. Set to 1 if the user is the white player and the computer
 *             is the black player, or set to 0 otherwise.
 */
typedef struct setting_t{
	int difficulty;
	int gameMode;
	int userColor;
} SPSetting;


/*
 * Allocates memory for a new instant of SPSetting and creates it.
 *
 * @return:
 * setting that has the following features (default setting):
 * difficulty = 2, gameMode = 1 and userColor = 1.
 *
 */
SPSetting* spSettingCreate();

/*
 * Updates the difficulty of certain setting.
 *
 * @param:
 * set - the setting to update.
 * difficulty - the value to update set.difficulty.
 *
 */
void spSettingSetDifficulty(SPSetting* set, int difficulty);

/*
 * Updates the gameMode of certain setting.
 *
 * @param:
 * set - the setting to update.
 * gameMode - the value to update set.gameMode.
 *
 */
void spSettingSetGameMode(SPSetting* set, int gameMode);

/*
 * Updates the userColor of certain setting.
 *
 * @param:
 * set - the setting to update.
 * color - the value to update set.userColor.
 *
 */
void spSettingSetUserColor(SPSetting* set, int color);

/*
 * Resets a certain setting to default values.
 *
 * @param:
 * set - the setting to update.
 *
 */
void spSettingResetToDefault(SPSetting* set);

/*
 * Prints to the screen the setting of the game, after the user invokes "print_setting" command.
 *
 * @param:
 * set - the setting that are printed.
 *
 */
void spSettingPrintSettings(SPSetting* set);

/*
 * Prints an invalid command message to the screen when the user trys to set the setting to invalid values.
 *
 */
void spSettingInvalidCommand();

/*
 * Sets the value of the game mode when the user invokes a "game_mode x" command, and prints
 * a fitting message to the screen. If x is invalid, an error message printed, and the game mode
 * is not updated.
 *
 * @param:
 * com - the parsed command.
 * set - the setting to update.
 *
 */
void spSettingInvokeGameMode(SPCommand com, SPSetting* set);

/*
 * Sets the value of the difficulty when the user invokes a "difficulty x" command, and prints
 * a fitting message to the screen. If x is invalid, an error message printed, and the difficulty
 * is not updated.
 *
 * @param:
 * com - the parsed command.
 * set - the setting to update.
 *
 */
void spSettingInvokeDifficulty(SPCommand com, SPSetting* set);


/*
 * Sets the value of the user color when the user invokes a "user_color x" command, and prints
 * a fitting message to the screen. If x is invalid, an error message printed, and the user color
 * is not updated.
 *
 * @param:
 * com - the parsed command.
 * set - the setting to update.
 *
 */
void spSettingInvokeUserColor(SPCommand com, SPSetting* set);


#endif /* SPCHESSSETTINGS_H_ */
