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

/*
#include "../../gfxutils/Model/model.h"
#include "../../gfxutils/Model/modelmgr.h"
#include "../../gfxutils/Texture/Texturemgr.h"
#include "../../gfxutils/Texture/Texture.h"
static const Model * model;
static const Texture * tex;
*/

u32    ticks = 0;    // time counter

/*---------------------------------------------------------------------------*
   Forward references
 *---------------------------------------------------------------------------*/
 
int        main            ( void );
static void CameraInit      ( Mtx v );
static void DrawInit        ( void );
static void DrawTick        ( Mtx v );
//static void PrintIntro      ( void );

/*---------------------------------------------------------------------------*
   Application main loop
 *---------------------------------------------------------------------------*/
//PADStatus   pad[PAD_MAX_CONTROLLERS];
int main ( void )
{
    //pad[0].button = 0;
	//PADInit();

	// operator new does this. Crap coding, someone fix it plz!
    //DEMOInit(NULL);    // Init os, pad, gx, vi

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

	//app.load();

	cout << "Application Initialized" << endl;

    while(true) //!(pad[0].button & PAD_BUTTON_MENU))
    {
		const float dt = timer.getDeltatime();
		app.update(dt);

		DEMOBeforeRender();
        //DrawTick(v);        // Draw the model.
		g.reset();
		app.render(g);
        DEMODoneRender();

		//PADRead(pad);
    }

	app.destroy();

    OSHalt("End of demo");

	return 0;
}



/*
static void DrawInit( void )
{ 
    GXColor black = {0, 0, 1, 0};

    GXSetCopyClear(black, 0x00ffffff);

    GXSetTevOp(GX_TEVSTAGE0, GX_DECAL);
}


static void DrawTick( Mtx v )
{
    Mtx    m;    // Model matrix.
    Mtx    mv;    // Modelview matrix.

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

	tex->bind();
	model->render();	
}
*/
/*
static void AnimTick( void )
{
    ticks++;
}
*/

