/*
 * SPChessFile.c
 *
 *  Created on: Aug 30, 2017
 *      Author: Maria Klimkin
 */

#include "SPChessFile.h"
#include "SPChessGame.h"
#include "SPChessSettings.h"

//#define NULL 0


/*
 * Returns the color of the current player of the game.
 *
 * @param:
 * src - the current game played.
 * @return:
 * A string:
 * "white" if its the white players turn in src,
 * "black" otherwise.
 */
const char* spFileCurrentPlayerColor(SPChessGame* src){
	const char* white = "white";
	const char* black = "black";
	if (spChessGameGetCurrentPlayer(src)){
		return white;
	}
	else{
		return black;
	}
}



/*
 * Writes the current state of the game board to a file as writen in the submission guidlines.
 * @param:
 * src - the current game played.
 * fb - the file to write inside the boardgame of src.
 */
void spFilePrintBoardToFile(SPChessGame* src, FILE* fb){
	int row;
	for (int i=0 ; i<SP_CHESS_DIM ; i++){
		row = 8-i;
		fprintf(fb, "<row_%d>", row);
		for (int j=0 ; j<SP_CHESS_DIM ; j++){
			fprintf(fb, "%c", (src->gameBoard[i][j]));
		}
		fprintf(fb, "</row_%d>\n", row);
	}
}



/*
 * Saves a given chess game and its setting to xml file. The file will contain the following information:
 * current turn (white or black), game mode (1-player or 2-player), difficulty, user color (relevant in case of 1-player mode),
 * and the board state.
 * In case the saving operation hasn't succeeded, an error message will be printed.
 */
void spFileSaveGameToFile(SPChessGame* src, SPSetting* set, char* path){
	FILE *file = fopen(path, "w");
	if (file==0){ //
		printf("File cannot be created or modified\n");
		return;
	}
	int currentTurn = spChessGameGetCurrentPlayer(src);
	fprintf(file , "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(file, "<game>\n");
	fprintf(file, "<current_turn>%d</current_turn>\n", currentTurn);
	fprintf(file, "<game_mode>%d</game_mode>\n", (set->gameMode));
	if (set->gameMode == 1){ /////////////////////
		fprintf(file, "<difficulty>%d</difficulty>\n", (set->difficulty));
		fprintf(file, "<user_color>%d</user_color>\n", (set->userColor));
	}
	fprintf(file, "<board>\n");
	spFilePrintBoardToFile(src, file);
	fprintf(file, "</board>\n");
	fprintf(file, "</game>\n");
	fclose(file);
}



/*
 * Calculates the length of a given file.
 * @param:
 * file - the file to calculate its length.
 * @return:
 * -1 - if an error acured while reading the file.
 * The length of the file otherwise.
 *
 */
long spFileCheckFilesLength(FILE* file){
	if (fseek(file, 0, SEEK_END)){
		return -1;
	}
	long len = ftell(file);
	if (len<0){
		return -1;
	}
	if(fseek(file, 0, SEEK_SET)){
		return -1;
	}
	return len;
}


/*
 * Parsers a string that holds information about a certain game and its setting and updates the given
 * SPChessGame and SPSetting accordingly.
 * @param:
 * str - the string to parse.
 * src - a SPChessGame instant to update.
 * set - a SPSetting instant to update.
 * @return:
 * SP_CHESS_GAME_FAILED - if an error occured.
 * SP_CHESS_GAME_SUCCESS - if the string interpreted correctly and src and set updated successfuly.
 */
SP_CHESS_GAME_MESSAGE spFileRestoreGameFromStr(char* str, SPChessGame* src, SPSetting* set){
	char* c = (char*) malloc (strlen(str) +1);
	if (!c){
		return SP_CHESS_GAME_FAILED;
	}
	char delimit[]="\n\t";
	strcpy(c,str);
	c[strlen(str)] = '\0';
	char *line3 = strtok(c, delimit);
	line3 = strtok(0, delimit); //
	if (line3[1] == 'm'){
		return SP_CHESS_GAME_FAILED;
	}
	line3 = strtok(0, delimit); //
	if (line3[14] == '1'){
		src->currentPlayer = 1;
	}
	else{
		src->currentPlayer = 0;
	}
	char *line4 = strtok(0, delimit); //
	set->gameMode = (int) (line4[11] - '0');
	if (set->gameMode == 1){ ///////////////////
		char *line5 = strtok(0, delimit); //
		set->difficulty = (int) (line5[12] - '0');
		src->level = set->difficulty;
		char *line6 = strtok(0, delimit);
		set->userColor = (int) (line6[12] - '0');
	}
	char *boardLine = strtok(0, delimit); //
	for (int i=0 ; i<SP_CHESS_DIM ; i++){
		boardLine = strtok(0, delimit); //
		for (int j=0 ; j<SP_CHESS_DIM ; j++){
			src->gameBoard[i][j] = boardLine[7+j];
			if ( boardLine[7+j] == 'k'){
				src->whiteKingRow = i;
				src->whiteKingCol = j;
			}
			else if ( boardLine[7+j] == 'K'){
				src->blackKingRow = i;
				src->blackKingCol = j;
			}
		}
	}
	free(c);
	return SP_CHESS_GAME_SUCCESS;
}



/*
 * Loads a chess game and its setting from a xml file.
 * @param:
 * path - a path to the xml file.
 * src - a SPChessGame instant to update according to the game saved in 'path'.
 * set - a SPSetting instant to update according to the setting saved in 'path'.
 * @return:
 * SP_CHESS_GAME_FAILED - if an error occured while opening or reading the file, or if an allocation error
 * occured.
 * SP_CHESS_GAME_SUCCESS - otherwise.
 */
SP_CHESS_GAME_MESSAGE spFileLoadGameFromFile(char* path, SPChessGame* src, SPSetting* set){
	char* BUF = 0; /////
	FILE* file = 0;
	file = fopen(path, "r");
	if (!file){
		return SP_CHESS_GAME_FAILED;
	}
	long fileLen = spFileCheckFilesLength(file);
	if (fileLen < 0){
		return SP_CHESS_GAME_FAILED;
	}
	BUF = (char*) malloc (fileLen+1);
	if (!BUF){
		fclose(file);
		return SP_CHESS_GAME_FAILED;
	}
	BUF[fileLen] = '\0';
	fread(BUF, 1, fileLen, file);
	fclose(file);
	if ( spFileRestoreGameFromStr(BUF, src, set) == SP_CHESS_GAME_FAILED){
		free(BUF);
		return SP_CHESS_GAME_FAILED;
	}
	free(BUF);
	return SP_CHESS_GAME_SUCCESS;
}



/*
 * Handels a load from file operation. If a certain load from file operation hasn't succeded, an error message will
 * be printed.
 * @param:
 * path - a path to xml file that holds a chess game and its setting.
 * src - a SPChessGame instant to update according to the game saved in 'path'.
 * set - a SPSetting instant to update according to the setting saved in 'path'.
 * @return:
 * An SP_COMMAND
 * SP_INVALID_LINE - if the load operation failed.
 * SP_LOAD - otherwise.
 */
SP_COMMAND spFileHandleLoad(char* path, SPChessGame* src, SPSetting* set){
	SP_CHESS_GAME_MESSAGE msg = spFileLoadGameFromFile(path, src, set);
	if (msg == SP_CHESS_GAME_FAILED){
		printf("Error: File doesn't exist or cannot be opened\n");
		return SP_INVALID_LINE;
	}
	else{
		return SP_LOAD;
	}
}


/*
 * Copies the content of one file (fp1) to another file (fp2).
 *
 */
void spFileCopyFileToOtherFile(FILE* fp1, FILE* fp2){
	char ch;
	while(1){
		ch = fgetc(fp1);
		if (ch==EOF){
			break;
		}
		else{
			putc(ch, fp2);
		}
	}
}


/*
 * Handles a save game event in GUI mode. The game is  automatially saved to game slot1, so this function
 * copies the previuos saved game in slot1 to slot2, the saved game in slot2 to slot3 and so on.
 * If there is a saved game in slot5 it is erased.
 *
 * @param:
 * src - the source game to save.
 * set - the setting of src.
 *
 * @return:
 * SP_CHESS_GAME_FAILED - if the saving of the game failed or if the copy of one of the slots failed.
 * SP_CHESS_GAME_SUCCESS - if the game saved.
 *
 */
SP_COMMAND spFileHandleSaveInSDL(SPChessGame* src, SPSetting* set){
	FILE *fp1, *fp2, *fp3, *fp4, *fp5;
	fp4 = fopen("./Slots/slot4.xml", "r");
	fp5 = fopen("./Slots/slot5.xml", "w");
	if (!fp4 || !fp5){
		return SP_CHESS_GAME_FAILED;
	}
	spFileCopyFileToOtherFile(fp4, fp5);
	fp3 = fopen("./Slots/slot3.xml", "r");
	fclose(fp4);
	fp4 = fopen("./Slots/slot4.xml", "w");
	if (!fp3 || !fp4){
		return SP_CHESS_GAME_FAILED;
	}
	spFileCopyFileToOtherFile(fp3, fp4);
	fp2 = fopen("./Slots/slot2.xml", "r");
	fclose(fp3);
	fp3 = fopen("./Slots/slot3.xml", "w");
	if (!fp2 || !fp3){
		return SP_CHESS_GAME_FAILED;
	}
	spFileCopyFileToOtherFile(fp2, fp3);
	fp1 = fopen("./Slots/slot1.xml", "r");
	fclose(fp2);
	fp2 = fopen("./Slots/slot2.xml", "w");
	if (!fp1 || !fp2){
		return SP_CHESS_GAME_FAILED;
	}
	spFileCopyFileToOtherFile(fp1, fp2);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
	char* path = "./Slots/slot1.xml";
	spFileSaveGameToFile(src, set, path);
	return SP_CHESS_GAME_SUCCESS;
}








