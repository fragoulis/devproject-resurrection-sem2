#include "app.h"
#include <glee.h>
#include <windows.h>
#include <commdlg.h>
#include <GL/glui.h>
#include <GL/gl.h>
#include "local.h"
#include <Texture/TextureMgr.h>
#include <Texture/Texture.h>
#include <Misc/utils.h>
#include <Camera/Camera.h>
#include <Math/vec2.h>
#include <Math/vec3.h>


bool mode3d = false;
bool modecraterize = false;

using namespace CKLib;

int oldX,oldY;
extern Terrain * terrain;

extern int main_window;

extern Camera cam;

bool lbtnheld = false;
vec2f captured_vec;


void glutResize(int width, int height)
{
	glViewport(0,0,width,height);
	if(mode3d)
		if(modecraterize)
			cam.setOrtho(-256,256,-256,256,40,2000);
		else
			cam.setFrustum(-10,10,-10,10,40,2000);
	else
		cam.setOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
}
void glutKeyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27 :
			FreeResources();
			break;
		// Rotation - movement from above
		case 'a' :
			cam.strafe(-3);
			break;
		case 'd' :
			cam.strafe(3);
			break;
		case 'w' :
			cam.raise(3);
			break;
		case 's' :
			cam.raise(-3);
			break;
		case 'q' :
			cam.move(2.4);
			break;
		case 'e' :
			cam.move(-2.4);
			break;

	}
}
void glutMouse(int button, int state, int x, int y)
{
	if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			lbtnheld = true;
			captured_vec = vec2f(float(x),float(y));
			if(modecraterize)
			{
				double projmat[16];
				double mview[16];
				int vp[4];
				vec3<double> out;

				glPushAttrib(GL_MATRIX_MODE);
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glLoadIdentity();
				cam.setEyeSpace();

				glGetDoublev(GL_PROJECTION_MATRIX,projmat);
				glGetDoublev(GL_MODELVIEW_MATRIX,mview);
				glGetIntegerv(GL_VIEWPORT,vp);

				glPopMatrix();
				glPopAttrib();
				gluUnProject(x,vp[3] - y,0,mview,projmat,vp,&(out.x()),&(out.y()),&(out.z()));
				captured_vec.x() = float(out.x());
				captured_vec.y() = float(out.z());
			}
		}
		else
		{
			lbtnheld = false;
			if(modecraterize)
			{
				/* update :		
					contribution tex
					lightmap
				*/
				double projmat[16];
				double mview[16];
				int vp[4];
				vec3<double> out;

				glPushAttrib(GL_MATRIX_MODE);
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glLoadIdentity();
				cam.setEyeSpace();

				glGetDoublev(GL_PROJECTION_MATRIX,projmat);
				glGetDoublev(GL_MODELVIEW_MATRIX,mview);
				glGetIntegerv(GL_VIEWPORT,vp);

				glPopMatrix();
				glPopAttrib();
				gluUnProject(x,vp[3] - y,0,mview,projmat,vp,&(out.x()),&(out.y()),&(out.z()));

				vec2f curVec(float(out.x()),float(out.z()));
				float radius = fabs(curVec.x() - captured_vec.x());
				float tformradius = fabs(curVec.y() - captured_vec.y());
				
				terrain->finalizeCrater(1.0f, //const float linbell_mix,
							 0.8f,//const float bell_area,
							 radius,//const float radius,
							 tformradius,//const float radius,
							 captured_vec,//const vec2<double>& center,
							 0.8f);//const float oldmapmix)
			}
		}
	}
}

void glutMotion(int x, int y)
{
	if(lbtnheld)
	{
		if(modecraterize)
		{

		}
		else
		{
			vec2f tmpvec = captured_vec - vec2f(float(x),float(y));
			float len = MIN(tmpvec.length(),400.0f);
			vec3f arb(tmpvec.x(),400 - len,tmpvec.y());
			terrain->lightdir() = normalize(arb);
			terrain->updateLightmap();
		}
	}
	oldX = x;
	oldY = y;
}

void glutSpecial(int value, int x, int y){} 

void glutIdle()
{
	glutSetWindow(main_window);
	glutPostRedisplay();
}

void glutDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cam.setEyeSpace();
	CHECK_GL_ERROR();

	TextureMgr::instance()->setTextureTarget(GL_TEXTURE_2D,0);		// Stupid glui
	glColor3f(1.0f,1.0f,1.0f);
	terrain->draw();

	TextureMgr::instance()->setTextureTarget(GL_TEXTURE_2D,0);

	glFlush();
    glutSwapBuffers();
}

std::string loadTextureDlg()
{
	OPENFILENAME OpenFileName;
	char *szFile = (char *)malloc(MAX_PATH);
	char CurrentDir[MAX_PATH];

	szFile[0] = 0;
	GetCurrentDirectory( MAX_PATH, CurrentDir );

	OpenFileName.lStructSize = sizeof( OPENFILENAME );
	OpenFileName.hwndOwner = NULL;
	OpenFileName.lpstrCustomFilter = NULL;
	OpenFileName.nMaxCustFilter = 0;
	OpenFileName.nFilterIndex = 0;
	OpenFileName.lpstrFile = szFile;
	OpenFileName.nMaxFile = MAX_PATH;
	OpenFileName.lpstrFileTitle = NULL;
	OpenFileName.nMaxFileTitle = 0;
	OpenFileName.lpstrInitialDir = CurrentDir;
	OpenFileName.lpstrTitle = "Open Texture..";
	OpenFileName.nFileOffset = 0;
	OpenFileName.nFileExtension = 0;
	OpenFileName.lpstrDefExt = NULL;
	OpenFileName.lCustData = 0;
	OpenFileName.lpfnHook = NULL;
	OpenFileName.lpTemplateName = NULL;
	OpenFileName.Flags = OFN_EXPLORER;
	//OpenFileName.FlagsEx = 0;
	OpenFileName.lpstrFilter="Texture files \0*.sgi;*.bmp;*.dds;*.tex\0\0";
	if (!GetOpenFileName(&OpenFileName)) 
		szFile[0] = '\0';
	std::string outs(szFile);
	free(szFile);

	SetCurrentDirectory(CurrentDir);
	return outs;
}