#pragma once

#include <string>
#include "PS_Base.h"

class Texture;
struct VAttribStatus;

/*
	The cloud particle system	
*/

class PS_Clouds : public PS_Base
{
private:

public :
	// 2 Ctors : Template & copy 
	// Template gets the vbo, copy gets the model
	PS_Clouds(const std::string& name,
			Model * model,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const VAttribStatus& status);

	PS_Clouds(const std::string& name,
			VBO * vbo,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const std::string& texture,
			const std::string& noisetex);

	void  _generateData(VBO * vbo,Texture * tex,Texture * tex2);			// The texture for the gen model

	virtual void render() const;		
	virtual void update(const float delta);		
	virtual void reset();						
	virtual PS_Base * clone() const;	

};