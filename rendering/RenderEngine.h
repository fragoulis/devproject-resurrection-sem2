//*****************************************************************************
//**
//**   File:               RenderEngine.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once
#include "../utility/ObjectReferenceMap.h"
#include "../utility/Singleton.h"
#include "ConstRenderSettings.h"
#include <vector>
#include <algorithm>
#include "../math/Vector3.h"
#include "../math/Vector2.h"
class Graphics;
class IRenderer;
class ConfParser;
class ParserSection;
class Point2;
class Point3;

class RenderEngine : public Singleton< RenderEngine >
{
public:

	//! Called by Application class on startup.
	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();


	//! activate a renderer. If it's not loaded yet, loads it.
	void activateRenderer(const std::string& name);
	void deactivateRenderer(const std::string& name);
	void deactivateAllRenderers();
	
	//! use to load a renderer in advance
	void loadRenderer(const std::string& name);

	//! use to free memory
	void unloadRenderer(const std::string& name);
	void unloadAllRenderers();

	//! renders all active renderers, in the order they were activated
	void render(Graphics& g) const;

	void update(float dt);

	// get a section
	const ParserSection * getParserSection(const std::string& secname) const;

	// viewport getset
	void setViewport(int x,int y,int width, int height);
	void getViewport(int vp[4]) const;
	Point3 getMapPositionFromScreenPosition(const Point2& p);

	// get entity settings
	const ConstRenderSettings& getConstRenderSettings() const {return m_settings;}

	// Convenience functions
	static void drawTexturedQuad(const Vector3& ll,const Vector3& right,const Vector3& up,
								 const Vector2& tex_ll, const Vector2& extents);

	static void drawQuad(const Vector3& ll,const Vector3& right,const Vector3& up);


private:
	typedef ObjectReferenceMap<IRenderer> RendererMap;
	RendererMap m_renderers;

	typedef std::vector<IRenderer*> RendererList;
	RendererList m_activeRenderers;

	ConfParser * m_confParser;

	int m_viewport[4];

	ConstRenderSettings m_settings;


	// helper functions
	IRenderer* findRenderer(const std::string& name) { return m_renderers.findObjectReference(name); }
	IRenderer* createRenderer(const std::string& name) const;

	void addRenderer(const std::string& name, IRenderer* r) { m_renderers.addObjectReference(r, name); }
	void removeRenderer(const std::string& name) { m_renderers.removeObjectReference(name); }

	void activateRenderer(IRenderer* r) { m_activeRenderers.push_back(r); }
	void deactivateRenderer(IRenderer* r) { remove(m_activeRenderers.begin(), m_activeRenderers.end(), r); }


	// singleton stuff
	friend Singleton< RenderEngine >;
	RenderEngine();
	~RenderEngine();
};
