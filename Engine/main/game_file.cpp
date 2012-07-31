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
// Game data file management
//

#include "main/mainheader.h"
#include "main/game_file.h"
#include "ac/common.h"
#include "ac/character.h"
#include "ac/charactercache.h"
#include "ac/dialogtopic.h"
#include "ac/draw.h"
#include "ac/gamesetupstruct.h"
#include "ac/gamestate.h"
#include "ac/gamestructdefines.h"
#include "ac/gui.h"
#include "ac/viewframe.h"
#include "ac/dynobj/all_dynamicclasses.h"
#include "ac/dynobj/all_scriptclasses.h"
#include "debug/debug.h"
#include "gui/guilabel.h"
#include "main/main.h"
#include "platform/base/agsplatformdriver.h"
#include "script/exports.h"
#include "script/script.h"



extern int engineNeedsAsInt; // defined in ac_game
extern char saveGameSuffix[MAX_SG_EXT_LENGTH + 1];

// Old dialog support
extern unsigned char** old_dialog_scripts; // defined in ac_conversation
extern char** old_speech_lines;

extern DynamicArray<GUILabel> guilabels; // defined in ac_guilabel
extern int numguilabels;

extern int ifacepopped;

extern GameSetupStruct game;
extern ViewStruct*views;
extern DialogTopic *dialog;
extern GUIMain*guis;
extern CharacterCache *charcache;
extern MoveList *mls;

extern CCGUIObject ccDynamicGUIObject;
extern CCCharacter ccDynamicCharacter;
extern CCHotspot   ccDynamicHotspot;
extern CCRegion    ccDynamicRegion;
extern CCInventory ccDynamicInv;
extern CCGUI       ccDynamicGUI;
extern CCObject    ccDynamicObject;
extern CCDialog    ccDynamicDialog;
extern ScriptString myScriptStringImpl;
extern ScriptObject scrObj[MAX_INIT_SPR];
extern ScriptGUI    *scrGui;
extern ScriptHotspot scrHotspot[MAX_HOTSPOTS];
extern ScriptRegion scrRegion[MAX_REGIONS];
extern ScriptInvItem scrInv[MAX_INV];
extern ScriptDialog scrDialog[MAX_DIALOG];

extern ScriptDialogOptionsRendering ccDialogOptionsRendering;
extern ScriptDrawingSurface* dialogOptionsRenderingSurface;

extern int our_eip;
extern int game_paused;

extern AGSPlatformDriver *platform;
extern ccScript* gamescript;
extern ccScript* dialogScriptsScript;
extern ccScript *scriptModules[MAX_SCRIPT_MODULES];
extern ccInstance *moduleInst[MAX_SCRIPT_MODULES];
extern ccInstance *moduleInstFork[MAX_SCRIPT_MODULES];
extern char *moduleRepExecAddr[MAX_SCRIPT_MODULES];
extern int numScriptModules;
extern GameState play;
extern char **characterScriptObjNames;
extern char objectScriptObjNames[MAX_INIT_SPR][MAX_SCRIPT_NAME_LEN + 5];
extern char **guiScriptObjNames;
extern int actSpsCount;
extern block *actsps;
extern IDriverDependantBitmap* *actspsbmp;
extern block *actspswb;
extern IDriverDependantBitmap* *actspswbbmp;
extern CachedActSpsData* actspswbcache;


int filever;
// PSP specific variables:
int psp_is_old_datafile = 0; // Set for 3.1.1 and 3.1.2 datafiles
char* game_file_name = NULL;


FILE * game_file_open()
{
	return NULL;
}

int game_file_read_version(FILE*iii)
{
    return 0;
}

void game_file_read_dialog_script(FILE*iii)
{
	
}

void game_file_read_script_modules(FILE*iii)
{
	
}

void game_file_read_views(FILE*iii)
{
	
}

void set_default_glmsg (int msgnum, const char* val) {
}

void game_file_set_default_glmsg()
{
    
}

void game_file_read_dialogs(FILE*iii)
{
	
}

void game_file_read_gui(FILE*iii)
{
	
}

void game_file_set_score_sound(GameSetupStruct::GAME_STRUCT_READ_DATA &read_data)
{
    
}

void init_and_register_characters()
{
	
}

void init_and_register_hotspots()
{
	
}

void init_and_register_regions()
{
	
}

void init_and_register_invitems()
{
	
}

void init_and_register_dialogs()
{
}

void init_and_register_guis()
{
	
}

void init_and_register_fonts()
{
	
}

void init_and_register_game_objects()
{       
}

int load_game_file() {

    return 0;
}
