#include "../OSinterface/Application.h"
#include "../../rendering/Graphics.h"
#include "../../utility/Time.h"
#include <demo.h>
using namespace std;

// Testing only
#include "../OSinterface/Input.h"
#include "../../utility/File.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


#include "gfxutils/Model/model.h"
#include "gfxutils/Model/modelmgr.h"
#include "gfxutils/Texture/Texturemgr.h"
#include "gfxutils/Texture/Texture.h"
static const Model * model;
static const Texture * tex;


/*---------------------------------------------------------------------------*
   The macro ATTRIBUTE_ALIGN provides a convenient way to align initialized 
   arrays.  Alignment of vertex arrays to 32B IS NOT required, but may result 
   in a slight performance improvement.
 *---------------------------------------------------------------------------*/
f32 Verts_f32[] ATTRIBUTE_ALIGN(32) = 
{
//      x, y, z       
    0.0f, 0.0f, 1.0f,    // 0:0
    0.0f, 1.0f, 0.0f,    // 0:1
    1.0f, 0.0f, 0.0f,    // 0:2
    0.0f, 0.0f, 1.0f,    // 1:0
    -1.0f, 0.0f, 0.0f,   // 1:1
    0.0f, 1.0f, 0.0f,    // 1:2
    0.0f, 0.0f, 1.0f,    // 2:0
    0.0f, -1.0f, 0.0f,   // 2:1
    -1.0f, 0.0f, 0.0f,   // 2:2
    0.0f, 0.0f, 1.0f,    // 3:0
    1.0f, 0.0f, 0.0f,    // 3:1
    0.0f, -1.0f, 0.0f,   // 3:2
    0.0f, 0.0f, -1.0f,   // 4:0
    1.0f, 0.0f, 0.0f,    // 4:1
    0.0f, 1.0f, 0.0f,    // 4:2
    0.0f, 0.0f, -1.0f,   // 5:0
    0.0f, 1.0f, 0.0f,    // 5:1
    -1.0f, 0.0f, 0.0f,   // 5:2
    0.0f, 0.0f, -1.0f,   // 6:0
    -1.0f, 0.0f, 0.0f,   // 6:1
    0.0f, -1.0f, 0.0f,   // 6:2
    0.0f, 0.0f, -1.0f,   // 7:0
    0.0f, -1.0f, 0.0f,   // 7:1
    1.0f, 0.0f, 0.0f     // 7:2
};

u8 Colors_rgba8[] ATTRIBUTE_ALIGN(32) = 
{
    //  r,   g,  b,  a
    255, 0, 0, 255,    // 0:0
    255, 0, 0, 255,    // 0:1
    255, 0, 0, 255,    // 0:2
    0, 255, 0, 255,    // 1:0
    0, 255, 0, 255,    // 1:1
    0, 255, 0, 255,    // 1:2
    255, 0, 0, 255,    // 2:0
    255, 0, 0, 255,    // 2:1
    255, 0, 0, 255,    // 2:2
    0, 255, 0, 255,    // 3:0
    0, 255, 0, 255,    // 3:1
    0, 255, 0, 255,    // 3:2
    0, 255, 0, 255,    // 4:0
    0, 255, 0, 255,    // 4:1
    0, 255, 0, 255,    // 4:2
    255, 0, 0, 255,    // 5:0
    255, 0, 0, 255,    // 5:1
    255, 0, 0, 255,    // 5:2
    0, 255, 0, 255,    // 6:0
    0, 255, 0, 255,    // 6:1
    0, 255, 0, 255,    // 6:2
    255, 0, 0, 255,    // 7:0
    255, 0, 0, 255,    // 7:1
    255, 0, 0, 255    // 7:2
};


u32    ticks = 0;    // time counter

/*---------------------------------------------------------------------------*
   Forward references
 *---------------------------------------------------------------------------*/
 
int        main            ( void );
static void CameraInit      ( Mtx v );
static void DrawInit        ( void );
static void DrawTick        ( Mtx v );
static void AnimTick        ( void );
//static void PrintIntro      ( void );

/*---------------------------------------------------------------------------*
   Application main loop
 *---------------------------------------------------------------------------*/
//PADStatus   pad[PAD_MAX_CONTROLLERS];
int main ( void )
{
    Mtx         v;   // view matrix


    //pad[0].button = 0;
	//PADInit();

	// operator new does this. Crap coding, someone fix it plz!
    //DEMOInit(NULL);    // Init os, pad, gx, vi
    
    CameraInit(v); // Initialize the camera.  
    DrawInit();    // Define my vertex formats and set array pointers.

	//typedef std::vector<std::string> StringVector;
	//StringVector strings;
	//strings.push_back("TEST");
	//for (StringVector::iterator it = strings.begin(); it != strings.end(); it++)
	//{
	//	std::cerr << *it << std::endl;
	//}

	Application app;
	Graphics g;
	Time timer;

	if (!app.init()) {
		cerr << "couldn't initialize app" << endl;
		return 0;
	}

	/*
	model = ModelMgr::instance().getModel("enforcer_proc.obj");
	TextureMgr::instance().loadPalette("enforcer.tpl","enforcerTPL.txt");
	tex = TextureMgr::instance().getTexture("enforcer");
	GXInvalidateTexAll();
	*/
	

	//FILE* fp = fopen("./config/config.txt", "rb");
	//if (fp == 0) cout << "Couldn't open file" << endl;
	//char buffer[32];
	//int bytesRead = fread(buffer, sizeof(char) * 32, 1, fp);
	//cout << "Read " << bytesRead << " bytes" << endl;

	//std::string fname("./config/config.txt");
	//ifstream ifs(fname.c_str());
	//if (!ifs.good()) cout << "Couldn't load file " << fname << endl;

	//DVDFileInfo fileInfo;
	//const char* fileName = "./config/config.txt";
	//DVDInit();
	//if (FALSE == DVDOpen(fileName, &fileInfo))
	//{
	//	cout << "Couldn't open file " << fileName << endl;
	//	return 0;
	//}
	//int fileSize = DVDGetLength(&fileInfo);
	//char* buffer = new char[fileSize];
	//if (DVDRead(&fileInfo, buffer, OSRoundUp32B(fileSize), 0) == 0)
	//{
	//	cout << "Couldn't read file " << fileName << endl;
	//	return 0;
	//}

	//File config("./config/config.txt");

	//cout << "Read file " << config.getBuffer() << endl;


	cout << "Application Initialized" << endl;
	//cout << "Control X: " << Input::safeInstance().getControlX(0) << endl;


    //PrintIntro(); // Print demo directions
    while(true) //!(pad[0].button & PAD_BUTTON_MENU))
    {
		AnimTick();
		const float dt = timer.getDeltatime();
		app.update(dt);

		DEMOBeforeRender();
        DrawTick(v);        // Draw the model.
		g.reset();
		app.render(g);
        DEMODoneRender();

		//cout << "Time: " << u32(OSGetTime() >> 18) << endl;
		//cout << "Time: " << timer.getCurrentTime() << endl;

		//PADRead(pad);
    }

	app.destroy();

    OSHalt("End of demo");

	return 0;
}


/*---------------------------------------------------------------------------*
   Functions
 *---------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*
    Name:           CameraInit
    
    Description:    Initialize the projection matrix and load into hardware.
                    Initialize the view matrix.
                    
    Arguments:      v      view matrix
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void CameraInit ( Mtx v )
{
	Mtx44 p;
    Vec   camPt = {0.0F, 300.0F, 0.0F};
    Vec   at    = {0.0F, 0.0F, 0.0F};
    Vec   up    = {0.0F, 0.0F, -1.0F};

    MTXFrustum(p, 12.0F,-12.0F,-12.0F, 12.0F, 10, 1000);
    GXSetProjection(p, GX_PERSPECTIVE);
    MTXLookAt(v, &camPt, &up, &at);
	GXLoadPosMtxImm(v, GX_PNMTX0);

	
}

/*---------------------------------------------------------------------------*
    Name:           DrawInit
    
    Description:    Initializes the vertex attribute format 0, and sets
                    the array pointers and strides for the indexed data.
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawInit( void )
{ 
    GXColor black = {0, 0, 1, 0};

    GXSetCopyClear(black, 0x00ffffff);

    // Set current vertex descriptor to enable position and color0.
    // Both use 8b index to access their data arrays.
	/*
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
            
    // Position has 3 elements (x,y,z), each of type f32
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    
    // Color 0 has 4 components (r, g, b, a), each component is 8b.
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    
    // stride = 3 elements (x,y,z) each of type s16
    GXSetArray(GX_VA_POS, Verts_f32, 3*sizeof(f32));
    // stride = 4 elements (r,g,b,a) each of type u8
    GXSetArray(GX_VA_CLR0, Colors_rgba8, 4*sizeof(u8));

	*/

    // Initialize lighting, texgen, and tev parameters
    //GXSetNumChans(1); // default, color = vertex color
    //GXSetNumTexGens(0); // no texture in this demo
    //GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_DECAL);
}

/*---------------------------------------------------------------------------*
    Name:           Vertex
    
    Description:    Create my vertex format
                    
    Arguments:      t        8-bit triangle index
            v        8-bit vertex index
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static inline void Vertex( u8 t, u8 v )
{
    u8 tv = (u8) (3 * t + v);
    GXPosition1x8(tv);
    GXColor1x8(tv);
}

/*---------------------------------------------------------------------------*
    Name:           DrawTick
    
    Description:    Draw the model once.
    
    Arguments:      v        view matrix
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawTick( Mtx v )
{
    Mtx    m;    // Model matrix.
    Mtx    mv;    // Modelview matrix.
    u8    iTri;    // index of triangle
    u8    iVert;    // index of vertex


	/*GXSetViewport( 
    100, 
    100, 
    50, 
    50, 
    10, 
    1000 ); */
	bool upDown = Input::instance().isButtonDown(0, PAD_BUTTON_A);
	int tickNumber = upDown ? 8 : 0;
	f32 posX = - Input::instance().getControlX(0) ;
	f32 posY = Input::instance().getControlY(0) ;

	//int tickNumber = (pad[0].button & PAD_BUTTON_UP) ? 8 : 0;
	//f32 posX = -pad[0].stickX*0.1f;
	//f32 posY = pad[0].stickY*0.1f;
    
    // model has a rotation about z axis
    MTXRotDeg(m, 'z', tickNumber * ticks);
	//view matrix
    MTXConcat(v, m, mv);
	//MTXConcat(Camera::viewMatrix, m, mv);


	//MTXRotDeg(m, 'x', 8 * ticks);
	//MTXConcat(mv, m, mv);
	MTXTrans(m, posX, posY, 0);
	MTXConcat(mv, m, mv);
    GXLoadPosMtxImm(mv, GX_PNMTX0);

	GXSetCurrentMtx(GX_PNMTX0);

	/*
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 24);
    
    // for all triangles of octahedron, ...
    for (iTri = 0; iTri < 8; ++iTri)
    {
        // for all vertices of triangle, ...
        for (iVert = 0; iVert < 3; ++iVert)
        {
            Vertex(iTri, iVert);
        }
    }
    GXEnd();
	*/

	/*
	tex->bind();
	model->render();
	*/
}

/*---------------------------------------------------------------------------*
    Name:           AnimTick
    
    Description:    Computes next time step.
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void AnimTick( void )
{
    ticks++;
}

/*---------------------------------------------------------------------------*
    Name:            PrintIntro
    
    Description:    Prints the directions on how to use this demo.
                    
    Arguments:        none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
//static void PrintIntro( void )
//{
//    OSReport("\n\n********************************\n");
//    OSReport("to quit:\n");
//    OSReport("     hit the start button\n");
//#ifdef MACOS
//    OSReport("     click on the text output window\n");
//    OSReport("     select quit from the menu or hit 'command q'\n");
//    OSReport("     select 'don't save'\n");
//#endif
//    OSReport("********************************\n");
//}

/*============================================================================*/
