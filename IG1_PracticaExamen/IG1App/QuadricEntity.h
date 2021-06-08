//#pragma once
#ifndef _H_QuadricEntity_H_
#define _H_QuadricEntity_H_

#include "Entity.h"

#include "Mesh.h"
#include "Light.h"
#include "Texture.h"
#include "CheckML.h"

using namespace glm;
//-------------------------------------------------------------------------

class QuadricEntity : public Abs_Entity {
public:
	QuadricEntity() { q = gluNewQuadric(); };
	~QuadricEntity() { gluDeleteQuadric(q); };
protected:
	GLUquadricObj* q;
};
//------------------------------------------------------------------------

class Sphere : public QuadricEntity {
public:
	Sphere(GLdouble radius) { this->radius = radius; };
	// r es el radio de la esfera
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble radius;
};
//------------------------------------------------------------------------

class Cylinder : public QuadricEntity {
public:
	Cylinder(GLdouble baseRadius, GLdouble topRadius, GLdouble height);
	// r es el radio de la esfera
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble baseRadius;
	GLdouble topRadius;
	GLdouble height;
};
//------------------------------------------------------------------------

class Disk : public QuadricEntity {
public:
	Disk(GLdouble innerRadius, GLdouble outerRadius);
	// r es el radio de la esfera
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble innerRadius;
	GLdouble outerRadius;
};
//------------------------------------------------------------------------

class PartialDisk : public QuadricEntity {
public:
	PartialDisk(GLdouble innerRadius, GLdouble outerRadius, GLdouble startAngle, GLdouble sweepAngle);
	// r es el radio de la esfera
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble innerRadius;
	GLdouble outerRadius;
	GLdouble startAngle;
	GLdouble sweepAngle;
};
//------------------------------------------------------------------------

#endif //_H_QuadricEntity_H_