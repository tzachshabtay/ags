/* Adventure Creator v2 Run-time engine
   Started 27-May-99 (c) 1999-2011 Chris Jones

  Adventure Game Studio source code Copyright 1999-2011 Chris Jones.
  All rights reserved.

  The AGS Editor Source Code is provided under the Artistic License 2.0
  http://www.opensource.org/licenses/artistic-license-2.0.php

  You MAY NOT compile your own builds of the engine without making it EXPLICITLY
  CLEAR that the code has been altered from the Standard Version.

*/

//
// Engine initialization
//

#include "main/mainheader.h"
#include "ac/game.h"
#include "ac/gamesetup.h"
#include "ac/gamesetupstruct.h"
#include "ac/objectcache.h"
#include "ac/roomstatus.h"
#include "main/graphics_mode.h"
#include "main/engine.h"
#include "main/main.h"
#include "ac/gui.h"
#include "ac/viewframe.h"
#include "media/audio/sound.h"
#include "ac/spritecache.h"
#include "ac/dynobj/scriptobject.h"

#if defined(MAC_VERSION) || (defined(LINUX_VERSION) && !defined(PSP_VERSION))
#include <pthread.h>
pthread_t soundthread;
#endif

#ifndef WINDOWS_VERSION
extern char **global_argv;
#endif


extern char* game_file_name;
extern char check_dynamic_sprites_at_exit;
extern int our_eip;
extern volatile char want_exit, abort_engine;
extern GameSetup usetup;
extern GameSetupStruct game;
extern RoomStatus *roomstats;
extern int proper_exit;
extern char pexbuf[STD_BUFFER_SIZE];
extern char saveGameDirectory[260];
extern int spritewidth[MAX_SPRITES],spriteheight[MAX_SPRITES];
extern SpriteCache spriteset;
extern ObjectCache objcache[MAX_INIT_SPR];
extern ScriptObject scrObj[MAX_INIT_SPR];
extern ViewStruct*views;
extern GUIMain*guis;
extern int displayed_room;
extern int eip_guinum;
extern int eip_guiobj;

char *music_file;
char *speech_file;
WCHAR directoryPathBuffer[MAX_PATH];

int errcod;


const char *get_engine_version()
{
    return "";
}

void		show_preload ()
{
}

void		init_game_settings()
{
}

int         initialize_engine(int argc,char*argv[])
{
    return 0;
}

int			initialize_engine_with_exception_handling(int argc,char*argv[])
{
    return 0;
}


#ifdef WINDOWS_VERSION
// in ac_minidump
extern int CustomExceptionHandler (LPEXCEPTION_POINTERS exinfo);
extern EXCEPTION_RECORD excinfo;
extern int miniDumpResultCode;
#endif

// defined in main/main
extern char tempmsg[100];
extern char*printfworkingspace;
