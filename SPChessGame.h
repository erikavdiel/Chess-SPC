/*
 * SPChessGame.h
 *
 *  Created on: 13 באוג 2017
 *      Author: ערי
 */

#ifndef SPCHESSGAME_H_
#define SPCHESSGAME_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SPArrayList.h"



//Definitions
#define SP_CHESS_DIM 8
#define SP_CHESS_BLACK_PLAYER 0
#define SP_CHESS_WHITE_PLAYER 1
#define SP_CHESS_TIE_SYMBOL '-'
#define SP_CHESS_EMPTY_ENTRY '_'


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
SPChessGame* spChessGameCreate(int historySize);

/**
 * checks if src==NULL
 *	@param src - the source Game.
 *	@return
 *	true if src is NULL
 *	false otherwise
 */
bool assertGame(SPChessGame* src);

/**
 *	Creates a copy of a given game.
 *	The new copy has the same status as the src game.
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 */
SPChessGame* spChessGameCopy(SPChessGame* src);

/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game.
 */
void spChessGameDestroy(SPChessGame* src);

/**
 * Returns the current player of the specified game.
 * @param src - the source game
 * @return
 * SP_CHESS_BLACK_PLAYER - if it's black player's turn.
 * SP_CHESS_WHITE_PLAYER - if it's white player's turn.
 */
int spChessGameGetCurrentPlayer(SPChessGame* src);

/**
 * Returns the previous player of the specified game.
 * @param src - the source game
 * @return
 * SP_CHESS_BLACK_PLAYER - if it's black player's turn.
 * SP_CHESS_WHITE_PLAYER - if it's white player's turn.
 */
int spChessGameGetPrevPlayer (SPChessGame* src);

/*
 * Switches the player that should play on the next turn. If currentPlayer is SP_CHESS_WHITE_PLAYER, it
 * will change it to SP_CHESS_BLACK_PLAYER and vice versa.
 *
 * @param:
 * src - the source game to swich the turns in.
 */
void changePlayer(SPChessGame* src);

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
bool spCheckValidLocation(CHMove* move);

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
bool spCheckBlackPieceInPos(SPChessGame* src, int row, int col);

/*
 * Checks if a certain position on the board contains a white piece.
 *
 * @param:
 * src - the source game.
 * row - the row index of the position.
 * col - the column index of the position.
 *
 * @return:
 * true - if there is a white piece in this position on the board.
 * false - otherwise.
 */
bool spCheckWhitePieceInPos(SPChessGame* src, int row, int col);

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
bool spCheckPieceInPosition(SPChessGame* src, CHMove* move);

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
bool spCheckEmptyPos(SPChessGame* src, int row, int col);

/*
 * Checks if a given CHMove of black pawn is valid. The move is valid iff it is a single move forward in the same
 * column, two moves forward if the pawn located at its starting position or one diagonal step forward if the
 * pawn captures a white tool.
 *
 * @param:
 * src - the source game.
 * move - the move to check its validness.
 *
 * @return:
 * true - if 'move' is a valid black pawn move.
 * false - otherwise or if src==NULL or move==NULL.
 */
bool spValidPMove (SPChessGame* src, CHMove* move);

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
bool spValidpMove (SPChessGame* src, CHMove* move);

/*
 * Checks if a given CHMove of rook (white or black) is valid. The move is valid iff it is a move along its
 * row or column, and there is no leap over other pieces.
 *
 * @param:
 * src - the source game.
 * move - the move to check its validness.
 *
 * @return:
 * true - if 'move' is a valid rook move.
 * false - otherwise or if src==NULL or move==NULL.
 */
bool spValidRrMove(SPChessGame* src, CHMove* move, int color);

/*
 * Checks if a given CHMove of knight (white or black) is valid. The move is valid iff it is a "L"- shape move.
 *
 * @param:
 * src - the source game.
 * move - the move to check its validness.
 *
 * @return:
 * true - if 'move' is a valid knight move.
 * false - otherwise or if src==NULL or move==NULL.
 */
bool spValidNnMove(SPChessGame* src, CHMove* move, int color);

/*
 * Checks if a given CHMove of bishop (white or black) is valid. The move is valid iff it is a diagonal
 * move (backward or forward) and there is no leap over other pieces.
 *
 * @param:
 * src - the source game.
 * move - the move to check its validness.
 *
 * @return:
 * true - if 'move' is a valid bishop move.
 * false - otherwise or if src==NULL or move==NULL.
 */
bool spValidBbmove(SPChessGame* src, CHMove* move, int color);

/*
 * Checks if a given CHMove of queen (white or black) is valid. The move is valid iff it is a move along its
 * row or column or it is a diagonal move, and there is no leap over other pieces.
 *
 * @param:
 * src - the source game.
 * move - the move to check its validness.
 *
 * @return:
 * true - if 'move' is a valid queen move.
 * false - otherwise or if src==NULL or move==NULL.
 */
bool spValidQqMove(SPChessGame* src, CHMove* move, int color);

/*
 * Checks if a given CHMove of king (white or black) is valid. The move is valid iff it is a single move to
 * any direction.
 *
 * @param:
 * src - the source game.
 * move - the move to check its validness.
 *
 * @return:
 * true - if 'move' is a valid king move.
 * false - otherwise or if src==NULL or move==NULL.
 */
bool spValidKkMove(SPChessGame* src, CHMove* move, int color);

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
bool spChessGameCheckByTool(SPChessGame* src, CHMove* move, int color);

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
SP_CHESS_GAME_MESSAGE spChessGameIsValidMove(SPChessGame* src, CHMove* move, int color);

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
SP_CHESS_GAME_MESSAGE spChessCheckForCheckOnWhite(SPChessGame* src);

/*
 * Checks if in the current state of the game board there is a check on the black king.
 * It is done by going over on all the possible moves of the black player and checking if at least one
 * of them results with avoiding the check.
 *
 * @param:
 * src - the source game.
 *
 * @return:
 * SP_CHESS_GAME_BLACK_CHECK - if there is a check on the black king.
 * SP_CHESS_GAME_NO_CHECK - otherwise.
 */
SP_CHESS_GAME_MESSAGE spChessCheckForCheckOnBlack(SPChessGame* src);

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
SP_CHESS_GAME_MESSAGE spCheckForCheck(SPChessGame* src);

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
SP_CHESS_GAME_MESSAGE spChessGameSetMove(SPChessGame* src, CHMove* move);

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
bool spChessCheckIfSelfCheck(SPChessGame* src, CHMove* move);

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
SPArrayList* spChessGetPawnPossibleMoves(SPChessGame* src, int row, int col);

/**
 * Calculates and returns a list of all possible (game depended validity) moves for a
 * certain white pawn.
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
SPArrayList* spChessGetpawnPossibleMoves(SPChessGame* src, int row, int col);

/**
 * Calculates and returns a list of all possible (game depended validity) moves for a
 * certain bishop.
 *
 * @param:
 * src - the current game played.
 * row - the row index of the bishop's position.
 * col - the col index of the bishop's position.
 *
 * @return:
 * NULL - if src==NULL.
 * A pointer to the first argument in the list.
 */
SPArrayList* spChessGetBishopPossibleMoves(SPChessGame* src, int row, int col);

/**
 * Calculates and returns a list of all possible (game depended validity) moves for a
 * certain rook.
 *
 * @param:
 * src - the current game played.
 * row - the row index of the rook's position.
 * col - the col index of the rook's position.
 *
 * @return:
 * NULL - if src==NULL.
 * A pointer to the first argument in the list.
 */
SPArrayList* spChessGetRookPossibleMoves(SPChessGame* src, int row, int col);

/**
 * Calculates and returns a list of all possible (game depended validity) moves for a
 * certain knight.
 *
 * @param:
 * src - the current game played.
 * row - the row index of the knight's position.
 * col - the col index of the knight's position.
 *
 * @return:
 * NULL - if src==NULL.
 * A pointer to the first argument in the list.
 */
SPArrayList* spChessGetKnightPossibleMoves(SPChessGame* src, int row, int col);

/**
 * Calculates and returns a list of all possible (game depended validity) moves for a
 * certain queen.
 *
 * @param:
 * src - the current game played.
 * row - the row index of the queen's position.
 * col - the col index of the queen's position.
 *
 * @return:
 * NULL - if src==NULL.
 * A pointer to the first argument in the list.
 */
SPArrayList* spChessGetQueenPossibleMoves(SPChessGame* src, int row, int col);

/**
 * Calculates and returns a list of all possible (game depended validity) moves for a
 * certain king.
 *
 * @param:
 * src - the current game played.
 * row - the row index of the king's position.
 * col - the col index of the king's position.
 *
 * @return:
 * NULL - if src==NULL.
 * A pointer to the first argument in the list.
 */
SPArrayList* spChessGetKingPossibleMoves(SPChessGame* src, int row, int col);

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
 * A pointer to the first argument in the list.
 */
SPArrayList* spChessGetMovesListForPiece(SPChessGame* src ,char tool, int row, int col);

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
SPArrayList* spChessGameGetAllPossibleMoves(SPChessGame* src);

/*
 * Checks if in the current state of the game board (check on the white king)
 * there is also a checkmate on the white king.
 * This is done by going over on all the possible moves for the white player. If there is at least one possible
 * legal move then there is no checkmate, otherwise it's a checkmate.
 *
 * @param:
 * src - the current game played.
 *
 * @return:
 * NULL - if src==NULL.
 * SP_CHESS_GAME_NO_CHECKMATE - if there is no checkmate.
 * SP_CHESS_GAME_CHECKMATE - if there checkmate.
 */
SP_CHESS_GAME_MESSAGE spChessGameCheckForCheckMateOnWhite(SPChessGame* src);

/*
 * Checks if in the current state of the game board (check on the black king)
 * there is also a checkmate on the black king.
 * This is done by going over on all the possible moves for the black player. If there is at least one possible
 * legal move then there is no checkmate, otherwise it's a checkmate.
 *
 * @param:
 * src - the current game played.
 *
 * @return:
 * NULL - if src==NULL.
 * SP_CHESS_GAME_NO_CHECKMATE - if there is no checkmate.
 * SP_CHESS_GAME_CHECKMATE - if there checkmate.
 */
SP_CHESS_GAME_MESSAGE spChessGameCheckForCheckMateOnBlack(SPChessGame* src);

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
SP_CHESS_GAME_MESSAGE spChessCheckforTieWhite(SPChessGame* src);

/*
 * Checks if there is a tie when it is the black player's turn.
 * This is done by going over on all the possible moves for each black tool on the game board.
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
SP_CHESS_GAME_MESSAGE spChessCheckforTieBlack(SPChessGame* src);

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
SP_CHESS_GAME_MESSAGE spChessCheckforTie(SPChessGame* src);

/*
 * Prints last move to undo in the format:
 * "Undo move for player white/black : <a,b> -> <x,y>".
 *
 * @param:
 * src - the current game played.
 * move - the move that needs to be undone.
 *
 */
void spGamePrintUndoMove(SPChessGame* src, CHMove* move);

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
SP_CHESS_GAME_MESSAGE spChessGameUndoPrevMove(SPChessGame* src);

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
SP_CHESS_GAME_MESSAGE spChessGamePrintBoard(SPChessGame* src);


#endif
