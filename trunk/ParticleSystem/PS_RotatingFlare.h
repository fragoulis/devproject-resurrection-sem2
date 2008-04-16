#pragma once

#include <string>
#include "PS_Base.h"

class Texture;
class Crater;
struct VAttribStatus;

/*
	This Particle system uses positions & velocity.
	In _generateData function, these 2 should be used.
	In the ctor for the copies, the m_usedAttribs should be modified appropriately
*/

class PS_RotatingFlare : public PS_Base
{
private:
	float m_tailSize;
	float m_speed;
	const Crater * m_crater;	

public :
	// 2 Ctors : Template & copy 
	// Template gets the vbo, copy gets the model
	PS_RotatingFlare(const std::string& name,
			Model * model,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const VAttribStatus& status,
			const float speed,
			const float tailsize);

	PS_RotatingFlare(const std::string& name,
			VBO * vbo,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const std::string& texture,
			const float speed,
			const float tailsize);

	void  _generateData(VBO * vbo,Texture * tex);			// The texture for the gen model

	virtual void render() const;		
	virtual void update(const float delta);		
	virtual void reset();						
	virtual PS_Base * clone() const;	

	void setCrater(const Crater *crater) { m_crater = crater; }
	const Crater * getCrater() const { return m_crater;}

};