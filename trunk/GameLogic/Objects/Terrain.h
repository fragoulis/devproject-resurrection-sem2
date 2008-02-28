#pragma once
#include <string>

class Terrain
{
public:
	Terrain(const std::string& id) : m_id(id) { }
	~Terrain() { }

	const std::string& getID() { return m_id; }

	// TODO: write this method
	// float getHeight(float x, float y);

private:
	std::string m_id;

	// TODO: store height data
};
