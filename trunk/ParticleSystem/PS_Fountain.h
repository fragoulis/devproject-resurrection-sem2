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

class PS_Fountain : public PS_Base
{
public:
	enum FountainType {
		FOUNTAIN_TYPE_FADE_IN,
		FOUNTAIN_TYPE_FADE_OUT,
		FOUNTAIN_TYPES_NUM,
	};

private:
	int m_particleColor;
	float m_radiusScale;
	FountainType m_fountainType;

public :
	// 2 Ctors : Template & copy 
	// Template gets the vbo, copy gets the model
	PS_Fountain(const std::string& name,
			Model * model,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const FountainType fountainType,
			const VAttribStatus& status);

	PS_Fountain(const std::string& name,
			VBO * vbo,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const FountainType fountainType,
			const std::string& texture);

	void  _generateData(VBO * vbo,Texture * tex);			// The texture for the gen model

	virtual void render() const;		
	virtual void update(const float delta);		
	virtual void reset();						
	virtual PS_Base * clone() const;					

	void setRadiusScale(float radiusScale) { m_radiusScale = radiusScale; };
	void setParticleColor(int particleColor) { m_particleColor = particleColor; };

};