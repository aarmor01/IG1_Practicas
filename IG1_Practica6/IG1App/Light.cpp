#include "Light.h"

#include <gtc/type_ptr.hpp>

GLuint Light::cont = 0;
//-------------------------------------------------------------------------

Light::Light()
{
	if (cont < GL_MAX_LIGHTS) {
		id = GL_LIGHT0 + cont;
		++cont;
		glEnable(id);
	}
}
//-------------------------------------------------------------------------

void Light::uploadL() const
{
	// Transfiere las características de la luz a la GPU
	glLightfv(id, GL_AMBIENT, value_ptr(ambient));
	glLightfv(id, GL_DIFFUSE, value_ptr(diffuse));
	glLightfv(id, GL_SPECULAR, value_ptr(specular));
}
//-------------------------------------------------------------------------

void Light::enable()
{
	if (id < GL_LIGHT0 + GL_MAX_LIGHTS)
		glEnable(id);
}
//-------------------------------------------------------------------------

void Light::disable()
{
	if (id < GL_LIGHT0 + GL_MAX_LIGHTS)
		glDisable(id);
}
//-------------------------------------------------------------------------

void DirLight::upload(glm::dmat4 const& modelViewMat) const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));
	glLightfv(id, GL_POSITION, value_ptr(posDir));
	uploadL();
}
//-------------------------------------------------------------------------

void PosLight::upload(glm::dmat4 const& modelViewMat) const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));
	glLightfv(id, GL_POSITION, value_ptr(posDir));
	glLightf(id, GL_CONSTANT_ATTENUATION, kc);
	glLightf(id, GL_LINEAR_ATTENUATION, kl);
	glLightf(id, GL_QUADRATIC_ATTENUATION, kq);
	uploadL();
}
//-------------------------------------------------------------------------

void SpotLight::upload(glm::dmat4 const& modelViewMat) const {
	PosLight::upload(modelViewMat);
	glLightfv(id, GL_SPOT_DIRECTION, value_ptr(direction));
	glLightf(id, GL_SPOT_CUTOFF, cutoff);
	glLightf(id, GL_SPOT_EXPONENT, exp);
}
//-------------------------------------------------------------------------

void Material::upload()
{
	glMaterialfv(face, GL_AMBIENT, value_ptr(ambient));
	glMaterialfv(face, GL_DIFFUSE, value_ptr(diffuse));
	glMaterialfv(face, GL_SPECULAR, value_ptr(specular));
	glMaterialf(face, GL_SHININESS, expF);
	glShadeModel(sh);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE); // Defecto
}
//-------------------------------------------------------------------------

void Material::setCopper()
{
	ambient = { 0.19125, 0.0735, 0.0225, 1 };
	diffuse = { 0.7038, 0.27048, 0.0828, 1 };
	specular = { 0.256777, 0.137622, 0.086014, 1 };
	expF = 12.8;
}
//-------------------------------------------------------------------------

