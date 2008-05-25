#ifndef __LAKE_H__
#define __LAKE_H__

#include "../math/Vector3.h"
#include <dolphin.h>

class Texture;
class Lake
{
	// Lake color
	// Wind direction

	Texture * m_texture;
	float  m_time;
	u8  m_repeats;
	float  m_speed;

	Vector3 m_ll,m_right,m_up;

public :

	Lake(unsigned color);			// Get texture from palette, also get extents
	~Lake();		// delete lake palette
	void render() const;
	void update(const float dt);
	const unsigned getPolynum() const {return 2;}
};


#endif