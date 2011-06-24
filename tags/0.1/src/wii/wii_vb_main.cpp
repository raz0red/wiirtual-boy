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

#include "main.h"
#include "sound.h"

#include "wii_app.h"
#include "wii_gx.h"
#include "wii_main.h"
#include "wii_sdl.h"

#include "wii_vb.h"
#include "wii_vb_input.h"
#include "wii_vb_main.h"
#include "wii_vb_sdl.h"

#include "wiimotenotsupported_png.h"

#ifdef WII_NETTRACE
#include <network.h>
#include "net_print.h"  
#endif

extern "C" 
{
void WII_VideoStart();
void WII_VideoStop();
void WII_ChangeSquare(int xscale, int yscale, int xshift, int yshift);
void WII_SetRenderCallback( void (*cb)(void) );
Mtx gx_view;
}

// Forward references
static void gxrender_callback();

// Mednafen external references
extern volatile Uint32 MainThreadID;
extern MDFNSetting DriverSettings[]; 
extern int DriverSettingsSize;
extern std::vector <MDFNSetting> NeoDriverSettings;
extern char *DrBaseDirectory;
extern volatile MDFN_Surface *VTReady;
extern volatile MDFN_Rect *VTLWReady;
extern volatile MDFN_Rect *VTDRReady;
extern MDFN_Rect VTDisplayRects[2];
extern volatile MDFN_Surface *VTBuffer[2];
extern MDFN_Rect *VTLineWidths[2];
extern SDL_Surface *screen;
extern volatile int NeedVideoChange;

namespace MDFN_IEN_VB
{
  extern void VIP_SetParallaxDisable(bool disabled);
  extern void VIP_SetAnaglyphColors(uint32 lcolor, uint32 rcolor);
  extern void VIP_Set3DMode(uint32 mode, bool reverse, uint32 prescale, uint32 sbs_separation);
}

extern int LoadGame(const char *force_module, const char *path);
extern int GameLoop(void *arg);
extern char *GetBaseDirectory(void);
extern void KillVideo(void);
extern void FPS_Init(void);
extern void MakeMednafenArgsStruct(void);
extern bool CreateDirs(void);
extern void MakeVideoSettings(std::vector <MDFNSetting> &settings);
extern void MakeInputSettings(std::vector <MDFNSetting> &settings);
extern void DeleteInternalArgs(void);
extern void KillInputSettings(void);
extern void CalcFramerates(char *virtfps, char *drawnfps, char *blitfps, size_t maxlen);

// The wiimote not supported image data
static gx_imagedata* mote_not_supported_idata = NULL;

/*
 * Initializes the emulator
 */
void wii_vb_init()
{
  std::vector<MDFNGI *> ExternalSystems;
  MainThreadID = SDL_ThreadID();

  DrBaseDirectory=GetBaseDirectory();

  MDFNI_printf(_("Starting Mednafen %s\n"), MEDNAFEN_VERSION);
  MDFN_indent(1);
  MDFN_printf(_("Base directory: %s\n"), DrBaseDirectory);

  // Look for external emulation modules here.
  if(!MDFNI_InitializeModules(ExternalSystems))
  {
    MDFN_PrintError( "Unable to initialize external modules" );
    exit( 0 );
  }

  for(unsigned int x = 0; x < DriverSettingsSize / sizeof(MDFNSetting); x++)
    NeoDriverSettings.push_back(DriverSettings[x]);

  MakeVideoSettings(NeoDriverSettings);
  MakeInputSettings(NeoDriverSettings);

  if(!(MDFNI_Initialize(DrBaseDirectory, NeoDriverSettings))) 
  {
    MDFN_PrintError( "Error during initialization" );
    exit( 0 );
  }

  MakeMednafenArgsStruct();

  VTReady = NULL;
  VTDRReady = NULL;
  VTLWReady = NULL;

  MDFN_PixelFormat nf;
#if BPP == 8
  nf.bpp = 8;
#elif BPP == 16
  nf.bpp = 16;
#else
  nf.bpp = 32;
#endif
  nf.colorspace = MDFN_COLORSPACE_RGB;

  VTBuffer[0] = new MDFN_Surface(NULL, VB_WIDTH, VB_HEIGHT, VB_WIDTH, nf);
  //VTBuffer[1] = new MDFN_Surface(NULL, VB_WIDTH, VB_HEIGHT, VB_WIDTH, nf);
  VTBuffer[1] = VTBuffer[0];
  VTLineWidths[0] = (MDFN_Rect *)calloc(VB_HEIGHT, sizeof(MDFN_Rect));
  //VTLineWidths[1] = (MDFN_Rect *)calloc(VB_HEIGHT, sizeof(MDFN_Rect));
  VTLineWidths[1] = VTLineWidths[0];

  FPS_Init();

  KillVideo();

  // Set the screen to our back surface
  screen = back_surface;  
}

/*
 * Free resources (closes) the emulator
 */
void wii_vb_free()
{
  CloseGame();

  if(VTBuffer[0])
  {
    delete VTBuffer[0];
    VTBuffer[0] = NULL;
  }

  if(VTLineWidths[0])
  {
    free(VTLineWidths[0]);
    VTLineWidths[0] = NULL;
  }

  MDFNI_Kill();
  DeleteInternalArgs();
  KillInputSettings();
}

/*
 * Loads the specified game
 *
 * game     The name of the game
 * return   1 if the load is successful, 0 if it fails
 */
int wii_vb_load_game( char* game )
{
  return LoadGame( NULL, game );
}


namespace MDFN_IEN_VB
{
  extern int vb_skip_frame;
  extern int vb_skip_sum;
}

/*
 * The emulation loop
 *
 * resume   Whether we are resuming
 */
void wii_vb_emu_loop( BOOL resume )
{
    // Reset frame skip information
    MDFN_IEN_VB::vb_skip_frame = 0;
#ifdef BOUND_HIGH_HACK
    // This is an extremely lame hack that allows for "Bound High!" to work
    // correctly with frame skipping. 
    MDFN_IEN_VB::vb_skip_sum = wii_get_render_rate();
#else
    MDFN_IEN_VB::vb_skip_sum = 0;
#endif

  wii_vb_db_apply_button_map( &wii_vb_db_entry );

  for(int i = 0; i < 2; i++)
    ((MDFN_Surface *)VTBuffer[i])->Fill(0, 0, 0, 0);

  const Vb3dMode mode = wii_get_vb_mode();
  if( wii_is_custom_mode( &mode ) )
  {
    MDFN_IEN_VB::VIP_SetParallaxDisable( !wii_custom_colors_parallax );
    MDFN_IEN_VB::VIP_SetAnaglyphColors( 
      Util_rgbatovalue( &wii_custom_colors[0], FALSE ),      
      Util_rgbatovalue( &wii_custom_colors[1], FALSE )
    );
  }
  else
  {
    MDFN_IEN_VB::VIP_SetParallaxDisable( !mode.isParallax );
    MDFN_IEN_VB::VIP_SetAnaglyphColors( mode.lColor, mode.rColor );
  }
  //MDFN_IEN_VB::VIP_Set3DMode( 0, false, 1, 0 );

  wii_sdl_black_back_surface();
  WII_SetRenderCallback( &gxrender_callback );  
  WII_ChangeSquare( wii_screen_x, wii_screen_y, 0, 0 );  
  WII_VideoStart();    
  ClearSound();
  PauseSound( 0 );

  GameThreadRun = 1;
  NeedVideoChange = 0;

  GameLoop( NULL );

  PauseSound( 1 );
  WII_VideoStop();     
}

#define CB_PIXELSIZE 14
#define CB_H CB_PIXELSIZE
#define CB_PADDING 2
#define CB_X -310
#define CB_Y 196

/*
 * GX render callback
 */
static void gxrender_callback()
{
  static int callback = 0;

  GX_SetVtxDesc( GX_VA_POS, GX_DIRECT );
  GX_SetVtxDesc( GX_VA_CLR0, GX_DIRECT );
  GX_SetVtxDesc( GX_VA_TEX0, GX_NONE );

  Mtx m;    // model matrix.
  Mtx mv;   // modelview matrix.

  guMtxIdentity( m ); 
  guMtxTransApply( m, m, 0, 0, -100 );
  guMtxConcat( gx_view, m, mv );
  GX_LoadPosMtxImm( mv, GX_PNMTX0 ); 

  if( wii_debug )
  {    
    static char virtfps[64];
    static char drawnfps[64];
    static char blitfps[64];
    static char text[256] = "";

    if( callback++ % 60 == 0 )
    {
      CalcFramerates( virtfps, drawnfps, blitfps, 64 );  
    }
    int renderRate = wii_get_render_rate();
    sprintf( 
      text, "%s %s %s (%d%%) hash:%s%s", 
      virtfps, drawnfps, blitfps, ( renderRate == -1 ? 100 : renderRate ),
      wii_cartridge_hash, ( wii_vb_db_entry.loaded ? " (db)" : "" ) );

    GXColor color = (GXColor){0x0, 0x0, 0x0, 0x80};                       
    wii_gx_drawrectangle( 
      CB_X + -CB_PADDING, 
      CB_Y + CB_H + CB_PADDING, 
      wii_gx_gettextwidth( CB_PIXELSIZE, text ) + (CB_PADDING<<1), 
      CB_H + (CB_PADDING<<1), 
      color, TRUE );

    wii_gx_drawtext( CB_X, CB_Y, CB_PIXELSIZE, text, ftgxWhite, FTGX_ALIGN_BOTTOM ); 
  }
}

// The current direction of the controls screen alpha
static int controls_alpha_dir = 0;
// The current alpha level for the controls screen
static int controls_alpha = 0;

/*
 * Callback used to display an image indicating the control configuration
 * for the current cartridge
 */
static void controls_render_callback()
{
  GX_SetVtxDesc( GX_VA_POS, GX_DIRECT );
  GX_SetVtxDesc( GX_VA_CLR0, GX_DIRECT );
  GX_SetVtxDesc( GX_VA_TEX0, GX_NONE );

  Mtx m;    // model matrix.
  Mtx mv;   // modelview matrix.

  guMtxIdentity( m ); 
  guMtxTransApply( m, m, 0, 0, -100 );
  guMtxConcat( gx_view, m, mv );
  GX_LoadPosMtxImm( mv, GX_PNMTX0 ); 

  if( mote_not_supported_idata == NULL )
  {
    mote_not_supported_idata =
      wii_gx_loadimagefrombuff( wiimotenotsupported_png );
  }

  wii_gx_drawimage( 
    -(mote_not_supported_idata->width>>1), 
    (mote_not_supported_idata->height>>1), 
    mote_not_supported_idata->width, 
    mote_not_supported_idata->height, 
    mote_not_supported_idata->data, 
    0, 1.0, 1.0, controls_alpha );
}

/*
 * Displays a screen indicating the controls configuration for the current
 * cartridge
 *
 * return   Returns 1 if we should continue past the screen
 */
int wii_vb_show_controls_screen()
{ 
  controls_alpha = controls_alpha_dir = 0;

  if( wii_vb_db_entry.wiimoteSupported )
  {
    // Wiimote is supported
    return 1;
  }

  WPAD_ScanPads();
  PAD_ScanPads();

  expansion_t exp;
  WPAD_Expansion( 0, &exp );

  if( exp.type == WPAD_EXP_CLASSIC || exp.type == WPAD_EXP_NUNCHUK )
  {
    // The classic or nunchuk is plugged in
    return 1;
  }

  const int ALPHA_INC = 10;
  const int MAX_TIME = 5 * 1000; // 5 seconds

  wii_sdl_black_back_surface();
  WII_SetRenderCallback( &controls_render_callback );  
  WII_VideoStart();  

  u32 startTime = SDL_GetTicks();

  int retVal = 1;
  do
  {
    int nextVal = controls_alpha;
    if( !controls_alpha_dir )
    {
      if( nextVal < 0xff )
      {
        nextVal+=ALPHA_INC;
      }
    }
    else
    {
      if( nextVal > 0 )
      {
        nextVal-=ALPHA_INC;
      }
    }

    if( nextVal >= 0xff )
    {
      nextVal = 0xff;
    }
    else if( nextVal < 0 )
    {
      nextVal = 0;
    }

    controls_alpha = nextVal;

    int pressed = wii_check_button_pressed();
    if( pressed == -1 )
    {
      retVal = 0;
      break;
    }

    if( pressed || 
        ( ( SDL_GetTicks() - startTime ) > MAX_TIME ) )
    {
      controls_alpha_dir = 1;
    }

    VIDEO_WaitVSync();
  }
  while( controls_alpha > 0 );

  WII_VideoStop();
  WII_SetRenderCallback( NULL );  

  return retVal;
}
