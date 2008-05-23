#ifndef _RES_BUFFCONTAINER_H_
#define _RES_BUFFCONTAINER_H_
#include <vector>
class Buff;
class WorldObject;


/**
 * Container for buffs. Can be added to NPCs or players to give them buffs :P
 */
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
	float getBuffDuration(int type) const;

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


#endif