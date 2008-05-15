#include <demo.h>
#include "Texture/TplPalette.h"
#include "utils/RawFile.h"

#include "VA/VertexArray.h"
#include "VA/VATTable.h"
#include "VA/VertexFormat.h"

#define BALL64_TEX_ID 0


/*---------------------------------------------------------------------------*
   Forward references
 *---------------------------------------------------------------------------*/
                  
static void CameraInit( Mtx v );

/*---------------------------------------------------------------------------*
   Application main loop
 *---------------------------------------------------------------------------*/


int main ( void )
{   
    PADStatus     pad[4];  // game pad state
    GXTexObj      texObj;  // texture object
    Mtx           v;       // view matrix
    
    pad[0].button = 0;

    DEMOInit(NULL);    // Init os, pad, gx, vi
	DVDInit();

	GXSetVerifyLevel(GX_WARN_ALL);

    CameraInit(v);
    GXLoadPosMtxImm(v, GX_PNMTX0);
    
    //  Describe the texture coordinate format
    //  fixed point format is unsigned 8.0
    

	

	VATTable::buildVAT();

	VFMT0::IndexStruct * indices = (VFMT0::IndexStruct *)OSAlloc(sizeof(VFMT0::IndexStruct)*6);
	indices[0].posi = indices[0].texi = 0;
	indices[1].posi = indices[1].texi = 3;
	indices[2].posi = indices[2].texi = 1;
	indices[3].posi = indices[3].texi = 1;
	indices[4].posi = indices[4].texi = 3;
	indices[5].posi = indices[5].texi = 2;

	VFMT0 * data = (VFMT0 *)OSAlloc(sizeof(VFMT0)*4);
	// TL
	data[0].xyz[0] = -100.0f;
	data[0].xyz[1] = 100.0f;
	data[0].xyz[2] = 0.0f;
	data[0].st[0] = 0;
	data[0].st[1] = 0;
	// BL
	data[1].xyz[0] = -100.0f;
	data[1].xyz[1] = -100.0f;
	data[1].xyz[2] = 0.0f;
	data[1].st[0] = 0;
	data[1].st[1] = 1;
	// BR
	data[2].xyz[0] = 100.0f;
	data[2].xyz[1] = -100.0f;
	data[2].xyz[2] = 0.0f;
	data[2].st[0] =	1;
	data[2].st[1] = 1;
	// TR
	data[3].xyz[0] = 100.0f;
	data[3].xyz[1] = 100.0f;
	data[3].xyz[2] = 0.0f;
	data[3].st[0] = 1;
	data[3].st[1] = 0;


	DCFlushRange(indices,sizeof(VFMT0::IndexStruct)*6);
	DCFlushRange(data,sizeof(VFMT0)*4);
	
	VertexArray<VFMT0> va;
	va.set(data,4,GX_TRIANGLES,6,indices);
	


	RawFile rf("text.txt");
	OSReport((char *)rf.data);

	//  Load the texture palette
	//TplPalette pal("out2.tpl","out2.names");
	TplPalette pal("gxTextrs.tpl",0);
    //  Initialize a texture object to contain the correct texture
	pal.getTexObj(texObj,1);

	 /*
	GXInitTexObjLOD(
        &texObj,
        GX_NEAR,
        GX_NEAR,
        0,
        0,
        0,
        GX_FALSE,
        GX_FALSE,
        GX_ANISO_1 );
	*/	

	GXInvalidateTexAll();

    GXSetTevOp(GX_TEVSTAGE0, GX_DECAL);
    
	OSReport("VFMT0 size : %u bytes\n",sizeof(VFMT0));
    OSReport("\n\n********************************\n");
    OSReport("to quit:\n");
    OSReport("     hit the start button\n");
    OSReport("********************************\n");

	u8 texsel = 0;
    
    while(!(pad[0].button & PAD_BUTTON_MENU))
    {
        DEMOBeforeRender();
        // Draw a triangle
		u16 res = (pad[0].button & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y));
		if(res)
			texsel = (texsel + 1)%pal.getSurfaceNum();
		pal.getTexObj(texObj,texsel);

		GXLoadTexObj(&texObj, GX_TEXMAP0);
		
		va.call();
        DEMODoneRender();
        PADRead(pad);
    }
    
    OSHalt("End of demo");
	return 0;
}

/*---------------------------------------------------------------------------*
   Functions
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    Name:           CameraInit
    
    Description:    Initialize the projection matrix and load into hardware.
                    Initialize the view matrix
                    
    Arguments:      v    view matrix
    
    Returns:        none
 *---------------------------------------------------------------------------*/

static void CameraInit ( Mtx v )
{
    Mtx44 p;
    Vec   camPt = {0.0F, 0.0F, 800.0F};
    Vec   at    = {0.0F, 0.0F, -100.0F};
    Vec   up    = {0.0F, 1.0F, 0.0F};

    MTXFrustum(p, 120.0F,-120.0F,-120.0F, 120.0F, 500, 2000);
    GXSetProjection(p, GX_PERSPECTIVE);
    MTXLookAt(v, &camPt, &up, &at);        
}

