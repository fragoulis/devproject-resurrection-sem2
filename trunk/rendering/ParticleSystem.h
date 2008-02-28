#pragma once

#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "Graphics.h"

class ParticleSystem {
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
		Vector3 m_acceleration; 
		float m_lifeTime;     // how long the particle lives for before dying  
		float m_age;          // current age of the particle  
		Vector4 m_color;        // current color of the particle  
		//Material m_material;  //current material of the particle
		Vector4 m_colorFade;    // how the color fades with respect to time
		bool m_isAlive;    
	};

public:
	virtual ~ParticleSystem() {};

	virtual void init() {};
	virtual void update(double deltaTime);
	virtual void render(Graphics g) const;

	void setEmitterPosition(const Vector3 &emitterPosition) { m_emitterPosition = emitterPosition; };
	void setEmitRate(float emitRate) { m_emitRate = emitRate; };
	void setNumParticles(int numParticles) { m_numParticles = numParticles; };
	
	bool isEmpty();
	bool isDead();

protected:
	virtual void removeDeadParticles();

	Vector3 m_emitterPosition;
	float m_emitRate;   // rate new particles are added to system
	float m_size;       // size of particles
	int m_numParticles; // max allowed particles system can have

	// sometimes we don't want to free the memory of a dead particle,
	// but rather respawn it instead.
	virtual void resetParticle(Particle* particle) = 0;

	//TODO need to add info about VBO, texture, shader, (material?)
};