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

#include "wii_input.h"

#include "wii_vb.h"
#include "wii_vb_main.h"

// Mednafen externs
extern int GameThreadRun;

// Current controls state
u16 wii_paddata = 0;

#define VB_KEY_A      0x0004
#define VB_KEY_B      0x0008
#define VB_KEY_R      0x0010
#define VB_KEY_L      0x0020
#define VB_KEY_START  0x1000
#define VB_KEY_SELECT 0x2000
#define VB_L_RIGHT    0x0100
#define VB_L_LEFT     0x0200
#define VB_L_DOWN     0x0400
#define VB_L_UP       0x0800
#define VB_R_RIGHT    0x0080
#define VB_R_LEFT     0x4000
#define VB_R_DOWN     0x8000
#define VB_R_UP       0x0040

/*
 * Updates the controls
 */
void wii_vb_update_controls()
{

//
// TODO: This is just temporary. Ultimately there will be a cartridge 
//       database control mappings.
//

  // Getting events for keyboard and/or joypad handling
  WPAD_ScanPads();
  PAD_ScanPads();

  // Check the state of the controllers
  u32 pressed = WPAD_ButtonsDown( 0 );
  u32 held = WPAD_ButtonsHeld( 0 );
  u32 gcPressed = PAD_ButtonsDown( 0 );
  u32 gcHeld = PAD_ButtonsHeld( 0 );

  // Analog controls
  expansion_t exp;
  WPAD_Expansion( 0, &exp );        
  float expX = wii_exp_analog_val( &exp, TRUE, FALSE );
  float expY = wii_exp_analog_val( &exp, FALSE, FALSE );
  float expRX = wii_exp_analog_val( &exp, TRUE, TRUE );
  float expRY = wii_exp_analog_val( &exp, FALSE, TRUE );
  s8 gcX = PAD_StickX( 0 );
  s8 gcY = PAD_StickY( 0 );
  s8 gcRX = PAD_SubStickX( 0 );
  s8 gcRY = PAD_SubStickY( 0 );

  // Classic or Nunchuck?
  BOOL isClassic = ( exp.type == WPAD_EXP_CLASSIC );

  if( ( pressed & WII_BUTTON_HOME ) ||
    ( gcPressed & GC_BUTTON_HOME ) ||
    wii_hw_button )
  {
    GameThreadRun = 0;
  }

  u16 result = 0;

  if( ( held & WII_BUTTON_VB_START ) ||
    ( gcHeld & GC_BUTTON_VB_START ) )
    result|=VB_KEY_START;

    if( ( held & WII_BUTTON_VB_SELECT ) ||
    ( gcHeld & GC_BUTTON_VB_SELECT ) )
    result|=VB_KEY_SELECT;

  if( held & ( WII_BUTTON_VB_A | 
      ( isClassic ? WII_CLASSIC_VB_A : WII_NUNCHUK_VB_A ) ) || 
      gcHeld & GC_BUTTON_VB_A )
    result|=VB_KEY_A;

  if( held & ( WII_BUTTON_VB_B | 
      ( isClassic ? WII_CLASSIC_VB_B : WII_NUNCHUK_VB_B ) ) || 
      gcHeld & GC_BUTTON_VB_B )
    result|=VB_KEY_B;

  if( held & ( WII_BUTTON_VB_L ) || 
    gcHeld & GC_BUTTON_VB_L )
    result|=VB_KEY_L;

  if( held & ( WII_BUTTON_VB_R ) || 
    gcHeld & GC_BUTTON_VB_R )
    result|=VB_KEY_R;

  if( wii_digital_right( TRUE, isClassic, held ) ||
      ( gcHeld & GC_BUTTON_RIGHT ) ||
      wii_analog_right( expX, gcX ) )
    result|=VB_L_RIGHT;

  if( wii_digital_left( TRUE, isClassic, held ) || 
      ( gcHeld & GC_BUTTON_LEFT ) ||                       
      wii_analog_left( expX, gcX ) )
    result|=VB_L_LEFT;

  if( wii_digital_up( TRUE, isClassic, held ) || 
      ( gcHeld & GC_BUTTON_UP ) ||
    wii_analog_up( expY, gcY ) )
    result|=VB_L_UP;

  if( wii_digital_down( TRUE, isClassic, held ) ||
      ( gcHeld & GC_BUTTON_DOWN ) ||
      wii_analog_down( expY, gcY ) )
    result|=VB_L_DOWN;

  if( wii_analog_right( expRX, gcRX ) )
  result|=VB_R_RIGHT;

  if( wii_analog_left( expRX, gcRX ) )
    result|=VB_R_LEFT;

  if( wii_analog_up( expRY, gcRY ) )
    result|=VB_R_UP;

  if( wii_analog_down( expRY, gcRY ) )
    result|=VB_R_DOWN;

  result|=0x0002;

  wii_paddata = result;
}
