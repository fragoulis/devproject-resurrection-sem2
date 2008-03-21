//*****************************************************************************
//**
//**   File:               RenderEngine.cpp
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-12 01:27:40 +0000 (Wed, 12 Mar 2008) $
//**   Last Revision:      $Revision: 147 $
//**
//*****************************************************************************

#include "RenderEngine.h"
#include "IRenderer.h"
#include "WorldRenderer.h"
#include "LoadingRenderer.h"
#include "HUDRenderer.h"
#include "MenuRenderer.h"
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


RenderEngine :: RenderEngine() : m_confParser(0)
{
	setViewport(0,0,800,600);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
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

	TextureMgr::init(m_confParser->getSection("Texture"));
	ShaderManager::init(m_confParser->getSection("Shader"));
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
	IRenderer* r = findRenderer(name);
	if (r == NULL) {
		r = createRenderer(name);
		assert(r != NULL);
		addRenderer(name, r);
	}
	activateRenderer(r);
}

void RenderEngine :: deactivateRenderer(const std::string& name)
{
	IRenderer* r = findRenderer(name);
	if (r != NULL) deactivateRenderer(r);
}

void RenderEngine :: deactivateAllRenderers()
{
	m_activeRenderers.clear();
}

void RenderEngine :: loadRenderer(const std::string& name)
{
	IRenderer* r = findRenderer(name);
	if (r == NULL) {
		r = createRenderer(name);
		assert(r != NULL);
		addRenderer(name, r);
	}
}

void RenderEngine :: unloadRenderer(const std::string& name)
{
	IRenderer* r = findRenderer(name);
	if (r != NULL) {
		deactivateRenderer(r);
		removeRenderer(name);
		delete r;
	}
}

IRenderer* RenderEngine :: getRenderer(const std::string& name)
{
	IRenderer* r = findRenderer(name);
	if (r == NULL) {
		r = createRenderer(name);
		assert(r != NULL);
		addRenderer(name, r);
	}
	return r;
}

void RenderEngine::unloadAllRenderers()
{
	struct RendererDeleter
	{
		void operator () (const std::string&, IRenderer* r) { delete r; }
	};
	m_renderers.visitObjects(RendererDeleter());
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

IRenderer* RenderEngine :: createRenderer(const std::string& name) const
{
	if (name == "world") return new WorldRenderer();
	if (name == "loading") return new LoadingRenderer();
	if (name == "hud") return new HUDRenderer();
	if (name == "menu") return new MenuRenderer();
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

Point3 RenderEngine :: getMapPositionFromScreenPosition(const Point2& p)
{
	// get world renderer
	WorldRenderer * wr = dynamic_cast<WorldRenderer *>(findRenderer("world"));
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