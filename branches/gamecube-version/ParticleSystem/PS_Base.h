#ifndef _RES_PS_BASE_H_
#define _RES_PS_BASE_H_

#include <string>
#include "../Math/CoordinateFrame.h"
#include "../Math/Vector3.h"
#include <vector>

using namespace std;
//#include "../gfx/VBO/VBO.H"

/*
	The particle system base class.
	Holds all the necessary PS variables.
*/

class TplPalette;
class PS_Base
{
protected:
	class Particle {
		public:
		Particle() {
			m_lifeTime = 0.0f;
			m_age      = 0.0f;
			m_isAlive  = true;
		};

		Vector3 m_position;     
		Vector3 m_velocity;     
		//Vector3 m_acceleration; 
		float m_lifeTime;     // how long the particle lives for before dying  
		float m_age;          // current age of the particle  
		float m_colorR;
		float m_colorG;
		float m_colorB;
		float m_colorA;
		bool m_hasLooped;
		bool m_isAlive;  
	};

public :
	const std::string& getName() const {return m_nameId;}

	virtual void render() const	=0;					
	virtual void update(const float delta)=0;
	virtual void reset() =0;						
	virtual PS_Base * clone() const =0;		

	void setTransform(const CoordinateFrame& transform);


	PS_Base(const std::string& name,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum/*,
			const int shindex,
			const bool deleteModelOnDestruction*/);			//Creates the ps with it's default parameters

	virtual ~PS_Base() = 0;

	const float &getCurrentTime() const { return m_currentTime; };
	const float &getSystemLife() const { return m_systemLife; };

protected :

	std::string   m_nameId;			// name of the particle system
	//VAttribStatus m_usedAttribs;	// used attribs by the system, hard-coded in ctor as the _generateData

	// Non - volatile ( most of them used in cloning)

	float		  m_particleSize;
	float		  m_systemLife;
	float		  m_particleLife;
	unsigned	  m_particleNum;
	//Model		* m_quadArray;
	//int			  m_shaderIndex;		//Might this override model's shader index for quadarray sharing??
	CoordinateFrame m_transform;

	// volatile ( most of them used in rendering / reset)
	float		  m_currentTime;

	TplPalette * m_flarePal;

	vector<Particle> m_particles;

private : 
	//bool		 m_deleteModelOnDestruction;

};

#endif