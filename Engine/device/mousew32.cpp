/* MOUSELIBW32.CPP

  Library of mouse functions for graphics and text mode

  (c) 1994 Chris Jones

  Win32 (allegro) update (c) 1999 Chris Jones

  This is UNPUBLISHED PROPRIETARY SOURCE CODE;
  the contents of this file may not be disclosed to third parties,
  copied or duplicated in any form, in whole or in part, without
  prior express permission from Chris Jones.
*/

#if !defined(LINUX_VERSION) && !defined(MAC_VERSION)
#include <dos.h>
#include <conio.h>
#include <process.h>
#endif

#include <stdio.h>

#include "util/wgt2allg.h"
#if !defined(WINDOWS_VERSION) && !defined(LINUX_VERSION) && !defined(MAC_VERSION)
#error This is the Windows 32-bit version
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#include "device/mousew32.h"


/*
int  minstalled();    // this returns number of buttons (or 0)
void minst();   // this exits if not installed
void mshow();
void mhide();
int  mgetbutton();
void mchangestyle(int,int);
void mgetpos();
void mgetgraphpos();
void msetpos(int,int);
void mconfine(int,int,int,int); // left top right bottom
void mgraphconfine(int,int,int,int);
int  mbutrelease(int);
void domouse(int=0);   // graphics mode cursor
void mfreemem();
void mloadcursor(char*);  // load from file
void mloadwcursor(char*);
void mnewcursor(char);
int  ismouseinbox(int,int,int,int);
void msethotspot(int,int);   // Graphics mode only. Useful for crosshair.
*/
void msetgraphpos(int,int);

extern long cliboffset(char *);
extern char lib_file_name[13];
extern void put_sprite_256(int, int, block);

char *mouselibcopyr = "MouseLib32 (c) 1994, 1998 Chris Jones";
const int NONE = -1, LEFT = 0, RIGHT = 1, MIDDLE = 2;
int aa;
char mouseturnedon = FALSE, currentcursor = 0;
int mousex = 0, mousey = 0, numcurso = -1, hotx = 0, hoty = 0;
int boundx1 = 0, boundx2 = 99999, boundy1 = 0, boundy2 = 99999;
int disable_mgetgraphpos = 0;
char ignore_bounds = 0;
extern char alpha_blend_cursor ;
block savebk = NULL, mousecurs[MAXCURSORS];
extern int vesa_xres, vesa_yres;
extern color palette[256];


IMouseGetPosCallback *callback = NULL;

void msetcallback(IMouseGetPosCallback *gpCallback) {
  
}

void mgraphconfine(int x1, int y1, int x2, int y2)
{
  
}

void mgetgraphpos()
{
  
}

void msetcursorlimit(int x1, int y1, int x2, int y2)
{
  
}

// [IKM] TODO: move this code to game's CMouseCursor class
// or something like that; device class should not touch
// any cursor types and sprites at all
void drawCursor() {
  if (alpha_blend_cursor) {
    set_alpha_blender();
    draw_trans_sprite(abuf, mousecurs[currentcursor], mousex, mousey);
  }
  else
    put_sprite_256(mousex, mousey, mousecurs[currentcursor]);
}

// [IKM] probably move this to game's CMouseCursor
int hotxwas = 0, hotywas = 0;
void domouse(int str)
{
  /*
     TO USE THIS ROUTINE YOU MUST LOAD A MOUSE CURSOR USING mloadcursor.
     YOU MUST ALSO REMEMBER TO CALL mfreemem AT THE END OF THE PROGRAM.
  */
  int poow = wgetblockwidth(mousecurs[currentcursor]);
  int pooh = wgetblockheight(mousecurs[currentcursor]);
  int smx = mousex - hotxwas, smy = mousey - hotywas;

  mgetgraphpos();
  mousex -= hotx;
  mousey -= hoty;

  if (mousex + poow >= vesa_xres)
    poow = vesa_xres - mousex;

  if (mousey + pooh >= vesa_yres)
    pooh = vesa_yres - mousey;

  wclip(0, 0, vesa_xres - 1, vesa_yres - 1);
  if ((str == 0) & (mouseturnedon == TRUE)) {
    if ((mousex != smx) | (mousey != smy)) {    // the mouse has moved
      wputblock(smx, smy, savebk, 0);
      wfreeblock(savebk);
      savebk = wnewblock(mousex, mousey, mousex + poow, mousey + pooh);
      drawCursor();
    }
  }
  else if ((str == 1) & (mouseturnedon == FALSE)) {
    // the mouse is just being turned on
    savebk = wnewblock(mousex, mousey, mousex + poow, mousey + pooh);
    drawCursor();
    mouseturnedon = TRUE;
  }
  else if ((str == 2) & (mouseturnedon == TRUE)) {    // the mouse is being turned off
    if (savebk != NULL) {
      wputblock(smx, smy, savebk, 0);
      wfreeblock(savebk);
    }

    savebk = NULL;
    mouseturnedon = FALSE;
  }

  mousex += hotx;
  mousey += hoty;
  hotxwas = hotx;
  hotywas = hoty;
}

// [IKM] redundant
int ismouseinbox(int lf, int tp, int rt, int bt)
{
  if ((mousex >= lf) & (mousex <= rt) & (mousey >= tp) & (mousey <= bt))
    return TRUE;
  else
    return FALSE;
}

// [IKM] Not needed because we will use classes for bitmaps and cursors
void mfreemem()
{
  for (int re = 0; re < numcurso; re++) {
    if (mousecurs[re] != NULL)
      wfreeblock(mousecurs[re]);
  }
}

// [IKM] probably move this to game's CMouseCursor
void mnewcursor(char cursno)
{
  domouse(2);
  currentcursor = cursno;
  domouse(1);
}

// [IKM] loading sprites should be done totally different way;
// anyway this is to be moved to game's CMouseCursor
void mloadwcursor(char *namm)
{
  color dummypal[256];
  if (wloadsprites(&dummypal[0], namm, mousecurs, 0, MAXCURSORS)) {
    //printf("C_Load_wCursor: Error reading mouse cursor file\n"); 
    exit(1);
  }
}

int butwas = 0;
int mgetbutton()
{
    return 0;
}

// [IKM] redundant, use CMouseDevice::GetPressedButtons() & kMB_*
const int MB_ARRAY[3] = { 1, 2, 4 };
int misbuttondown(int buno)
{
  poll_mouse();
  if (mouse_b & MB_ARRAY[buno])
    return TRUE;
  return FALSE;
}

void msetgraphpos(int xa, int ya)
{ 
  
}

// [IKM] move this to game's CMouseCursor
void msethotspot(int xx, int yy)
{
  hotx = xx;  // mousex -= hotx; mousey -= hoty;
  hoty = yy;  // mousex += hotx; mousey += hoty;
}

int minstalled()
{
  return 0;
}
