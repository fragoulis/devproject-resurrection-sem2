#pragma once

#include "Color.h"

//opengl material utility class
class Material {
private:
	Color m_ambient;
	Color m_diffuse;
	Color m_specular;
	Color m_emission;

	GLfloat m_shininess;

public:
	Material();

	void Set(const Color &ambient=Color::white(), const Color &diffuse=Color::white(), const Color &specular=Color::white(), const Color &emission=Color::null(), GLfloat shininess=0.0f);
	void Set(const float r, const float g, const float b, const float a) {
		Color color;
		color.Set(r, g, b, a);

		Set(color, color, color);
	};

	void SetAmbient(const Color &ambient);
	const Color &GetAmbient() const { return m_ambient; };
	void SetDiffuse(const Color &diffuse);
	const Color &GetDiffuse() const { return m_diffuse; };
	void SetSpecular(const Color &specular);
	const Color &GetSpecular() const { return m_specular; };
	void SetEmission(const Color &emission);
	const Color &GetEmission() const { return m_emission; };
	void SetShininess(const GLfloat &shininess);
	const GLfloat &GetShininess() const { return m_shininess; };

	void Apply() const;

	Material &operator=(const Material &material);
};