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

#include "wii_vb_language.h"

#include <stdio.h>

using namespace std;

Language::Language() {}

Language::~Language() {}

/*

Loads a language file. Returns true if successful. False if not.

*/

bool Language::languageLoad(char *filepath)
{

    string filebuffer;
	
	FILE *fp = fopen(filepath, "rb");
	if(!fp)
		return false;
	
	do
	{
	
		filebuffer += fgetc(fp);
	
	} while(!feof(fp));
	
	this->name = this->parseTag("name", filebuffer);
	this->control = this->parseTag("control", filebuffer);
	this->menu[0] = this->parseTag("menu0", filebuffer);
	this->load[0] = this->parseTag("load0", filebuffer);
	this->load[1] = this->parseTag("load1", filebuffer);
	this->menu[1] = this->parseTag("menu1", filebuffer);
	this->save[0] = this->parseTag("save0", filebuffer);
	this->save[1] = this->parseTag("save1", filebuffer);
	this->save[2] = this->parseTag("save2", filebuffer);
	this->menu[2] = this->parseTag("menu2", filebuffer);
	this->display[0] = this->parseTag("display0", filebuffer);
	this->display[1] = this->parseTag("display1", filebuffer);
	this->menu[3] = this->parseTag("menu3", filebuffer);
	this->advanced[0] = this->parseTag("advanced0", filebuffer);
	this->advanced[1] = this->parseTag("advanced1", filebuffer);
	this->advanced[2] = this->parseTag("advanced2", filebuffer);

	return true;
}

/*

Returns the string stored inbetween a tag.

*/
string Language::parseTag(string tag, string filebuffer)
{

	string ret;
	
	string tag1 = "<" + tag + ">";
	string tag2 = "</" + tag + ">";
	
	int first = filebuffer.find(tag1);
	int last = filebuffer.find(tag2);
	
	ret = filebuffer.substr(first + tag1.size(), last - first - tag1.size());
	
	return ret;

}

/*

Generates the english language file. Returns true if successful. False if not.

*/

bool generateEnglishLanguageFile(char *filepath)
{

	FILE *fp = fopen(filepath, "wb");
	
	if(!fp)
		return false;
		
	fprintf( fp, "<language>\n");
    fprintf( fp, "<name>English</name>\n");
	fprintf( fp, "<control>U/D = Scroll. A = Select. Home = Exit</control>\n");
    fprintf( fp, "<menu0>Load Cartridge</menu0>\n");
    fprintf( fp, "<load0>U/D = Scroll. L/R = Page. A = Select. B = Back. Home = Exit</load0>\n");
    fprintf( fp, "<load1> cartridges found. displaying </load1>\n");
    fprintf( fp, "<menu1>Save state management</menu1>\n");
    fprintf( fp, "<save0>Auto load:</save0>\n");
    fprintf( fp, "<save1>Auto save:</save1>\n");
    fprintf( fp, "<save2>Load saved state</save2>\n");
    fprintf( fp, "<menu2>Display settings</save2>\n");
    fprintf( fp, "<display0>Screen size:</display0>\n");
    fprintf( fp, "<display1>Display mode:</display1>\n");
    fprintf( fp, "<menu3>Advanced</menu3>\n");
    fprintf( fp, "<advanced0>Debug mode:</advanced0>\n");
    fprintf( fp, "<advanced1>Top menu exit:</advanced1>\n");
    fprintf( fp, "<advanced2>Wiimote [menu]:</advanced2>\n");
	fprintf( fp, "</language>");
	
	fclose(fp);

	return true;
}