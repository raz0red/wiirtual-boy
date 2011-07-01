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

#include "main.h"

#include "wii_app.h"
#include "wii_config.h"
#include "wii_input.h"
#include "wii_sdl.h"
#include "wii_snapshot.h"

#include "wii_vb.h"
#include "wii_vb_main.h"

#ifdef WII_NETTRACE
#include <network.h>
#include "net_print.h"  
#endif

extern "C" void WII_VideoStart();
extern "C" void WII_ChangeSquare(int xscale, int yscale, int xshift, int yshift);

namespace MDFN_IEN_VB
{
  extern bool PatchROM(bool checkROM);
}

/*
 * Starts the emulator for the specified rom file.
 *
 * romfile  The rom file to run in the emulator
 * savefile The name of the save file to load. If this value is NULL, no save
 *          is explicitly loaded (auto-load may occur). If the value is "", 
 *          no save is loaded and auto-load is ignored (used for reset).
 * reset    Whether we are resetting the current game
 * resume   Whether we are resuming the current game
 */
void wii_start_emulation( char *romfile, const char *savefile, bool reset, bool resume )
{
  // Write out the current config
  wii_write_config();

  bool succeeded = true;
  char autosavename[WII_MAX_PATH] = "";

  // Determine the name of the save file
  if( wii_auto_save_state || wii_auto_load_state )
  {
    wii_snapshot_handle_get_name( romfile, autosavename );
  }

  // If a specific save file was not specified, and we are auto-loading 
  // see if a save file exists
  if( ( savefile == NULL ) &&
      ( wii_auto_load_state && 
        wii_check_snapshot( autosavename ) == 0 ) )
  {
    savefile = autosavename;
  }        

  // Start emulation
  if( !resume )
  {
    // Clear the DB entry
    memset( &wii_vb_db_entry, 0x0, sizeof( VbDbEntry ) );
    wii_cartridge_hash[0] = '\0'; // Reset the cartridge hash

    succeeded = ( wii_vb_load_game( romfile ) != 0 );    

    if( succeeded )
    {
      // Look up the cartridge in the database
      wii_vb_db_get_entry( wii_cartridge_hash, &wii_vb_db_entry );
      wii_current_rom_patched =
        MDFN_IEN_VB::PatchROM( wii_rom_patching_enabled( &wii_vb_db_entry ) );      
    
#ifdef WII_NETTRACE
      net_print_string( NULL, 0, "%s, %s, %s, %s\n", 
        wii_vb_db_entry.name, 
        wii_cartridge_hash_with_header,
        wii_cartridge_hash,
        ( wii_vb_db_entry.frameSkip ? "Yes" : "No" ) );
#endif

      // Load the save if applicable
      if( !reset && succeeded &&
          ( savefile != NULL && strlen( savefile ) > 0 ) )
      {
        // Ensure the save is valid
        int sscheck = wii_check_snapshot( savefile );
        if( sscheck < 0 )
        {
          if( sscheck == -2 )            
          {
            wii_set_status_message(
              "The save specified is not valid." );                
          }
          else
          {
            wii_set_status_message(
              "Unable to find the specified save state file." );                
          }

          succeeded = false;
        }
        else
        {
          succeeded = MDFNI_LoadState( savefile, NULL );                    

          if( !succeeded )
          {
            wii_set_status_message(
              "Error loading the specified save state file." );                
          }
        }
      }
    }
  }

  if( succeeded )
  {
    int retVal = 1;
    if( !resume && !reset )
    {
      // Wait until no buttons are pressed
      wii_wait_until_no_buttons( 2 );

      // Show the controls screen (if applicable)
      retVal = wii_vb_show_controls_screen();
    }

    if( retVal )
    {
      // Wait until no buttons are pressed
      wii_wait_until_no_buttons( 2 );

      // Start the emulator loop
      wii_vb_emu_loop( resume );            

      // Auto save?
      if( wii_auto_save_state )
      {
        wii_save_snapshot( autosavename, TRUE );
      }        
    }

    // Store the name of the last rom (for resuming later)        
    // Do it in this order in case they passed in the pointer
    // to the last rom variable
    char *last = strdup( romfile );
    if( wii_last_rom != NULL )
    {
      free( wii_last_rom );    
    }

    wii_last_rom = last;

    if( wii_top_menu_exit )
    {
      // Pop to the top
      while( wii_menu_pop() != NULL );
    }
  } 
  else
  {
    // Reset the last rom that was loaded
    if( wii_last_rom != NULL )
    {
      free( wii_last_rom );
      wii_last_rom = NULL;
    }
  }
}

/*
 * Resumes emulation of the current game
 */
void wii_resume_emulation()
{
  wii_start_emulation( wii_last_rom, "", false, true );
}

/*
 * Resets the current game
 */
void wii_reset_emulation()
{
  wii_start_emulation( wii_last_rom, "", true, false );
}