/*
 * SPChessMainAux.h
 *
 *  Created on: 13 באוג 2017
 *      Author: ערי
 */

#ifndef SPCHESSMAINAUX_H_
#define SPCHESSMAINAUX_H_


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
SPCommand spMainAuxInitiateGame(SPSetting* setting, SPChessGame* game);

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
const char* spMainAuxToolToString(char tool);

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
char* spMainAuxMoveToString(CHMove* move);

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
void spMainAuxHandleUndoMove(SPChessGame* src, SPSetting* set);

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
bool spMainAuxHandlePostMove(SPChessGame* src, SPSetting* set);

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
SPCommand spMainAuxHandleCheckMateOrTie(SPChessGame* src, SPSetting* set);

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
SPCommand spMainAuxComputerTurn(SPChessGame* src, SPSetting* set);

/*
 * Prints to the screen the message: "x player - enter your move:", where x stands for white or black,
 * depending on the current player.
 *
 * @param:
 * color - can be 0 or 1, representing the black or the white player respectively.
 *
 */
void spMainAuxPrintEnterMove(int color);

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
bool spMainAuxHandlePlayersMove(SPChessGame* src, CHMove* move, SP_CHESS_GAME_MESSAGE message);

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
SPCommand spMainAuxPlayerTurn(SPChessGame* src, SPSetting* set);

/*
 * Begins the program and the game in console mode.
 * This function is called only from main.
 *
 */
void spChessConsoleMode();

#endif /* SPCHESSMAINAUX_H_ */
