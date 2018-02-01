/*
 * SPChessMiniMax.c
 *
 *  Created on: 24 באוג 2017
 *      Author: ערי
 */


#ifndef SPCHESSMINIMAX_H_
#define SPCHESSMINIMAX_H_
#define NULL 0

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "SPChessMiniMax.h"
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
int spMiniMaxScoreTheTool(char tool){
	if (tool == 'M' || tool == 'm'){
		return 1;
	}
	else if (tool == 'N' || tool =='B' || tool == 'n' || tool =='b'){
		return 3;
	}
	else if (tool == 'R' || tool == 'r'){
		return 5;
	}
	else if (tool == 'Q' || tool == 'q'){
		return 9;
	}
	else if (tool == 'K' || tool == 'k'){
		return 100;
	}
	else{
		return 0;
	}
}



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
int spMiniMaxScoringFunc(SPChessGame* src, bool aiColor){
	int whiteScore = 0;
	int blackScore = 0;
	for (int i=0; i<8; i++){
		for (int j=0; j<8; j++){
			char tool = src->gameBoard[i][j];
			if (spCheckBlackPieceInPos(src, i, j)){
				blackScore = blackScore + spMiniMaxScoreTheTool(tool);
			}
			else if (spCheckWhitePieceInPos(src, i, j)){
				whiteScore = whiteScore + spMiniMaxScoreTheTool(tool);
			}
		}
	}
	if (aiColor){
			return (whiteScore - blackScore);
	}
	else{
			return (blackScore - whiteScore);
	}
}



/*
 * Recursive function that calculates the max score among the leaves of the minimax tree.
 * The calculation is done as in the beta pruning algorithm.
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
int spMiniMaxAlgorithm(SPChessGame* src, int depth, int alpha, int beta, bool aiColor, bool minOrMax){
	if (depth == src->level){
		return spMiniMaxScoringFunc(src, aiColor);
	}
	else if (spChessCheckforTie(src) == SP_CHESS_GAME_TIE){
		return spMiniMaxScoringFunc(src, aiColor);
	}
	else{
		SPArrayList* movesList = spChessGameGetAllPossibleMoves(src);
		int index = 0;
		while (index<movesList->actualSize && alpha<beta){
			if (spChessCheckIfSelfCheck(src, spArrayListGetAt(movesList, index))){
				index++;
			}
			else{
				SPChessGame* gameCopy = spChessGameCopy(src);
				spChessGameSetMove(gameCopy, spArrayListGetAt(movesList, index));
				int score = spMiniMaxAlgorithm(gameCopy, depth+1, alpha, beta, aiColor, !minOrMax);
				if (minOrMax){
					if (score>alpha){
						alpha = score;
					}
				}
				else{
					if (score<beta){
						beta = score;
					}
				}
				spChessGameDestroy(gameCopy);
				index++;
			}
		}
		spArrayListDestroy(movesList);
		if (minOrMax){
			return alpha;
		}
		else{
			return beta;
		}
	}
}



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
CHMove* spMiniMaxSuggestMove(SPChessGame* src){
	if (assertGame(src)){
		return NULL;
	}
	SPArrayList* movesList = spChessGameGetAllPossibleMoves(src);
	CHMove* bestMove = spChessCopyMove(spArrayListGetAt(movesList, 0));
	int alpha = INT_MIN;
	int beta = INT_MAX;
	for (int i=0 ; i<movesList->actualSize ; i++){
		if (spChessCheckIfSelfCheck(src, spArrayListGetAt(movesList, i))){
			continue;
		}
		else{
			SPChessGame* gameCopy = spChessGameCopy(src);
			spChessGameSetMove(gameCopy, spArrayListGetAt(movesList, i));
			int score = spMiniMaxAlgorithm(gameCopy, 1, alpha, beta, src->currentPlayer, false);
			if (score > alpha){
				alpha = score;
				spChessDestroyMove(bestMove);
				bestMove = spChessCopyMove(spArrayListGetAt(movesList, i));
			}
			spChessGameDestroy(gameCopy);
		}
	}
	spArrayListDestroy(movesList);
	return bestMove;
}



#endif /* SPCHESSMINIMAX_H_ */
