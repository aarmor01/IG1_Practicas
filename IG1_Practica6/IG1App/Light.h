#ifndef _H_Light_H_
#define _H_Light_H_

#include <GL/freeglut.h>
#include <glm.hpp>

class Light {
protected:
	static GLuint cont;						// Atributo para poder generar un nuevo id cada vez
	GLuint id = GL_LIGHT0 + GL_MAX_LIGHTS;  // Primer id no válido

	// Atributos lumínicos y geométrico de la fuente de luz
	glm::fvec4 ambient = { 0.1, 0.1, 0.1, 1 };
	glm::fvec4 diffuse = { 0.5, 0.5, 0.5, 1 };
	glm::fvec4 specular = { 0.5, 0.5, 0.5, 1 };
	glm::fvec4 posDir = { 0, 0, 1, 0 };

public:
	Light();
	virtual ~Light() { disable(); }

	void uploadL() const;

	virtual void upload(glm::dmat4 const& modelViewMat) const = 0;

	void enable();
	void disable();

	void setAmb(glm::fvec4 amb) { ambient = amb; uploadL(); };
	void setDiff(glm::fvec4 diff) { diffuse = diff; uploadL(); };
	void setSpec(glm::fvec4 spec) { specular = spec; uploadL(); };
};
//-------------------------------------------------------------------------

class DirLight : public Light {
public:
	virtual void upload(glm::dmat4 const& modelViewMat) const;
	void setPosDir(glm::fvec3 dir) { posDir = glm::fvec4(dir, 0.0); };
};
//-------------------------------------------------------------------------

class PosLight : public Light {
protected:
	// Factores de atenuación
	GLfloat kc = 1, kl = 0, kq = 0;

public:
	virtual void upload(glm::dmat4 const& modelViewMat) const;

	void setAtte(GLfloat kc, GLfloat kl, GLfloat kq) {
		this->kc = kc; this->kl = kl; this->kq = kq;
	};

	void setPosDir(glm::fvec3 dir) { posDir = glm::fvec4(dir, 1.0); };
};
//-------------------------------------------------------------------------

class SpotLight : public PosLight {
protected:
	// Atributos del foco
	glm::fvec4 direction = { 0, 0, -1, 0 };
	GLfloat cutoff = 180;
	GLfloat exp = 0;

public:
	SpotLight(glm::fvec3 pos = { 0, 0, 0 }) : PosLight() {
		posDir = glm::fvec4(pos, 1.0);
	};

	virtual void upload(glm::dmat4 const& modelViewMat) const;

	void setSpot(glm::fvec3 dir, GLfloat cf, GLfloat e) {
		direction = glm::fvec4(dir, 0.0);
		cutoff = cf;
		exp = e;
	};
};
//-------------------------------------------------------------------------

class Material {
protected:
	// Coeficientes de reflexión
	glm::fvec4 ambient = { 0.2, 0.2, 0.2, 1.0 };
	glm::fvec4 diffuse = { 0.8, 0.8, 0.8, 1.0 };
	glm::fvec4 specular = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat expF = 0; // Exponente para la reflexión especular
	GLuint face = GL_FRONT_AND_BACK;
	GLuint sh = GL_SMOOTH; // Tipo de matizado

public:
	Material() {};
	virtual ~Material() {};
	virtual void upload();
	void setCopper();
};
//-------------------------------------------------------------------------

#endif