//*****************************************************************************
//**
//**   File:               ParticleSystemsRenderer.cpp
//**   Author:             $Author: manuele.bonanno $
//**   Last Change:        $Date: 2008-03-19 19:24:13 +0000 (Wed, 19 Mar 2008) $
//**   Last Revision:      $Revision: 199 $
//**
//*****************************************************************************

#include <gl/glee.h>
#include "ParticleSystemsRenderer.h"
#include "../ParticleSystem/PS_Manager.h"
#include "../GameLogic/GameLogic.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Objects/Ebomb.h"
#include "../GameLogic/Objects/Crater.h"
#include "../ParticleSystem/PS_Jet.h"
#include "../ParticleSystem/PS_EnergyLoss.h"
#include "../ParticleSystem/PS_Fountain.h"

ParticleSystemsRenderer :: ParticleSystemsRenderer()
{

	EventManager::instance().registerEventListener<Key_GoingDown>(this); //DEBUG PURPOSES

	EventManager::instance().registerEventListener<Enemy_Despawned>(this);
	EventManager::instance().registerEventListener<Player_EnergyDrained>(this);
	EventManager::instance().registerEventListener<Player_Spawned>(this);
	EventManager::instance().registerEventListener<Ebomb_Despawned>(this);
	EventManager::instance().registerEventListener<Life_Restored>(this);

	m_isJetCreated = false;

}

ParticleSystemsRenderer :: ~ParticleSystemsRenderer()
{

	std::vector<PS_Base *>::iterator it = m_psList.begin();
	while(it != m_psList.end())
	{
		PS_Base *ps = *it;
		it = m_psList.erase(it);
		delete ps;
	}
}



void ParticleSystemsRenderer :: render( Graphics& g ) const
{
	// render particle systems
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	for(std::vector<PS_Base *>::const_iterator it = m_psList.begin();
		it != m_psList.end();
		++it)
	{
		(*it)->render();
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void ParticleSystemsRenderer :: update( float dt )
{
	std::vector<PS_Base *>::iterator it = m_psList.begin();
	// Do particle system updates
	//for(std::vector<PS_Base *>::iterator it = m_psList.begin();
	//	it != m_psList.end();
	//	++it)
	while(it != m_psList.end())
	{
		(*it)->update(dt);

		//remove dead particle systems
		if ((*it)->getCurrentTime() > (*it)->getSystemLife()) { 
			PS_Base *ps = *it;
			it = m_psList.erase(it);
			delete ps;
		} else
			it++;
	}
}

//LISTENING THE KEYBOARD FOR DEBUGGING PURPOSES ONLY
void ParticleSystemsRenderer::onEvent(Key_GoingDown &key) {
	//FIXME : Adding the hacky way the dummy ps
	int keyPressed = key.getValue();

	CoordinateFrame cf;
	PS_Fountain *ps_Fountain = NULL;

	switch (keyPressed) {
		case 'Q':
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_Explosion"));
			cf.move(Vector3(64,286,-64));
			m_psList.back()->setTransform(cf);
			break;
		case 'E':
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnergyLoss"));
			cf.move(Vector3(64,286,-64));
			m_psList.back()->setTransform(cf);
			break;
		case 'R':
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_ColouredExplosion"));
			cf.move(Vector3(64,286,-64));
			m_psList.back()->setTransform(cf);
			break;
		case 'T':
			ps_Fountain = (PS_Fountain*) PS_Manager::instance().fetchNewPS("PS_BlueFountainIn");
			ps_Fountain->setRadiusScale(20.0f);
			m_psList.push_back(ps_Fountain);
			cf.move(Vector3(64,286,-64));
			m_psList.back()->setTransform(cf);
			break;
		case 'G':
			ps_Fountain = (PS_Fountain*) PS_Manager::instance().fetchNewPS("PS_BlueFountainOut");
			ps_Fountain->setRadiusScale(20.0f);
			m_psList.push_back(ps_Fountain);
			cf.move(Vector3(64,286,-64));
			m_psList.back()->setTransform(cf);
			break;
		case 'Y':
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_RedEnemyExplosion"));
			cf.move(Vector3(64,286,-64));
			m_psList.back()->setTransform(cf);
			//m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnemyExplosion"));
			//m_psList.back()->setTransform(cf);
			//m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_BlueEnemyExplosion"));
			//m_psList.back()->setTransform(cf);
			break;
		case 'U':
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_Explosion"));
			cf.move(Vector3(64,286,-64));
			m_psList.back()->setTransform(cf);
			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_ColouredExplosion"));
			m_psList.back()->setTransform(cf);
			break;
	}
}

void ParticleSystemsRenderer::onEvent(Enemy_Despawned &enemy) {

	CoordinateFrame cf = enemy.getValue()->getCoordinateFrame();
	EnergyType energyType = enemy.getValue()->getEnergyType();
	//depending on the enemy energy type generates a different explosion
	if (energyType == EnergyTypeFromString("red")) {
		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_RedEnemyExplosion"));
		m_psList.back()->setTransform(cf);
		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_RedEnergyLoss"));
	}
	else if (energyType == EnergyTypeFromString("yellow")) {
		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnemyExplosion"));
		m_psList.back()->setTransform(cf);
		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnergyLoss"));
	}
	else if (energyType == EnergyTypeFromString("blue")) {
		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_BlueEnemyExplosion"));
		m_psList.back()->setTransform(cf);
		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_BlueEnergyLoss"));
	}
	m_psList.back()->setTransform(cf);
}

void ParticleSystemsRenderer::onEvent(Player_EnergyDrained& playerEnergy) {

	PS_EnergyLoss *ps_EnergyLoss = NULL;
	//CoordinateFrame cf = playerEnergy.getValue1()->getCoordinateFrame(); 
	EnergyType energyType = playerEnergy.getValue2();
	//depending on the drained energy type generates a different effect
	if (energyType == EnergyTypeFromString("red"))
		ps_EnergyLoss = (PS_EnergyLoss*) PS_Manager::instance().fetchNewPS("PS_RedEnergyLoss");
	else if (energyType == EnergyTypeFromString("yellow"))
		ps_EnergyLoss = (PS_EnergyLoss*) PS_Manager::instance().fetchNewPS("PS_YellowEnergyLoss");
	else if (energyType == EnergyTypeFromString("blue"))
		ps_EnergyLoss = (PS_EnergyLoss*) PS_Manager::instance().fetchNewPS("PS_BlueEnergyLoss");

	ps_EnergyLoss->setEmitterShip(playerEnergy.getValue1());
	m_psList.push_back(ps_EnergyLoss);
	//m_psList.back()->setTransform(cf);

}

void ParticleSystemsRenderer::onEvent(Player_Spawned &player) {

	if (!m_isJetCreated) {
		PS_Jet *ps_jet = (PS_Jet*) PS_Manager::instance().fetchNewPS("PS_Jet");
		ps_jet->setEmitterShip(player.getValue());
		m_psList.push_back(ps_jet);
	}

}

void ParticleSystemsRenderer::onEvent(Ebomb_Despawned &eBomb) {

	CoordinateFrame cf = eBomb.getValue()->getCoordinateFrame();
	m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_Explosion"));
	m_psList.back()->setTransform(cf);
	m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_ColouredExplosion"));
	m_psList.back()->setTransform(cf);

}

void ParticleSystemsRenderer::onEvent(Life_Restored& restoredCrater)
{
	PS_Fountain *ps_Fountain = NULL;
	Crater* crater = restoredCrater.getValue();
	CoordinateFrame cf = crater->getCoordinateFrame();
	EbombType ebombType = crater->getEbombType();

	if (ebombType == EbombTypeFromString("red"))
		ps_Fountain = (PS_Fountain*) PS_Manager::instance().fetchNewPS("PS_RedFountainOut");
	else if (ebombType == EbombTypeFromString("yellow"))
		ps_Fountain = (PS_Fountain*) PS_Manager::instance().fetchNewPS("PS_YellowFountainOut");
	else if (ebombType == EbombTypeFromString("blue"))
		ps_Fountain = (PS_Fountain*) PS_Manager::instance().fetchNewPS("PS_BlueFountainOut");
	else  //FIX ME:  add more olors
		ps_Fountain = (PS_Fountain*) PS_Manager::instance().fetchNewPS("PS_BlueFountainOut");

	ps_Fountain->setRadiusScale(20.0f); //crater->getAffectedAreaRadius()/30.0f); 
	m_psList.push_back(ps_Fountain);
	cf.move(Vector3(0,0,0));  //?
	m_psList.back()->setTransform(cf);

}

