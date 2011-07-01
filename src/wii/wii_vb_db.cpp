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

#include "wii_app.h"
#include "wii_util.h"

#include "wii_vb.h"
#include "wii_vb_db.h"
#include "wii_vb_input.h"

#ifdef WII_NETTRACE
#include <network.h>
#include "net_print.h"  
#endif

/*
 * The virtual boy values and their associated names
 */
VbButton VbButtons[VB_BUTTON_COUNT] =
{ 
  { "(none)",   VB_NONE        },
  { "A",        VB_KEY_A       }, 
  { "B",        VB_KEY_B       }, 
  { "L",        VB_KEY_L       },
  { "R",        VB_KEY_R       },
  { "Start",    VB_KEY_START   },
  { "Select",   VB_KEY_SELECT  },
  { "L-Left",   VB_L_LEFT      },
  { "L-Right",  VB_L_RIGHT     },
  { "L-Up",     VB_L_UP        },
  { "L-Down",   VB_L_DOWN      },
  { "R-Left",   VB_R_LEFT      },
  { "R-Right",  VB_R_RIGHT     },
  { "R-Up",     VB_R_UP        },
  { "R-Down",   VB_R_DOWN      }
};

#define VBK_MAP_NONE    0
#define VBK_MAP_A       1
#define VBK_MAP_B       2
#define VBK_MAP_L       3
#define VBK_MAP_R       4
#define VBK_MAP_START   5
#define VBK_MAP_SELECT  6
#define VBK_MAP_LLEFT   7
#define VBK_MAP_LRIGHT  8
#define VBK_MAP_LUP     9
#define VBK_MAP_LDOWN   10
#define VBK_MAP_RLEFT   11
#define VBK_MAP_RRIGHT  12
#define VBK_MAP_RUP     13
#define VBK_MAP_RDOWN   14

/*
 * The names of the Wii controllers 
 */
const char* WiiControllerNames[WII_CONTROLLER_COUNT] =
{
  "Wiimote", "Wiimote + Nunchuk", "Classic", "GameCube"
};

/*
 * Descriptions of the different Wii mappable buttons. 
 */
WiiButton WiiButtons[WII_CONTROLLER_COUNT][WII_MAP_BUTTON_COUNT] =
{ 
  {
    { "Plus",   WPAD_BUTTON_PLUS,   VBK_MAP_START  },
    { "Minus",  WPAD_BUTTON_MINUS,  VBK_MAP_SELECT },
    { "2",      WPAD_BUTTON_2,      VBK_MAP_A      }, 
    { "1",      WPAD_BUTTON_1,      VBK_MAP_B      },
    { "A",      WPAD_BUTTON_A,      VBK_MAP_R      },
    { "B",      WPAD_BUTTON_B,      VBK_MAP_L      },
    { NULL,     0,                  VBK_MAP_NONE   },
    { NULL,     0,                  VBK_MAP_NONE   },
    { NULL,     0,                  VBK_MAP_NONE   },
    { NULL,     0,                  VBK_MAP_NONE   }
  },
  {
    { "Plus",   WPAD_BUTTON_PLUS,       VBK_MAP_START  },
    { "Minus",  WPAD_BUTTON_MINUS,      VBK_MAP_SELECT },
    { "2",      WPAD_BUTTON_2,          VBK_MAP_NONE   }, 
    { "1",      WPAD_BUTTON_1,          VBK_MAP_NONE   },
    { "A",      WPAD_BUTTON_A,          VBK_MAP_A      },
    { "B",      WPAD_BUTTON_B,          VBK_MAP_R      },
    { "C",      WPAD_NUNCHUK_BUTTON_C,  VBK_MAP_B      },
    { "Z",      WPAD_NUNCHUK_BUTTON_Z,  VBK_MAP_L      },
    { NULL,     0,                      VBK_MAP_NONE   },
    { NULL,     0,                      VBK_MAP_NONE   }

  },
  {
    { "Plus",   WPAD_CLASSIC_BUTTON_PLUS,     VBK_MAP_START  },
    { "Minus",  WPAD_CLASSIC_BUTTON_MINUS,    VBK_MAP_SELECT },
    { "A",      WPAD_CLASSIC_BUTTON_A,        VBK_MAP_A      }, 
    { "B",      WPAD_CLASSIC_BUTTON_B,        VBK_MAP_B      },
    { "X",      WPAD_CLASSIC_BUTTON_X,        VBK_MAP_NONE   },
    { "Y",      WPAD_CLASSIC_BUTTON_Y,        VBK_MAP_NONE   },
    { "R",      WPAD_CLASSIC_BUTTON_FULL_R,   VBK_MAP_R      },
    { "L",      WPAD_CLASSIC_BUTTON_FULL_L,   VBK_MAP_L      },
    { "zR",     WPAD_CLASSIC_BUTTON_ZR,       VBK_MAP_R      },
    { "zL",     WPAD_CLASSIC_BUTTON_ZL,       VBK_MAP_L      }
  },
  {
    { "Start",  PAD_BUTTON_START,             VBK_MAP_START  },
    { "A",      PAD_BUTTON_A,                 VBK_MAP_A      },
    { "B",      PAD_BUTTON_B,                 VBK_MAP_B      }, 
    { "X",      PAD_BUTTON_X,                 VBK_MAP_SELECT },
    { "Y",      PAD_BUTTON_Y,                 VBK_MAP_SELECT },
    { "R",      PAD_TRIGGER_R,                VBK_MAP_R      },
    { "L",      PAD_TRIGGER_L,                VBK_MAP_L      },
    { NULL,     0,                            VBK_MAP_NONE   },
    { NULL,     0,                            VBK_MAP_NONE   },
    { NULL,     0,                            VBK_MAP_NONE   },
  }
};

#define DB_FILE_PATH WII_FILES_DIR      "wiivb.db"
#define DB_TMP_FILE_PATH WII_FILES_DIR  "wiivb.db.tmp"
#define DB_OLD_FILE_PATH WII_FILES_DIR  "wiivb.db.old"

// The database file
static char db_file[WII_MAX_PATH] = "";
// The database temp file
static char db_tmp_file[WII_MAX_PATH] = "";
// The database old file
static char db_old_file[WII_MAX_PATH] = "";

/*
 * Returns the path to the database file
 *
 * return   The path to the database file
 */
static char* get_db_path()
{
  if( db_file[0] == '\0' )
  {
    snprintf( 
      db_file, WII_MAX_PATH, "%s%s", wii_get_fs_prefix(), 
      DB_FILE_PATH );
  }

  return db_file;
}

/*
 * Returns the path to the database temporary file
 *
 * return   The path to the database temporary file
 */
static char* get_db_tmp_path()
{
  if( db_tmp_file[0] == '\0' )
  {
    snprintf( 
      db_tmp_file, WII_MAX_PATH, "%s%s", wii_get_fs_prefix(), 
      DB_TMP_FILE_PATH );
  }

  return db_tmp_file;
}

/*
 * Returns the path to the database old file
 *
 * return   The path to the database old file
 */
static char* get_db_old_path()
{
  if( db_old_file[0] == '\0' )
  {
    snprintf( 
      db_old_file, WII_MAX_PATH, "%s%s", wii_get_fs_prefix(), 
      DB_OLD_FILE_PATH );
  }

  return db_old_file;
}

/*
 * Populates the specified entry with default values.
 *
 * entry      The entry to populate with default values
 * fullClear  Whether to fully clear the entry
 */
void wii_vb_db_get_defaults( VbDbEntry* entry )
{
  entry->frameSkip = 0;
  entry->renderRate = MAX_RENDER_RATE;
  entry->wiimoteSupported = 1;
  entry->romPatch = ROM_PATCH_DEFAULT;
  memset( entry->buttonMap, 0x0, sizeof(entry->buttonMap) );

  // Set the default button map values
  for( int i = 0; i < WII_CONTROLLER_COUNT; i++ )
  {
    for( int j = 0; j < WII_MAP_BUTTON_COUNT; j++ )
    {
      entry->buttonMap[i][j] = WiiButtons[i][j].defaultMapping;
    }
  }

  wii_vb_db_apply_button_map( entry );
}


/*
 * Applies (expands) the wii button values for the currently mapped 
 * buttons. The result is stored in the "appliedButtonMap".
 *
 * entry      The entry to apply the button map to
 */
void wii_vb_db_apply_button_map( VbDbEntry* entry )
{
  memset( entry->appliedButtonMap, 0x0, sizeof(entry->appliedButtonMap) );

  for( int i = 0; i < WII_CONTROLLER_COUNT; i++ )
  {
    for( int j = 0; j < WII_MAP_BUTTON_COUNT; j++ )
    {
      u8 mappedButton = entry->buttonMap[i][j];
      if( mappedButton != VBK_MAP_NONE )
      {
        entry->appliedButtonMap[i][mappedButton] |= WiiButtons[i][j].button;
      }
    }
  }
}

/*
 * Attempts to locate a hash in the specified source string. If it
 * is found, it is copied into dest.
 *
 * source   The source string
 * dest     The destination string
 * return   non-zero if the hash was found and copied
 */
static int get_hash( char* source, char* dest )
{
  int db_hash_len = 0;  // The length of the hash
  char *end_idx;		    // End index of the hash  
  char *start_idx;      // Start index of the hash

  start_idx = source;
  if( *start_idx == '[' )
  {
    ++start_idx;
    end_idx = strrchr( start_idx, ']' );
    if( end_idx != 0 )
    {				  			
      db_hash_len = end_idx - start_idx;  
      strncpy( dest, start_idx, db_hash_len );		  
      dest[db_hash_len] = '\0';
      return 1;
    }    
  }

  return 0;
} 

/*
 * Writes the database entry to the specified file
 *
 * file	  The file to write the entry to
 * hash	  The hash for the entry
 * entry  The entry
 */
static void write_entry( FILE* file, char* hash, VbDbEntry *entry )
{
  int i;

  if( !entry ) return;

  char hex[64] = "";

  fprintf( file, "[%s]\n", hash );
  fprintf( file, "name=%s\n", entry->name );
  fprintf( file, "frameSkip=%d\n", entry->frameSkip );
  fprintf( file, "renderRate=%d\n", entry->renderRate );
  fprintf( file, "wiimoteSupported=%d\n", entry->wiimoteSupported );
  fprintf( file, "romPatch=%d\n", entry->romPatch );

  for( int i = 0; i < WII_CONTROLLER_COUNT; i++ )
  {
    for( int j = 0; j < WII_MAP_BUTTON_COUNT; j++ )
    {
      u8 val = entry->buttonMap[i][j];
      if( val != WiiButtons[i][j].defaultMapping )
      {
        fprintf( file, "btn.%d.%d=%d\n", i, j, val );
      }
    }
  }

  for( i = 1; i < VB_BUTTON_COUNT; i++ )
  {
    char* desc = entry->buttonDesc[i];
    if( desc[0] != '\0' )
    {      
      fprintf( file, "btnDesc%d=%s\n", i, desc );
    }
  }
}

/*
 * Returns the database entry for the game with the specified hash
 *
 * hash	  The hash of the game
 * entry  The entry to populate for the specified game
 */
void wii_vb_db_get_entry( char* hash, VbDbEntry* entry )
{
  char buff[255];     // The buffer to use when reading the file    
  FILE* db_file;      // The database file
  char db_hash[255];  // A hash found in the file we are reading from
  int read_mode = 0;  // The current read mode 
  char* ptr;          // Pointer into the current entry value

  // Populate the entry with the defaults
  memset( entry, 0x0, sizeof( VbDbEntry ) );
  wii_vb_db_get_defaults( entry );  

  db_file = fopen( get_db_path(), "r" );

  if( db_file != 0 )
  {	
    while( fgets( buff, sizeof(buff), db_file ) != 0 )
    {                
      if( read_mode == 2 )
      {
        // We moved past the current record, exit.
        break;
      }
      
      if( read_mode == 1 )
      {
        // Read from the matching database entry        
        ptr = strchr( buff, '=' );
        if( ptr )
        {
          *ptr++ = '\0';
          Util_trim( buff );
          Util_trim( ptr );
          
          if( !strcmp( buff, "name" ) )
          {
            Util_strlcpy( entry->name, ptr, sizeof(entry->name) );          
          }
          else if( !strcmp( buff, "frameSkip" ) )
          {
            entry->frameSkip = Util_sscandec( ptr );
          }          
          else if( !strcmp( buff, "renderRate" ) )
          {
            entry->renderRate = Util_sscandec( ptr );
          }
          else if( !strcmp( buff, "wiimoteSupported" ) )
          {
            entry->wiimoteSupported = Util_sscandec( ptr );
          }
          else if( !strcmp( buff, "romPatch" ) )
          {
            entry->romPatch = Util_sscandec( ptr );
          }

          int i;
          bool btnFound = false;
          for( i = 0; !btnFound && i < WII_CONTROLLER_COUNT; i++ )
          {
            for( int j = 0; !btnFound && j < WII_MAP_BUTTON_COUNT; j++ )
            {
              char btnName[64];
              snprintf( btnName, sizeof(btnName), "btn.%d.%d", i, j );
              if( !strcmp( buff, btnName ) )
              {
                entry->buttonMap[i][j] = Util_sscandec( ptr );
                btnFound = true;
              }
            }
          }

          btnFound = false;
          for( i = 1; !btnFound && i < VB_BUTTON_COUNT; i++ )
          {
            char button[255];
            snprintf( button, 
              sizeof(button), "btnDesc%d", i );
            if( !strcmp( buff, button ) )
            {
              Util_strlcpy( entry->buttonDesc[i], 
                ptr, sizeof(entry->buttonDesc[i]) );
              btnFound = true;
            }          
          }
        }                
      }
    
      // Search for the hash
      if( get_hash( buff, db_hash ) && read_mode < 2 )
      {        
        if( read_mode || !strcmp( hash, db_hash ) )
        {
          entry->loaded = 1;
          read_mode++;        
        }                
      }
    }

    fclose( db_file );
  }
}

/*
 * Deletes the entry from the database with the specified hash
 *
 * hash		The hash of the game
 * return	Whether the delete was successful
 */
int wii_vb_db_delete_entry( char* hash )
{
  return wii_vb_db_write_entry( hash, 0 );
}

/*
 * Writes the specified entry to the database for the game with the specified
 * hash.
 *
 * hash		The hash of the game
 * entry	The entry to write to the database (null to delete the entry)
 * return	Whether the write was successful
 */
int wii_vb_db_write_entry( char* hash, VbDbEntry *entry )
{  
  char buff[255];		    // The buffer to use when reading the file  
  char db_hash[255];	  // A hash found in the file we are reading from
  int copy_mode = 0;	  // The current copy mode 
  FILE* tmp_file = 0;	  // The temp file
  FILE* old_file = 0;	  // The old file

  // The database file
  FILE* db_file = fopen( get_db_path(), "r" );

  // A database file doesn't exist, create a new one
  if( !db_file )
  {
    db_file = fopen( get_db_path(), "w" );
    if( !db_file )
    {
      // Unable to create DB file
      return 0;
    }

    // Write the entry
    write_entry( db_file, hash, entry );

    fclose( db_file );
  }  
  else
  {
    //
    // A database exists, search for the appropriate hash while copying
    // its current contents to a temp file
    //

    // Open up the temp file
    tmp_file = fopen( get_db_tmp_path(), "w" );
    if( !tmp_file )
    {
      fclose( db_file );

      // Unable to create temp file
      return 0;
    }

    //
    // Loop and copy	
    //

    while( fgets( buff, sizeof(buff), db_file ) != 0 )
    {	  	            
      // Check if we found a hash
      if( copy_mode < 2 && get_hash( buff, db_hash ) )
      {
        if( copy_mode )
        {
          copy_mode++;
        }
        else if( !strcmp( hash, db_hash ) )
        {		  
          // We have matching hashes, write out the new entry
          write_entry( tmp_file, hash, entry );
          copy_mode++;
        }
      }

      if( copy_mode != 1 )
      {	
        fprintf( tmp_file, "%s", buff );
      }
    }

    if( !copy_mode )
    {
      // We didn't find the hash in the database, add it
      write_entry( tmp_file, hash, entry );	
    }

    fclose( db_file );
    fclose( tmp_file );  

    //
    // Make sure the temporary file exists
    // We do this due to the instability of the Wii SD card
    //
    tmp_file = fopen( get_db_tmp_path(), "r" );
    if( !tmp_file )
    {      
      // Unable to find temp file
      return 0;
    }
    fclose( tmp_file );

    // Delete old file (if it exists)
    if( ( old_file = fopen( get_db_old_path(), "r" ) ) != 0 )
    {
      fclose( old_file );
      if( remove( get_db_old_path() ) != 0 )
      {
        return 0;
      }
    }

    // Rename database file to old file
    if( rename( get_db_path(), get_db_old_path() ) != 0 )
    {
      return 0;
    }

    // Rename temp file to database file	
    if( rename( get_db_tmp_path(), get_db_path() ) != 0 )
    {
      return 0;
    }
  }

  return 1;
}

/*
 * Whether to patch ROMs
 *
 * entry    The database entry
 * return   Whether to patch ROMs
 */
BOOL wii_rom_patching_enabled( VbDbEntry *entry )
{
  return
    ( ( entry->romPatch == ROM_PATCH_ENABLED ) ||
      ( ( entry->romPatch == ROM_PATCH_DEFAULT ) &&
        ( wii_patch_rom ) ) );
}
