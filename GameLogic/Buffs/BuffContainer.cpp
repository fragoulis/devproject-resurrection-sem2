//*****************************************************************************
//**
//**   File:               BuffContainer.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************

#include "BuffContainer.h"
#include "BuffFactory.h"
#include "Buff.h"
#include "../../utility/deleters.h"
#include <functional>
#include <algorithm>
using namespace std;


class BuffTypeEqualPredicate
{
public:
	BuffTypeEqualPredicate(int type) : m_type(type) { }
	inline bool operator() (const Buff* a) { return a->getType() == m_type; }
private:
	int m_type;
};


BuffContainer::BuffContainer()
{
}

BuffContainer :: ~BuffContainer()
{
	deleteVector(m_buffs);
}


int BuffContainer :: getBuffStacks(int type) const
{
	const Buff* buff = getBuff(type);
	if (buff == 0) return 0;
	return buff->getCurrentStacks();
}

float BuffContainer :: getBuffTimeLeft(int type) const
{
	const Buff* buff = getBuff(type);
	if (buff == 0) return 0.0f;
	return buff->getTimeLeft();
}

bool BuffContainer :: hasBuff(int type) const
{
	const Buff* buff = getBuff(type);
	if (buff == 0) return false;
	return buff->isActive();
}

void BuffContainer :: addBuff(int type)
{
	Buff* buff = _getBuff(type);
	if (buff == 0) {
		buff = BuffFactory::instance().createBuff(type);
		m_buffs.push_back(buff);
	}
	buff->addStack();
}

void BuffContainer :: removeBuff(int type)
{
	Buff* buff = _getBuff(type);
	buff->removeAllStacks();
}

void BuffContainer :: removeStack(int type)
{
	BuffVectorIt it = _findBuff(type);
	if (it == m_buffs.end()) return;
	Buff* buff = *it;
	buff->removeStack(); // update cycle will check if we have any stacks left
}

void BuffContainer :: removeAll()
{
	for_each(m_buffs.begin(), m_buffs.end(), mem_fun(&Buff::removeAllStacks));
}

void BuffContainer :: removeAllBuffs()
{
	for (BuffVectorIt it = m_buffs.begin(); it != m_buffs.end(); ++it)
	{
		Buff* buff = *it;
		if (!buff->isDebuff()) buff->removeAllStacks();
	}
}

void BuffContainer :: removeAllDebuffs()
{
	for (BuffVectorIt it = m_buffs.begin(); it != m_buffs.end(); ++it)
	{
		Buff* buff = *it;
		if (buff->isDebuff()) buff->removeAllStacks();
	}
}


void BuffContainer :: update(float dt, WorldObject* target)
{
	for (BuffVectorIt it = m_buffs.begin(); it != m_buffs.end(); ++it)
	{
		Buff* buff = *it;
		buff->update(dt, target);
	}
}


BuffContainer::BuffVectorIt BuffContainer :: _findBuff(int type)
{
	return find_if(m_buffs.begin(), m_buffs.end(), BuffTypeEqualPredicate(type));
}

BuffContainer::BuffVectorCIt BuffContainer :: _findBuff(int type) const
{
	return find_if(m_buffs.begin(), m_buffs.end(), BuffTypeEqualPredicate(type));
}

Buff* BuffContainer :: _getBuff(int type)
{
	BuffVectorIt it = _findBuff(type);
	if (it == m_buffs.end()) return 0;
	return *it;
}

const Buff* BuffContainer :: _getBuff(int type) const
{
	BuffVectorCIt it = _findBuff(type);
	if (it == m_buffs.end()) return 0;
	return *it;
}
