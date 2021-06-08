#ifndef _H_Material_H_
#define _H_Material_H_

#include <GL/freeglut.h>
#include <glm.hpp>

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