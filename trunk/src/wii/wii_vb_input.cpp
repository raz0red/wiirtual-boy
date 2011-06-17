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

#include "wii_input.h"

#include "wii_vb.h"
#include "wii_vb_main.h"
#include "wii_vb_input.h"

#ifdef WII_NETTRACE
#include <network.h>
#include "net_print.h"  
#endif

// Mednafen externs
extern int GameThreadRun;

// Current controls state
u16 wii_paddata = 0;

/*
 * Updates the controls
 */
void wii_vb_update_controls()
{
  WPAD_ScanPads();
  PAD_ScanPads();

  // Check the state of the controllers
  u32 pressed = WPAD_ButtonsDown( 0 );
  u32 held = WPAD_ButtonsHeld( 0 );  
  u32 gcPressed = PAD_ButtonsDown( 0 );
  u32 gcHeld = PAD_ButtonsHeld( 0 );

  // Classic or Nunchuck?
  expansion_t exp;
  WPAD_Expansion( 0, &exp );          

  BOOL isClassic = ( exp.type == WPAD_EXP_CLASSIC );
  BOOL isNunchuk = ( exp.type == WPAD_EXP_NUNCHUK );

  // Mask off the Wiimote d-pad depending on whether a nunchuk
  // is connected. (Wiimote d-pad is left when nunchuk is not
  // connected, right when it is).
  u32 heldLeft = ( isNunchuk ? ( held & ~0x0F00 ) : held );
  u32 heldRight = ( !isNunchuk ? ( held & ~0x0F00 ) : held );

  // Analog for Wii controls
  float expX = wii_exp_analog_val( &exp, TRUE, FALSE );
  float expY = wii_exp_analog_val( &exp, FALSE, FALSE );
  float expRX = isClassic ? wii_exp_analog_val( &exp, TRUE, TRUE ) : 0;
  float expRY = isClassic ? wii_exp_analog_val( &exp, FALSE, TRUE ) : 0;

  // Analog for Gamecube controls
  s8 gcX = PAD_StickX( 0 );
  s8 gcY = PAD_StickY( 0 );
  s8 gcRX = PAD_SubStickX( 0 );
  s8 gcRY = PAD_SubStickY( 0 );

  // Check for home

  if( ( pressed & WII_BUTTON_HOME ) ||
    ( gcPressed & GC_BUTTON_HOME ) ||
    wii_hw_button )
  {
    GameThreadRun = 0;
  }

  u16 result = 0;

  // Mapped buttons

  for( int i = 0; i < VB_BUTTON_COUNT; i++ )
  {
    if( ( held &
          ( ( isClassic ? 
                wii_vb_db_entry.appliedButtonMap[ 
                  WII_CONTROLLER_CLASSIC ][ i ] : 0 ) |
            ( isNunchuk ?
                wii_vb_db_entry.appliedButtonMap[
                  WII_CONTROLLER_CHUK ][ i ] :
                wii_vb_db_entry.appliedButtonMap[
                  WII_CONTROLLER_MOTE ][ i ] ) ) ) ||
        ( gcHeld &
            wii_vb_db_entry.appliedButtonMap[
              WII_CONTROLLER_CUBE ][ i ] ) )
    {
      result |= VbButtons[ i ].button;
    }
  }    

  // Left sticks and pads

  if( wii_digital_right( !isNunchuk, isClassic, heldLeft ) ||
      ( gcHeld & GC_BUTTON_RIGHT ) ||
      wii_analog_right( expX, gcX ) )
    result|=VB_L_RIGHT;

  if( wii_digital_left( !isNunchuk, isClassic, heldLeft ) || 
      ( gcHeld & GC_BUTTON_LEFT ) ||                       
      wii_analog_left( expX, gcX ) )
    result|=VB_L_LEFT;

  if( wii_digital_up( !isNunchuk, isClassic, heldLeft ) || 
      ( gcHeld & GC_BUTTON_UP ) ||
      wii_analog_up( expY, gcY ) )
    result|=VB_L_UP;

  if( wii_digital_down( !isNunchuk, isClassic, heldLeft ) ||
      ( gcHeld & GC_BUTTON_DOWN ) ||
      wii_analog_down( expY, gcY ) )
    result|=VB_L_DOWN;  

  // Right sticks and pads

  if( wii_digital_right( !isNunchuk, 0, heldRight ) || 
      wii_analog_right( expRX, gcRX ) )
    result|=VB_R_RIGHT;

  if( wii_digital_left( !isNunchuk, 0, heldRight ) || 
      wii_analog_left( expRX, gcRX ) )
    result|=VB_R_LEFT;

  if( wii_digital_up( !isNunchuk, 0, heldRight ) || 
      wii_analog_up( expRY, gcRY ) )
    result|=VB_R_UP;

  if( wii_digital_down( !isNunchuk, 0, heldRight ) ||  
      wii_analog_down( expRY, gcRY ) )
    result|=VB_R_DOWN;

  // Always have to do this...

  result|=0x0002;

  wii_paddata = result;
}
