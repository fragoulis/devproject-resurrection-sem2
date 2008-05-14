#pragma once
#include <vector>
#include <string>

class IAIState;
class Playership;
class AIEnemy;

/**
 * A behaviour comprises of a number of states and is
 * independent of any enemy, just like the state.
 * This means that for any number of enemies only one instance
 * of each behaviour will exist.
 */

class AIBehaviour 
{
private:
    typedef std::vector<IAIState*> StateList;

    // Enable flag
    bool m_enabled;

    // The list of states
    StateList m_stateList;

    // Time constraints for the states
    float m_minTime, m_maxTime;

public:
    AIBehaviour();

    // Updates the behaviour
    void update( float dt, Playership*, AIEnemy* );
    void addState( IAIState *state ) { m_stateList.push_back(state); }

    void enable() { m_enabled = true; }
    void disable() { m_enabled = false; }
    bool isEnabled() const { return m_enabled; }

    void setMinTime( float t ) { m_minTime = t; }
    void setMaxTime( float t ) { m_maxTime = t; }

    float getMinTime() const { return m_minTime; }
    float getMaxTime() const { return m_maxTime; }

    IAIState* getState( int index ) { return m_stateList[index]; }
};