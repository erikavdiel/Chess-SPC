/*
 * SPChessMiniMax.h
 *
 *  Created on: 24 באוג 2017
 *      Author: ערי
 */

#ifndef SPCHESSMINIMAX_H_
#define SPCHESSMINIMAX_H_


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "SPChessGame.h"
#include "SPArrayList.h"
#include "SPChessMove.h"




/**
 * Gives a score for a chess tool. Pawn gets a score of 1, knight and bishop scored with 3,
 * rook scored with 5, queen with 9 and king with 100.
 *
 * @param:
 * tool - char that represents the tool to score.
 *
 * @return:
 * The score of 'tool'.
 *
 */
int spMiniMaxScoreTheTool(char tool);

/**
 * Calculets the score of the current game board. The board score = computer score - user score.
 *
 * @param:
 * src - the current game played.
 * aiColor - represent the computers color, true for white, false for black.
 *
 * @return:
 * The score of the game board.
 *
 */
int spMiniMaxScoringFunc(SPChessGame* src, bool aiColor);

/*
 * Recursive function that calculets the max score among the leaves of the minimax tree.
 * The calculation is done as in the beta purning algorithm.
 *
 * @param:
 * src - the source game, with the matching game board.
 * depth - indicates in what level of the tree we are, the max depth can be the difficulty of src.
 * alpha - the current alpha value of the node.
 * beta - the current beta value of the node.
 * aicolor - represent the color of the computer, can be 1 or 0 for white and black respectively.
 * minOrMax - true if it is a max node, false if it is a min node.
 *
 * @return:
 * The max score of the minmax tree.
 *
 */
int spMiniMaxAlgorithm(SPChessGame* src, int depth, int alpha, int beta, bool aiColor, bool minOrMax);

/*
 * Calculates what is the best move for the computer to do.
 * The best move is calculated from the all possible moves list by the minimax algorithm.
 *
 * @param:
 * src - the current game played.
 *
 * @return:
 * The best move for the computer to do.
 *
 */
CHMove* spMiniMaxSuggestMove(SPChessGame* src);

#endif /* SPCHESSMINIMAX_H_ */
