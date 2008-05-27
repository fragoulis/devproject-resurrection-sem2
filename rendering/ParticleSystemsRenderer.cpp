//#include <gl/glee.h>
#include "ParticleSystemsRenderer.h"
#include "../ParticleSystem/PS_Manager.h"
#include "../GameLogic/GameLogic.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Objects/Ebomb.h"
#include "../GameLogic/Objects/Crater.h"
//#include "../ParticleSystem/PS_Jet.h"
//#include "../ParticleSystem/PS_EnergyLoss.h"
//#include "../ParticleSystem/PS_Fountain.h"
#include "../ParticleSystem/PS_RotatingFlare.h"
#include "../ParticleSystem/PS_EnemyEnergyViz.h"
//#include "../ParticleSystem/PS_Explosion2.h"
#include "../ParticleSystem/PS_Explosion.h"
#include "../utility/deleters.h"

ParticleSystemsRenderer :: ParticleSystemsRenderer()
{
	//EventManager::instance().registerEventListener<Key_GoingDown>(this); //DEBUG PURPOSES

	EventManager::instance().registerEventListener<Enemy_Spawned>(this);
	EventManager::instance().registerEventListener<Enemy_Despawned>(this);
	EventManager::instance().registerEventListener<Enemy_Destroyed>(this);
	EventManager::instance().registerEventListener<Player_EnergyDrained>(this);
	EventManager::instance().registerEventListener<Player_Spawned>(this);
	EventManager::instance().registerEventListener<Player_Respawned>(this);
	EventManager::instance().registerEventListener<Player_Destroyed>(this);
	EventManager::instance().registerEventListener<Ebomb_Despawned>(this);
	EventManager::instance().registerEventListener<Life_Restored>(this);
	EventManager::instance().registerEventListener<Terrain_Changed>(this);
	EventManager::instance().registerEventListener<Crater_Spawned>(this);
	EventManager::instance().registerEventListener<Level_Unload>(this);

	m_isJetCreated = false;

}

ParticleSystemsRenderer :: ~ParticleSystemsRenderer()
{
	deleteVector(m_psList);

	//EventManager::instance().unRegisterEventListener<Key_GoingDown>(this); //DEBUG PURPOSES

	EventManager::instance().unRegisterEventListener<Enemy_Spawned>(this);
	EventManager::instance().unRegisterEventListener<Enemy_Despawned>(this);
	EventManager::instance().unRegisterEventListener<Enemy_Destroyed>(this);
	EventManager::instance().unRegisterEventListener<Player_EnergyDrained>(this);
	EventManager::instance().unRegisterEventListener<Player_Spawned>(this);
	EventManager::instance().unRegisterEventListener<Player_Respawned>(this);
	EventManager::instance().unRegisterEventListener<Player_Destroyed>(this);
	EventManager::instance().unRegisterEventListener<Ebomb_Despawned>(this);
	EventManager::instance().unRegisterEventListener<Life_Restored>(this);
	EventManager::instance().unRegisterEventListener<Terrain_Changed>(this);
	EventManager::instance().unRegisterEventListener<Crater_Spawned>(this);
	EventManager::instance().unRegisterEventListener<Level_Unload>(this);

}

void ParticleSystemsRenderer :: onEvent(Level_Unload& e)
{
	deleteVector(m_psList);
}


void ParticleSystemsRenderer :: render( Graphics& g ) const
{
	// render particle systems
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	//glEnable(GL_BLEND);
	//glDepthMask(GL_FALSE);
	for(std::vector<PS_Base *>::const_iterator it = m_psList.begin();
		it != m_psList.end();
		++it)
	{
		(*it)->render();
	}
	//glDepthMask(GL_TRUE);
	//glDisable(GL_BLEND);
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
//void ParticleSystemsRenderer::onEvent(Key_GoingDown &key) {
//	//FIXME : Adding the hacky way the dummy ps
//	int keyPressed = key.getValue();
//
//	CoordinateFrame cf;
//	PS_Fountain *ps_Fountain = NULL;
//
//	switch (keyPressed) {
//		case 'Q':
//			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_Explosion"));
//			cf.move(Vector3(64,286,-64));
//			m_psList.back()->setTransform(cf);
//			break;
//		case 'E':
//			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnergyLoss"));
//			cf.move(Vector3(64,286,-64));
//			m_psList.back()->setTransform(cf);
//			break;
//		case 'R':
//			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_ColouredExplosion"));
//			cf.move(Vector3(64,286,-64));
//			m_psList.back()->setTransform(cf);
//			break;
//		case 'G':
//			ps_Fountain = (PS_Fountain*) PS_Manager::instance().fetchNewPS("PS_Fountain");
//			ps_Fountain->setRadiusScale(20.0f);
//			m_psList.push_back(ps_Fountain);
//			cf.move(Vector3(64,286,-64));
//			m_psList.back()->setTransform(cf);
//			break;
//		case 'Y':
//			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_RedEnemyExplosion"));
//			cf.move(Vector3(64,286,-64));
//			m_psList.back()->setTransform(cf);
//			//m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnemyExplosion"));
//			//m_psList.back()->setTransform(cf);
//			//m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_BlueEnemyExplosion"));
//			//m_psList.back()->setTransform(cf);
//			break;
//		case 'U':
//			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_Explosion"));
//			cf.move(Vector3(64,286,-64));
//			m_psList.back()->setTransform(cf);
//			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_ColouredExplosion"));
//			m_psList.back()->setTransform(cf);
//			break;
//
//		case 'O':
//			m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_EnemyExplosion2"));
//			cf.move(Vector3(0,300,0));
//			m_psList.back()->setTransform(cf);
//			break;
//	}
//}

void ParticleSystemsRenderer::onEvent(Terrain_Changed &evt) 
{
	//CoordinateFrame cf;
	//m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_Clouds"));
	//m_psList.back()->setTransform(cf);
}

#include "../gfxutils/Misc/Logger.h"
#include "../gfxutils/Misc/utils.h"


void ParticleSystemsRenderer::onEvent(Enemy_Destroyed& evt)
{
	Enemyship* enemy = evt.getValue();
	CoordinateFrame cf = enemy->getCoordinateFrame();
	//if(0)
	//{
	//	EnergyType energyType = enemy.getValue()->getEnergyType();
	//	//depending on the enemy energy type generates a different explosion
	//	if (energyType == EnergyTypeFromString("red")) {
	//		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_RedEnemyExplosion"));
	//		m_psList.back()->setTransform(cf);
	//		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_RedEnergyLoss"));
	//	}
	//	else if (energyType == EnergyTypeFromString("yellow")) {
	//		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnemyExplosion"));
	//		m_psList.back()->setTransform(cf);
	//		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnergyLoss"));
	//	}
	//	else if (energyType == EnergyTypeFromString("blue")) {
	//		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_BlueEnemyExplosion"));
	//		m_psList.back()->setTransform(cf);
	//		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_BlueEnergyLoss"));
	//	}
	//	m_psList.back()->setTransform(cf);
	//}
	//else
	//{
	//	m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_EnemyExplosion2"));
	//	m_psList.back()->setTransform(cf);
	//	reinterpret_cast<PS_Explosion2 *>(m_psList.back())->setSizeModifier(enemy.getValue()->getRadius());

	//	EnergyType energyType = enemy.getValue()->getEnergyType();
	//	if (energyType == EnergyTypeFromString("red")) 
	//		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_RedEnergyLoss"));
	//	else if (energyType == EnergyTypeFromString("yellow"))
	//		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnergyLoss"));
	//	else if (energyType == EnergyTypeFromString("blue"))
	//		m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_BlueEnergyLoss"));
	//	m_psList.back()->setTransform(cf);
	//}

	EnergyType energyType = enemy->getEnergyType();
	PS_Explosion* ps = static_cast<PS_Explosion*>(PS_Manager::instance().fetchNewPS("PS_Explosion"));
	ps->setTransform(cf);
	m_psList.push_back(ps);
	//ps->setVelocity(enemy->getVelocity());

	_removeEnemyViz(enemy);
}

void ParticleSystemsRenderer::onEvent(Enemy_Despawned &evt)
{
	_removeEnemyViz(evt.getValue());
}

void ParticleSystemsRenderer::_removeEnemyViz(Enemyship* enemy)
{
	for(std::vector<PS_Base *>::iterator it = m_psList.begin();
		it != m_psList.end();
		++it)
	{
		if((*it)->getName() == "PS_EnemyEnergyViz")
		{
			if(reinterpret_cast<PS_EnemyEnergyViz *>(*it)->getEnemyship() == enemy)
			{
				delete *it;
				m_psList.erase(it);
				break;
			}
		}
	}
}



void ParticleSystemsRenderer::onEvent(Enemy_Spawned &enemy) 
{
	m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_EnemyEnergyViz"));
	reinterpret_cast<PS_EnemyEnergyViz *>(m_psList.back())->setEnemyship(enemy.getValue());
}

void ParticleSystemsRenderer::onEvent(Player_EnergyDrained& playerEnergy)
{
	//PS_EnergyLoss *ps_EnergyLoss = NULL;
	////CoordinateFrame cf = playerEnergy.getValue1()->getCoordinateFrame(); 
	//EnergyType energyType = playerEnergy.getValue2();
	////depending on the drained energy type generates a different effect
	//if (energyType == EnergyTypeFromString("red"))
	//	ps_EnergyLoss = (PS_EnergyLoss*) PS_Manager::instance().fetchNewPS("PS_RedEnergyLoss");
	//else if (energyType == EnergyTypeFromString("yellow"))
	//	ps_EnergyLoss = (PS_EnergyLoss*) PS_Manager::instance().fetchNewPS("PS_YellowEnergyLoss");
	//else if (energyType == EnergyTypeFromString("blue"))
	//	ps_EnergyLoss = (PS_EnergyLoss*) PS_Manager::instance().fetchNewPS("PS_BlueEnergyLoss");

	//ps_EnergyLoss->setEmitterShip(playerEnergy.getValue1());
	//m_psList.push_back(ps_EnergyLoss);
	////m_psList.back()->setTransform(cf);
}


void ParticleSystemsRenderer::onEvent(Player_Spawned &player)
{
	//if (!m_isJetCreated) {
	//	PS_Jet *ps_jet = (PS_Jet*) PS_Manager::instance().fetchNewPS("PS_Jet");
	//	ps_jet->setEmitterShip(player.getValue());
	//	m_psList.push_back(ps_jet);
	//	m_isJetCreated = true;
	//}
}

void ParticleSystemsRenderer::onEvent(Player_Respawned &player)
{
	//if (!m_isJetCreated) {
	//	PS_Jet *ps_jet = (PS_Jet*) PS_Manager::instance().fetchNewPS("PS_Jet");
	//	ps_jet->setEmitterShip(player.getValue());
	//	m_psList.push_back(ps_jet);
	//	m_isJetCreated = true;
	//}
}

void ParticleSystemsRenderer::onEvent(Player_Destroyed &player)
{
	//CoordinateFrame cf = player.getValue1()->getCoordinateFrame();

	//m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_RedEnemyExplosion"));
	//m_psList.back()->setTransform(cf);
	//m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_YellowEnemyExplosion"));
	//m_psList.back()->setTransform(cf);
	//m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_BlueEnemyExplosion"));
	//m_psList.back()->setTransform(cf);

	//for(std::vector<PS_Base *>::iterator it = m_psList.begin();
	//	it != m_psList.end();
	//	++it)
	//{
	//	if((*it)->getName() == "PS_Jet")
	//	{
	//		delete *it;
	//		m_psList.erase(it);
	//		m_isJetCreated = false;
	//		break;
	//	}
	//}
}

void ParticleSystemsRenderer::onEvent(Ebomb_Despawned &eBomb)
{
	//CoordinateFrame cf = eBomb.getValue()->getCoordinateFrame();
	//m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_Explosion"));
	//m_psList.back()->setTransform(cf);
	//m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_ColouredExplosion"));
	//m_psList.back()->setTransform(cf);

}

void ParticleSystemsRenderer::onEvent(Life_Restored& restoredCrater)
{
	//PS_Fountain *ps_Fountain = NULL;
	//Crater* crater = restoredCrater.getValue();
	//CoordinateFrame cf = crater->getCoordinateFrame();
	//EbombType ebombType = crater->getEbombType();

	//ps_Fountain = (PS_Fountain*) PS_Manager::instance().fetchNewPS("PS_Fountain");

	//if (ebombType == EbombTypeFromString("red")) {
	//	ps_Fountain->setParticleColor(0);
	//} else if (ebombType == EbombTypeFromString("yellow")) {
	//	ps_Fountain->setParticleColor(1);
	//} else if (ebombType == EbombTypeFromString("blue")) {
	//	ps_Fountain->setParticleColor(2);
	//} else if (ebombType == EbombTypeFromString("green")) {
	//	ps_Fountain->setParticleColor(3);
	//} else if (ebombType == EbombTypeFromString("orange")) {
	//	ps_Fountain->setParticleColor(4);
	//} else if (ebombType == EbombTypeFromString("purple")) {
	//	ps_Fountain->setParticleColor(5);
	//} else  {
	//	ps_Fountain->setParticleColor(0);
	//}

	//ps_Fountain->setRadiusScale(20.0f); //crater->getAffectedAreaRadius()/30.0f); 
	//m_psList.push_back(ps_Fountain);
	//cf.move(Vector3(0,0,0));  //?
	//m_psList.back()->setTransform(cf);

	for(std::vector<PS_Base *>::iterator it = m_psList.begin();
		it != m_psList.end();
		++it)
	{
		if((*it)->getName() == "PS_RotatingFlare")
		{
			if(reinterpret_cast<PS_RotatingFlare *>(*it)->getCrater() == restoredCrater.getValue())
			{
				m_psList.erase(it);
				break;
			}
		}
	}
}

void ParticleSystemsRenderer::onEvent(Crater_Spawned &crater)
{
	CoordinateFrame cf = crater.getValue()->getCoordinateFrame();
	m_psList.push_back(PS_Manager::instance().fetchNewPS("PS_RotatingFlare"));
	m_psList.back()->setTransform(cf);
	reinterpret_cast<PS_RotatingFlare *>(m_psList.back())->setCrater(crater.getValue());
}