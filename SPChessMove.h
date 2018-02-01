/*
 * SPChessMove.h
 *
 *  Created on: 13 באוג 2017
 *      Author: ערי
 */

#ifndef SPCHESSMOVE_H_
#define SPCHESSMOVE_H_


#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "SPChessParser.h"

/*
 * A structure that represents a certain move that the user or the computer wants to execute.
 * emptyMove = true, if it is an empty move, or false otherwise.
 * tool = M/m/B/b/N/n/R/r/Q/q/K/k, a char that represent the tool which moves on the board.
 * currRow = 0/1/2/3/4/5/6/7 (equal to the formal indexing 1/2/3/4/5/6/7/8 respectively),
 * the row index of the position the tool moves to.
 * currCol = 0/1/2/3/4/5/6/7 (equal to the formal indexing A/B/C/D/E/F/G/H respectively),
 * the column index of the position the tool moves to.
 * prevRow = 0/1/2/3/4/5/6/7 (equal to the formal indexing 1/2/3/4/5/6/7/8 respectively),
 * the row index of the position the tool moves from.
 * prevCol = 0/1/2/3/4/5/6/7 (equal to the formal indexing A/B/C/D/E/F/G/H respectively),
 * the column index of the position the tool moves from.
 * didCapture = true or false, if the tool captured other tool during the move or not, respectively.
 * captured = M/m/B/b/N/n/R/r/Q/q, if didCapture==true, else not defined.
 */
typedef struct chess_move{
	bool emptyMove;
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
CHMove spChessCreateMove(char tool, int currRow, int currCol, int prevRow, int prevCol);

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
bool assertMove(CHMove* move);

/*
 * Makes a copy of a given move.
 *
 * @param:
 * src - the move to copy.
 *
 * @return:
 * CHMove* - new copy of src.
 *
 */
CHMove* spChessCopyMove(CHMove* src);

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
CHMove* spMoveGetMoveFromCommandArgs(SPCommand com);

/*
 * If src!=NULL, deletes it and frees all the memory allocated with src.
 *
 * @param:
 * src - the move to destroy.
 *
 */
void spChessDestroyMove(CHMove* src);


#endif /* SPCHESSMOVE_H_ */
