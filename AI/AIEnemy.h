#pragma once

class AIBehaviour;
class Enemyship;
class Playership;
class Point3;
class Vector3;

/**
 * Holds info about enemies and states.
 * According to this structure each enemy has one state and this
 * state is interchangable with any other from within its update.
 */
class AIEnemy
{
private:
    AIBehaviour *m_behaviour;
    Enemyship *m_enemy;
    int m_state; // current state index

    // Time control variables
    float m_timeToChange;
    float m_currentTime;

    // Thruster power before state change
    float m_previousThrusterPower;

public:
    AIEnemy();

    void update( float dt, Playership *player );
    
    const Point3& getEnemyPosition() const;
    void setEnemyThrusterDirection( const Vector3& dir );
    
    void setEnemyThrusterPower( float power );
    float getEnemyThrusterPower() const;

    void setEnemyship( Enemyship *enemy ) { m_enemy = enemy; }
    void setBehaviour( AIBehaviour *behaviour );

    const Enemyship* getEnemyship() const { return m_enemy; }
    Enemyship* getEnemyship() { return m_enemy; }

    int getState() const { return m_state; }
    void setState( int i ) { m_state = i; }

    void setPreviousThrusterPower( float ptp ) { m_previousThrusterPower = ptp; }
    float getPreviousThrusterPower() const { return m_previousThrusterPower; }

    // Check time interval
    bool readyToChange( float dt );
};