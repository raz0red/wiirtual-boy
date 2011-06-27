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

control     - "U/D = Scroll. A = Select. Home = Exit"
menu0       - "Load Cartridge"
load0       - "U/D = Scroll. L/R = Page. A = Select. B = Back. Home = Exit"
load1       - " cartridges found. displaying "
menu1       - "Save state management"
save0       - "Auto load:"
save1       - "Auto save:"
save2       - "Load saved state"
menu2       - "Display settings"
display0    - "Screen size:"
display1    - "Display mode:"
menu3       - "Advanced"
advanced0   - "Debug mode:"
advanced1   - "Top menu exit:"
advanced2   - "Wiimote [menu]:"

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
	string control;
	string menu[4];
	string load[2];
	string save[3];
	string display[2];
	string advanced[3];
	
	private:
	
	string parseTag(string tag, string filebuffer);
	
};

bool generateEnglishLanguageFile(char *filepath);

#endif