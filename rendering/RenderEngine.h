//*****************************************************************************
//**
//**   File:               RenderEngine.h
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-12 01:27:40 +0000 (Wed, 12 Mar 2008) $
//**   Last Revision:      $Revision: 147 $
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
#include "../math/Vector4.h"
#include "../math/Point3.h"
#include "../GameLogic/EbombTypes.h"
#include "../GameLogic/EnergyTypes.h"
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

	//! use to get direct access to a renderer. If doesn't exist, creates it.
	IRenderer* getRenderer(const std::string& name);

	//! renders all active renderers, in the order they were activated
	void render(Graphics& g) const;

	void update(float dt);

	// get a section
	const ParserSection * getParserSection(const std::string& secname) const;

	// viewport getset
	void setViewport(int x,int y,int width, int height);
	void getViewport(int vp[4]) const;

	// level light getset
	void setLevelLight(const Vector4& llight) {m_levelLightPos = llight;}
	const Vector4& getLevelLight() const {return m_levelLightPos;}
	void setLevelLightColor(const Vector4& llight) {m_levelLightColor = llight;}
	const Vector4& getLevelLightColor() const {return m_levelLightColor;}

	// camera constants getset
	const float getCameraHeightAbovePlane() const {return m_camHeightAbovePlane;}
	void setCameraHeightAbovePlane(const float val) {m_camHeightAbovePlane = val;}

	// boundary stuff - represented in WCS as leftmost,rightmost,bottom-most?,topmost (looking at screen)
	const Vector4& getPlayerBounds() const {return m_playerBounds;}
	const Vector4& getCameraBounds() const {return m_cameraBounds;}

	void boundPlayerPosition(Vector3& playerpos);
	void boundCameraPosition(Vector3& camerapos);

	void setPlayerBounds(const Vector4& pb) {m_playerBounds = pb;}
	void setCameraBounds(const Vector4& cb) {m_cameraBounds = cb;}


	// mapping stuff
	void getWsScreenEdges(Point3 pts[4]);
	void getWsScreenEdges(Point3 pts[4],const float h);
	void computeWsScreenEdges();
	Point3 getMapPositionFromScreenPosition(const Point2& p);
	Point3 getMapPositionFromScreenPosition(const Point2& p,const float h);

	// get entity settings
	const ConstRenderSettings& getConstRenderSettings() const {return m_settings;}

	const Vector4 getColorFromEbombType (const EbombType& et) const ;
	const Vector4 getColorFromEnergyType (const EnergyType& et) const ;

	const Vector3& getLevelExtents() const {return m_levelExtents;}
	void setLevelExtents(const Vector3& le) {m_levelExtents = le;}

	// Convenience functions
	static void drawTexturedQuad(const Vector3& ll,const Vector3& right,const Vector3& up,
								 const Vector2& tex_ll, const Vector2& extents);
	
	static void drawArbTexturedQuad(const Vector3& ll,const Vector3& lr,const Vector3& ur,const Vector3& ul,
								 const Vector2& tex_ll, const Vector2& extents);

	static void drawQuad(const Vector3& ll,const Vector3& right,const Vector3& up);


private:
	typedef ObjectReferenceMap<IRenderer> RendererMap;
	RendererMap m_renderers;

	typedef std::vector<IRenderer*> RendererList;
	RendererList m_activeRenderers;

	ConfParser * m_confParser;

	int m_viewport[4];
	Vector4 m_levelLightPos;
	Vector4 m_levelLightColor;
	float m_camHeightAbovePlane;

	Point3 m_wsScreenEdges[4];		// ll, -> ccw

	ConstRenderSettings m_settings;

	Vector4 m_playerBounds;		// player boundaries for the current level
	Vector4 m_cameraBounds;		// camera boundaries for the current level

	Vector3 m_levelExtents;


	// helper functions
	IRenderer* _findRenderer(const std::string& name) { return m_renderers.findObjectReference(name); }
	IRenderer* _createRenderer(const std::string& name) const;

	void _addRenderer(const std::string& name, IRenderer* r) { m_renderers.addObjectReference(r, name); }
	void _removeRenderer(const std::string& name) { m_renderers.removeObjectReference(name); }

	void _activateRenderer(IRenderer* r) { m_activeRenderers.push_back(r); }
	void _deactivateRenderer(IRenderer* r) { remove(m_activeRenderers.begin(), m_activeRenderers.end(), r); }


	// singleton stuff
	friend Singleton< RenderEngine >;
	RenderEngine();
	~RenderEngine();
};
