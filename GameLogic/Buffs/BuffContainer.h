//*****************************************************************************
//**
//**   File:               BuffContainer.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************

#pragma once
#include <vector>
class Buff;
class WorldObject;

class BuffContainer
{
public:
	BuffContainer();
	virtual ~BuffContainer();

	bool hasBuff(int type) const;
	void addBuff(int type);
	void removeBuff(int type);
	void removeStack(int type);
	void removeAll();
	void removeAllBuffs();
	void removeAllDebuffs();
	const Buff* getBuff(int type) const { return _getBuff(type); }
	int getBuffStacks(int type) const;
	float getBuffTimeLeft(int type) const;

	// updates timers and removes expired buffs
	void update(float dt, WorldObject* target);

private:
	typedef std::vector<Buff*> BuffVector;
	typedef BuffVector::iterator BuffVectorIt;
	typedef BuffVector::const_iterator BuffVectorCIt;
	BuffVector m_buffs;


	BuffVectorIt _findBuff(int type);
	BuffVectorCIt _findBuff(int type) const;
	Buff* _getBuff(int type);
	const Buff* _getBuff(int type) const;
	void _removeBuff(BuffVectorIt it);
};
