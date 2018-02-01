/*
 * SPChessParser.h
 *
 *  Created on: 13 באוג 2017
 *      Author: ערי
 */

#ifndef SPCHESSPARSER_H_
#define SPCHESSPARSER_H_

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>




//specify the maximum line length
#define SP_MAX_LINE_LENGTH 1024

//a type used to represent a command
 typedef enum SP_COMMAND{
	SP_GAME_MODE,
	SP_DIFFICULTY,
	SP_USER_COLOR,
	SP_LOAD,
	SP_DEFAULT,
	SP_PRINT_SETTING,
	SP_START,
	SP_SAVE,
	SP_UNDO,
	SP_MOVE,
	SP_QUIT,
	SP_RESET,
	SP_INVALID_LINE,
	SP_GAME_FINISHED,
 }SP_COMMAND;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	SP_COMMAND cmd;
	bool validArg1;
	bool validArg2;
	bool validArgInt;//is set to true if the line contains a valid argument
	char arg1[SP_MAX_LINE_LENGTH];
	char arg2[5];
	int argInt;

} SPCommand;




/**
 * Checks if a specified string represents a valid integer.
 *
 * @param:
 * str - the string to parse.
 *
 * @return:
 * true - if the string represents a valid integer.
 * false - otherwise.
 *
 */
bool spParserIsInt(const char* str);

/**
 * Parses a specified line to a command which doesn't have an integer
 * argument.
 *
 * @param:
 * cmd - the command to parse.
 *
 * @return:
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg1 - is set to false.
 *   validArg2 - is set to false.
 *   validArgInt - is set to false.
 *
 */
SPCommand nonArgCommand(SP_COMMAND cmd);

/*
 * Converts a string that represents an integer to int.
 *
 * @param:
 * str - the string to convert.
 *
 * @return:
 * The integer in str.
 *
 */
int StringToInt(char* str);

/*
 * Checks if a given string represents an ivalid chess game level. A level is valid if
 * 4<=level<=1.
 *
 * @param:
 * level - a string to parse.
 *
 * @return:
 * true - if str is not an integer, or if the level invalid.
 * false - otherwise (str represents a valid level).
 *
 */
bool InvalidLevel(char* level);

/*
 * Parses a string that doesn't contain ' ' (space) char in it (one word string) to a command.
 * The cmd of command might be one of the following: SP_DEFAULT, SP_PRINT_SETTING, SP_QUIT, SP_START or
 * SP_INVALID_LINE if the string is invalid.
 *
 * @param:
 * str - the string to parse.
 *
 * @return:
 * The fitted SPCommand.
 *
 */
SPCommand oneStringPreCommand(char* str);

/*
 * Parses a string that contains one ' ' (space) character in it (two word string) to a command that is given
 * by the user before the game begins.
 *
 * @param:
 * str1 - first part of the command.
 * str2 - second part of the command.
 *
 * @return:
 * An SPCommand, that can be:
 * SP_GAME_MODE - if the user entered "game_mode x" command (x for integer).
 * SP_DIFFICULTY - if the user entered "difficulty x" command (x for integer).
 * SP_USER_COLOR - if the user entered "user_color x" command (x for integer).
 * SP_LOAD - if the user entered "load x" command (x for a path).
 * SP_INVALID_LINE - if the user entered some invalid command.
 *
 */
SPCommand twoStringsPreCommand(char* str1, char* str2);

/*
 * Parses a string to a command that is given
 * by the user before the game begins.
 *
 * @param:
 * str - the string to parse.
 *
 * @return:
 * A SPCommand that can be:
 * SP_DEFAULT - if the user entered "default" (setting) command.
 * SP_PRINT_SETTING - if the user entered "print_setting" command.
 * SP_QUIT - if the user entered "quit" command.
 * SP_START - if the user entered "start" command.
 * SP_GAME_MODE - if the user entered "game_mode x" command (x for integer).
 * SP_DIFFICULTY - if the user entered "difficulty x" command (x for integer).
 * SP_USER_COLOR - if the user entered "user_color x" command (x for integer).
 * SP_LOAD - if the user entered "load x" command (x for a path).
 * SP_INVALID_LINE - if the user entered some invalid command.
 *
 */
SPCommand spParserPreCommand(const char* str);

/**
 * Checks if a move command (a valid move command is of the form
 * "move <a,b> to <x,y>" where 1<=a,x<=8 and A<=b,y<=H) is valid.
 *
 * @param:
 * str1, str2, str3, str4 - represents the four parts of the "move" command.
 *
 * @return:
 * true - if the move command is valid.
 * false - otherwise.
 *
 */
bool checkMoveCommand(char* str1, char* str2, char* str3, char* str4);

/*
 * Parses a string that doesn't contain ' ' (space) char in it (one word string) to a command that is given
 * by the user after the game begins.
 *
 * @param:
 * str - the string to parse.
 *
 * @return:
 * A SPCommand that can be:
 * SP_UNDO - if the user entered "undo" command.
 * SP_RESET - if the user entered "reset" command.
 * SP_INVALID_LINE - if the user entered some invalid command..
 *
 */
SPCommand oneStringInCommand(char* str);

/*
 * Parses a string to a command that is given
 * by the user after the game begins.
 *
 * @param:
 * str - the string to parse.
 *
 * @return:
 * A SPCommand that can be:
 * SP_UNDO - if the user entered "undo" command.
 * SP_RESET - if the user entered "reset" command.
 * SP_SAVE - if the user entered "save x" command (where x is a path).
 * SP_MOVE - if the user entered "move <a,b> to <x,y>" command.
 * SP_INVALID_LINE - if the user entered some invalid command.
 *
 */
SPCommand spParserInCommand(const char* str);


#endif /* SPCHESSPARSER_H_ */
