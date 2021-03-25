//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "CheckML.h"


using namespace glm;

//-------------------------------------------------------------------------

class Abs_Entity  // abstract class
{
public:
	Abs_Entity() : mModelMat(1.0), mColor(1) {};  // 4x4 identity matrix
	virtual ~Abs_Entity() { delete mMesh; mMesh = nullptr; };

	Abs_Entity(const Abs_Entity& e) = delete;  // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete;  // no copy assignment

	virtual void render(dmat4 const& modelViewMat) const = 0;  // abstract method

	// modeling matrix
	dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(dmat4 const& aMat) { mModelMat = aMat; };
	void setColor(dvec4 const& aColor) { mColor = aColor; };
	void setTexture(Texture* const& text) { mTexture = text; };
	virtual void update() {};

protected:

	Mesh* mMesh = nullptr;   // the mesh
	Texture* mTexture = nullptr; // texture
	dmat4 mModelMat;    // modeling matrix
	dvec4 mColor;		 // color vector

	// transfers modelViewMat to the GPU
	virtual void upload(dmat4 const& mModelViewMat) const;
};
//-------------------------------------------------------------------------

class EjesRGB : public Abs_Entity
{
public:
	explicit EjesRGB(GLdouble l);
	virtual ~EjesRGB() {};
	virtual void render(dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

class Poligono : public Abs_Entity {
public:
	explicit Poligono(GLuint numL, GLdouble rd);
	virtual ~Poligono() {};
	virtual void render(dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

class Sierpinski : public Abs_Entity {
public:
	explicit Sierpinski(GLuint numP, GLdouble rd);
	virtual ~Sierpinski() {};
	virtual void render(dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

class TrianguloRGB : public Abs_Entity {
public:
	explicit TrianguloRGB(GLdouble rd);
	virtual ~TrianguloRGB() {};
	virtual void update();
	virtual void render(dmat4 const& modelViewMat) const;

private:
	GLdouble rad = 260.0;
	GLdouble localAngle = 0.0, globalAngle = 90.0;
};
//-------------------------------------------------------------------------

class RectanguloRGB : public Abs_Entity {
public:
	explicit RectanguloRGB(GLdouble w, GLdouble h);
	virtual ~RectanguloRGB() {};
	virtual void render(dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

class Estrella3D : public Abs_Entity {
public:
	explicit Estrella3D(GLdouble re, GLuint np, GLdouble h);
	virtual ~Estrella3D() {};
	virtual void render(dmat4 const& modelViewMat) const;
	virtual void update();
	void setPosition(const dvec3& position);
private:
	GLdouble zAngle = 0.0, yAngle = 0.0;
	dvec3 position;
};
//-------------------------------------------------------------------------

class Caja : public Abs_Entity {
public:
	explicit Caja(GLdouble ld);
	virtual ~Caja() {};
	virtual void render(dmat4 const& modelViewMat) const;
	void setTexture(Texture* const& t, Texture* const& t2);
protected:
	Texture* mText2 = nullptr;
};
//-------------------------------------------------------------------------

class CajaConFondo : public Caja {
public:
	explicit CajaConFondo(GLdouble ld);
	virtual ~CajaConFondo();
	virtual void render(dmat4 const& modelViewMat) const;
	virtual void update();
	void setPosition(const dvec3& position);
	void translateAll();
private:
	Mesh* meshFloor = nullptr;
	dmat4 modelMatFloor;
	dvec3 position;
	GLdouble angle;
	GLdouble ld_;
};
//-------------------------------------------------------------------------

class Suelo : public Abs_Entity {
public:
	explicit Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	virtual ~Suelo() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

class Foto : public Abs_Entity {
public:
	explicit Foto(GLdouble w, GLdouble h);
	virtual ~Foto() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
};
//-------------------------------------------------------------------------

class Planta : public Abs_Entity
{
public:
	explicit Planta(GLdouble w, GLdouble h);
	virtual ~Planta() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update() {};
};
//-------------------------------------------------------------------------

#endif //_H_Entities_H_