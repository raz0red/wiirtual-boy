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

#ifndef WII_APP_COMMON_H
#define WII_APP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define WII_BASE_APP_DIR "sd:/apps/wiivb/"
#define WII_SAVE_GAME_EXT "sav"

#define WII_FILES_DIR "/wiivb/"
#define WII_ROMS_DIR WII_FILES_DIR "roms/"
#define WII_SAVES_DIR WII_FILES_DIR "saves/"
#define WII_CONFIG_FILE WII_FILES_DIR "wiivb.conf"

/*
 * The different types of nodes in the menu
 */
enum NODETYPE
{
  NODETYPE_ROOT = 0,
  NODETYPE_SPACER,
  NODETYPE_LOAD_ROM,
  NODETYPE_ROM,
  NODETYPE_EXIT,
  NODETYPE_RESUME,
  NODETYPE_ADVANCED,
  NODETYPE_SAVE_STATE_MANAGEMENT,
  NODETYPE_DEBUG_MODE,
  NODETYPE_TOP_MENU_EXIT,
  NODETYPE_AUTO_LOAD_STATE,
  NODETYPE_AUTO_SAVE_STATE,
  NODETYPE_RESET,
  NODETYPE_SAVE_STATE,
  NODETYPE_LOAD_STATE,
  NODETYPE_DELETE_STATE,
  NODETYPE_STATE_SAVE,
  NODETYPE_VSYNC,
  NODETYPE_DISPLAY_SETTINGS,
  NODETYPE_CONTROLS_SETTINGS,
  NODETYPE_CARTRIDGE_SETTINGS_CURRENT,
  NODETYPE_CARTRIDGE_SETTINGS_CURRENT_SPACER,
  NODETYPE_CARTRIDGE_SETTINGS_DEFAULT,
  NODETYPE_SAVE_CARTRIDGE_SETTINGS,
  NODETYPE_DELETE_CARTRIDGE_SETTINGS,
  NODETYPE_REVERT_CARTRIDGE_SETTINGS,
  NODETYPE_CONTROLS_MODE,
  NODETYPE_RESIZE_SCREEN,
  NODETYPE_WIIMOTE_MENU_ORIENT,
  NODETYPE_VB_MODE,
  NODETYPE_CARTRIDGE_SETTINGS_DISPLAY,
  NODETYPE_CARTRIDGE_SETTINGS_CONTROLS,
  NODETYPE_CARTRIDGE_SETTINGS_ADVANCED,
  NODETYPE_CART_FRAME_SKIP,
  NODETYPE_CART_RENDER_RATE,
  NODETYPE_CONTROLLER,
  NODETYPE_WIIMOTE_SUPPORTED,
  NODETYPE_BUTTON1, NODETYPE_BUTTON2, NODETYPE_BUTTON3, NODETYPE_BUTTON4,
  NODETYPE_BUTTON5, NODETYPE_BUTTON6, NODETYPE_BUTTON7, NODETYPE_BUTTON8,
  NODETYPE_BUTTON9, NODETYPE_BUTTON10,
  NODETYPE_ROM_PATCH,
  NODETYPE_ROM_PATCH_CART
};

#ifdef __cplusplus
}
#endif

#endif
