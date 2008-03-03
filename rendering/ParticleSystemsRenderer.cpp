#include <gl/glee.h>
#include "ParticleSystemsRenderer.h"
#include "../ParticleSystem/PS_Manager.h"

ParticleSystemsRenderer :: ParticleSystemsRenderer()
{

}

ParticleSystemsRenderer :: ~ParticleSystemsRenderer()
{
	for(std::vector<PS_Base *>::iterator it = _psList.begin();
		it != _psList.end();
		++it)
	{
		delete (*it);
		(*it) = 0;
	}
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
