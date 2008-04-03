#pragma once

class Playership;
class Enemyship;

class IAIState
{
public:
    virtual ~IAIState() = 0 {};
    virtual void update( Playership*, Enemyship* ) const = 0;
};