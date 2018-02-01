/*
 * SPChessSettings.c
 *
 *  Created on: 27 באוג 2017
 *      Author: ערי
 */


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "SPChessSettings.h"
#include "SPChessParser.h"

/*
 * A structure that contains inside it the setting of a specific chess game.
 * difficulty - can be set to 1,2,3 or 4.
 * gameMode - set to 1 if the game is a one-player game, or 2 if the it's a two-players game.
 * userColor - defined only if gameMode==1. Set to 1 if the user is the white player and the computer
 *             is the black player, or set to 0 otherwise.

typedef struct setting_t{
	int difficulty;
	int gameMode;
	int userColor;
} SPSetting;
*/


/*
 * Allocates memory for a new instant of SPSetting and creates it.
 *
 * @return:
 * setting that has the following features (default setting):
 * difficulty = 2, gameMode = 1 and userColor = 1.
 *
 */
SPSetting* spSettingCreate(){
	SPSetting* setting = (SPSetting*) malloc (sizeof(SPSetting));
	setting->difficulty = 2;
	setting->gameMode = 1;
	setting->userColor = 1;
	return setting;
}



/*
 * Updates the difficulty of certain setting.
 *
 * @param:
 * set - the setting to update.
 * difficulty - the value to update set.difficulty.
 *
 */
void spSettingSetDifficulty(SPSetting* set, int difficulty){
	set->difficulty = difficulty;
}



/*
 * Updates the gameMode of certain setting.
 *
 * @param:
 * set - the setting to update.
 * gameMode - the value to update set.gameMode.
 *
 */
void spSettingSetGameMode(SPSetting* set, int gameMode){
	set->gameMode = gameMode;
}



/*
 * Updates the userColor of certain setting.
 *
 * @param:
 * set - the setting to update.
 * color - the value to update set.userColor.
 *
 */
void spSettingSetUserColor(SPSetting* set, int color){
	set->userColor = color;
}



/*
 * Resets a certain setting to default values.
 *
 * @param:
 * set - the setting to update.
 *
 */
void spSettingResetToDefault(SPSetting* set){
	set->difficulty = 2;
	set->gameMode = 1;
	set->userColor = 1;
}



/*
 * Prints to the screen the setting of the game, after the user invokes "print_setting" command.
 *
 * @param:
 * set - the setting that are printed.
 *
 */
void spSettingPrintSettings(SPSetting* set){
	printf("SETTINGS:\n");
	if (set->gameMode==1){
		if(set->userColor==1){
			printf("GAME_MODE: 1\nDIFFICULTY_LVL: %d\nUSER_CLR: WHITE\n", (set->difficulty));
		}
		else{
			printf("GAME_MODE: 1\nDIFFICULTY_LVL: %d\nUSER_CLR: BLACK\n", (set->difficulty));
			}
	}
	else{
		printf("GAME_MODE: 2\n");
	}
}



/*
 * Prints an invalid command message to the screen when the user trys to set the setting to invalid values.
 *
 */
void spSettingInvalidCommand(){
	printf("Invalid Command!\n");
}



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
void spSettingInvokeGameMode(SPCommand com, SPSetting* set){
	com.validArg1 = true;
	if (com.argInt == 1){
		spSettingSetGameMode(set, com.argInt);
		printf("Game mode is set to 1 player\n");
	}
	else if (com.argInt == 2){
		spSettingSetGameMode(set, com.argInt);
		printf("Game mode is set to 2 players\n");
	}
	else{
		printf("Wrong game mode\n");
	}
}



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
void spSettingInvokeDifficulty(SPCommand com, SPSetting* set){
	if (set->gameMode == 2){
		spSettingInvalidCommand();
		return;
	}
	else if (com.argInt > 0 && com.argInt < 5){
		spSettingSetDifficulty(set, com.argInt);
		return;
	}
	else if (com.argInt == 5){
		printf("Expert level not supported, please choose a value between 1 to 4:\n");
		return;
	}
	else{
		printf("Wrong difficulty level. The value should be between 1 to 5\n");
	}
}



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
void spSettingInvokeUserColor(SPCommand com, SPSetting* set){
	if (set->gameMode == 2){
		spSettingInvalidCommand();
		return;
	}
	else if (com.argInt == 1 || com.argInt == 0){
		spSettingSetUserColor(set, com.argInt);
		return;
	}
	else {
		spSettingInvalidCommand();
	}
}

