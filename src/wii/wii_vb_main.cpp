/*
WiirtualBoy : Wii port of the Mednafen Virtual Boy emulator

Copyright (C) 2011
raz0red (www.twitchasylum.com)

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1.	The origin of this software must not be misrepresented; you
must not claim that you wrote the original software. If you use
this software in a product, an acknowledgment in the product
documentation would be appreciated but is not required.

2.	Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3.	This notice may not be removed or altered from any source
distribution.
*/

#include "main.h"

#include "wii_app.h"
#include "wii_gx.h"
#include "wii_main.h"
#include "wii_sdl.h"

#include "wii_vb.h"
#include "wii_Vb_input.h"
#include "wii_vb_main.h"
#include "wii_vb_sdl.h"
#include "wii_vb_sound.h"

#ifdef WII_NETTRACE
#include <network.h>
#include "net_print.h"  
#endif

extern "C" 
{
void WII_VideoStart();
void WII_VideoStop();
void WII_ChangeSquare(int xscale, int yscale, int xshift, int yshift);
void WII_SetRenderCallback( void (*cb)(void) );
Mtx gx_view;
}

// Forward references
static void gxrender_callback();

// Mednafen external references
extern int mednafen_main( int argc, char *argv[] );            
extern void CalcFramerates(char *virtfps, char *drawnfps, char *blitfps, size_t maxlen);

/*
 * Initializes the emulator
 */
void wii_vb_init()
{
}

/*
 * Loads the specified game
 *
 * game     The name of the game
 * return   1 if the load is successful, 0 if it fails
 */
int wii_vb_load_game( char* game )
{
#if 0
  return LoadGame( NULL, game );
#endif

// 
// TODO: This is a total hack for now. We are just calling the Mednafen
//       main method. Ultimately, the init, load, resume, save, etc. 
//       functionality will be extracted so it can be called from the 
//       front-end.
//

  wii_sdl_black_back_surface();
  WII_SetRenderCallback( &gxrender_callback );
  WII_VideoStart();     
  WII_ChangeSquare( wii_screen_x, wii_screen_y, 0, 0 );

  char* cmdline[] = { "", game };
  mednafen_main( sizeof(cmdline)/sizeof(char *), cmdline );

  exit( 0 );
}

#define CB_PIXELSIZE 14
#define CB_H CB_PIXELSIZE
#define CB_PADDING 2
#define CB_X -310
#define CB_Y 196

/*
 * GX render callback
 */
static void gxrender_callback()
{
  static int callback = 0;

  GX_SetVtxDesc( GX_VA_POS, GX_DIRECT );
  GX_SetVtxDesc( GX_VA_CLR0, GX_DIRECT );
  GX_SetVtxDesc( GX_VA_TEX0, GX_NONE );

  Mtx m;    // model matrix.
  Mtx mv;   // modelview matrix.

  guMtxIdentity( m ); 
  guMtxTransApply( m, m, 0, 0, -100 );
  guMtxConcat( gx_view, m, mv );
  GX_LoadPosMtxImm( mv, GX_PNMTX0 ); 

  if( wii_debug )
  {    
    static char virtfps[64];
    static char drawnfps[64];
    static char blitfps[64];
    static char text[256] = "";

    if( callback++ % 60 == 0 )
    {
      CalcFramerates( virtfps, drawnfps, blitfps, 64 );  
    }
    sprintf( text, "%s %s %s", virtfps, drawnfps, blitfps );

    GXColor color = (GXColor){0x0, 0x0, 0x0, 0x80};                       
    wii_gx_drawrectangle( 
      CB_X + -CB_PADDING, 
      CB_Y + CB_H + CB_PADDING, 
      wii_gx_gettextwidth( CB_PIXELSIZE, text ) + (CB_PADDING<<1), 
      CB_H + (CB_PADDING<<1), 
      color, TRUE );

    wii_gx_drawtext( CB_X, CB_Y, CB_PIXELSIZE, text, ftgxWhite, FTGX_ALIGN_BOTTOM ); 
  }
}

/*
 * The emulation loop
 */
void wii_vb_emu_loop()
{
}