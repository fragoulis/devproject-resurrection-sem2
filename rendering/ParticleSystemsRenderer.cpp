#include <gl/glee.h>
#include "ParticleSystemsRenderer.h"
#include "../ParticleSystem/PS_Manager.h"

ParticleSystemsRenderer :: ParticleSystemsRenderer()
{
	// FIXME : Adding the hacky way the dummy ps
	_psList.push_back(PS_Manager::instance().fetchNewPS("TestPS"));
	CoordinateFrame cf;
	cf.move(Vector3(64,150,-64));
	_psList[0]->setTransform(cf);
}

ParticleSystemsRenderer :: ~ParticleSystemsRenderer()
{
	// FIXME : Deleting the hacky way the dummy ps
	delete _psList[0];
	_psList.clear();
}



void ParticleSystemsRenderer :: render( Graphics& g ) const
{
	// render particle systems
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);
	for(std::vector<PS_Base *>::const_iterator it = _psList.begin();
		it != _psList.end();
		++it)
	{
		(*it)->render();
	}
	glDisable(GL_BLEND);
}

void ParticleSystemsRenderer :: update( float dt )
{
	// Do particle system updates
	for(std::vector<PS_Base *>::iterator it = _psList.begin();
		it != _psList.end();
		++it)
	{
		(*it)->update(dt);
	}
}
