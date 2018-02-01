/*
 * SPChessParser.c
 *
 *  Created on: 13 באוג 2017
 *      Author: ערי
 */


#ifndef SPCHESSPARSER_H_
#define SPCHESSPARSER_H_
#define NULL 0

#include "SPChessParser.h"

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
bool spParserIsInt(const char* str){
		if(str==NULL){
			return false;
		}
		int i = 1;
	    if ((*str>='0' && *str<='9') || *str=='-'){
	    	while(*(str+i) != '\0' && *(str+i) != '\n'){
	    		if(*(str+i)<'0' || *(str+i)>'9'){
	    			return false;
	    		}
	    		i++;
	    	}
	    	return true;
	    }
	    return false;
}



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
SPCommand nonArgCommand(SP_COMMAND cmd){
	SPCommand com;
	com.cmd = cmd;
	com.validArg1 = false;
	com.validArg2 = false;
	com.validArgInt = false;
	return com;
}



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
int StringToInt(char* str){
	int newInt = 0;
	int len = strlen(str);
	for(int i=0; i<=len-1; i++){
		newInt = newInt * 10 + ( str[i] - '0' );
	}
	return newInt;
}



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
bool InvalidLevel(char* level){
	if (spParserIsInt(level)){ //if the input is an integer
		int levelAsInt = StringToInt(level);
		if (levelAsInt>0 && levelAsInt<=5){
			return false; //the level is valid
		}
	}
	return true; //level invalid (not an integer or integer>5 or integer<1)
}



/*
 * Parses a string that doesn't contain ' ' (space) char in it (one word string) to a command that is given
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
 * SP_INVALID_LINE - if the user entered some invalid command..
 *
 */
SPCommand oneStringPreCommand(char* str){
	SPCommand com;
	if (!(strcmp(str, "default"))){
		com = nonArgCommand(SP_DEFAULT);
	}
	else if (!(strcmp(str,"print_setting"))){
		com = nonArgCommand(SP_PRINT_SETTING);
	}
	else if (!(strcmp(str,"quit"))){
		com = nonArgCommand(SP_QUIT);
	}
	else if (!(strcmp(str,"start"))){
		com = nonArgCommand(SP_START);
	}
	else{
		com = nonArgCommand(SP_INVALID_LINE);
	}
	return com;
}



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
SPCommand twoStringsPreCommand(char* str1, char* str2){
	SPCommand com;
	if (spParserIsInt(str2) && strcmp(str1, "load")){
		com.validArgInt = true;
		com.argInt = StringToInt(str2);
		if (!(strcmp(str1, "game_mode"))){
			com.cmd = SP_GAME_MODE;
		}
		else if (!(strcmp(str1, "difficulty"))){
			com.cmd = SP_DIFFICULTY;
		}
		else if (!(strcmp(str1, "user_color"))){
			com.cmd = SP_USER_COLOR;
		}
		else{
			com = nonArgCommand(SP_INVALID_LINE);
		}
	}
	else if (!(strcmp(str1,"load"))){
		com.cmd = SP_LOAD;
		com.validArg1 = true;
		strcpy(com.arg1, str2);
	}
	else{
		com = nonArgCommand(SP_INVALID_LINE);
	}
	return com;
}



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
SPCommand spParserPreCommand(const char* str){
	SPCommand com;
	if(*str == '\n'){
		com.cmd = SP_INVALID_LINE;
		return com;
	}
	char* c = malloc(strlen(str)+5);
	char delimit[]=" \t\r\n\v\f";
	strcpy(c,str);
	c[strlen(str)+1] = '\0';
	char *str1 = strtok(c, delimit);
	char *str2;
	if (strcmp(str1,"load")==0){
		str2 = strtok(NULL,"\n");
	}
	else{
		str2 = strtok(NULL, delimit);
	}
	if(str2 != NULL){
		char *str3 = strtok(NULL,delimit);
		if (str3 != NULL){
			com = nonArgCommand(SP_INVALID_LINE);
		}
		else{
			com = twoStringsPreCommand(str1, str2);
		}
	}
	else{
		com = oneStringPreCommand(str1);
	}
	free(c);
	return com;
}




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
bool checkMoveCommand(char* str1, char* str2, char* str3, char* str4){
	if (strcmp(str1, "move")){
		return false;
	}
	if (strcmp(str3, "to")){
		return false;
	}
	int str2len = strlen(str2);
	int str4len = strlen(str4);
	if (str2[0] != '<' || str4[0] != '<'){
		return false;
	}
	if (str2[str2len-1] != '>' || str4[str4len-1] != '>'){
		return false;
	}
	return true;
}



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
SPCommand oneStringInCommand(char* str){
	SPCommand com;
	com.cmd = SP_INVALID_LINE;
	if (!(strcmp(str, "undo"))){
		com = nonArgCommand(SP_UNDO);
	}
	else if (!(strcmp(str, "reset"))){
		com = nonArgCommand(SP_RESET);
	}
	else if (!(strcmp(str, "quit"))){
		com = nonArgCommand(SP_QUIT);
	}
	return com;
}



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
SPCommand spParserInCommand(const char* str){
	SPCommand com;
		if(*str == '\n'){
			com.cmd = SP_INVALID_LINE;
			return com;
		}
		char* c = malloc(strlen(str)+5);
		char delimit[]=" \t\r\n\v\f";
		strcpy(c,str);
		c[strlen(str)+1] = '\0';
		char *str1 = strtok(c, delimit);
		char *str2;
		if((strcmp(str1,"save")==0)){
			str2 = strtok(NULL, "\n");
		}
		else{
			str2 = strtok(NULL, delimit);
		}
		if(str2 != NULL && (strcmp(str1,"save")!=0)){
			char *str3 = strtok(NULL,delimit);
			if (str3 != NULL){
				bool flag = true;
				char *str4 = strtok(NULL,delimit);
				if (str4 != NULL){
					char *str5 = strtok(NULL,delimit);
					if(str5 != NULL){
						flag = false;
					}
					else{
						flag = checkMoveCommand(str1, str2, str3, str4);
					}
				}
				else{
					flag = false;
				}
				if (flag){
					com.cmd = SP_MOVE;
					com.validArg1 = true;
					com.validArg2 = true;
					strcpy(com.arg1, str2);
					strcpy(com.arg2, str4);
				}
				else{
					com = nonArgCommand(SP_INVALID_LINE);
				}
			}
		}
		else if(!(strcmp(str1, "save"))){
			com.cmd = SP_SAVE;
			com.validArg1 = true;
			strcpy(com.arg1, str2);
		}
		else{
			com = oneStringInCommand(str1);
		}
		free(c);
		return com;
}

#endif
