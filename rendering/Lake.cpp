#include <string>
#include "Lake.h"
#include "RenderEngine.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/Texture/Texture.h"
#include "../gfxutils/Texture/TplPalette.h"
#include "../control/Gamecube/gc_new_delete.h"
#include "../gfxutils/VA/Vertexformat.h"
#include "../gfxutils/VA/VertexArray.h"

using namespace std;

Lake :: Lake(unsigned color)			// Get texture from palette, also get extents
:m_texture(0),
m_time(0.0f)
{
	string laketex = RenderEngine::instance().getConstRenderSettings().getLakeTexture();
	TplPalette * pal = TextureMgr::instance().loadPalette(laketex,"lakeTPL.txt");
	m_texture = new Texture(pal,0,"lake");

	Vector3 mapext = RenderEngine::instance().getLevelExtents();
	const float ext = mapext.getX()*0.5f;

	m_ll.set(-ext,0.0f,ext);
	m_right.set(2.0f*ext,0.0f,0.0f);
	m_up.set(0.0f,0.0f,-2.0f*ext);

	m_repeats = u8(RenderEngine::instance().getConstRenderSettings().getWaveRepeats(ext));
	m_speed = RenderEngine::instance().getConstRenderSettings().getWaveSpeed();
}

Lake :: ~Lake()		// delete lake palette
{
	if(m_texture)
	{
		TextureMgr::instance().unloadPalette(m_texture->parentPalette());
		m_texture = 0;
	}
}

void Lake :: render() const
{
	// set texture matrix

	Mtx tm1; 
 
	//MTXScale(tm1, 300, 300, 1); 
	MTXTrans(tm1,0.01*m_time,0.01*m_time,0.01*m_time);
	 
	GXLoadTexMtxImm(tm1, GX_TEXMTX0, GX_MTX2x4); 
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);

	m_texture->bind();
	GXSetVtxDescv(VATTable::getVDL(7));
	RenderEngine::drawQuad(m_ll,m_right,m_up,m_repeats);
	

	// unset texture matrix
	MTXIdentity(tm1);
	GXLoadTexMtxImm(tm1,GX_TEXMTX0,GX_MTX2x4);
}

void Lake :: update(const float dt)
{
	m_time += dt;
}