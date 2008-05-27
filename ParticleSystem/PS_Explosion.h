#ifndef _RES_PS_EXPLOSION_H_
#define _RES_PS_EXPLOSION_H_

#include <string>
#include "PS_Base.h"

#include <dolphin.h>

class Texture;
//struct VAttribStatus;

/*
	This Particle system uses positions & velocity.
	In _generateData function, these 2 should be used.
	In the ctor for the copies, the m_usedAttribs should be modified appropriately
*/

static const GXColor explosionColor[1] =
		{
			{ 255, 255, 0, 255 }
		};

class PS_Explosion : public PS_Base
{


public :
	// 2 Ctors : Template & copy 
	// Template gets the vbo, copy gets the model
	PS_Explosion(const std::string& name,
			/*Model * model,*/
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum/*,
			const int shindex,*/
			/*const VAttribStatus& status*/);

	PS_Explosion(const std::string& name,
			/*VBO * vbo,*/
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			/*const int shindex,*/
			const std::string& texture);

	~PS_Explosion();

	//void  _generateData(VBO * vbo,Texture * tex);			// The texture for the gen model

	void setVelocity(const Vector3& v);
	virtual void render() const;		
	virtual void update(const float delta);		
	virtual void reset();						
	virtual PS_Base * clone() const;					

};

#endif