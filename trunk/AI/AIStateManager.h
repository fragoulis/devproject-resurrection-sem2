#pragma once
#include "../utility/Singleton.h"
#include <map>
#include <string>
class IAIState;

class AIStateManager : public Singleton<AIStateManager>
{
private:
    typedef std::string state_id_t;
    typedef std::map< const state_id_t, IAIState* > StateList;

    // List with all ai states
    StateList m_states;

public:
    void init();
    IAIState* getRandomState();
    IAIState* getState( const state_id_t &key ) {
        return m_states[ key ];
    }

private:
    friend Singleton<AIStateManager>;
    ~AIStateManager();
};