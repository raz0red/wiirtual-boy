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
	this->root[0] = this->parseTag("root0", filebuffer);
	this->root[1] = this->parseTag("root1", filebuffer);
	this->root[2] = this->parseTag("root2", filebuffer);
	this->root[3] = this->parseTag("root3", filebuffer);
	this->save[0] = this->parseTag("save0", filebuffer);
	this->save[1] = this->parseTag("save1", filebuffer);
	this->save[2] = this->parseTag("save2", filebuffer);
	this->save[3] = this->parseTag("save3", filebuffer);
	this->save[4] = this->parseTag("save4", filebuffer);
	this->save[5] = this->parseTag("save5", filebuffer);
	this->controlsub[0] = this->parseTag("controlsub0", filebuffer);
	this->controlsub[1] = this->parseTag("controlsub1", filebuffer);
	this->controlsub[2] = this->parseTag("controlsub2", filebuffer);
	this->controlsub[3] = this->parseTag("controlsub3", filebuffer);
	this->displaysub[0] = this->parseTag("displaysub0", filebuffer);
	this->displaysub[1] = this->parseTag("displaysub1", filebuffer);
	this->displaysub[2] = this->parseTag("displaysub2", filebuffer);
	this->advancedsub[0] = this->parseTag("advancedsub0", filebuffer);
	this->advancedsub[1] = this->parseTag("advancedsub1", filebuffer);
	this->savesub[0] = this->parseTag("savesub0", filebuffer);
	this->savesub[1] = this->parseTag("savesub1", filebuffer);
	this->savesub[2] = this->parseTag("savesub2", filebuffer);
	this->display[0] = this->parseTag("display0", filebuffer);
	this->display[1] = this->parseTag("display1", filebuffer);
	this->display[2] = this->parseTag("display2", filebuffer);
	this->advanced[0] = this->parseTag("advanced0", filebuffer);
	this->advanced[1] = this->parseTag("advanced1", filebuffer);
	this->advanced[2] = this->parseTag("advanced2", filebuffer);
	this->advanced[3] = this->parseTag("advanced3", filebuffer);
	this->advanced[4] = this->parseTag("advanced4", filebuffer);
	this->advanced[5] = this->parseTag("advanced5", filebuffer);

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
	fprintf( fp, "<root0>Resume</root0>\n");
	fprintf( fp, "<root1>Reset</root1>\n");
	fprintf( fp, "<root2>Load Cartridge</root2>\n");
	fprintf( fp, "<root3>Cartridge settings (current cartridge)</root3>\n");
	fprintf( fp, "<save0>Save state management</save0>\n");
	fprintf( fp, "<save1>Autoload </save1>\n");
	fprintf( fp, "<save2>Autosave </save2>\n");
	fprintf( fp, "<save3>Load saved state</save3>\n");
	fprintf( fp, "<save4>Save state (current cartridge)</save4>\n");
	fprintf( fp, "<save5>Delete saved state (current cartridge)</save5>\n");
	fprintf( fp, "<controlsub0>Control settings</controlsub0>\n");
	fprintf( fp, "<controlsub1>Controller </controlsub1>\n");
	fprintf( fp, "<controlsub2>Supported </controlsub2>\n");
	fprintf( fp, "<controlsub3>Button </controlsub3>\n");
	fprintf( fp, "<displaysub0>Display settings</displaysub0>\n");
	fprintf( fp, "<displaysub1>Frameskip </displaysub1>\n");
	fprintf( fp, "<displaysub2>Render rate (%) </displaysub2>\n");
	fprintf( fp, "<advancedsub0>Advanced</advnacedsub0>\n");
	fprintf( fp, "<advancedsub1>ROM Patching </advancedsub1>\n");
	fprintf( fp, "<savesub0>Save settings</savesub0>\n");
	fprintf( fp, "<savesub1>Revert to saved settings</savesub1>\n");
	fprintf( fp, "<savesub2>Delete settings</savesub2>\n");
	fprintf( fp, "<display0>Display settings</display0>\n");
	fprintf( fp, "<display1>Screen size </display1>\n");
	fprintf( fp, "<display2>Display mode </display2>\n");
	fprintf( fp, "<advanced0>Advanced</advanced0>\n");
	fprintf( fp, "<advanced1>Debug mode </advanced1>\n");
	fprintf( fp, "<advanced2>Top menu exit </advanced2>\n");
	fprintf( fp, "<advanced3>Wiimote (menu) </advanced3>\n");
	fprintf( fp, "<advanced4>ROM Patching </advanced4>\n");
	fprintf( fp, "<advanced5>Language </advanced5>\n");
	fprintf( fp, "</language>");
	
	fclose(fp);

	return true;
}