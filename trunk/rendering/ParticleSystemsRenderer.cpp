//*****************************************************************************
//**
//**   File:               ParticleSystemsRenderer.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include <gl/glee.h>
#include "ParticleSystemsRenderer.h"
#include "../ParticleSystem/PS_Manager.h"

ParticleSystemsRenderer :: ParticleSystemsRenderer()
{

	EventManager::instance().registerEventListener(this);

	
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
	glDepthMask(GL_FALSE);
	for(std::vector<PS_Base *>::const_iterator it = _psList.begin();
		it != _psList.end();
		++it)
	{
		(*it)->render();
	}
	glDepthMask(GL_TRUE);
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

void ParticleSystemsRenderer::onEvent(Key_GoingDown& key) {
	// FIXME : Adding the hacky way the dummy ps
	int keyPressed = key.getValue();

	CoordinateFrame cf;

	switch (keyPressed) {
		case 'Q':
			_psList.push_back(PS_Manager::instance().fetchNewPS("PS_Explosion"));
			cf.move(Vector3(64,1400,-64));
			_psList.back()->setTransform(cf);
			break;
		case 'E':
			_psList.push_back(PS_Manager::instance().fetchNewPS("PS_EnergyLoss"));
			cf.move(Vector3(64,1400,-64));
			_psList.back()->setTransform(cf);
			break;
		case 'R':
			_psList.push_back(PS_Manager::instance().fetchNewPS("PS_ColouredExplosion"));
			cf.move(Vector3(64,1400,-64));
			_psList.back()->setTransform(cf);
			break;
	}
}
