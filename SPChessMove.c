/*
 * SPChessMove.c
 *
 *  Created on: 13 באוג 2017
 *      Author: ערי
 */


#ifndef SPCHESSMOVE_H_
#define SPCHESSMOVE_H_
#define NULL 0

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "SPChessMove.h"
#include "SPChessParser.h"


typedef struct chess_move{
	char tool;
	int currRow;
	int currCol;
	int prevRow;
	int prevCol;
	bool didCapture;
	char captured;
} CHMove;



/*
 * Creates move - a new instant of CHMove.
 *
 * @param
 * tool - move.tool will be tool.
 * currRow - move.currRow will be currRow.
 * currCol - move.currCol will be currCol.
 * prevRow - move.prevRow will be prevRow.
 * prevCol - move.prevCol wiil be prevCol.
 *
 * @return:
 * The new move created.
 *
 */
CHMove spChessCreateMove(char tool, int currRow, int currCol, int prevRow, int prevCol){
	CHMove move;
	move.tool = tool;
	move.currRow = currRow;
	move.currCol = currCol;
	move.prevRow = prevRow;
	move.prevCol = prevCol;
	move.didCapture = false;
	return move;
}



/*
 * Checks if move==NULL.
 *
 * @param:
 * move - the move to check
 *
 * @return:
 * true - if move==NULL.
 * false - otherwise.
 *
 */
bool assertMove(CHMove* move){
	if(move==NULL){
		return true;
	}
	return false;
}



/*
 * Allocates memory for a new copy of a given move and makes the copy.
 *
 * @param:
 * src - the move to copy.
 *
 * @return:
 * CHMove* - new copy of src.
 *
 */
CHMove* spChessCopyMove(CHMove* src){
	CHMove* move = (CHMove*) malloc (sizeof(CHMove));
	move->tool = src->tool;
	move->currRow = src->currRow;
	move->currCol = src->currCol;
	move->prevRow = src->prevRow;
	move->prevCol = src->prevCol;
	move->didCapture = src->didCapture;
	move->captured = src->captured;
	return move;
}





/*
 * Makes a CHMove instant from a SP_MOVE command.
 *
 * @param:
 * com - the command to parse.
 *
 * @return:
 * CHMove* - the new move that fits the SP_MOVE command.
 *
 */
CHMove* spMoveGetMoveFromCommandArgs(SPCommand com){
	CHMove move;
	if (strlen(com.arg1) != 5 || strlen(com.arg2) != 5 || com.arg1[2] != ',' || com.arg2[2] != ','){
		move = spChessCreateMove('_', 100, 100, 100, 100);
	}
	else{
		int prevRow = (int) (56-com.arg1[1]);
		int currRow = (int) (56-com.arg2[1]);
		int prevCol = (int) (com.arg1[3] - 65);
		int currCol = (int) (com.arg2[3] - 65);
		move = spChessCreateMove('_', currRow, currCol, prevRow, prevCol);
	}
	CHMove* finalMove = spChessCopyMove(&move);
	return finalMove;
}



/*
 * If src!=NULL, deletes it and frees all the memory allocated with src.
 *
 * @param:
 * src - the move to destroy.
 *
 */
void spChessDestroyMove(CHMove* src){
	if (assertMove(src)){
		return;
	}
	free(src);
}






#endif


