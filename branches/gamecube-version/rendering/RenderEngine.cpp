#include "RenderEngine.h"
#include "IRenderer.h"
#include "WorldRenderer.h"
#include "LoadingRenderer.h"
#include "HUDRenderer.h"
#include "MenuRenderer.h"
//#include "PauseRenderer.h"
//#include "LevelCompleteRenderer.h"
#include "GameOverRenderer.h"
#include "../ParticleSystem/PS_Manager.h"
#include "../math/Point2.h"
#include "../math/Point3.h"
#include <string>
#include <iostream>

#include "../gfxutils/ConfParser/ConfParser.h"
#include "../GameLogic/GameLogic.h"
#include "../gfxutils/Model/ModelMgr.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/VA/VATTable.h"

using namespace std;

unsigned RenderEngine :: POLY_COUNT(0);

RenderEngine :: RenderEngine() 
: m_confParser(0),
m_playerBounds(0.0f,0.0f,0.0f,0.0f),
m_cameraBounds(0.0f,0.0f,0.0f,0.0f),
m_wireframe(false)
{
	int scrw = 640,scrh = 480;

	setViewport(0,0,scrw,scrh);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glClearColor(0.0f,0.0f,0.0f,0.0f);

	m_camHeightAbovePlane = 2000.0f;
}

RenderEngine :: ~RenderEngine()
{
	// TODO: when we implement a loading screen, remove this
	onApplicationUnload();
}

void RenderEngine :: init()
{
	// TODO: remove m_confParser entirely and read global data now, from ps
	m_confParser = new ConfParser("config/config.gfx");
	TextureMgr::safeInstance().init(m_confParser->getSection("Texture"));
	VATTable::buildVAT();

	// set clear color to black
    GXColor black = {0, 0, 0, 0};
    GXSetCopyClear(black, 0x00ffffff);
}

void RenderEngine :: onApplicationLoad(const ParserSection& ps)
{

	//TextureMgr::init(m_confParser->getSection("Texture"));
	//ShaderManager::init(m_confParser->getSection("Shader"));
	//VBOMgr::safeInstance().init(m_confParser->getSection("VBOSettings"));
	//ModelMgr::safeInstance().init(m_confParser->getSection("ModelSettings"));
	PS_Manager::safeInstance().init(m_confParser->getSection("ParticleSystem"));

	ModelMgr::safeInstance().init(m_confParser->getSection("ModelSettings"));

	m_settings.init(m_confParser->getSection("EntitySettings"));

	delete m_confParser;
}

void RenderEngine :: onApplicationUnload()
{
	PS_Manager::instance().clear();
	PS_Manager::destroy();

	ModelMgr::destroy();
	TextureMgr::destroy();

	if (m_confParser != 0) delete m_confParser;
}

void RenderEngine :: activateRenderer(const std::string& name)
{
	IRenderer* r = _findRenderer(name);
	if (r == NULL) {
		r = _createRenderer(name);
		assert(r != NULL);
		_addRenderer(name, r);
	}
	_activateRenderer(r);
}

void RenderEngine :: deactivateRenderer(const std::string& name)
{
	IRenderer* r = _findRenderer(name);
	if (r != NULL) _deactivateRenderer(r);
}

void RenderEngine :: deactivateAllRenderers()
{
	m_activeRenderers.clear();
}

void RenderEngine :: loadRenderer(const std::string& name)
{
	IRenderer* r = _findRenderer(name);
	if (r == NULL) {
		r = _createRenderer(name);
		assert(r != NULL);
		_addRenderer(name, r);
	}
}

void RenderEngine :: unloadRenderer(const std::string& name)
{
	IRenderer* r = _findRenderer(name);
	if (r != NULL) {
		_deactivateRenderer(r);
		//_removeRenderer(name);
		//delete r;
	}
}

IRenderer* RenderEngine :: getRenderer(const std::string& name)
{
	IRenderer* r = _findRenderer(name);
	if (r == NULL) {
		r = _createRenderer(name);
		assert(r != NULL);
		_addRenderer(name, r);
	}
	return r;
}

struct RendererDeleter
{
	void operator () (const std::string&, IRenderer* r) { delete r; }
};
void RenderEngine::unloadAllRenderers()
{
	deactivateAllRenderers();
	//m_renderers.visitObjects(RendererDeleter());
	//m_renderers.clear();
}


void RenderEngine :: render(Graphics &g) const
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (RendererList::const_iterator i = m_activeRenderers.begin(); i != m_activeRenderers.end(); ++i)
		(*i)->render(g);
}

void RenderEngine :: update ( float dt )
{
	for (RendererList::const_iterator i = m_activeRenderers.begin(); i != m_activeRenderers.end(); ++i)
		(*i)->update(dt);
}

IRenderer* RenderEngine :: _createRenderer(const std::string& name) const
{
	if (name == "world") return new WorldRenderer();
	if (name == "loading") return new LoadingRenderer();
	if (name == "hud") return new HUDRenderer();
	if (name == "menu") return new MenuRenderer();
	//if (name == "pause") return new PauseRenderer();
	//if (name == "LevelComplete") return new LevelCompleteRenderer();
	if (name == "GameOver") return new GameOverRenderer();
	return 0;
}

//const ParserSection * RenderEngine :: getParserSection(const std::string& secname) const
//{
//	return m_confParser->getSection(secname);
//}

void RenderEngine :: setViewport(int x,int y,int width, int height)
{
	m_viewport[0] = x;
	m_viewport[1] = y;
	m_viewport[2] = width;
	m_viewport[3] = height;
	//glViewport(m_viewport[0],m_viewport[1],m_viewport[2],m_viewport[3]);
}

void RenderEngine :: getViewport(int vp[4]) const
{
	vp[0] = m_viewport[0];
	vp[1] = m_viewport[1];
	vp[2] = m_viewport[2];
	vp[3] = m_viewport[3];
}

//Point3 RenderEngine :: getScreenPositionFromMapPosition(const Point3& p)
//{
//	// get world renderer
//	WorldRenderer * wr = static_cast<WorldRenderer *>(_findRenderer("world"));
//	// get it's modelview & proj matrices
//	const double * mv = wr->getModelViewMatrixd();
//	const double * proj = wr->getProjectionMatrixd();
//	
//	// Project
//	double outx,outy,outz;
//	gluProject(p.getX(),p.getY(),p.getZ(),mv,proj,m_viewport,&outx,&outy,&outz);
//	return Point3(float(outx),float(outy),float(outz));
//}
//
//Point3 RenderEngine :: getMapPositionFromScreenPosition(const Point2& p)
//{
//	// get world renderer
//	WorldRenderer * wr = dynamic_cast<WorldRenderer *>(_findRenderer("world"));
//	// get it's modelview & proj matrices
//	const double * mv = wr->getModelViewMatrixd();
//	const double * proj = wr->getProjectionMatrixd();
//	
//	// UnProject - get the ray
//	double ray1x,ray1y,ray1z,
//		   ray2x,ray2y,ray2z;
//	gluUnProject(p.getX(),m_viewport[3] - p.getY(),0.0,mv,proj,m_viewport,&ray1x,&ray1y,&ray1z);
//	gluUnProject(p.getX(),m_viewport[3] - p.getY(),1.0,mv,proj,m_viewport,&ray2x,&ray2y,&ray2z);
//
//	const Vector3 ray(float(ray2x - ray1x),float(ray2y - ray1y),float(ray2z - ray1z));
//	
//
//	const float planeheight = GameLogic::instance().getGamePlaneHeight();
//	const float pcent = float((planeheight - ray1y) / (ray2y - ray1y));
//	const Vector3 pos = ray*pcent + Vector3(float(ray1x),float(ray1y),float(ray1z));
//
//	return Point3(pos.getX(),pos.getY(),pos.getZ());
//}
//
//Point3 RenderEngine :: getMapPositionFromScreenPosition(const Point2& p,const float h)
//{
//	// get world renderer
//	WorldRenderer * wr = dynamic_cast<WorldRenderer *>(_findRenderer("world"));
//	// get it's modelview & proj matrices
//	const double * mv = wr->getModelViewMatrixd();
//	const double * proj = wr->getProjectionMatrixd();
//	
//	// UnProject - get the ray
//	double ray1x,ray1y,ray1z,
//		   ray2x,ray2y,ray2z;
//	gluUnProject(p.getX(),m_viewport[3] - p.getY(),0.0,mv,proj,m_viewport,&ray1x,&ray1y,&ray1z);
//	gluUnProject(p.getX(),m_viewport[3] - p.getY(),1.0,mv,proj,m_viewport,&ray2x,&ray2y,&ray2z);
//
//	const Vector3 ray(float(ray2x - ray1x),float(ray2y - ray1y),float(ray2z - ray1z));
//	
//
//	const float planeheight = h;
//	const float pcent = float((planeheight - ray1y) / (ray2y - ray1y));
//	const Vector3 pos = ray*pcent + Vector3(float(ray1x),float(ray1y),float(ray1z));
//
//	return Point3(pos.getX(),pos.getY(),pos.getZ());
//}

void RenderEngine :: drawTexturedQuad(const Vector3& ll,const Vector3& right,const Vector3& up,
											 const Vector2& tex_ll, const Vector2& extents)
{
	const Vector3 lr(ll + right);
	const Vector3 ul(ll + up);
	const Vector3 ur(ul + right);
	//glBegin(GL_QUADS);
	//	glTexCoord2fv(tex_ll.cfp());
	//	glVertex3fv(ll.cfp());
	//	glTexCoord2f(tex_ll.getX() + extents.getX(),tex_ll.getY());
	//	glVertex3fv(lr.cfp());
	//	glTexCoord2f(tex_ll.getX() + extents.getX(),tex_ll.getY() + extents.getY());
	//	glVertex3fv(ur.cfp());
	//	glTexCoord2f(tex_ll.getX(),tex_ll.getY() + extents.getY());
	//	glVertex3fv(ul.cfp());
	//glEnd();
}

void RenderEngine :: drawArbTexturedQuad(const Vector3& ll,const Vector3& lr,const Vector3& ur,const Vector3& ul,
								 const Vector2& tex_ll, const Vector2& extents)
{
	//glBegin(GL_QUADS);
	//	glTexCoord2fv(tex_ll.cfp());
	//	glVertex3fv(ll.cfp());
	//	glTexCoord2f(tex_ll.getX() + extents.getX(),tex_ll.getY());
	//	glVertex3fv(lr.cfp());
	//	glTexCoord2f(tex_ll.getX() + extents.getX(),tex_ll.getY() + extents.getY());
	//	glVertex3fv(ur.cfp());
	//	glTexCoord2f(tex_ll.getX(),tex_ll.getY() + extents.getY());
	//	glVertex3fv(ul.cfp());
	//glEnd();
}

void RenderEngine :: drawQuad(const Vector3& ll,const Vector3& right,const Vector3& up,const u8 repeats)
{
	const Vector3 lr(ll + right);
	const Vector3 ul(ll + up);
	const Vector3 ur(ul + right);
	//return;
	GXBegin(GX_QUADS,GX_VTXFMT7,4);
		GXPosition3f32( ul.getX(), ul.getY(), ul.getZ() );
		GXTexCoord2u8( 0,0);
		GXPosition3f32( ll.getX(), ll.getY(), ll.getZ() );
		GXTexCoord2u8( 0,repeats);
		GXPosition3f32( lr.getX(), lr.getY(), lr.getZ() );
		GXTexCoord2u8( repeats,repeats);
		GXPosition3f32( ur.getX(), ur.getY(), ur.getZ() );
		GXTexCoord2u8( repeats,0);
	GXEnd();
}

void RenderEngine :: drawTexturedRectangle(s16 left, s16 bottom, s16 width, s16 height, u8 repeats)
{
	GXBegin(GX_QUADS, GX_VTXFMT1, 4);
	{
		GXPosition3s16(left, bottom + height, 0);
		//GXColor1u32(u32(0xff0000ff));
		GXTexCoord2u8(0, 0);

		GXPosition3s16(left + width, bottom + height, 0);
		//GXColor1u32(u32(0x00ff00ff));
		GXTexCoord2u8(repeats, 0);

		GXPosition3s16(left + width, bottom, 0);
		//GXColor1u32(u32(0x0000ffff));
		GXTexCoord2u8(repeats, repeats);

		GXPosition3s16(left, bottom, 0);
		//GXColor1u32(u32(0x0000ffff));
		GXTexCoord2u8(0, repeats);
	}
	GXEnd();
}

void RenderEngine :: drawTexturedColoredRectangle(u32 color, s16 left, s16 bottom, s16 width, s16 height, u8 repeats)
{
	GXBegin(GX_QUADS, GX_VTXFMT6, 4);
	{
		GXPosition3s16(left, bottom + height, 0);
		GXTexCoord2u8(0, 0);
		GXColor1u32(color);

		GXPosition3s16(left + width, bottom + height, 0);
		GXTexCoord2u8(repeats, 0);
		GXColor1u32(color);

		GXPosition3s16(left + width, bottom, 0);
		GXTexCoord2u8(repeats, repeats);
		GXColor1u32(color);

		GXPosition3s16(left, bottom, 0);
		GXTexCoord2u8(0, repeats);
		GXColor1u32(color);
	}
	GXEnd();
}

//void RenderEngine :: getWsScreenEdges(Point3 pts[4])
//{
//	pts[0] = m_wsScreenEdges[0];
//	pts[1] = m_wsScreenEdges[1];
//	pts[2] = m_wsScreenEdges[2];
//	pts[3] = m_wsScreenEdges[3];
//}
//
//void RenderEngine :: computeWsScreenEdges()
//{
//	m_wsScreenEdges[0] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0]),float(m_viewport[1] + m_viewport[3])));
//	m_wsScreenEdges[1] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0] + m_viewport[2]),float(m_viewport[1] + m_viewport[3])));
//	m_wsScreenEdges[2] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0] + m_viewport[2]),float(m_viewport[1])));
//	m_wsScreenEdges[3] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0]),float(m_viewport[1])));
//}
//
//void RenderEngine :: getWsScreenEdges(Point3 pts[4],const float h)
//{
//	pts[0] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0]),float(m_viewport[1] + m_viewport[3])),h);
//	pts[1] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0] + m_viewport[2]),float(m_viewport[1] + m_viewport[3])),h);
//	pts[2] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0] + m_viewport[2]),float(m_viewport[1])),h);
//	pts[3] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0]),float(m_viewport[1])),h);
//}

void RenderEngine :: boundPlayerPosition(Vector3& playerpos)
{
	playerpos.setX(min(max(playerpos.getX(),m_playerBounds.getX()),m_playerBounds.getY()));
	playerpos.setZ(min(max(playerpos.getZ(),m_playerBounds.getW()),m_playerBounds.getZ()));
}

void RenderEngine :: boundCameraPosition(Vector3& camerapos)
{
	camerapos.setX(min(max(camerapos.getX(),m_cameraBounds.getX()),m_cameraBounds.getY()));
	camerapos.setZ(min(max(camerapos.getZ(),m_cameraBounds.getW()),m_cameraBounds.getZ()));
}

const Vector4 RenderEngine :: getColorFromEbombType  (const EbombType& et) const
{
	static const Vector4 EbombColours[6] = 
	{
		Vector4(1.0f,0.0f,0.0f,1.0f),
		Vector4(1.0f,1.0f,0.0f,1.0f),
		Vector4(0.0f,0.0f,1.0f,1.0f),
		Vector4(1.0f,0.5f,0.0f,1.0f),
		Vector4(0.0f,1.0f,0.0f,1.0f),
		Vector4(1.0f,0.0f,1.0f,1.0f)
	};
	return (EbombColours[unsigned(et)]);
}

const Vector4 RenderEngine :: getColorFromEnergyType (const EnergyType& et) const 
{
	static const Vector4 EnergyColours[3] = 
	{
		Vector4(1.0f,0.0f,0.0f,1.0f),
		Vector4(1.0f,1.0f,0.0f,1.0f),
		Vector4(0.0f,0.0f,1.0f,1.0f)
	};
	return (EnergyColours[unsigned(et)]);
}

void RenderEngine :: togglePostProc()
{
	WorldRenderer * wr = reinterpret_cast<WorldRenderer *>(_findRenderer("world"));
	if(wr)
		wr->togglePostProc();
}

void RenderEngine :: toggleWireframe()
{
	WorldRenderer * wr = reinterpret_cast<WorldRenderer *>(_findRenderer("world"));
	if(wr)
		wr->toggleWireframe();
}

void RenderEngine :: adjustCamera(const float amount)
{
	cerr<<"Offset now is "<<m_camHeightAbovePlane - 2000.0f<<endl;
	m_camHeightAbovePlane += amount;
	WorldRenderer * wr = reinterpret_cast<WorldRenderer *>(_findRenderer("world"));
	if(wr)
		wr->adjustCameras();

}

void RenderEngine :: setLevelLight(const Vector4& llight)
{
	m_levelLightPos = llight;
}

void RenderEngine :: setLevelLightColor(const Vector4& llight)
{
	m_levelLightColor = llight;
}

void RenderEngine :: disableLight()
{
	GXSetChanCtrl(
        GX_COLOR0A0,
        GX_DISABLE,  // disable channel
        GX_SRC_VTX,  // amb source
        GX_SRC_VTX,  // mat source
        GX_LIGHT0,   // light mask
        GX_DF_NONE,  // diffuse function
        GX_AF_NONE);
}

void RenderEngine :: setLight()
{
	static GXColor WHITE = {255,255,255,255};
	static GXColor DARK = {0,0,0,0};
	
	Vector3 lightn(-m_levelLightPos.getX(),-m_levelLightPos.getY(),-m_levelLightPos.getZ());
	lightn.normalize();
	GXInitLightDir(&m_gc_light,lightn.getX(),lightn.getY(),lightn.getZ());

	GXColor col;
	col.a = u8(m_levelLightColor.getW()*255);
	col.r = u8(m_levelLightColor.getX()*255);
	col.g = u8(m_levelLightColor.getY()*255);
	col.b = u8(m_levelLightColor.getZ()*255);
	GXInitLightColor(&m_gc_light,col);

    GXLoadLightObjImm(&m_gc_light, GX_LIGHT0);

    // Lighting channel
    GXSetNumChans(1); // number of active color channels
    GXSetChanCtrl(
        GX_COLOR0A0,
        GX_ENABLE,   // enable channel
        GX_SRC_REG,  // amb source
        GX_SRC_REG,  // mat source
        GX_LIGHT0,   // light mask
        GX_DF_CLAMP, // diffuse function
        GX_AF_NONE);
    // set up ambient color
    GXSetChanAmbColor(GX_COLOR0A0, DARK);
    // set up material color
    GXSetChanMatColor(GX_COLOR0A0, WHITE);

}