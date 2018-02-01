/*
 * SPChessGame.c
 *
 *  Created on: 13 באוג 2017
 *      Author: ערי
 */
#ifndef SPCHESSGAME_H_
#define SPCHESSGAME_H_
#define NULL 0

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SPChessGame.h"
#include "SPArrayList.h"



//Definitions
#define SP_CHESS_DIM 8
#define SP_CHESS_BLACK_PLAYER 0
#define SP_CHESS_WHITE_PLAYER 1
#define SP_CHESS_TIE_SYMBOL '-'
#define SP_CHESS_EMPTY_ENTRY '_'


/*
 * A structure used to represent a certain chess game and it state.
 * gameBoard - A two dimensional array of chars that represents the current state of the game board. Upper case
 *             letters represents black tools, lower case letters represents white tool and '_' represents an empty entery.
 * currentPlayer - set to 1 if the white player needs to play next, otherwise set to 0.
 * historyMoves - An array of the 'historySize' previous moves done in the game.
 * historySize - The max size of historyMoves.
 * whiteKingRow - the row index of the white king.
 * whiteKingCol - the column index of the white king.
 * blackKingRow - the row index of the black king.
 * blackKingCol - the column index of the black king.
 * level - the difficulty level of the game.
 */
typedef struct sp_chess_game_t {
	char gameBoard[SP_CHESS_DIM][SP_CHESS_DIM];
	int currentPlayer;
	SPArrayList* historyMoves;
	int historySize;
	int whiteKingRow;
	int whiteKingCol;
	int blackKingRow;
	int blackKingCol;
	int level;
} SPChessGame;



/**
 * Type used for returning error codes from game functions
 */
typedef enum sp_chess_game_message_t {
	SP_CHESS_GAME_INVALID_POSITION,
	SP_CHESS_GAME_POSITION_NOT_CONTAIN_PIECE,
	SP_CHESS_GAME_ILLEGAL_MOVE,
	SP_CHESS_GAME_LEGAL_MOVE,
	SP_CHESS_GAME_INVALID_ARGUMENT,
	SP_CHESS_GAME_WHITE_CHECK,
	SP_CHESS_GAME_BLACK_CHECK,
	SP_CHESS_GAME_NO_CHECK,
	SP_CHESS_GAME_CHECKMATE,
	SP_CHESS_GAME_NO_CHECKMATE,
	SP_CHESS_GAME_TIE,
	SP_CHESS_GAME_NO_TIE,
	SP_CHESS_GAME_NO_HISTORY,
	SP_CHESS_GAME_SUCCESS,
	SP_CHESS_GAME_FAILED,
} SP_CHESS_GAME_MESSAGE;



/**
 * Creates a new game with a specified history size. The history size is a
 * parameter which specifies the number
 * of moves played so far. If the number of moves exceeds this parameter, then first moves stored will
 * be discarded in order for new moves to be stored.
 *
 * @param:
 * historySize - The total number of moves to undo,
 * a player can undo at most historySizeMoves turns.
 * @return
 * NULL if either a memory allocation failure occurs or historySize <= 0.
 * Otherwise, a new game instant is returned.
 */
SPChessGame* spChessGameCreate(int historySize){
	if(historySize<=0){
		return NULL;
	}
	SPChessGame* game = (SPChessGame*)malloc(sizeof(SPChessGame));
	game->historySize = historySize;
	game->currentPlayer = SP_CHESS_WHITE_PLAYER;
	for (int i =2 ; i<SP_CHESS_DIM-2; i++){
		for (int j=0 ; j<SP_CHESS_DIM; j++){
			game->gameBoard[i][j] = SP_CHESS_EMPTY_ENTRY;
		}
	}
	game->gameBoard[0][0] = 'R';
	game->gameBoard[0][7] = 'R';
	game->gameBoard[0][1] = 'N';
	game->gameBoard[0][6] = 'N';
	game->gameBoard[0][2] = 'B';
	game->gameBoard[0][5] = 'B';
	game->gameBoard[0][3] = 'Q';
	game->gameBoard[0][4] = 'K';
	for (int i=0 ; i<SP_CHESS_DIM; i++){
		game->gameBoard[1][i] = 'M';
		game->gameBoard[6][i] = 'm';
	}
	game->gameBoard[7][0] = 'r';
	game->gameBoard[7][7] = 'r';
	game->gameBoard[7][1] = 'n';
	game->gameBoard[7][6] = 'n';
	game->gameBoard[7][2] = 'b';
	game->gameBoard[7][5] = 'b';
	game->gameBoard[7][3] = 'q';
	game->gameBoard[7][4] = 'k';

	game->historyMoves = spArrayListCreate(historySize);
	game->blackKingCol = 4;
	game->blackKingRow = 0;
	game->whiteKingCol = 4;
	game->whiteKingRow = 7;
	return game;
}



/**
 * checks if src==NULL
 *	@param src - the source Game.
 *	@return
 *	true if src is NULL
 *	false otherwise
 */
bool assertGame(SPChessGame* src){
	if(src==NULL){
		return true;
	}
	return false;
}



/**
 *	Creates a copy of a given game.
 *	The new copy has the same status as the src game.
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 *
 */
SPChessGame* spChessGameCopy(SPChessGame* src){
	if (assertGame(src)){
		return NULL;
	}
	SPChessGame* copy = (SPChessGame*)malloc(sizeof(SPChessGame));
	copy->currentPlayer = src->currentPlayer;
	for( int i = 0 ; i<SP_CHESS_DIM; i++){
		for( int j = 0; j<SP_CHESS_DIM; j++){
			copy->gameBoard[i][j] = src->gameBoard[i][j];
		}
	}
	copy->historyMoves = spArrayListCopy(src->historyMoves);
	copy->historySize = src->historySize;
	copy->blackKingRow = src->blackKingRow;
	copy->blackKingCol = src->blackKingCol;
	copy->whiteKingRow = src->whiteKingRow;
	copy->whiteKingCol = src->whiteKingCol;
	copy->level = src->level;
	return copy;
}



/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
void spChessGameDestroy(SPChessGame* src){
	if (assertGame(src)){
		return;
	}
	spArrayListDestroy(src->historyMoves);
	free(src);
	return;
}



/**
 * Returns the current player of the specified game.
 * @param src - the source game
 * @return
 * SP_CHESS_BLACK_PLAYER - if it's black player's turn.
 * SP_CHESS_WHITE_PLAYER - if it's white player's turn.
 */
int spChessGameGetCurrentPlayer(SPChessGame* src){
	return src->currentPlayer;
}



/**
 * Returns the previous player of the specified game.
 * @param src - the source game
 * @return
 * SP_CHESS_BLACK_PLAYER - if it's black player's turn.
 * SP_CHESS_WHITE_PLAYER - if it's white player's turn.
 */
int spChessGameGetPrevPlayer (SPChessGame* src){
	if (spChessGameGetCurrentPlayer(src) == SP_CHESS_WHITE_PLAYER){
		return SP_CHESS_BLACK_PLAYER;
	}
	else{
		return SP_CHESS_WHITE_PLAYER;
	}
}



/*
 * Switches the player that should play on the next turn. If currentPlayer is SP_CHESS_WHITE_PLAYER, it
 * will change it to SP_CHESS_BLACK_PLAYER and vice versa.
 *
 * @param:
 * src - the source game to swich the turns in.
 */
void changePlayer(SPChessGame* src){
	if (spChessGameGetCurrentPlayer(src) == SP_CHESS_WHITE_PLAYER){
		src->currentPlayer = SP_CHESS_BLACK_PLAYER;
	}
	else {
		src->currentPlayer = SP_CHESS_WHITE_PLAYER;
	}
}



/**
 * Checks if a given move has valid indexes of rows and columns of the game board.
 *
 * @param:
 * move - the move to check the validness of its indexes in the board.
 * @return
 * true  - if the indexes of the current and previous rows and columns are between 0 and 7.
 * false - otherwise.
 *
 */
bool spCheckValidLocation(CHMove* move){
	if (move->currCol>7 || move->currCol<0){
		return false;
	}
	if (move->currRow>7 || move->currRow<0){
		return false;
	}
	if (move->prevCol>7 || move->prevCol<0){
		return false;
	}
	if (move->prevRow>7 || move->prevRow<0){
		return false;
	}
	return  true;
}



/*
 * Checks if a certain position on the board contains a black piece.
 *
 * @param:
 * src - the source game.
 * row - the row index of the position.
 * col - the column index of the position.
 *
 * @return:
 * true - if there is a black piece in this position on the board.
 * false - otherwise.
 */
bool spCheckBlackPieceInPos(SPChessGame* src, int row, int col){
	char piece = src->gameBoard[row][col];
	if (piece >= 'A' && piece <= 'Z'){
		return true;
	}
	return false;
}



bool spCheckWhitePieceInPos(SPChessGame* src, int row, int col){
	char piece = src->gameBoard[row][col];
	if (piece >= 'a' && piece <= 'z'){
		return true;
	}
	return false;
}



/*
 * Checks if a certain position on the board contains a tool with a certain color.
 *
 * @param:
 * src - the source game.
 * move - an instant of CHMove, the row of the position to check is the prevRow of move,
 * and the column of the position to check is the prevCol of move.
 * color - 0 or 1. If 0, so it checks if there is a black tool in the position, if 1, checks if there
 * is a white tool in the position.
 *
 * @return:
 * true - if there is a tool (white or black according to 'color') in this position on the board.
 * false - otherwise.
 */
bool spCheckPieceInPosition(SPChessGame* src, CHMove* move, int color){
	if(color){
		return spCheckWhitePieceInPos(src, move->prevRow, move->prevCol);
	}
	else{
		return spCheckBlackPieceInPos(src, move->prevRow, move->prevCol);
	}
}



/*
 * Checks if a certain position on the board is empty ('_').
 *
 * @param:
 * src - the source game.
 * row - the row index of the position.
 * col - the column index of the position.
 *
 * @return:
 * true - if there is no tool in this position on the board.
 * false - otherwise.
 */
bool spCheckEmptyPos(SPChessGame* src, int row, int col){
	if (src->gameBoard[row][col]=='_'){
		return true;
	}
	return false;
}



/*
 * Checks if a given CHMove of white pawn is valid. The move is valid iff it is a single move forward in the same
 * column, two moves forward if the pawn located at its starting position or one diagonal step forward if the
 * pawn captures a black tool.
 *
 * @param:
 * src - the source game.
 * move - the move to check its validness.
 *
 * @return:
 * true - if 'move' is a valid white pawn move.
 * false - otherwise or if src==NULL or move==NULL.
 */
bool spValidpMove (SPChessGame* src, CHMove* move){
	if (assertGame(src) || assertMove(move)){
		return false;
	}
	if (move->prevRow - move->currRow != 1){
		if (move->prevRow == 6){  //starting position
			if (move->prevRow - move->currRow == 2){
				if (move->currCol != move->prevCol){
					return false;
				}
				if (!(spCheckEmptyPos(src, move->currRow, move->currCol))){
					return false;
				}
				if (!spCheckEmptyPos(src, move->currRow+1, move->currCol)){
					return false;
				}
				return true;
			}
		}
		return false;
	}
	else{ //diagonal move
		if (move->currCol == move->prevCol){
			if(spCheckEmptyPos(src, move->currRow, move->currCol)){
				return true;
			}
		}
		else if (move->prevCol + 1 == move->currCol || move->prevCol - 1 == move->currCol){
			if (spCheckBlackPieceInPos(src, move->currRow, move->currCol)){
				return true;
			}
		}
		return false;
	}
}



bool spValidPMove (SPChessGame* src, CHMove* move){
	if (assertGame(src) || assertMove(move)){
		return false;
	}
	if (move->currRow - move->prevRow  != 1){
		if (move->prevRow == 1){ //starting position
			if (move->currRow - move->prevRow == 2){
				if (move->currCol != move->prevCol){
					return false;
				}
				if (!(spCheckEmptyPos(src, move->currRow, move->currCol))){
					return false;
				}
				if (!spCheckEmptyPos(src, move->currRow-1, move->currCol)){
					return false;
				}
				return true;
			}
		}
		return false;
	}
	else{
		if (move->currCol == move->prevCol){
			if(spCheckEmptyPos(src, move->currRow, move->currCol)){
				return true;
			}
		}
		else if (move->prevCol + 1 == move->currCol || move->prevCol - 1 == move->currCol){
			if (spCheckWhitePieceInPos(src, move->currRow, move->currCol)){
				return true;
			}
		}
		return false;
	}
}




bool spValidRrMove(SPChessGame* src, CHMove* move, int color){
	if (assertGame(src) || assertMove(move)){
		return false;
	}
	int colDiff = move->currCol - move->prevCol;
	int rowDiff = move->currRow - move->prevRow;
	if ((colDiff != 0 && rowDiff != 0) || rowDiff==colDiff){
		return false;
	}
	else if (colDiff == 0){
		for (int i=1; i < abs(rowDiff); i++){
			if (rowDiff < 0){
				if(!spCheckEmptyPos(src, move->prevRow-i, move->prevCol)){
					return false;
				}
			}
			else{
				if(!spCheckEmptyPos(src, move->prevRow+i, move->prevCol)){
					return false;
				}
			}
		}
	}
	else{
		for (int i=1; i < abs(colDiff); i++){
			if (colDiff < 0){
				if(!spCheckEmptyPos(src, move->prevRow, move->prevCol-i)){
					return false;
				}
			}
			else{
				if(!spCheckEmptyPos(src, move->prevRow, move->prevCol+i)){
					return false;
				}
			}
		}
	}
	if (color){
		if (spCheckWhitePieceInPos(src, move->currRow, move->currCol)){
			return false;
		}
	}
	else{
		if (spCheckBlackPieceInPos(src, move->currRow, move->currCol)){
			return false;
		}
	}
	return true;
}



bool spValidNnMove(SPChessGame* src, CHMove* move, int color){
	if (assertGame(src) || assertMove(move)){
		return false;
	}
	int colDiff = abs(move->currCol - move->prevCol);
	int rowDiff = abs(move->currRow - move->prevRow);
	if ((colDiff == 1 && rowDiff == 2) || (colDiff == 2 && rowDiff == 1)){
		if (color){
			if(!spCheckWhitePieceInPos(src, move->currRow, move->currCol)){
				return true;
			}
		}
		else{
			if(!spCheckBlackPieceInPos(src, move->currRow, move->currCol)){
				return true;
			}
		}
	}
	return false;
}



bool spValidBbmove(SPChessGame* src, CHMove* move, int color){
	if (assertGame(src) || assertMove(move)){
		return false;
	}
	int colDiff = move->currCol - move->prevCol;
	int rowDiff = move->currRow - move->prevRow;
	if ((abs(colDiff) != abs(rowDiff)) || colDiff == 0){
		return false;
	}
	else{
		for (int i = 1; i<abs(colDiff); i++){
			if (colDiff > 0 && rowDiff > 0){
				if(!spCheckEmptyPos(src, move->prevRow+i, move->prevCol+i)){
					return false;
				}
			}
			else if (colDiff > 0 && rowDiff < 0){
				if(!spCheckEmptyPos(src, move->prevRow-i, move->prevCol+i)){
					return false;
				}
			}
			else if (colDiff < 0 && rowDiff < 0){
				if(!spCheckEmptyPos(src, move->prevRow-i, move->prevCol-i)){
					return false;
				}
			}
			else if (colDiff < 0 && rowDiff > 0){
				if(!spCheckEmptyPos(src, move->prevRow+i, move->prevCol-i)){
					return false;
				}
			}
		}
		if (color){
			if (spCheckWhitePieceInPos(src, move->currRow, move->currCol)){
				return false;
			}
		}
		else{
			if (spCheckBlackPieceInPos(src,move->currRow, move->currCol)){
				return false;
			}
		}
		return true;
	}
}



bool spValidQqMove(SPChessGame* src, CHMove* move, int color){
	if (assertGame(src) || assertMove(move)){
		return false;
	}
	if (spValidBbmove(src, move, color) || spValidRrMove(src, move, color)){
		return true;
	}
	return  false;
}



bool spValidKkMove(SPChessGame* src, CHMove* move, int color){
	if (assertGame(src) || assertMove(move)){
		return false;
	}
	int colDiff = abs(move->currCol - move->prevCol);
	int rowDiff = abs(move->currRow - move->prevRow);
	if (colDiff == 0 && rowDiff == 0){
		return false;
	}
	else if (colDiff > 1 || rowDiff > 1){
		return false;
	}
	else{
		if (color){
			if(spCheckWhitePieceInPos(src, move->currRow, move->currCol)){
				return false;
			}
		}
		else{
			if(spCheckBlackPieceInPos(src, move->currRow, move->currCol)){
				return false;
			}
		}
	}
	return true;
}



/*
 * Checks if a certain move of a certain player (black or white) is valid according to the tool
 * of the instant of CHMove.
 *
 * @param:
 * src - the source game.
 * move - the move to check its validness.
 * color - the color of the tool of 'move'.
 *
 * @return:
 * true - if the move is valid.
 * false - otherwise.
 */
bool spChessGameCheckByTool(SPChessGame* src, CHMove* move, int color){
	char tool = move->tool;
	if (tool=='m'){
		return spValidpMove(src, move);
	}
	if (tool=='M'){
		return spValidPMove(src, move);
	}
	if (tool=='b' || tool=='B'){
		return spValidBbmove(src, move, color);
	}
	if (tool=='r' || tool=='R'){
		return spValidRrMove(src, move, color);
	}
	if (tool=='n' || tool=='N'){
		return spValidNnMove(src, move, color);
	}
	if (tool=='q' || tool=='Q'){
		return spValidQqMove(src, move, color);
	}
	else{
		return spValidKkMove(src, move, color);
	}
}



/*
 * Checks if a certain CHMove is legel and valid. A move ( from <a,b> to <x,y>) is legel and valid if the following conditions
 * takes place: 1. The indexes are legal- inside the game board ( 1<=a,x<=8 and A<=b,y<=H ).
 * 2. There is a tool of color 'color' located at <a,b>.
 * 3. The move is valid for the tool of 'move'.
 *
 * @param:
 * src - the source game.
 * move - the move to check if it is legal and valid.
 * color - the color of the tool that moves on the board.
 *
 * @return:
 * false - if src==NULL or move==NULL.
 * SP_CHESS_GAME_INVALID_POSITION - if the index of prevCol or prevRow or currCol or currRow (a,b,x,y)
 * is illegal.
 * SP_CHESS_GAME_POSITION_NOT_CONTAIN_PIECE - if the position <prevRow, prevCol> does not contain a piece
 * of color 'color'.
 * SP_CHESS_ILLEGAL_MOVE - if the movement <prevRow, prevCol> -> <currRow, currCol> is not legel for the
 * tool of 'move'.
 * SP_CHESS_GAME_LEGAL_MOVE - if the move is legal and valid.
 */
SP_CHESS_GAME_MESSAGE spChessGameIsValidMove(SPChessGame* src, CHMove* move, int color){
	if (assertGame(src) || assertMove(move)){
		return false;
	}
	if (!(spCheckValidLocation(move))){
		return SP_CHESS_GAME_INVALID_POSITION;
	}
	if (!(spCheckPieceInPosition(src, move, color))){
		return SP_CHESS_GAME_POSITION_NOT_CONTAIN_PIECE;
	}
	move->tool = src->gameBoard[move->prevRow][move->prevCol];
	if (!(spChessGameCheckByTool(src, move, color))){
		return SP_CHESS_GAME_ILLEGAL_MOVE;
	}
	return SP_CHESS_GAME_LEGAL_MOVE;
}



/*
 * Checks if in the current state of the game board there is a check on the white king.
 * It is done by going over on all the possible moves of the white player and checking if at least one
 * of them results with avoiding the check.
 *
 * @param:
 * src - the source game.
 *
 * @return:
 * SP_CHESS_GAME_WHITE_CHECK - if there is a check on the white king.
 * SP_CHESS_GAME_NO_CHECK - otherwise.
 */
SP_CHESS_GAME_MESSAGE spChessCheckForCheckOnWhite(SPChessGame* src){
	int kingsRow = src->whiteKingRow;
	int kingsCol = src->whiteKingCol;
	for (int i=0 ; i<8 ; i++){
		for (int j=0 ; j<8 ; j++){
			if (spCheckBlackPieceInPos(src, i, j)){
				CHMove newMove = spChessCreateMove(src->gameBoard[i][j], kingsRow, kingsCol, i, j);
				if (spChessGameIsValidMove(src, &newMove, SP_CHESS_BLACK_PLAYER) == SP_CHESS_GAME_LEGAL_MOVE){
					return SP_CHESS_GAME_WHITE_CHECK;
				}
			}
		}
	}
	return SP_CHESS_GAME_NO_CHECK;
}



SP_CHESS_GAME_MESSAGE spChessCheckForCheckOnBlack(SPChessGame* src){
	int kingsRow = src->blackKingRow;
	int kingsCol = src->blackKingCol;
	for (int i=0 ; i<8 ; i++){
		for (int j=0 ; j<8 ; j++){
			if (spCheckWhitePieceInPos(src, i, j)){
				CHMove newMove = spChessCreateMove(src->gameBoard[i][j], kingsRow, kingsCol, i, j);
				if (spChessGameIsValidMove(src, &newMove, SP_CHESS_WHITE_PLAYER)== SP_CHESS_GAME_LEGAL_MOVE){
					return SP_CHESS_GAME_BLACK_CHECK;
				}
			}
		}
	}
	return SP_CHESS_GAME_NO_CHECK;
}



/*
 * Checks if in the current state of the game board there is a check on the white or black king.
 * It is done by going over on all the possible moves of the white and black players and checking if at least one
 * of them results with avoiding the check.
 *
 * @param:
 * src - the source game.
 *
 * @return:
 * SP_CHESS_GAME_INVALID_ARGUMENT - if src==NULL.
 * SP_CHESS_GAME_WHITE_CHECK - if there is a check on the white king.
 * SP_CHESS_GAME_BLACK_CHECK - if there is a check on the black king.
 * SP_CHESS_GAME_NO_CHECK - otherwise.
 */
SP_CHESS_GAME_MESSAGE spCheckForCheck(SPChessGame* src){
	if (assertGame(src)){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	else if (spChessGameGetCurrentPlayer(src)){
		return spChessCheckForCheckOnWhite(src);
	}
	else{
		return spChessCheckForCheckOnBlack(src);
	}
}



/**
 * Sets the next move in a given game according to the instant of CHMove. It moves the
 * piece located at <prevRow, prevCol> to the position <currRow, currCol>.
 *
 * @param:
 * src - The target game.
 * move - the move to execute.
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if src==NULL or move==NULL.
 * SP_CHESS_GAME_SUCCESS - otherwise.
 */
SP_CHESS_GAME_MESSAGE spChessGameSetMove(SPChessGame* src, CHMove* move){
	if (assertGame(src) || assertMove(move)){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	move->tool = src->gameBoard[move->prevRow][move->prevCol];
	move->captured = src->gameBoard[move->currRow][move->currCol];
	if(move->captured != '_'){
		move->didCapture = true;
	}
	char tool = src->gameBoard[move->prevRow][move->prevCol];
	src->gameBoard[move->prevRow][move->prevCol] = SP_CHESS_EMPTY_ENTRY;
	src->gameBoard[move->currRow][move->currCol] = tool;
	if (tool == 'k'){
		src->whiteKingRow = move->currRow;
		src->whiteKingCol = move->currCol;
	}
	if (tool == 'K'){
		src->blackKingRow = move->currRow;
		src->blackKingCol = move->currCol;
	}
	if (spArrayListIsFull(src->historyMoves)){
		spArrayListRemoveFirst(src->historyMoves);
	}
	spArrayListAddLast(src->historyMoves, move);

	changePlayer(src);

	return SP_CHESS_GAME_SUCCESS;
}



/*
 * Checks if a certain move of a white or black player, will result with a self check
 * (and in that case will be an illegal move).
 *
 * @param:
 * src - the current game played.
 * move - the move to check.
 *
 * @return:
 * true - if the move results with a self check.
 * false - otherwise.
 */
bool spChessCheckIfSelfCheck(SPChessGame* src, CHMove* move){
	SPChessGame* gameCopy = spChessGameCopy(src);
	spChessGameSetMove(gameCopy, move);
	bool flag = false;
	if (spChessGameGetPrevPlayer(gameCopy)){
		if (spChessCheckForCheckOnWhite(gameCopy) == SP_CHESS_GAME_WHITE_CHECK){
			flag = true;
		}
	}
	else{
		if (spChessCheckForCheckOnBlack(gameCopy) == SP_CHESS_GAME_BLACK_CHECK){
			flag = true;
		}
	}
	spChessGameDestroy(gameCopy);
	return flag;
}



/**
 * Calculates and returns a list of all possible (game depended validity) moves for a
 * certain black pawn.
 *
 * @param:
 * src - the current game played.
 * row - the row index of the pawn's position.
 * col - the col index of the pawn's position.
 *
 * @return:
 * NULL - if src==NULL.
 * A pointer to the first argument in the list.
 */
SPArrayList* spChessGetPawnPossibleMoves(SPChessGame* src, int row, int col){
	if (assertGame(src)){
		return NULL;
	}
	SPArrayList* pawnList = spArrayListCreate(4);
	CHMove newMove = spChessCreateMove('M', row+1, col, row, col);
	CHMove* move = &newMove;
	if (spChessGameIsValidMove(src, move, SP_CHESS_BLACK_PLAYER) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(pawnList, move);
	}
	move->currRow = row+2;
	if (spChessGameIsValidMove(src, move, SP_CHESS_BLACK_PLAYER) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(pawnList, move);
	}
	move->currRow = row+1;
	move->currCol = col+1;
	if (spChessGameIsValidMove(src, move, SP_CHESS_BLACK_PLAYER) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(pawnList, move);
	}
	move->currCol = col-1;
	if (spChessGameIsValidMove(src, move, SP_CHESS_BLACK_PLAYER) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(pawnList, move);
	}
	return pawnList;
}



SPArrayList* spChessGetpawnPossibleMoves(SPChessGame* src, int row, int col){
	if (assertGame(src)){
		return NULL;
	}
	SPArrayList* pawnList = spArrayListCreate(4);
	CHMove newMove = spChessCreateMove('m', row-1, col, row, col);
	CHMove* move = &newMove;
	if (spChessGameIsValidMove(src, move, SP_CHESS_WHITE_PLAYER) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(pawnList, move);
	}
	move->currRow = row-2;
	if (spChessGameIsValidMove(src, move, SP_CHESS_WHITE_PLAYER) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(pawnList, move);
	}
	move->currRow = row-1;
	move->currCol = col+1;
	if (spChessGameIsValidMove(src, move, SP_CHESS_WHITE_PLAYER) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(pawnList, move);
	}
	move->currCol = col-1;
	if (spChessGameIsValidMove(src, move, SP_CHESS_WHITE_PLAYER) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(pawnList, move);
	}
	return pawnList;
}



SPArrayList* spChessGetBishopPossibleMoves(SPChessGame* src, int row, int col){
	if (assertGame(src)){
		return NULL;
	}
	char colorBish = src->gameBoard[row][col];
	SPArrayList* bishopList = spArrayListCreate(13);
	CHMove newMove = spChessCreateMove(colorBish, row, col, row, col);
	CHMove *move = &newMove;
	for (int i=1; i<8; i++){
		move->currRow = row+i;
		move->currCol = col+i;
		if (spChessGameIsValidMove(src, move, src->currentPlayer) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
			spArrayListAddFirst(bishopList, move);
		}
		move->currCol = col-i;
		if (spChessGameIsValidMove(src, move, src->currentPlayer) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
			spArrayListAddFirst(bishopList, move);
		}
		move->currRow = row-i;
		if (spChessGameIsValidMove(src, move, src->currentPlayer) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
			spArrayListAddFirst(bishopList, move);
		}
		move->currCol = col+i;
		if (spChessGameIsValidMove(src, move, src->currentPlayer) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
			spArrayListAddFirst(bishopList, move);
		}
	}
	return bishopList;
}



SPArrayList* spChessGetRookPossibleMoves(SPChessGame* src, int row, int col){
	if (assertGame(src)){
		return NULL;
	}
	int color = src->currentPlayer;
	char colorRook = src->gameBoard[row][col];
	SPArrayList* rookList = spArrayListCreate(14);
	CHMove newMove = spChessCreateMove(colorRook, row, col, row, col);
	CHMove* move = &newMove;
	for (int i=1; i<8; i++){
		move->currCol = col;
		move->currRow = row+i;
		if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
			spArrayListAddFirst(rookList, move);
		}
		move->currRow = row-i;
		if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
			spArrayListAddFirst(rookList, move);
		}
		move->currRow = row;
		move->currCol = col+i;
		if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
			spArrayListAddFirst(rookList, move);
		}
		move->currCol = col-i;
		if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
			spArrayListAddFirst(rookList, move);
		}
	}
	return rookList;
}



SPArrayList* spChessGetKnightPossibleMoves(SPChessGame* src, int row, int col){
	if (assertGame(src)){
		return NULL;
	}
	int color = src->currentPlayer;
	char colorKnight = src->gameBoard[row][col];
	SPArrayList* knightList = spArrayListCreate(8);
	CHMove newMove = spChessCreateMove(colorKnight, row, col, row, col);
	CHMove* move = &newMove;
	move->currRow = row+2;
	move->currCol = col+1;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(knightList, move);
	}
	move->currCol = col-1;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(knightList, move);
	}
	move->currRow = row-2;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(knightList, move);
	}
	move->currCol = col+1;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(knightList, move);
	}
	move->currRow = row+1;
	move->currCol = col+2;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(knightList, move);
	}
	move->currCol = col-2;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(knightList, move);
	}
	move->currRow = row-1;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(knightList, move);
	}
	move->currCol = col+2;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(knightList, move);
	}
	return knightList;
}



SPArrayList* spChessGetQueenPossibleMoves(SPChessGame* src, int row, int col){
	if (assertGame(src)){
		return NULL;
	}
	SPArrayList* queenList = spArrayListCreate(27);
	SPArrayList* bishopList = spChessGetBishopPossibleMoves(src, row, col);
	SPArrayList* rookList = spChessGetRookPossibleMoves(src, row, col);
	for (int i=0 ; i<bishopList->actualSize ; i++){
		spArrayListAddFirst(queenList, spArrayListGetAt(bishopList, i));
	}
	for (int i=0 ; i<rookList->actualSize ; i++){
		spArrayListAddFirst(queenList, spArrayListGetAt(rookList, i));
	}
	spArrayListDestroy(bishopList);
	spArrayListDestroy(rookList);
	return queenList;
}



SPArrayList* spChessGetKingPossibleMoves(SPChessGame* src, int row, int col){
	if (assertGame(src)){
		return NULL;
	}
	int color = src->currentPlayer;
	SPArrayList* kingList = spArrayListCreate(8);
	char colorKing = src->gameBoard[row][col];
	CHMove newMove = spChessCreateMove(colorKing, row, col, row, col);
	CHMove* move = &newMove;
	move->currRow = row+1;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(kingList, move);
	}
	move->currCol = col+1;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(kingList, move);
	}
	move->currCol = col-1;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(kingList, move);
	}
	move->currRow = row;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(kingList, move);
	}
	move->currCol = col+1;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(kingList, move);
	}
	move->currRow = row-1;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(kingList, move);
	}
	move->currCol = col;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(kingList, move);
	}
	move->currCol = col-1;
	if (spChessGameIsValidMove(src, move, color) == SP_CHESS_GAME_LEGAL_MOVE && !(spChessCheckIfSelfCheck(src, move))){
		spArrayListAddFirst(kingList, move);
	}
	return kingList;
}



/**
 * Calculates and returns a list of all possible (game depended validity) moves for a
 * certain piece.
 *
 * @param:
 * src - the current game played.
 * row - the row index of the piece's position.
 * col - the col index of the piece's position.
 *
 * @return:
 * NULL - if src==NULL.
 * A pointer to the first argument in the list otherwise.
 */
SPArrayList* spChessGetMovesListForPiece(SPChessGame* src ,char tool, int row, int col){
	if (assertGame(src)){
		return NULL;
	}
	else if (tool == 'm'){
		return spChessGetpawnPossibleMoves(src, row, col);
	}
	else if (tool == 'M'){
		return spChessGetPawnPossibleMoves(src, row, col);
	}
	else if (tool == 'b' || tool == 'B'){
		return spChessGetBishopPossibleMoves(src, row, col);
	}
	else if (tool == 'r' || tool == 'R'){
		return spChessGetRookPossibleMoves(src, row, col);
	}
	else if (tool == 'n' || tool == 'N'){
		return spChessGetKnightPossibleMoves(src, row, col);
	}
	else if (tool == 'q' || tool == 'Q'){
		return spChessGetQueenPossibleMoves(src, row, col);
	}
	else {
		return spChessGetKingPossibleMoves(src, row, col);
	}
}



/*
 * Creates a list of all the possible moves for the player that needs to play the next turn.
 * It is done by concatenating between all the lists of the possible moves for each white or black piece
 * (depends on the current player) on the game board.
 *
 * @param:
 * src - the current game played.
 *
 * @return:
 * NULL - if src==NULL.
 * A pointer to the first argument in the list otherwise.
 */
SPArrayList* spChessGameGetAllPossibleMoves(SPChessGame* src){
	if (assertGame(src)){
			return NULL;
	}
	SPArrayList* movesList = spArrayListCreate(137);
	for (int i=0; i<8; i++){
		for (int j=0; j<8; j++){
			char tool = src->gameBoard[i][j];
			if(src->currentPlayer){
				if(spCheckWhitePieceInPos(src, i, j)){
					SPArrayList* pieceList = spChessGetMovesListForPiece(src, tool, i, j);
					SPArrayListCopyIntoList(movesList, pieceList);
					spArrayListDestroy(pieceList);
				}
			}
			else if (spCheckBlackPieceInPos(src, i, j)){
				SPArrayList* pieceList = spChessGetMovesListForPiece(src, tool, i, j);
				SPArrayListCopyIntoList(movesList, pieceList);
				spArrayListDestroy(pieceList);
			}
		}
	}
	return movesList;
}



/*
 * Checks if in the current state of the game board (check on the white king)
 * there is also a checkmate on the white king.
 * This is done by going over all the possible moves for the white player, if there is at least one possible
 * legal move then there is no checkmate, otherwise it's a checkmate.
 *
 * @param:
 * src - the current game played.
 *
 * @return:
 * NULL - is src==NULL.
 * SP_CHESS_GAME_NO_CHECKMATE - if there is no checkmate.
 * SP_CHESS_GAME_CHECKMATE - if there checkmate.
 */
SP_CHESS_GAME_MESSAGE spChessGameCheckForCheckMateOnWhite(SPChessGame* src){
	//if (assertGame(src)){
		//return NULL;
	//}
	for (int i=0 ; i<8 ; i++){
		for (int j=0 ; j<8 ; j++){
			if (spCheckWhitePieceInPos(src, i ,j)){
				char tool = src->gameBoard[i][j];
				SPArrayList* legalMoves = spChessGetMovesListForPiece(src, tool, i, j);
				for (int k=0; k<legalMoves->actualSize; k++){
					SPChessGame* gameCopy = spChessGameCopy(src);
					CHMove* move = spArrayListGetAt(legalMoves, k);
					spChessGameSetMove(gameCopy, move);
					if (spChessCheckForCheckOnWhite(gameCopy) == SP_CHESS_GAME_NO_CHECK){
						spChessGameDestroy(gameCopy);
						spArrayListDestroy(legalMoves);
						return SP_CHESS_GAME_NO_CHECKMATE;
					}
					spChessGameDestroy(gameCopy);
				}
				spArrayListDestroy(legalMoves);
			}
		}
	}
	return SP_CHESS_GAME_CHECKMATE;
}


SP_CHESS_GAME_MESSAGE spChessGameCheckForCheckMateOnBlack(SPChessGame* src){
	//if (assertGame(src)){
	//	return NULL;
	//}
	for (int i=0 ; i<8 ; i++){
		for (int j=0 ; j<8 ; j++){
			if (spCheckBlackPieceInPos(src, i ,j)){
				char tool = src->gameBoard[i][j];
				SPArrayList* legalMoves = spChessGetMovesListForPiece(src, tool, i, j);
				for (int k=0; k<legalMoves->actualSize; k++){
					SPChessGame* gameCopy = spChessGameCopy(src);
					CHMove* move = spArrayListGetAt(legalMoves, k);
					spChessGameSetMove(gameCopy, move);
					if (spChessCheckForCheckOnBlack(gameCopy) == SP_CHESS_GAME_NO_CHECK){
						spChessGameDestroy(gameCopy);
						spArrayListDestroy(legalMoves);
						return SP_CHESS_GAME_NO_CHECKMATE;
					}
					spChessGameDestroy(gameCopy);
				}
				spArrayListDestroy(legalMoves);
			}
		}
	}
	return SP_CHESS_GAME_CHECKMATE;
}



/*
 * Checks if there is a tie when it is the white player's turn.
 * This is done by going over on all the possible moves for each white tool on the game board.
 * If at least one legal move found so there is no tie, otherwise it's a tie.
 *
 * @param:
 * src - the current game played.
 *
 * @return:
 * NULL - if src==NULL.
 * SP_CHESS_GAME_NO_TIE - if there is no tie.
 * SP_CHESS_GAME_TIE - if it is a tie.
 */
SP_CHESS_GAME_MESSAGE spChessCheckforTieWhite(SPChessGame* src){
	//if (assertGame(src)){
	//	return NULL;
	//}
	for (int i=0; i<8; i++){
		for (int j=0; j<8; j++){
			if (spCheckWhitePieceInPos(src, i, j)){
				char tool = src->gameBoard[i][j];
				SPArrayList* legalMoves = spChessGetMovesListForPiece(src, tool, i, j);
				if (legalMoves->actualSize>0){
					spArrayListDestroy(legalMoves);
					return SP_CHESS_GAME_NO_TIE;
				}
				spArrayListDestroy(legalMoves);
			}
		}
	}
	return SP_CHESS_GAME_TIE;
}



SP_CHESS_GAME_MESSAGE spChessCheckforTieBlack(SPChessGame* src){
	//if (assertGame(src)){
	//	return NULL;
	//}
	for (int i=0; i<8; i++){
		for (int j=0; j<8; j++){
			if (spCheckBlackPieceInPos(src, i, j)){
				char tool = src->gameBoard[i][j];
				SPArrayList* legalMoves = spChessGetMovesListForPiece(src, tool, i, j);
				if (legalMoves->actualSize>0){
					spArrayListDestroy(legalMoves);
					return SP_CHESS_GAME_NO_TIE;
				}
				spArrayListDestroy(legalMoves);
			}
		}
	}
	return SP_CHESS_GAME_TIE;
}


/*
 * Checks if there is a tie in the current game state.
 *
 * @param:
 * src - the current game played.
 *
 * @return:
 * NULL - if src==NULL.
 * SP_CHESS_GAME_NO_TIE - if there is no tie.
 * SP_CHESS_GAME_TIE - if it is a tie.
 *
 */
SP_CHESS_GAME_MESSAGE spChessCheckforTie(SPChessGame* src){
	if (spChessGameGetCurrentPlayer(src)){
		return spChessCheckforTieWhite(src);
	}
	else{
		return spChessCheckforTieBlack(src);
	}
}



/*
 * Prints the last move to undo in the format:
 * "Undo move for player white/black : <a,b> -> <x,y>".
 *
 * @param:
 * src - the current game played.
 * move - the move that needs to be undone.
 *
 */
void spGamePrintUndoMove(SPChessGame* src, CHMove* move){
	char prevRow = (char) ((8-move->prevRow) + 48);
	char prevCol = (char) (move->prevCol + 65);
	char currRow = (char) ((8-move->currRow) + 48);
	char currCol = (char) (move->currCol + 65);
	char moveStr[15] = {'<',currRow,',',currCol,'>',' ','-','>',' ','<',prevRow,',',prevCol,'>','\0'};
	if (spChessGameGetPrevPlayer(src)){
		printf("Undo move for player white : %s\n", moveStr);
	}
	else{
		printf("Undo move for player black : %s\n", moveStr);
	}
}



/**
 * Removes the tool that was moved in the previous move ,returns it to its
 * previous position and changes the current
 * player's turn. If the user invoked this command more than historySize times
 * in a row then an error occurs.
 *
 * @param:
 * src - The source game
 * @return:
 * SP_CHESS_GAME_INVALID_ARGUMENT - if src == NULL
 * SP_CHESS_GAME_NO_HISTORY       - if the user invoked this function more then
 *                                 historySize in a row.
 * SP_CHESS_GAME_SUCCESS          - on success. The last tool that was moved on the
 *                                 board is returned to its previous
 *                                 position and the current player is changed.
 */
SP_CHESS_GAME_MESSAGE spChessGameUndoPrevMove(SPChessGame* src){
	if (assertGame(src)){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	if (src->historyMoves->actualSize == 0){
		return SP_CHESS_GAME_NO_HISTORY;
	}

	CHMove* lastMove = spArrayListGetLast(src->historyMoves);
	src->gameBoard[lastMove->prevRow][lastMove->prevCol] = lastMove->tool;
	if (lastMove->didCapture){
		src->gameBoard[lastMove->currRow][lastMove->currCol] = lastMove->captured;
	}
	else {
		src->gameBoard[lastMove->currRow][lastMove->currCol] = SP_CHESS_EMPTY_ENTRY;
	}
	spGamePrintUndoMove(src, lastMove);
	spArrayListRemoveLast(src->historyMoves);
	changePlayer(src);

	return SP_CHESS_GAME_SUCCESS;
}



/**
 * On success, the function prints the board game. If an error occurs, then the
 * function does nothing. The characters 'm', 'M', 'b', 'B', 'r', 'R', 'n', 'N', 'q', 'Q',
 * 'k' and 'K' are used to represent
 * the tools of the white and the black players.
 *
 * @param:
 * src - the target game
 *
 * @return:
 * SP_CHESS_GAME_INVALID_ARGUMENT - if src==NULL
 * SP_FIAR_GAME_SUCCESS - otherwise
 *
 */
SP_CHESS_GAME_MESSAGE spChessGamePrintBoard(SPChessGame* src){
	if (assertGame(src)){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	for ( int i=0 ; i<=SP_CHESS_DIM-1 ; i++){
		printf("%d| ",  SP_CHESS_DIM-i);
		for ( int j=0 ; j <= SP_CHESS_DIM-1 ; j++){
			printf("%c ", src->gameBoard[i][j]);
		}
		printf("|\n");
	}
	printf("  -----------------\n   A B C D E F G H\n");
	return SP_CHESS_GAME_SUCCESS;
}




#endif
