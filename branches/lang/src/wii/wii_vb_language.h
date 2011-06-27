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

/*

text definitions:

root0        -    Resume
root1        -    Reset
root2        -    Load Cartridge
root3        -    Cartridge settings (current cartridge)
save0        -    Save state management
save1        -    Autoload
save2        -    Autosave
save3        -    Load saved state
save4        -    Save state (current cartridge)
save5        -    Delete saved state (current cartridge)
controlsub0  -    Control settings
controlsub1  -    Controller
controlsub2  -    Supported
controlsub3  -    Button
displaysub0  -    Display settings
displaysub1  -    Frameskip
displaysub2  -    Render rate (%)
advancedsub0 -    Advanced
advancedsub1 -    ROM Patching 
savesub0     -    Save settings
savesub1     -    Revert to saved settings
savesub2     -    Delete settings
display0     -    Display settings
display1     -    Screen size
display2     -    Display mode
advanced0    -    Advanced
advanced1    -    Debug mode
advanced2    -    Top menu exit
advanced3    -    Wiimote (menu)
advanced4    -    ROM Patching
advanced5    -    Language



*/

#ifndef WII_VB_LANGUAGE_H
#define WII_VB_LANGUAGE_H

#include <string>

using namespace std;

class Language 
{

    public:
	
	Language();
	~Language();
	
	bool languageLoad(char* filepath);
	
	string name;//Language name
	string root[4];
	string save[6];
	string controlsub[4];
	string displaysub[3];
	string advancedsub[2];
	string savesub[3];
	string display[3];
	string advanced[6];
	
	private:
	
	string parseTag(string tag, string filebuffer);
	
};

bool generateEnglishLanguageFile(char *filepath);

#endif