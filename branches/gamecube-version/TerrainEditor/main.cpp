#include <vector>
#include <glee.h>
#include <GL/glui.h>
#include <stdlib.h>
#include <stdio.h>
#include "app.h"

#include <ConfParser/ConfParser.h>
#include <Misc/Logger.h>
#include <Texture/TextureMgr.h>
#include <Texture/TextureIO.h>
#include <MemManager/MemMgr_RawData.h>
#include <Shaders/ShaderManager.h>
#include <Camera/Camera.h>

#include "local.h"


#define CB_DISPLAYMODE	 0x00000000
#define CB_DISPLAYLAYERS 0x00000001
#define CB_BLENDBL1 0x00000002
#define CB_BLENDBL2 0x00000003
#define CB_BLENDBL3 0x00000004
#define CB_BLENDCH1 0x00000005
#define CB_BLENDCH2 0x00000006
#define CB_BLENDCH3 0x00000007
#define CB_BLENDSEA 0x00000008
#define CB_REPEATS0 0x00000009
#define CB_REPEATS1 0x0000000A
#define CB_REPEATS2 0x0000000B
#define CB_REPEATS3 0x0000000C
#define CB_TREEGEN  0x0000000D
#define CB_GENNORMS  0x0000000E
#define CB_H2M  0x0000000F
#define CB_XZSCALE  0x00000010
#define CB_YSCALE  0x00000011
#define CB_DEFAULT  0x00000012
#define CB_USELMAP  0x00000013
#define CB_SHADOW  0x00000014
#define CB_LMAPTEX  0x00000015
#define CB_GENTERRA 0x00000016
#define CB_LAYERSELECT 0x00000017
#define CB_CRATERUNDO 0x00000018
#define CB_DISPLAYTREES 0x00000019
#define CB_LIGHTCOLOR 0x0000001A
#define CB_DISPCRATERS 0x0000001B
#define CB_GENOBJ 0x0000001C
#define CB_TEXSWITCH_TFORM0 0x0000001D
#define CB_TEXSWITCH_TFORM1 0x0000001E
#define CB_TEXSWITCH_TFORM2 0x0000001F
#define CB_TEXSWITCH_TFORM3 0x00000020
#define CB_TEXSWITCH_BARREN0 0x00000021
#define CB_TEXSWITCH_BARREN1 0x00000022
#define CB_TEXSWITCH_BARREN2 0x00000023
#define CB_TEXSWITCH_BARREN3 0x00000024

#define SCR_T 50
#define SCR_L 50
#define SCR_W 800
#define SCR_H 600

using namespace CKLib;
using namespace std;

extern bool mode3d;
extern bool modecraterize;

int main_window(0);
GLUI * glui;

Terrain * terrain(0);

Camera cam;

GLUI_Checkbox * cb_displayLayer, * cb_lmaptex, *cb_dispTrees, *cb_displayCraters;
GLUI_RadioGroup * rg_mainmode, * rg_layerselect;
GLUI_Spinner * sp_blendlayer_change[3],* sp_blendlayer_bleed[3], * sp_sealevel,
				* sp_repeats[4],
				* sp_treelow, * sp_treehigh, * sp_treedensity,
				* sp_xzscale,* sp_yscale, * sp_shadow, * sp_interdistance,
				* sp_lightcolorx, * sp_lightcolory, * sp_lightcolorz,
				* sp_export_rescale;

void control_cb(int value)
{
	std::string texdlg;
	switch(value)
	{
	case CB_DISPLAYLAYERS :
		terrain->drawLayerTextures() = cb_displayLayer->get_int_val() ? true : false;
		break;
	case CB_DISPLAYMODE :
		{
		int mode = rg_mainmode->get_int_val();
		modecraterize = (mode > D_3D) ? true : false;
		terrain->displayMode() = MIN(D_3D,(DisplayMode)mode);
		if(terrain->displayMode() == D_3D)
		{
			float center = terrain->dim()*0.5f*terrain->xzscale();
			if(modecraterize)
				cam.setOrtho(-256,256,-256,256,40,2000);
			else
				cam.setFrustum(-10,10,-10,10,40,2000);
			cam.pos() = vec3f(center,1040,-center);
			cam.view() = vec3f(0.0f,-1.0f,0.0f);
			cam.up() = vec3f(0.0f,0.0f,-1.0f);
			mode3d = true;

			glPushAttrib(GL_MATRIX_MODE);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glPopMatrix();
			glPopAttrib();
		}
		else
		{
			cam.setOrtho(-1,1,-1,1,-1,1);
			cam.pos() = vec3f(0.0f,0.0f,0.5f);
			cam.view() = vec3f(0.0f,0.0f,-1.0f);
			cam.up() = vec3f(0.0f,1.0f,0.0f);
			mode3d = false;
		}
		}
		break;
	case CB_BLENDCH1 : 
		terrain->layerInfo(0)._changeAt = sp_blendlayer_change[0]->get_float_val();
		terrain->updateContribTex();
		break;
	case CB_BLENDCH2 : 
		terrain->layerInfo(1)._changeAt = sp_blendlayer_change[1]->get_float_val();
		terrain->updateContribTex();
		break;
	case CB_BLENDCH3 : 
		terrain->layerInfo(2)._changeAt = sp_blendlayer_change[2]->get_float_val();
		terrain->updateContribTex();
		break;
	case CB_BLENDBL1 : 
		terrain->layerInfo(0)._bleed = sp_blendlayer_bleed[0]->get_float_val();
		terrain->updateContribTex();
		break;
	case CB_BLENDBL2 : 
		terrain->layerInfo(1)._bleed = sp_blendlayer_bleed[1]->get_float_val();
		terrain->updateContribTex();
		break;
	case CB_BLENDBL3 : 
		terrain->layerInfo(2)._bleed = sp_blendlayer_bleed[2]->get_float_val();
		terrain->updateContribTex();
		break;
	case CB_BLENDSEA : 
		terrain->seaLevel() = sp_sealevel->get_float_val();
		break;
	case CB_REPEATS0 : 
		terrain->textureRepeats(0) = sp_repeats[0]->get_int_val();
		break;
	case CB_REPEATS1 : 
		terrain->textureRepeats(1) = sp_repeats[1]->get_int_val();
		break;
	case CB_REPEATS2 : 
		terrain->textureRepeats(2) = sp_repeats[2]->get_int_val();
		break;
	case CB_REPEATS3 : 
		terrain->textureRepeats(3) = sp_repeats[3]->get_int_val();
		break;
	case CB_TREEGEN : 
		terrain->countValidHeights(sp_treelow->get_float_val(),sp_treehigh->get_float_val());
		cout<<"Trees generated : "<<terrain->generateTreePositions(sp_treedensity->get_float_val(),
																   unsigned(sp_interdistance->get_int_val()))<<endl;
		break;
	case CB_XZSCALE : 
		terrain->xzscale() = sp_xzscale->get_float_val();
		terrain->updatePositionsNormals();
		break;
	case CB_YSCALE : 
		terrain->yscale() = sp_yscale->get_float_val();
		terrain->updatePositionsNormals();
		break;
	case CB_SHADOW : 
		terrain->shadowEffect() = sp_shadow->get_float_val();
		terrain->updateLightmap();
		break;
	case CB_LMAPTEX : 
		terrain->drawTexturedLightmap() = cb_lmaptex->get_int_val() ? true : false;
		break;
	case CB_GENTERRA : 
		{
			// Make the directory
			const string commstr(string("mkdir ") + terrain->outDirectory());
			system(commstr.c_str());
			terrain->generateTerraMap();
			terrain->switchTextureLayers();
			terrain->generateTerraMap();
			terrain->switchTextureLayers();
			terrain->generateTerraData(sp_export_rescale->get_float_val());
		}
		break;
	case CB_LAYERSELECT : 
		terrain->switchTextureLayers();
		break;
	case CB_CRATERUNDO : 
		terrain->undoCrater();
	case CB_DISPLAYTREES : 
		terrain->displayTrees() = !(terrain->displayTrees());
		
		break;
	case CB_LIGHTCOLOR : 
		terrain->lightColor() = vec4f(sp_lightcolorx->get_float_val(),
									sp_lightcolory->get_float_val(),
									sp_lightcolorz->get_float_val(),1.0f);
		
		break;
	case CB_DISPCRATERS : 
		terrain->displayCraters() = cb_displayCraters->get_int_val() ? true : false;
		
		break;
	case CB_GENOBJ : 
		{
			const string commstr(string("mkdir ") + terrain->outDirectory());
			system(commstr.c_str());
			terrain->generateTerraMap();
			terrain->generateTerraOBJ(sp_export_rescale->get_float_val());		
		}
		break;
	
	case CB_TEXSWITCH_TFORM0 : 
	case CB_TEXSWITCH_TFORM1 : 
	case CB_TEXSWITCH_TFORM2 : 
	case CB_TEXSWITCH_TFORM3 : 
		texdlg = loadTextureDlg();
		glui->set_main_gfx_window( main_window );
		glutSetWindow(main_window);
		terrain->setTFormTexture(value - CB_TEXSWITCH_TFORM0,TextureIO::instance()->loadImageFullPath(texdlg));
		// load a bitmap, in the tform layer 
		break;
	case CB_TEXSWITCH_BARREN0 : 
	case CB_TEXSWITCH_BARREN1 : 
	case CB_TEXSWITCH_BARREN2 : 
	case CB_TEXSWITCH_BARREN3 : 
		// load a bitmap, in the barren layer 
		texdlg = loadTextureDlg();
		glui->set_main_gfx_window( main_window );
		glutSetWindow(main_window);
		terrain->setBarrenTexture(value - CB_TEXSWITCH_BARREN0,TextureIO::instance()->loadImageFullPath(texdlg));
		break;
	}

}

void FreeResources()
{
	if(terrain)
		delete terrain;
	ShaderManager::destroy();
	TextureMgr::destroy();
	MemMgrRaw::destroy();
	Logger::destroy();
	exit(0);
}

void InitializeOGL()
{
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glPointSize(3.0f);

	ConfParser parser("config.ini");
	Logger::init(parser.getSection("Logger"));
	MemMgrRaw::init(parser.getSection("MemManager:RawData"));
	TextureMgr::init(parser.getSection("Texture"));
	ShaderManager::init(parser.getSection("Shader"));

	cam.setOrtho(-1,1,-1,1,-1,1);
	cam.pos() = vec3f(0.0f,0.0f,0.5f);
	cam.view() = vec3f(0.0f,0.0f,-1.0f);
	cam.up() = vec3f(0.0f,1.0f,0.0f);

	CHECK_GL_ERROR();
	terrain = new Terrain(parser.getSection("Terrain"));
	CHECK_GL_ERROR();
}

int main ( int argc, char ** argv)
{
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_MULTISAMPLE );
    glutInitWindowPosition( SCR_T, SCR_L );
    glutInitWindowSize( SCR_W, SCR_H );
    glutInit( &argc, argv );

    main_window = glutCreateWindow("Terrain Editor");

	InitializeOGL();

    GLUI_Master.set_glutDisplayFunc( glutDisplay );
    GLUI_Master.set_glutReshapeFunc( glutResize );  
    GLUI_Master.set_glutKeyboardFunc( glutKeyboard );
    GLUI_Master.set_glutSpecialFunc( glutSpecial );
    GLUI_Master.set_glutMouseFunc( glutMouse );
	glutMotionFunc( glutMotion );
	GLUI_Master.set_glutIdleFunc( glutIdle );

	glui = GLUI_Master.create_glui( "Controls", 0, 400, 50 );

	glui->set_main_gfx_window( main_window );

	rg_layerselect = glui->add_radiogroup(0,CB_LAYERSELECT,control_cb);
	glui->add_radiobutton_to_group(rg_layerselect,"Terraformed");
	glui->add_radiobutton_to_group(rg_layerselect,"Barren");

	// Display Layers CheckBox
	cb_displayLayer = glui->add_checkbox("Display layer textures",0,CB_DISPLAYLAYERS,control_cb);

	// Main Display Mode Radiogroup
	rg_mainmode = glui->add_radiogroup(0,CB_DISPLAYMODE,control_cb);
	glui->add_radiobutton_to_group(rg_mainmode,"Grayscale");
	glui->add_radiobutton_to_group(rg_mainmode,"Coloured");
	glui->add_radiobutton_to_group(rg_mainmode,"Textured");
	glui->add_radiobutton_to_group(rg_mainmode,"Normals");
	glui->add_radiobutton_to_group(rg_mainmode,"3d Yay");
	glui->add_radiobutton_to_group(rg_mainmode,"Craterize Mode");

	// Layer Blending Panel
	GLUI_Rollout * roll_blend = glui->add_rollout("Blend Options",true);
	sp_blendlayer_change[0] = glui->add_spinner_to_panel(roll_blend,"Layer Dirt - Grass change at",GLUI_SPINNER_FLOAT,0,CB_BLENDCH1,control_cb);
	sp_blendlayer_change[0]->set_float_limits(0,1);
	sp_blendlayer_change[0]->set_float_val(terrain->layerInfo(0)._changeAt);
	sp_blendlayer_bleed[0] = glui->add_spinner_to_panel(roll_blend,"Layer Dirt - Grass blend    ",GLUI_SPINNER_FLOAT,0,CB_BLENDBL1,control_cb);
	sp_blendlayer_bleed[0]->set_float_limits(0,1);
	sp_blendlayer_bleed[0]->set_float_val(terrain->layerInfo(0)._bleed);
	sp_blendlayer_change[1] = glui->add_spinner_to_panel(roll_blend,"Layer Grass - Rock change at",GLUI_SPINNER_FLOAT,0,CB_BLENDCH2,control_cb);
	sp_blendlayer_change[1]->set_float_limits(0,1);
	sp_blendlayer_change[1]->set_float_val(terrain->layerInfo(1)._changeAt);
	sp_blendlayer_bleed[1] = glui->add_spinner_to_panel(roll_blend,"Layer Grass - Rock blend    ",GLUI_SPINNER_FLOAT,0,CB_BLENDBL2,control_cb);
	sp_blendlayer_bleed[1]->set_float_limits(0,1);
	sp_blendlayer_bleed[1]->set_float_val(terrain->layerInfo(1)._bleed);
	sp_blendlayer_change[2] = glui->add_spinner_to_panel(roll_blend,"Layer Rock - Snow change at",GLUI_SPINNER_FLOAT,0,CB_BLENDCH3,control_cb);
	sp_blendlayer_change[2]->set_float_limits(0,1);
	sp_blendlayer_change[2]->set_float_val(terrain->layerInfo(2)._changeAt);
	sp_blendlayer_bleed[2] = glui->add_spinner_to_panel(roll_blend,"Layer Rock - Snow blend    ",GLUI_SPINNER_FLOAT,0,CB_BLENDBL3,control_cb);
	sp_blendlayer_bleed[2]->set_float_limits(0,1);
	sp_blendlayer_bleed[2]->set_float_val(terrain->layerInfo(2)._bleed);

	sp_sealevel = glui->add_spinner_to_panel(roll_blend,"Sea level  ",GLUI_SPINNER_FLOAT,0,CB_BLENDSEA,control_cb);
	sp_sealevel->set_float_limits(0,1);
	sp_sealevel->set_float_val(terrain->seaLevel());

	// Texture Switcher
	GLUI_Rollout * roll_texswitch = glui->add_rollout("Texture Switch",false);
	GLUI_Button * btn_texswitch_tform[4];
	GLUI_Button * btn_texswitch_barren[4];
	for(int i=0;i<4;++i)
	{
		btn_texswitch_tform[i] = glui->add_button_to_panel(roll_texswitch,(string("TForm Layer ") + ToString<unsigned>(i)).c_str(),
													  CB_TEXSWITCH_TFORM0 + i,control_cb);
		btn_texswitch_barren[i] = glui->add_button_to_panel(roll_texswitch,(string("Barren Layer ") + ToString<unsigned>(i)).c_str(),
													  CB_TEXSWITCH_BARREN0 + i,control_cb);
	}

	// Texture Repeats
	GLUI_Rollout * roll_repeats = glui->add_rollout("Texture Repeats",true);
	for(int i=0;i<4;++i)
	{
		sp_repeats[i] = glui->add_spinner_to_panel(roll_repeats,(string("Layer ") + ToString<unsigned>(i)).c_str()
													,GLUI_SPINNER_INT,0,CB_REPEATS0 + i,control_cb);
		sp_repeats[i]->set_int_limits(0,100);
		sp_repeats[i]->set_int_val(terrain->textureRepeats(i));
	}

	// Tree Generation
	GLUI_Rollout * roll_trees = glui->add_rollout("Tree Generation");
	sp_treelow = glui->add_spinner_to_panel(roll_trees,"Tree Low Height",GLUI_SPINNER_FLOAT,0,CB_TREEGEN,control_cb);
	sp_treelow->set_float_limits(0,1);
	sp_treehigh = glui->add_spinner_to_panel(roll_trees,"Tree High Height",GLUI_SPINNER_FLOAT,0,CB_TREEGEN,control_cb);
	sp_treehigh->set_float_limits(0,1);
	sp_treedensity = glui->add_spinner_to_panel(roll_trees,"Tree Density",GLUI_SPINNER_FLOAT,0,CB_TREEGEN,control_cb);
	sp_treedensity->set_float_limits(0,1);

	sp_treelow->set_float_val(0.0f);
	sp_treehigh->set_float_val(1.0f);
	sp_treedensity->set_float_val(0.0f);

	sp_interdistance = glui->add_spinner_to_panel(roll_trees,"Tree minimum inter-distance",GLUI_SPINNER_INT,0,CB_TREEGEN,control_cb);
	sp_interdistance->set_int_limits(0,30);
	sp_interdistance->set_int_val(0);

	cb_dispTrees = glui->add_checkbox_to_panel(roll_trees,"Draw Trees",0,CB_DISPLAYTREES,control_cb);

	// Heightmap to model
	GLUI_Rollout * roll_h2m = glui->add_rollout("Model update");
	sp_xzscale = glui->add_spinner_to_panel(roll_h2m,"X-Z scale",GLUI_SPINNER_FLOAT,0,CB_XZSCALE,control_cb);
	sp_xzscale->set_float_limits(1.0f,1000.0f);
	sp_xzscale->set_float_val(terrain->xzscale());
	sp_yscale = glui->add_spinner_to_panel(roll_h2m,"Y scale",GLUI_SPINNER_FLOAT,0,CB_YSCALE,control_cb);
	sp_yscale->set_float_limits(10.0f,1000);
	sp_yscale->set_float_val(terrain->yscale());
	sp_shadow = glui->add_spinner_to_panel(roll_h2m,"Shadow Effect",GLUI_SPINNER_FLOAT,0,CB_SHADOW,control_cb);
	sp_shadow->set_float_limits(0.0f,1.0f);
	sp_shadow->set_float_val(terrain->shadowEffect());

	cb_lmaptex = glui->add_checkbox("Draw Textured Lightmap - Final Form",0,CB_LMAPTEX,control_cb);


	GLUI_Rollout * roll_light = glui->add_rollout("Lighting");
	sp_lightcolorx = glui->add_spinner_to_panel(roll_light,"Red : ",GLUI_SPINNER_FLOAT,0,CB_LIGHTCOLOR,control_cb);
	sp_lightcolorx->set_float_val(1.0f);
	sp_lightcolorx->set_float_limits(0.0f,1.0f);
	sp_lightcolory = glui->add_spinner_to_panel(roll_light,"Green : ",GLUI_SPINNER_FLOAT,0,CB_LIGHTCOLOR,control_cb);
	sp_lightcolory->set_float_val(1.0f);
	sp_lightcolory->set_float_limits(0.0f,1.0f);
	sp_lightcolorz = glui->add_spinner_to_panel(roll_light,"Blur : ",GLUI_SPINNER_FLOAT,0,CB_LIGHTCOLOR,control_cb);
	sp_lightcolorz->set_float_val(1.0f);
	sp_lightcolorz->set_float_limits(0.0f,1.0f);

	glui->add_separator();

	// Craters
	GLUI_Rollout * roll_crater = glui->add_rollout("Craterize",false);
	cb_displayCraters = glui->add_checkbox_to_panel(roll_crater,"Display radii",0,CB_DISPCRATERS,control_cb);
	GLUI_Button * btn_crater_undo = glui->add_button_to_panel(roll_crater,"Undo last",CB_CRATERUNDO,control_cb);

	GLUI_Rollout * roll_export = glui->add_rollout("Export",false);
	sp_export_rescale = glui->add_spinner_to_panel(roll_export,"Rescale xz to :",GLUI_SPINNER_FLOAT);
	sp_export_rescale->set_float_limits(512.0f,8192.0f);
	sp_export_rescale->set_float_val(512.0f);
	GLUI_Button * terrabtn = glui->add_button_to_panel(roll_export,"Generate Terra Maps + Info",CB_GENTERRA,control_cb);
	GLUI_Button * objbtn = glui->add_button_to_panel(roll_export,"Export to OBJ",CB_GENOBJ,control_cb);

	glutMainLoop();

	return 0;


}