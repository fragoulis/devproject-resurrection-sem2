#ifndef _RES_BUFF_H_
#define _RES_BUFF_H_
class ParserSection;
class WorldObject;



/**
 * Holds data for a generic buff.
 * A buff is a powerup for the player or an NPC (think WoW).
 * A buff has a fixed duration and number of stacks.
 * Multiple stacks increase the effect of the buff.
 * A buff that has a negative on its subject is called a debuff.
 * Debuffs use this class as well.
 * If a buff has 0 stacks or the timer has expired, it is inactive.
 * Inactive buffs are not removed from the container, but simply flagged as inactive.
 */
class Buff
{
public:
	Buff(int type);
	virtual ~Buff();

	int getType() const { return m_type; }
	float getTimeLeft() const { return m_timeLeft; }
	float getDuration() const { return m_duration; }
	int getCurrentStacks() const { return m_currentStacks; }
	int getMaxStacks() const { return m_maxStacks; }
	bool isDebuff() const { return m_debuff; }
	bool isExpired() const { return m_timeLeft < 0.0f; }
	bool hasStacks() const { return m_currentStacks > 0; }
	bool isActive() const { return m_active; }

	void setType(int type) { m_type = type; }
	void setDuration(float d) { m_timeLeft = d; }
	void setMaxStacks(int s) { m_maxStacks = s; }
	void setIsDebuff(bool b) { m_debuff = b; }
	void addStack() { if (m_currentStacks < m_maxStacks) { m_currentStacks++; refreshTimer(); } }
	void removeStack() { m_currentStacks--; }
	void removeAllStacks() { m_currentStacks = 0; }
	void refreshTimer() { m_timeLeft = m_duration; }


	void update(float dt, WorldObject* target);
	virtual void loadSettings(const ParserSection&);
	virtual Buff* clone() const;

private:
	int m_type;
	float m_timeLeft;
	float m_duration;
	int m_currentStacks;
	int m_maxStacks;
	bool m_debuff;
	bool m_active;

	virtual void _onActivate(WorldObject* target) { }
	virtual void _onDeactivate(WorldObject* target) { }
	virtual void _onUpdate(float dt, WorldObject* target) { }
};


#endif
