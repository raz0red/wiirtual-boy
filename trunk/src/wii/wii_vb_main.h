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

#ifndef WII_VB_MAIN_H
#define WII_VB_MAIN_H

/*
 * Initializes the emulator
 */
extern void wii_vb_init();

/**
 * Loads the specified game
 *
 * game     The name of the game
 * return   1 if the load is successful, 0 if it fails
 */
extern int wii_vb_load_game( char* game );

/*
 * The emulation loop
 */
extern void wii_vb_emu_loop();

#endif
