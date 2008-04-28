#pragma once

#include <string>
#include "PS_Base.h"

class Texture;
struct VAttribStatus;

/*
	This Particle system uses positions & velocity.
	In _generateData function, these 2 should be used.
	In the ctor for the copies, the m_usedAttribs should be modified appropriately
*/

class PS_Explosion2 : public PS_Base
{
	float m_sizeModifier;

public :
	// 2 Ctors : Template & copy 
	// Template gets the vbo, copy gets the model
	PS_Explosion2(const std::string& name,
			Model * model,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const VAttribStatus& status);

	PS_Explosion2(const std::string& name,
			VBO * vbo,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const std::string& texture);

	void  _generateData(VBO * vbo,Texture * tex);			// The texture for the gen model

	void setSizeModifier(const float sizemod) {m_sizeModifier = sizemod / 50.0f;}	//the std size is 50.0
	virtual void render() const;		
	virtual void update(const float delta);		
	virtual void reset();						
	virtual PS_Base * clone() const;					

};