#include "RenderEngine.h"
#include "IRenderer.h"
#include "WorldRenderer.h"
#include "LoadingRenderer.h"
#include "HUDRenderer.h"
#include "MenuRenderer.h"
#include "PauseRenderer.h"
#include "LevelCompleteRenderer.h"
#include "GameOverRenderer.h"
#include "../math/Point2.h"
#include "../math/Point3.h"
#include <cassert>
#include <string>

#include "../gfxutils/ConfParser/ConfParser.h"
#include "../gfx/Texture/TextureMgr.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Model/ModelMgr.h"
#include "../gfx/VBO/VBOMgr.h"
#include "../gfxutils/MemManager/MemMgr_RawData.h"
#include "../ParticleSystem/PS_Manager.h"
#include "../GameLogic/GameLogic.h"

#include <gl/glu.h>

using namespace std;


RenderEngine :: RenderEngine() 
: m_confParser(0),
m_cameraBounds(0.0f,0.0f,0.0f,0.0f),
m_playerBounds(0.0f,0.0f,0.0f,0.0f),
m_wireframe(false)
{
	int scrw = 1280,scrh = 1024;

	ConfParser* cp = new ConfParser("./config/config.txt");
	if(cp->getSection("Screen"))
	{
		string hres = cp->getSection("Screen")->getVal("HorzRes"),
				vres = cp->getSection("Screen")->getVal("VerticalRes");
		if(hres != "")
			scrw = atoi(hres.c_str());
		if(vres != "")
			scrh = atoi(vres.c_str());
	}
	delete cp;

	setViewport(0,0,scrw,scrh);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	m_camHeightAbovePlane = 2000.0f;
}

RenderEngine :: ~RenderEngine()
{
	// TODO: when we implement a loading screen, remove this
	onApplicationUnload();
}

void RenderEngine :: onApplicationLoad(const ParserSection& ps)
{
	// TODO: remove m_confParser entirely and read global data now, from ps
	m_confParser = new ConfParser("./config/config.gfx");

	//TextureMgr::init(m_confParser->getSection("Texture"));
	//ShaderManager::init(m_confParser->getSection("Shader"));
	VBOMgr::safeInstance().init(m_confParser->getSection("VBOSettings"));
	ModelMgr::safeInstance().init(m_confParser->getSection("ModelSettings"));
	PS_Manager::safeInstance().init(m_confParser->getSection("ParticleSystem"));

	m_settings.init(m_confParser->getSection("EntitySettings"));
}

void RenderEngine :: onApplicationUnload()
{
	PS_Manager::instance().clear();
	PS_Manager::destroy();
	ModelMgr::destroy();
	VBOMgr::destroy();
	ShaderManager::destroy();
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
		_removeRenderer(name);
		delete r;
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

void RenderEngine::unloadAllRenderers()
{
	deactivateAllRenderers();
	struct RendererDeleter
	{
		void operator () (const std::string&, IRenderer* r) { delete r; }
	};
	m_renderers.visitObjects(RendererDeleter());
	m_renderers.clear();
}


void RenderEngine :: render(Graphics &g) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	if (name == "world") 
	{
		WorldRenderer * wr = new WorldRenderer();
		return wr;
	}
	if (name == "loading") return new LoadingRenderer();
	if (name == "hud") return new HUDRenderer();
	if (name == "menu") return new MenuRenderer();
	if (name == "pause") return new PauseRenderer();
	if (name == "LevelComplete") return new LevelCompleteRenderer();
	if (name == "GameOver") return new GameOverRenderer();
	return NULL;
}

const ParserSection * RenderEngine :: getParserSection(const std::string& secname) const
{
	return m_confParser->getSection(secname);
}

void RenderEngine :: setViewport(int x,int y,int width, int height)
{
	m_viewport[0] = x;
	m_viewport[1] = y;
	m_viewport[2] = width;
	m_viewport[3] = height;
	glViewport(m_viewport[0],m_viewport[1],m_viewport[2],m_viewport[3]);
}

void RenderEngine :: getViewport(int vp[4]) const
{
	vp[0] = m_viewport[0];
	vp[1] = m_viewport[1];
	vp[2] = m_viewport[2];
	vp[3] = m_viewport[3];
}

Point3 RenderEngine :: getScreenPositionFromMapPosition(const Point3& p)
{
	// get world renderer
	WorldRenderer * wr = dynamic_cast<WorldRenderer *>(_findRenderer("world"));
	// get it's modelview & proj matrices
	const double * mv = wr->getModelViewMatrixd();
	const double * proj = wr->getProjectionMatrixd();
	
	// Project
	double outx,outy,outz;
	gluProject(p.getX(),p.getY(),p.getZ(),mv,proj,m_viewport,&outx,&outy,&outz);
	return Point3(float(outx),float(outy),float(outz));
}

Point3 RenderEngine :: getMapPositionFromScreenPosition(const Point2& p)
{
	// get world renderer
	WorldRenderer * wr = dynamic_cast<WorldRenderer *>(_findRenderer("world"));
	// get it's modelview & proj matrices
	const double * mv = wr->getModelViewMatrixd();
	const double * proj = wr->getProjectionMatrixd();
	
	// UnProject - get the ray
	double ray1x,ray1y,ray1z,
		   ray2x,ray2y,ray2z;
	gluUnProject(p.getX(),m_viewport[3] - p.getY(),0.0,mv,proj,m_viewport,&ray1x,&ray1y,&ray1z);
	gluUnProject(p.getX(),m_viewport[3] - p.getY(),1.0,mv,proj,m_viewport,&ray2x,&ray2y,&ray2z);

	const Vector3 ray(float(ray2x - ray1x),float(ray2y - ray1y),float(ray2z - ray1z));
	

	const float planeheight = GameLogic::instance().getGamePlaneHeight();
	const float pcent = float((planeheight - ray1y) / (ray2y - ray1y));
	const Vector3 pos = ray*pcent + Vector3(float(ray1x),float(ray1y),float(ray1z));

	return Point3(pos.getX(),pos.getY(),pos.getZ());
}

Point3 RenderEngine :: getMapPositionFromScreenPosition(const Point2& p,const float h)
{
	// get world renderer
	WorldRenderer * wr = dynamic_cast<WorldRenderer *>(_findRenderer("world"));
	// get it's modelview & proj matrices
	const double * mv = wr->getModelViewMatrixd();
	const double * proj = wr->getProjectionMatrixd();
	
	// UnProject - get the ray
	double ray1x,ray1y,ray1z,
		   ray2x,ray2y,ray2z;
	gluUnProject(p.getX(),m_viewport[3] - p.getY(),0.0,mv,proj,m_viewport,&ray1x,&ray1y,&ray1z);
	gluUnProject(p.getX(),m_viewport[3] - p.getY(),1.0,mv,proj,m_viewport,&ray2x,&ray2y,&ray2z);

	const Vector3 ray(float(ray2x - ray1x),float(ray2y - ray1y),float(ray2z - ray1z));
	

	const float planeheight = h;
	const float pcent = float((planeheight - ray1y) / (ray2y - ray1y));
	const Vector3 pos = ray*pcent + Vector3(float(ray1x),float(ray1y),float(ray1z));

	return Point3(pos.getX(),pos.getY(),pos.getZ());
}

void RenderEngine :: drawTexturedQuad(const Vector3& ll,const Vector3& right,const Vector3& up,
											 const Vector2& tex_ll, const Vector2& extents)
{
	const Vector3 lr(ll + right);
	const Vector3 ul(ll + up);
	const Vector3 ur(ul + right);
	glBegin(GL_QUADS);
		glTexCoord2fv(tex_ll.cfp());
		glVertex3fv(ll.cfp());
		glTexCoord2f(tex_ll.getX() + extents.getX(),tex_ll.getY());
		glVertex3fv(lr.cfp());
		glTexCoord2f(tex_ll.getX() + extents.getX(),tex_ll.getY() + extents.getY());
		glVertex3fv(ur.cfp());
		glTexCoord2f(tex_ll.getX(),tex_ll.getY() + extents.getY());
		glVertex3fv(ul.cfp());
	glEnd();
}

void RenderEngine :: drawArbTexturedQuad(const Vector3& ll,const Vector3& lr,const Vector3& ur,const Vector3& ul,
								 const Vector2& tex_ll, const Vector2& extents)
{
	glBegin(GL_QUADS);
		glTexCoord2fv(tex_ll.cfp());
		glVertex3fv(ll.cfp());
		glTexCoord2f(tex_ll.getX() + extents.getX(),tex_ll.getY());
		glVertex3fv(lr.cfp());
		glTexCoord2f(tex_ll.getX() + extents.getX(),tex_ll.getY() + extents.getY());
		glVertex3fv(ur.cfp());
		glTexCoord2f(tex_ll.getX(),tex_ll.getY() + extents.getY());
		glVertex3fv(ul.cfp());
	glEnd();
}

void RenderEngine :: drawQuad(const Vector3& ll,const Vector3& right,const Vector3& up)
{
	const Vector3 lr(ll + right);
	const Vector3 ul(ll + up);
	const Vector3 ur(ul + right);
	glBegin(GL_QUADS);
		glVertex3fv(ll.cfp());
		glVertex3fv(lr.cfp());
		glVertex3fv(ur.cfp());
		glVertex3fv(ul.cfp());
	glEnd();
}

void RenderEngine :: getWsScreenEdges(Point3 pts[4])
{
	pts[0] = m_wsScreenEdges[0];
	pts[1] = m_wsScreenEdges[1];
	pts[2] = m_wsScreenEdges[2];
	pts[3] = m_wsScreenEdges[3];
}

void RenderEngine :: computeWsScreenEdges()
{
	m_wsScreenEdges[0] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0]),float(m_viewport[1] + m_viewport[3])));
	m_wsScreenEdges[1] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0] + m_viewport[2]),float(m_viewport[1] + m_viewport[3])));
	m_wsScreenEdges[2] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0] + m_viewport[2]),float(m_viewport[1])));
	m_wsScreenEdges[3] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0]),float(m_viewport[1])));
}

void RenderEngine :: getWsScreenEdges(Point3 pts[4],const float h)
{
	pts[0] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0]),float(m_viewport[1] + m_viewport[3])),h);
	pts[1] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0] + m_viewport[2]),float(m_viewport[1] + m_viewport[3])),h);
	pts[2] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0] + m_viewport[2]),float(m_viewport[1])),h);
	pts[3] = getMapPositionFromScreenPosition(Point2(float(m_viewport[0]),float(m_viewport[1])),h);
}

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