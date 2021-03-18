//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Mesh.h"

//-------------------------------------------------------------------------

class Abs_Entity  // abstract class
{
public:
	Abs_Entity() : mModelMat(1.0), mColor(1) {};  // 4x4 identity matrix
	virtual ~Abs_Entity() { delete mMesh; mMesh = nullptr; };

	Abs_Entity(const Abs_Entity& e) = delete;  // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete;  // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0;  // abstract method

	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };
	void setColor(glm::dvec4 const& aColor) { mColor = aColor; };
	virtual void update() {};

protected:

	Mesh* mMesh = nullptr;   // the mesh
	glm::dmat4 mModelMat;    // modeling matrix
	glm::dvec4 mColor;		 // color vector

	// transfers modelViewMat to the GPU
	virtual void upload(glm::dmat4 const& mModelViewMat) const;
};
//-------------------------------------------------------------------------

class EjesRGB : public Abs_Entity
{
public:
	explicit EjesRGB(GLdouble l);
	virtual ~EjesRGB() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

class Poligono : public Abs_Entity {
public:
	explicit Poligono(GLuint numL, GLdouble rd);
	virtual ~Poligono() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

class Sierpinski : public Abs_Entity {
public:
	explicit Sierpinski(GLuint numP, GLdouble rd);
	virtual ~Sierpinski() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

class TrianguloRGB : public Abs_Entity {
public:
	explicit TrianguloRGB(GLdouble rd);
	virtual ~TrianguloRGB() {};
	virtual void update();
	virtual void render(glm::dmat4 const& modelViewMat) const;

private:
	GLdouble rad = 260.0;
	GLdouble localAngle = 0.0, globalAngle = 90.0;
};
//-------------------------------------------------------------------------

class RectanguloRGB : public Abs_Entity {
public:
	explicit RectanguloRGB(GLdouble w, GLdouble h);
	virtual ~RectanguloRGB() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

class Estrella3D : public Abs_Entity {
public:
	explicit Estrella3D(GLdouble re, GLuint np, GLdouble h);
	virtual ~Estrella3D() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
private:
	GLdouble zAngle = 0.0, yAngle = 0.0;
};
//-------------------------------------------------------------------------

class Caja : public Abs_Entity {
public:
	explicit Caja(GLdouble ld);
	virtual ~Caja() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
	/*virtual void setTexture(Texture* const& t, Texture* const& t2) {
		Abs_Entity::setTexture(t);
		text2 = t2;
	}*/
protected:
	//Texture* text2 = nullptr;
};
//-------------------------------------------------------------------------

#endif //_H_Entities_H_