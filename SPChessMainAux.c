/*
 * SPChessMainAux.c
 *
 *  Created on: 27 באוג 2017
 *      Author: ערי
 */

#ifndef SPCHESSMAINAUX_H_
#define SPCHESSMAINAUX_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "SPChessMainAux.h"
#include "SPChessGame.h"
#include "SPChessMiniMax.h"
#include "SPChessSettings.h"
#include "SPChessParser.h"
#include "SPChessMove.h"
#include "SPChessFile.h"



/*
 * Handels all the pre-game commands that the user can invoke before the game begins. the commands can be:
 * game_mode, difficulty, user_color, load, default, print_setting, quit and start.
 *
 * @param:
 * setting - an instant of SPSetting. 'setting' is initiated to the default setting and
 * 			 will be updated according to the users commands.
 * game - the source game.
 *
 * @return:
 * An instant of SPCommand - com. com.cmd might be:
 * SP_QUIT - if a quit command invoked.
 * SP_LOAD - if a load command invoked.
 * SP_START - if a start command invoked.
 */
SPCommand spMainAuxInitiateGame(SPSetting* setting, SPChessGame* game){
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
	char input[1025];
	SPCommand com;
	com.cmd = SP_INVALID_LINE;
	while (com.cmd != SP_QUIT && com.cmd != SP_LOAD && com.cmd != SP_START){
		fgets(input, 1024, stdin);
		com = spParserPreCommand(input);
		if (com.cmd == SP_GAME_MODE){
			spSettingInvokeGameMode(com, setting);
		}
		else if (com.cmd == SP_DIFFICULTY){
			spSettingInvokeDifficulty(com, setting);
		}
		else if (com.cmd == SP_USER_COLOR){
			spSettingInvokeUserColor(com, setting);
		}
		else if (com.cmd == SP_DEFAULT){
			spSettingResetToDefault(setting);
		}
		else if (com.cmd == SP_PRINT_SETTING){
			spSettingPrintSettings(setting);
		}
		else if (com.cmd == SP_INVALID_LINE){
			spSettingInvalidCommand();
		}
		else if (com.cmd == SP_LOAD){
			com.cmd = spFileHandleLoad(com.arg1, game, setting);
			///SP_CHESS_GAME_MESSAGE msg = spFileLoadGameFromFile(com.arg1, game, setting);
			///if (msg == SP_CHESS_GAME_FAILED){
				///printf("Error: File doesn't exist or cannot be opened\n");
				///com.cmd = SP_INVALID_LINE;
			///}
		}
	}
	return com;
}


/*
 * Gets a char that represents a chess tool and returns the name of the tool as a string.
 *
 * @param:
 * tool - a char that represents a chess tool.
 *
 * @return:
 * "pawn" - if tool=='M' or 'm'.
 * "bishop" - if tool=='B' or 'b'.
 * "knight" - if tool=='N' or 'n'.
 * "rook" - if tool=='R' or 'r'.
 * "queen" - if tool=='Q' or 'q'.
 * "king" - if tool=='K' or 'k'.
 *
 */
const char* spMainAuxToolToString(char tool){
	static char* tools[] = {"pawn", "bishop", "knight", "rook", "queen", "king"};
	if (tool == 'M' || tool == 'm'){
		return tools[0];
	}
	else if (tool =='B'  || tool =='b'){
		return tools[1];
	}
	else if (tool == 'N' || tool == 'n'){
		return tools[2];
	}
	else if (tool == 'R' || tool == 'r'){
		return tools[3];
	}
	else if (tool == 'Q' || tool == 'q'){
		return tools[4];
	}
	else if (tool == 'K' || tool == 'k'){
		return tools[5];
	}
	else{
		return 0;
	}
}



/*
 * Makes a string that represent a certain instant of CHMove in following way:
 * "<prevRow, prevCol> to <currRow, currCol>".
 *
 * @param:
 * move - the move to interpertate.
 *
 * @return:
 * A string of the form: "<prevRow, prevCol> to <currRow, currCol>".
 *
 */
char* spMainAuxMoveToString(CHMove* move){
	char prevRow = (char) ((8-move->prevRow) + 48);
	char prevCol = (char) (move->prevCol + 65);
	char currRow = (char) ((8-move->currRow) + 48);
	char currCol = (char) (move->currCol + 65);
	char moveStr[15] = {'<',prevRow,',',prevCol,'>',' ','t','o',' ','<',currRow,',',currCol,'>','\0'};
	char* moveS = (char*) malloc(15);
	strcpy(moveS, moveStr);
	return moveS;
}



/*
 * Handles an undo_move command execution by the user. If there is no moves in the moves history list,
 * so an error message will be printed. If the game mode is set to 2-players mode, other error message
 * will be printed. Otherwise, the previous move will undo.
 *
 * @param:
 * src - the current game played.
 * set - the setting of 'game'.
 *
 */
void spMainAuxHandleUndoMove(SPChessGame* src, SPSetting* set){
	if (set->gameMode == 1){
		SP_CHESS_GAME_MESSAGE message = spChessGameUndoPrevMove(src);
		if (message == SP_CHESS_GAME_NO_HISTORY){
			printf("Empty history, move cannot be undone\n");
		}
		else if (message == SP_CHESS_GAME_SUCCESS){
			spChessGameUndoPrevMove(src); //do it twice
		}
	}
	else {
		printf("Undo command not available in 2 players mode\n");
	}
}



/*
 * After an execution of a move, checks if the last move done lead to a check,
 * checkmate or a tie. If so, prints the right message to the screen.
 *
 * @param:
 * src - the current game played.
 *
 * @return:
 * true - if there is a tie, check or checkmate.
 * false - otherwise.
 *
 */
bool spMainAuxHandlePostMove(SPChessGame* src, SPSetting* set){
	SP_CHESS_GAME_MESSAGE msg = spCheckForCheck(src);
	if (msg == SP_CHESS_GAME_BLACK_CHECK){
		if (spChessGameCheckForCheckMateOnBlack(src) == SP_CHESS_GAME_CHECKMATE){
			printf("Checkmate! white player wins the game\n");
			return true;
		}
		else{
			if (set->gameMode == 1){
				if (set->userColor == src->currentPlayer){
					printf("Check!\n");
				}
				else{
					printf("Check: black King is threatened!\n");
				}
				return false;
			}
			else{
				printf("Check: black King is threatened!\n");
				return false;
			}
		}
	}
	else if (msg == SP_CHESS_GAME_WHITE_CHECK){
		if (spChessGameCheckForCheckMateOnWhite(src) == SP_CHESS_GAME_CHECKMATE){
			printf("Checkmate! black player wins the game\n");
			return true;
		}
		else{
			if (set->gameMode == 1){
				if (set->userColor == src->currentPlayer){
					printf("Check!\n");
				}
				else{
					printf("Check: white King is threatened!\n");
				}
				return false;
			}
			else{
				printf("Check: white King is threatened!\n");
				return false;
			}
		}
	}
	msg = spChessCheckforTie(src);
	if (msg == SP_CHESS_GAME_TIE){
		printf("The game is tied\n");
		return true;
	}
	return false;
}



/*
 * Handels the users commands after a tie or a checkmate done by the computer
 * in the game. In that case any 'move' command is illegal, and if it invoked so an error
 * message will be printed.
 *
 * @param:
 * src - the current game played.
 * set - the setting of 'game'.
 *
 * @return:
 * A constant of SPCommand - com. com.cmd can be one of the following:
 * SP_UNDO - if an undo_move command invoked by the user.
 * SP_QUIT - if a quit command invoked by the user.
 * SP_SAVE - if a save command invoked by the user.
 * SP_RESET - if a reset command invoked by the user.
 *
 */
SPCommand spMainAuxHandleCheckMateOrTie(SPChessGame* src, SPSetting* set){
	SPCommand com;
	com.cmd = SP_INVALID_LINE;
	char input[1025];
	while (com.cmd == SP_INVALID_LINE){
			fgets(input, 1024, stdin);
			com = spParserInCommand(input);
			if (com.cmd == SP_MOVE){
				com.cmd =SP_INVALID_LINE;
				printf("Illegal move\n");
			}
			else if (com.cmd == SP_UNDO){
				spMainAuxHandleUndoMove(src, set);
			}
	}
	return com;
}



/*
 * Executes a computer turn in the chess game. The move to do is calculated by minimax algorythem.
 * Afterwards the move is printed to the screen.
 *
 * @param:
 * src - the current game played.
 * set - the setting of 'src'.
 *
 * @return:
 * An instant of SPCommand - com. com.cmd can be one of the following:
 * SP_CHESS_GAME_TIE - if the move lead to a tie.
 * SP_CHESS_GAME_CHECKMATE - if the move lead to checkmate.
 * SP_MOVE - otherwise.
 *
 */
SPCommand spMainAuxComputerTurn(SPChessGame* src, SPSetting* set){
	CHMove* moveToDo = spMiniMaxSuggestMove(src);
	spChessGameSetMove(src, moveToDo);
	const char* toolStr = spMainAuxToolToString(moveToDo->tool);
	char* moveStr = spMainAuxMoveToString(moveToDo);
	printf("Computer: move %s at %s\n", toolStr, moveStr);
	free(moveStr);
	free(moveToDo);
	if (spMainAuxHandlePostMove(src, set)){
		return spMainAuxHandleCheckMateOrTie(src, set);
	}
	else{
		SPCommand com;
		com.cmd = SP_MOVE;
		com.validArg1 = false;
		com.validArg2 = false;
		return com;
	}
}



/*
 * Prints to the screen the message: "x player - enter your move:", where x stands for white or black,
 * depending on the current player.
 *
 * @param:
 * color - can be 0 or 1, representing the black or the white player respectively.
 *
 */
void spMainAuxPrintEnterMove(int color){
	if (color){
		printf("white player - enter your move:\n");
	}
	else{
		printf("black player - enter your move:\n");
	}
}



/*
 * Handles the move the user wants to execute in the game. If either the position on the board is not valid, the
 * origin position does not contain a user's piece, the move results with a self check
 * or if the move is not legal for the piece, a fitting error
 * message will be printed to the screen. Otherwise the move will be executed.
 *
 * @param:
 * src - the current game played.
 * move - the move the user wants to execute.
 * message - indicates if 'move' is legal or not.
 *
 * @return:
 * true - if the move is valid.
 * false - otherwise.
 *
 */
bool spMainAuxHandlePlayersMove(SPChessGame* src, CHMove* move, SP_CHESS_GAME_MESSAGE message){
	if (message == SP_CHESS_GAME_INVALID_POSITION){
		printf("Invalid position on the board\n");
		return false;
	}
	else if (message == SP_CHESS_GAME_POSITION_NOT_CONTAIN_PIECE){
		printf("The specified position does not contain your piece\n");
		return false;
	}
	else if (message == SP_CHESS_GAME_ILLEGAL_MOVE){
		printf("Illegal move\n");
		return false;
	}
	else{
		if (spChessCheckIfSelfCheck(src, move)){
			printf("Illegal move\n");
				return false;
		}
		else{
			spChessGameSetMove(src, move);
			return true;
		}
	}
}



/*
 * Manages a users turn. First the game board and enter move massege are printed to the screen,
 * and then the user enters his command - and the function handles each type of command differently.
 *
 * @param:
 * src - the current game played.
 * set - the setting of src.
 *
 * @return:
 * An instant of SPCommand - com. com.cmd might be:
 * SP_MOVE - if a legal move invoked by the user.
 * SP_GAME_FINISHED - if the move resulted with a tie or a checkmate.
 * SP_UNDO - if an undo command invoked by the user.
 * SP_reset - if a reset command invoked by the user.
 *
 */
SPCommand spMainAuxPlayerTurn(SPChessGame* src, SPSetting* set){
	spChessGamePrintBoard(src);
	spMainAuxPrintEnterMove(src->currentPlayer);
	SPCommand com;
	com.cmd = SP_INVALID_LINE;
	char input[1025];
	while (com.cmd == SP_INVALID_LINE){
		fgets(input, 1024, stdin);
		com = spParserInCommand(input);
		if (com.cmd == SP_MOVE){
			CHMove* move = spMoveGetMoveFromCommandArgs(com);
			SP_CHESS_GAME_MESSAGE message = spChessGameIsValidMove(src, move, src->currentPlayer);
			if (!spMainAuxHandlePlayersMove(src, move, message)){
				com.cmd = SP_INVALID_LINE;
				spChessDestroyMove(move);
				spMainAuxPrintEnterMove(src->currentPlayer);
			}
			else{
				if (spMainAuxHandlePostMove(src, set)){
					com.cmd = SP_GAME_FINISHED;
				}
				spChessDestroyMove(move);
			}
		}
		else if (com.cmd == SP_UNDO){
			spMainAuxHandleUndoMove(src, set);
		}
		else if (com.cmd == SP_SAVE){
			spFileSaveGameToFile(src, set, com.arg1);
			com.cmd = SP_INVALID_LINE;
		}
	}
	return com;
}



void spChessConsoleMode(){
	SPCommand superCommand;
	superCommand.cmd = SP_INVALID_LINE;
	SPSetting* setting = spSettingCreate();
	while (superCommand.cmd != SP_QUIT && superCommand.cmd != SP_GAME_FINISHED){
		SPChessGame* game = spChessGameCreate(6);
		superCommand = spMainAuxInitiateGame(setting, game);
		if (superCommand.cmd == SP_START){
			game->level = setting->difficulty;
		}
		while (superCommand.cmd != SP_QUIT && superCommand.cmd != SP_RESET && superCommand.cmd != SP_GAME_FINISHED){
			if(setting->gameMode == 1 && (setting->userColor != game->currentPlayer)){
				superCommand = spMainAuxComputerTurn(game, setting);
			}
			else{
				superCommand = spMainAuxPlayerTurn(game, setting);
			}
		}
		spChessGameDestroy(game);
		if (superCommand.cmd == SP_RESET){
			printf("Restarting...\n");
		}
	}
	if (superCommand.cmd == SP_QUIT){
		printf("Exiting...\n");
	}
	free(setting);
	return;

}

#endif /* SPCHESSMAINAUX_H_ */


