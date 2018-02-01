#ifndef SPWINDOW_H_
#define SPWINDOW_H_

#include <SDL.h>
#include <SDL_video.h>


/*
 * An enum that represents menu events.
 * This events informs the gui maneger with what kind of event it needs to handle next.
 *
 */
typedef enum {
	SP_MEVENT_EXIT,
	SP_MEVENT_START,
	SP_MEVENT_GOTOLOAD,
	SP_MEVENT_CHANGESETTINGS_1,
	SP_MEVENT_CHANGESETTINGS_2,
	SP_MEVENT_CHANGESETTINGS_3,
	SP_MEVENT_CHANGESETTINGS_4,
	SP_MEVENT_CHANGESETTINGS_5,
	SP_MEVENT_LOAD,
	SP_MEVENT_NEXT,
	SP_MEVENT_QUIT,
	SP_MEVENT_MAINMENU,
	SP_MEVENT_BACK,
	SP_MEVENT_SAVE,
	SP_MEVENT_NONE,
	SP_MEVENT_MOVE,
} SP_WINDOW_EVENT;


/*
 * An enum that represents a window in the program.
 */
typedef enum sp_window_t{
	SP_WINDOW_MAINMENU,
	SP_WINDOW_LOAD,
	SP_WINDOW_DIFFICULTY,
	SP_WINDOW_GAMEMODE,
	SP_WINDOW_COLOR,
	SP_WINDOW_GAME,
} SP_WINDOW;



/*
 * A structure that represents a window in the program.
 */
typedef struct spwindow_t SPWindow;
struct spwindow_t {
	void* data;
	void (*drawWindow)(SPWindow* );
	SP_WINDOW_EVENT (*handleEventWindow)(SPWindow* , SDL_Event* );
	void (*destroyWindow)(SPWindow* );
	SP_WINDOW (*getType)(SPWindow*);
};

//NULL safe FUNCTION
void destroyWindow(SPWindow* src);

#endif
