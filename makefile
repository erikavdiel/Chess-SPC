CC = gcc
OBJS =  SPChessParser.o SPChessMove.o SPArrayList.o SPChessGame.o SPChessMiniMax.o SPChessSettings.o SPChessFile.o \
SPChessMainAux.o SPChessTool.o  SPChessButton.o SPWindow.o SPChessMenuWindow.o SPChessLoadWindow.o SPChessGameWindow.o  SPChessGUIManager.o main.o 
EXEC = chessprog
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2/ -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main


$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
SPChessParser.o: SPChessParser.c SPChessParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessMove.o: SPChessMove.c SPChessMove.h SPChessParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPArrayList.o: SPArrayList.h SPArrayList.c SPChessMove.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessGame.o: SPChessGame.h SPChessGame.c SPArrayList.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessMiniMax.o: SPChessMiniMax.c SPChessMiniMax.h SPChessGame.h SPArrayList.h SPChessMove.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessSettings.o: SPChessSettings.c SPChessSettings.h SPChessParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessFile.o: SPChessFile.c SPChessFile.h SPChessGame.h SPChessSettings.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessMainAux.o: SPChessMainAux.c SPChessMainAux.h SPChessGame.h SPChessSettings.h SPChessMiniMax.h SPChessParser.h SPChessMove.h SPChessFile.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessButton.o: SPChessButton.c SPChessButton.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessTool.o: SPChessTool.c SPChessTool.h SPChessButton.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPWindow.o: SPWindow.c SPWindow.h  
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessMenuWindow.o: SPChessMenuWindow.c SPChessMenuWindow.h SPWindow.h SPChessButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessLoadWindow.o: SPChessLoadWindow.c SPChessLoadWindow.h SPWindow.h SPChessButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessGameWindow.o: SPChessGameWindow.c SPChessGameWindow.h SPWindow.h SPChessButton.h SPChessTool.h SPChessSettings.h SPChessMiniMax.h SPChessGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessGUIManager.o: SPChessGUIManager.c SPChessGUIManager.h SPChessMenuWindow.h SPChessLoadWindow.h SPWindow.h SPChessGameWindow.h SPChessGame.h SPChessSettings.h SPChessFile.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
main.o: main.c SPChessGUIManager.h SPChessMainAux.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
clean:
	rm -f *.o $(EXEC)
