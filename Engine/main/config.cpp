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
// Game configuration
//

#include "util/wgt2allg.h"
#include "ac/gamesetup.h"
#include "ac/gamestate.h"
#include "main/mainheader.h"
#include "main/config.h"
#include "ac/spritecache.h"

extern GameSetup usetup;
extern int spritewidth[MAX_SPRITES],spriteheight[MAX_SPRITES];
extern SpriteCache spriteset;
extern int psp_video_framedrop;
extern int psp_audio_enabled;
extern int psp_midi_enabled;
extern int psp_ignore_acsetup_cfg_file;
extern int psp_clear_cache_on_room_change;
extern int psp_midi_preload_patches;
extern int psp_audio_cachesize;
extern char psp_game_file_name[];
extern int psp_gfx_smooth_sprites;
extern char psp_translation[];
extern int force_letterbox;
extern char replayfile[MAX_PATH];
extern GameState play;

//char datname[80]="ac.clb";
char ac_conf_file_defname[MAX_PATH] = "acsetup.cfg";
char *ac_config_file = &ac_conf_file_defname[0];
char conffilebuf[512];

char filetouse[MAX_PATH] = "nofile";

// Replace the filename part of complete path WASGV with INIFIL
void INIgetdirec(char *wasgv, char *inifil) {
    

}

char *INIreaditem(const char *sectn, const char *entry) {
    FILE *fin = fopen(filetouse, "rt");
    if (fin == NULL)
        return NULL;

    char templine[200];
    char wantsect[100];
    sprintf (wantsect, "[%s]", sectn);

    while (!feof(fin)) {
        fgets (templine, 199, fin);
        // find the section
        if (strnicmp (wantsect, templine, strlen(wantsect)) == 0) {
            while (!feof(fin)) {
                // we're in the right section, find the entry
                fgets (templine, 199, fin);
                if (templine[0] == '[')
                    break;
                if (feof(fin))
                    break;
                // Strip CRLF
                char *lastchar = &templine[strlen(templine) -1];
                while(*lastchar == '\r' || *lastchar == '\n') {
                    *lastchar = 0;
                    lastchar--;
                }
                // Have we found the entry?
                if (strnicmp (templine, entry, strlen(entry)) == 0) {
                    char *pptr = &templine[strlen(entry)];
                    while ((pptr[0] == ' ') || (pptr[0] == '\t'))
                        pptr++;
                    if (pptr[0] == '=') {
                        pptr++;
                        while ((pptr[0] == ' ') || (pptr[0] == '\t'))
                            pptr++;
                        char *toret = (char*)malloc (strlen(pptr) + 5);
                        strcpy (toret, pptr);
                        fclose (fin);
                        return toret;
                    }
                }
            }
        }
    }
    fclose (fin);
    return NULL;
}

int INIreadint (const char *sectn, const char *item, int errornosect = 1) {
    char *tempstr = INIreaditem (sectn, item);
    if (tempstr == NULL)
        return -1;

    int toret = atoi(tempstr);
    free (tempstr);
    return toret;
}
