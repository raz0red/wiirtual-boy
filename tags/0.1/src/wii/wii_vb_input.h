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

#ifndef WII_VB_INPUT_H
#define WII_VB_INPUT_H

/*
 * Updates the controls
 */
extern void wii_vb_update_controls();

/** The current controls state */
extern u16 wii_paddata;

#define VB_NONE       0x0000
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

#endif
