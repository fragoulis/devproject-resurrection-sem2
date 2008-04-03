#pragma once
class IAIState;
class Enemyship;
class Playership;
class Point3;
class Vector3;

/**
 * Holds info about enemies and states.
 * According to this structure each enemy has one state and this
 * state is interchangable with any other from within its update.
 */
class AIStateEnemyCouple
{
private:
    IAIState *m_state;
    Enemyship *m_enemy;
    float m_timeToChange;
    float m_currentTime;

public:
    AIStateEnemyCouple();
    void update( Playership *player, float dt );
    bool readyToChange( float dt );
    
    void setTimeToChange( float t ) { m_timeToChange = t; }
    const Point3& getEnemyPosition() const;
    void setEnemyThrusterDirection( const Vector3& dir );

    void setEnemyship( Enemyship *enemy ) { m_enemy = enemy; }
    void setState( IAIState *state ) { m_state = state; }

    const Enemyship* getEnemyship() const { return m_enemy; }
    Enemyship* getEnemyship() { return m_enemy; }
};