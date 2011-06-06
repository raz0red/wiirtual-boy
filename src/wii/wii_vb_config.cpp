/*
WiirtualBoy : Wii port of the Mednafen Virtual Boy emulator

Copyright (C) 2011
raz0red and Arikado

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

#include <stdio.h>

#include "wii_util.h"

#include "wii_vb.h"

/*
* Handles reading a particular configuration value
*
* name   The name of the config value
* value  The config value
*/
extern "C" void wii_config_handle_read_value( char *name, char* value )
{
  if( strcmp( name, "DEBUG" ) == 0 )
  {
    wii_debug = Util_sscandec( value );				
  }
  else if( strcmp( name, "TOP_MENU_EXIT" ) == 0 )
  {
    wii_top_menu_exit = Util_sscandec( value );				
  }
  else if( strcmp( name, "AUTO_LOAD_STATE" ) == 0 )
  {
    wii_auto_load_state = Util_sscandec( value );				
  }
  else if( strcmp( name, "AUTO_SAVE_STATE" ) == 0 )
  {
    wii_auto_save_state = Util_sscandec( value );				
  }
  else if( strcmp( name, "VSYNC" ) == 0 )
  {
    wii_vsync = Util_sscandec( value );
  }
  else if( strcmp( name, "SEL_OFFSET" ) == 0 )
  {
    wii_menu_sel_offset = Util_sscandec( value );
  }
  else if( strcmp( name, "SEL_COLOR" ) == 0 )
  {
    Util_hextorgba( value, &wii_menu_sel_color );
  }
  else if( strcmp( name, "MOTE_MENU_VERTICAL" ) == 0 )
  {
    wii_mote_menu_vertical = Util_sscandec( value );				
  }
  else if( strcmp( name, "SCREEN_X" ) == 0 )
  {
    wii_screen_x = Util_sscandec( value );
  }
  else if( strcmp( name, "SCREEN_Y" ) == 0 )
  {
    wii_screen_y = Util_sscandec( value );
  }
  else if( strcmp( name, "SCREEN_MODE" ) == 0 )
  {
    Util_strlcpy( wii_vb_mode_key, value, sizeof(wii_vb_mode_key) );
  }
}

/*
* Handles the writing of the configuration file
*
* fp   The file pointer
*/
extern "C" void wii_config_handle_write_config( FILE *fp )
{
  fprintf( fp, "DEBUG=%d\n", wii_debug );
  fprintf( fp, "TOP_MENU_EXIT=%d\n", wii_top_menu_exit );
  fprintf( fp, "AUTO_LOAD_STATE=%d\n", wii_auto_load_state );
  fprintf( fp, "AUTO_SAVE_STATE=%d\n", wii_auto_save_state );
  fprintf( fp, "VSYNC=%d\n", wii_vsync );
  fprintf( fp, "SEL_OFFSET=%d\n", wii_menu_sel_offset );  
  fprintf( fp, "MOTE_MENU_VERTICAL=%d\n", wii_mote_menu_vertical );  
  fprintf( fp, "SCREEN_X=%d\n", wii_screen_x );
  fprintf( fp, "SCREEN_Y=%d\n", wii_screen_y );
  fprintf( fp, "SCREEN_MODE=%s\n", wii_vb_mode_key );
}

/*
* Writes the mednafen configuration file
*
* fp   The file pointer
* op   Option - Sets what mode to render in
*               1 - Magenta/Cyan (3D)
*				 2 - Black/White  (2D)
*				 3 - Red/Black    (2D)
*/
extern "C" void wii_mednafen_write_config( FILE *fp, int op )
{
  fprintf( fp, ";VERSION 0.9.0\n");
  fprintf( fp, ";Edit this file at your own risk!\n");
  fprintf( fp, ";File format: <key><single space><value><LF or CR+LF>\n\n");

  fprintf( fp, "vb.instant_display_hack 1\n");
  fprintf( fp, "vb.allow_draw_skip 1\n\n");

  fprintf( fp, ";3D mode.\n");
  fprintf( fp, "vb.3dmode anaglyph\n\n");

  fprintf( fp, ";Anaglyph maximum-brightness color for left view.\n");
  if(op == 2)
    fprintf( fp, "vb.anaglyph.lcolor 0xFFFFFF\n\n");
  if(op == 1 || op == 3)
    fprintf( fp, "vb.anaglyph.lcolor 0xFF0000\n\n");

  fprintf( fp, ";Anaglyph maximum-brightness color for right view.");
  if(op == 1)
    fprintf( fp, "vb.anaglyph.rcolor 0x0080FF\n\n");
  if(op == 2 || op == 3)
    fprintf( fp, "vb.anaglyph.rcolor 0x000000\n\n");

  fprintf( fp, ";Anaglyph preset colors.\n");
  if(op == 1)
    fprintf( fp, "vb.anaglyph.preset enabled\n\n");
  if(op == 2 || op == 3)
    fprintf( fp, "vb.anaglyph.preset disabled\n\n");

  fprintf( fp, ";Select CPU emulation mode.\n");
  fprintf( fp, "vb.cpu_emulation fast\n\n");

  fprintf( fp, ";Default maximum-brightness color to use in non-anaglyph 3D modes.\n");
  fprintf( fp, "vb.default_color 0xFFFFFF\n\n");

  fprintf( fp, ";Disable parallax for BG and OBJ rendering.\n");
  if(op == 1)
    fprintf( fp, "vb.disable_parallax 0\n\n");
  if(op == 2 || op == 3)
    fprintf( fp, "vb.disable_parallax 1\n\n");

  fprintf( fp, ";Force monophonic sound output.\n");
  fprintf( fp, "vb.forcemono 0");
}

