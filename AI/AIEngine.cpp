//*****************************************************************************
//**
//**   File:               AIEngine.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 18:24:39 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 133 $
//**
//*****************************************************************************

#include "AIEngine.h"
#include "Behaviours/AIBehaviour.h"
#include "Behaviours/States/AIChase.h"
#include "Behaviours/States/AISurround.h"
#include "Behaviours/States/AIFlank.h"
#include "Behaviours/States/AIFlee.h"
#include "Behaviours/States/AIIdle.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/EnergyTypes.h"
#include "../GameLogic/WorldObjectTypeManager.h"
#include "../gfxutils/ConfParser/ConfParser.h"
#include "../gfxutils/Misc/utils.h"
#include "../utility/RandomGenerator.h"
#include <iostream>
using namespace std;




AIEngine::AIEngine(): 
m_playership(0)
{
	EventManager::instance().registerEventListener< Level_Load >(this);
	EventManager::instance().registerEventListener< Level_Unload >(this);
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Enemy_Spawned >(this);
	EventManager::instance().registerEventListener< Enemy_Despawned >(this);
    EventManager::instance().registerEventListener< Player_Destroyed >(this);
}

AIEngine::~AIEngine()
{
	// TODO: when we implement a loading screen, remove this
	onApplicationUnload();
}

// ----------------------------------------------------------------------------
void AIEngine::onApplicationLoad(const ParserSection& ps)
{
    const std::string conf_root("config/");

    // Get Ai file name
    const std::string ai_file = ps.getSection("AI")->getVal("file");
    const ConfParser ai_conf( conf_root + ai_file );

    readStates(ai_conf);
    readBehaviours(ai_conf);
}

// ----------------------------------------------------------------------------
void AIEngine::onApplicationUnload()
{
    for(StateListIt i = m_states.begin(); i != m_states.end(); ++i)
    {
        delete i->second;
        i->second = 0;
    }

    for(BehaviourListIt i = m_behaviours.begin(); i != m_behaviours.end(); ++i)
    {
        delete i->second;
        i->second = 0;
    }
}

// ----------------------------------------------------------------------------
void AIEngine::onEvent(Level_Load& ll)
{
    typedef std::vector<std::string> StringList;

	const ParserSection *ps = ll.getValue1();
    const std::string conf_root("config/levels/");

    // Open AI config file
    const std::string ai_file = ps->getSection("AI")->getVal("file");
    const ConfParser ai_conf( conf_root + ai_file );

    // Load LOD data for the level
    /* - Color sequence probability */

    const ParserSection *s_behaviours = ai_conf.getSection("Behaviours");
    const StringList behaviours = s_behaviours->getValVector("Names");

    StringList::const_iterator i = behaviours.begin();
    for(; i != behaviours.end(); ++i )
    {
        m_behaviours[ *i ]->enable();
    }

    m_minThrusterPower = FromString<float>(s_behaviours->getVal("MinEnemyThrusterPower"));
    m_maxThrusterPower = FromString<float>(s_behaviours->getVal("MaxEnemyThrusterPower"));
}

// ----------------------------------------------------------------------------
void AIEngine::onEvent(Level_Unload&)
{
    // Disable all behaviours
    BehaviourListIt i = m_behaviours.begin();
    for(; i != m_behaviours.end(); ++i)
        i->second->disable();
}

// ----------------------------------------------------------------------------
void AIEngine::onEvent( Player_Spawned& es )
{
	m_playership = es.getValue();
}

// ----------------------------------------------------------------------------
void AIEngine::onEvent( Player_Destroyed& pd )
{
	Playership* ps = pd.getValue1();
	EnergyType type = pd.getValue2();
}

// ----------------------------------------------------------------------------
void AIEngine::onEvent(Enemy_Spawned& es)
{
	Enemyship* enemyship = es.getValue();
    int type = enemyship->getType();
    const std::string s_type = WorldObjectTypeManager::instance().getNameFromType(type);
    
    // Dont give any ai to carriers
    if( s_type == "EnemyCarrier" ) return;
    
    // Give initial thruster power
    float power = RandomGenerator::GET_RANDOM_FLOAT( m_minThrusterPower, m_maxThrusterPower );
    enemyship->setThrusterPower(power);

    AIEnemy sec;
    sec.setBehaviour( getRandomBehaviour() );
    sec.setEnemyship( enemyship );
    m_enemyList.push_back(sec);
}

// ----------------------------------------------------------------------------
void AIEngine::onEvent(Enemy_Despawned& es)
{
	Enemyship* enemyship = es.getValue();

    for( EnemyListIt i = m_enemyList.begin(); i != m_enemyList.end(); ++i )
    {
        const AIEnemy& couple = *i;
        if( couple.getEnemyship() == enemyship )
        {
            m_enemyList.erase(i);
            break;
        }
    }
}

// ----------------------------------------------------------------------------
void AIEngine::update(float dt)
{
    EnemyListIt i = m_enemyList.begin();
    for(; i != m_enemyList.end(); ++i ) 
    {
        (*i).update( dt, m_playership );
    }
}

// ----------------------------------------------------------------------------
void AIEngine::readStates(const ConfParser& conf)
{
    //typedef std::vector<std::string> StringList;
    //const ParserSection& states_section = *(conf.getSection("States"));
    //const StringList states = states_section.getValVector("Enum");

    m_states.insert( std::make_pair( "Idle", new AIIdle ) );
    m_states.insert( std::make_pair( "Chase", new AIChase ) );
    m_states.insert( std::make_pair( "Surround", new AISurround ) );
    m_states.insert( std::make_pair( "Flank", new AIFlank ) );
    m_states.insert( std::make_pair( "Flee", new AIFlee ) );
}

// ----------------------------------------------------------------------------
void AIEngine::readBehaviours(const ConfParser& conf)
{
    typedef std::vector<std::string> StringList;

    const ParserSection& behaviours_section = *(conf.getSection("Behaviours"));
    const StringList behaviours = behaviours_section.getValVector("Enum");

    StringList::const_iterator i = behaviours.begin();
    for(; i != behaviours.end(); ++i )
    {
        AIBehaviour *b = new AIBehaviour;

        const std::string behaviour     = *i;
        const std::string s_section     = "Behaviours:" + behaviour;
        const ParserSection& b_section  = *(conf.getSection( s_section ));

        const StringList states = b_section.getValVector("States");
        
        // Push states to behaviour
        StringList::const_iterator k = states.begin();
        for(; k != states.end(); ++k )
        {
            b->addState( m_states[ *k ] );
        }
        
        // Read additional data
        if( states.size() > 1 )
        {
            float minTime = FromString<float>(b_section.getVal("MinTime"));
            float maxTime = FromString<float>(b_section.getVal("MaxTime"));
            b->setMinTime(minTime);
            b->setMaxTime(maxTime);
        }

        m_behaviours.insert( std::make_pair( behaviour, b ) );
    }
}

// ----------------------------------------------------------------------------
AIBehaviour* AIEngine::getRandomBehaviour()
{
    AIBehaviour *out = 0;
    
    int max = (int)m_behaviours.size(); // cache array size
    int index = RandomGenerator::GET_RANDOM_INT(0,max);

    do
    {
        //std::cerr << "index = " << index << std::endl;

        assert(index<max);
        BehaviourListIt iter = m_behaviours.begin();
        for( int i=0; i<index; ++i ) 
            ++iter;

        if( iter->second->isEnabled() ) 
        {
            out = iter->second;
            //std::cerr << "Enemy spawned with behaviour: " << iter->first << std::endl;
        }

        index++;
        index %= max;
    }
    while(!out);

    assert(out);
    return out;
}