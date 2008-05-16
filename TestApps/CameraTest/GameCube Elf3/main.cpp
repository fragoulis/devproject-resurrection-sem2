/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     smp-octa.c

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/gxdemo/src/Simple/smp-octa.c $
    
    5     2/14/01 11:09a Hirose
    Changed to accept start button only to quit. Deleted obsolete ifdefs
    
    4     10/25/00 8:59p Hirose
    A flag fix MAC -> MACOS
    
    3     7/21/00 3:10p Hirose
    fixed camera parameter
    removed MAC only instructions from other targets
    
    2     3/23/00 8:01p Hirose
    fixed instruction message
    
    1     3/16/00 7:24p Alligator
    rename to match other demos conventions
    
    1     3/06/00 12:11p Alligator
    move from gxdemos to gxdemos/Simple
    
    2     1/25/00 3:51p Danm
    Changed video mode to use default mode.
    
    1     1/19/00 6:21p Danm
    Create a demo of a rotating octahedron in garish colors.
      


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <demo.h>

#include "Camera.h"

Camera camera;


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
static void PrintIntro      ( void );

/*---------------------------------------------------------------------------*
   Application main loop
 *---------------------------------------------------------------------------*/
PADStatus   pad[PAD_MAX_CONTROLLERS];
int main ( void )
{
    Mtx         v;   // view matrix


    pad[0].button = 0;
	PADInit();

	
    
    DEMOInit(NULL);    // Init os, pad, gx, vi
    
    CameraInit(v); // Initialize the camera.  
    DrawInit();    // Define my vertex formats and set array pointers.

    PrintIntro(); // Print demo directions
    while(!(pad[0].button & PAD_BUTTON_MENU))
    {   
        DEMOBeforeRender();
        DrawTick(v);        // Draw the model.
        DEMODoneRender();
        AnimTick();        // Update animation.
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
                    Initialize the view matrix.
                    
    Arguments:      v      view matrix
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void CameraInit ( Mtx v )
{
    Mtx44   p;      // projection matrix
    Vec     up      = {0.0F, 1.0F, 0.0F};
    Vec     camLoc  = {0.0F, 3.0F, 1.0F};
    Vec     objPt   = {0.0F, 0.0F, 0.0F};
    f32     top     = 0.0375F;
    f32     left    = -0.050F;
    f32     znear   = 0.1F;
    f32     zfar    = 10.0F;
    
    //MTXFrustum(p, top, -top, left, -left, znear, zfar);
    //GXSetProjection(p, GX_PERSPECTIVE);

	camera.setPerspective(60, 640/480, 0.1f, 100.0f);
    
	//MTXLookAt(Camera::viewMatrix, &camLoc, &up, &objPt);   
	camera.setPosition(Vector3(0.0F, 0.0F, 10.0F), Vector3(0.0F, 0.0F, 0.0F), Vector3(0.0F, 1.0F, 0.0F) );

	
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
    GXColor black = {0, 0, 0, 0};

    GXSetCopyClear(black, 0x00ffffff);

    // Set current vertex descriptor to enable position and color0.
    // Both use 8b index to access their data arrays.
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

    // Initialize lighting, texgen, and tev parameters
    GXSetNumChans(1); // default, color = vertex color
    GXSetNumTexGens(0); // no texture in this demo
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
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
    
    GXSetNumTexGens( 0 );
    GXSetNumTevStages( 1 );
    GXSetTevOp( GX_TEVSTAGE0, GX_PASSCLR );


	/*GXSetViewport( 
    100, 
    100, 
    50, 
    50, 
    10, 
    1000 ); */
	int tickNumber = (pad[0].button & PAD_BUTTON_UP) ? 8 : 0;

	f32 posX = -pad[0].stickX*0.1f;
	f32 posY = pad[0].stickY*0.1f;
    
    // model has a rotation about z axis
    MTXRotDeg(m, 'z', tickNumber * ticks);
	//view matrix
    //MTXConcat(v, m, mv);
	MTXConcat(Camera::viewMatrix, m, mv);


	//MTXRotDeg(m, 'x', 8 * ticks);
	//MTXConcat(mv, m, mv);
	MTXTrans(m, posX, posY, 0);
	MTXConcat(mv, m, mv);
    GXLoadPosMtxImm(mv, GX_PNMTX0);

	GXSetCurrentMtx(GX_PNMTX0);

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
static void PrintIntro( void )
{
    OSReport("\n\n********************************\n");
    OSReport("to quit:\n");
    OSReport("     hit the start button\n");
#ifdef MACOS
    OSReport("     click on the text output window\n");
    OSReport("     select quit from the menu or hit 'command q'\n");
    OSReport("     select 'don't save'\n");
#endif
    OSReport("********************************\n");
}

/*============================================================================*/
