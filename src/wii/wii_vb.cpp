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

#include "wii_app.h"
#include "wii_config.h"
#include "wii_input.h"
#include "wii_sdl.h"

#include "wii_vb.h"
#include "wii_vb_main.h"
#include "wii_vb_menu.h"

#include "FreeTypeGX.h"

#include "font_ttf.h"

extern "C" {
  void WII_VideoStop();
}

// The last cartridge hash
char wii_cartridge_hash[33];
// Whether to display debug info (FPS, etc.)
BOOL wii_debug = FALSE;
// Auto save state?
BOOL wii_auto_save_state = FALSE;
// Auto load state?
BOOL wii_auto_load_state = TRUE;
// The screen X size
int wii_screen_x = DEFAULT_SCREEN_X;
// The screen Y size
int wii_screen_y = DEFAULT_SCREEN_Y;
// Maximum frame rate
u8 wii_max_frames = DEFAULT_FPS;

// The list of available 3d modes
Vb3dMode wii_vb_modes[] =
{
  { "white_black", "White/black (2d)", 0xFFFFFF, 0x000000, false },
  { "red_black", "Red/black (2d)", 0xFF0000, 0x000000, false },
  { "red_blue", "Red/blue (3d)", 0xFF0000, 0x0000FF, true },
  { "red_green", "Red/green (3d)", 0xFF0000, 0x00FF00, true },
  { "yellow_blue", "Yellow/blue (3d)", 0xFFFF00, 0x0000FF, true },
};

int wii_vb_mode_count = sizeof( wii_vb_modes ) / sizeof( Vb3dMode );

// The current 3d mode
char wii_vb_mode_key[255] = DEFAULT_VB_MODE_KEY;

/*
 * Returns the index of the specified 3d mode key
 *
 * key    The 3d mode key
 * return The index of the 3d mode (or -1 if not found)
 */
int wii_get_vb_mode_index( const char* key )
{
  for( int i = 0; i < wii_vb_mode_count; i++ )
  {
    Vb3dMode mode = wii_vb_modes[i];
    if( !strcmp( mode.key, key ) )
    {
      return i;
    }
  }

  return -1;
}

/*
 * Returns the current 3d mode index
 *
 * return   The current 3d mode index
 */
int wii_get_vb_mode_index()
{
  int index = wii_get_vb_mode_index( wii_vb_mode_key );
  if( index == -1 )
  {
    index = wii_get_vb_mode_index( DEFAULT_VB_MODE_KEY );
  }
  return index;
}


/*
 * Returns the current 3d mode
 *
 * return   The current 3d mode
 */
Vb3dMode wii_get_vb_mode()
{
  return wii_vb_modes[wii_get_vb_mode_index()];
}

/*
 * Initializes the application
 */
void wii_handle_init()
{  
  // Read the config values
  wii_read_config();

  // Startup the SDL
  if( !wii_sdl_init() ) 
  {
    fprintf( stderr, "FAILED : Unable to init SDL: %s\n", SDL_GetError() );
    exit( EXIT_FAILURE );
  }

  // FreeTypeGX
  InitFreeType( (uint8_t*)font_ttf, (FT_Long)font_ttf_size  );

  // Initializes the menu
  wii_vb_menu_init();

  // Initializes emulator
  wii_vb_init();
}

/*
 * Frees resources prior to the application exiting
 */
void wii_handle_free_resources()
{  
  // We be done, write the config settings, free resources and exit
  wii_write_config();

  // Free resources from the emulator
  wii_vb_free();

  wii_sdl_free_resources();
  SDL_Quit();
}

/*
 * Runs the application (main loop)
 */
void wii_handle_run()
{
  WII_VideoStop();

  // Show the menu
  wii_menu_show();
}

// The roms dir
static char roms_dir[WII_MAX_PATH] = "";

/*
 * Returns the roms directory
 *
 * return   The roms directory
 */
char* wii_get_roms_dir()
{
  if( roms_dir[0] == '\0' )
  {
    snprintf( 
      roms_dir, WII_MAX_PATH, "%s%s", wii_get_fs_prefix(), WII_ROMS_DIR );
  }

  return roms_dir;
}

// The saves dir
static char saves_dir[WII_MAX_PATH] = "";

/*
 * Returns the saves directory
 *
 * return   The saves directory
 */
char* wii_get_saves_dir()
{
  if( saves_dir[0] == '\0' )
  {
    snprintf( 
      saves_dir, WII_MAX_PATH, "%s%s", wii_get_fs_prefix(), WII_SAVES_DIR );
  }

  return saves_dir;
}

// The base dir
static char base_dir[WII_MAX_PATH] = "";

/*
 * Returns the base directory
 *
 * return   The bae directory
 */
char* wii_get_base_dir()
{
  if( base_dir[0] == '\0' )
  {
    snprintf( 
      base_dir, WII_MAX_PATH, "%s%s", wii_get_fs_prefix(), WII_FILES_DIR );
    base_dir[strlen(base_dir)-1]='\0'; // Remove the trailing slash
  }

  return base_dir;
}

