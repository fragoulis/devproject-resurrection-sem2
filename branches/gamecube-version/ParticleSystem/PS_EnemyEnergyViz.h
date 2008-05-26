//#ifndef _RES_PS_ENEMY_ENERGY_VIZ_H_
//#define _RES_PS_ENEMY_ENERGY_VIZ_H_
//
//#include <string>
//#include "PS_Base.h"
//
//class Texture;
//class Enemyship;
//struct VAttribStatus;
//
///*
//	A particle system for the visualization of the enemy energy type
//*/
//
//class PS_EnemyEnergyViz : public PS_Base
//{
//private:
//	float m_tailSize;
//	float m_speed;
//	const Enemyship * m_ship;	
//
//public :
//	// 2 Ctors : Template & copy 
//	// Template gets the vbo, copy gets the model
//	PS_EnemyEnergyViz(const std::string& name,
//			Model * model,
//			const float psize,
//			const float syslife,
//			const float plife,
//			const unsigned pnum,
//			const int shindex,
//			const VAttribStatus& status,
//			const float speed,
//			const float tailsize);
//
//	PS_EnemyEnergyViz(const std::string& name,
//			VBO * vbo,
//			const float psize,
//			const float syslife,
//			const float plife,
//			const unsigned pnum,
//			const int shindex,
//			const std::string& texture,
//			const float speed,
//			const float tailsize);
//
//	void  _generateData(VBO * vbo,Texture * tex);			// The texture for the gen model
//
//	virtual void render() const;		
//	virtual void update(const float delta);		
//	virtual void reset();						
//	virtual PS_Base * clone() const;	
//
//	void setEnemyship(const Enemyship *ship) { m_ship = ship; }
//	const Enemyship * getEnemyship() const { return m_ship;}
//
//};
//
//#endif