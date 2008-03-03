#include "ParticleSystemsRenderer.h"

ParticleSystemsRenderer :: ParticleSystemsRenderer()
{
}

ParticleSystemsRenderer :: ~ParticleSystemsRenderer()
{
}



void ParticleSystemsRenderer :: render( Graphics& g ) const
{
	// render particle systems
	for(std::vector<PS_Base *>::const_iterator it = _psList.begin();
		it != _psList.end();
		++it)
	{
		(*it)->render();
	}
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
