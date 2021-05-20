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

	Abs_Entity(const Abs_Entity& e) = delete;			  // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete;  // no copy assignment

	virtual void render(dmat4 const& modelViewMat) const = 0;  // abstract method

	// modeling matrix
	dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(dmat4 const& aMat) { mModelMat = aMat; };
	void setColor(dvec4 const& aColor) { mColor = aColor; };
	void setTexture(Texture* const& text) { mTexture = text; };
	virtual void update() {};

protected:

	Mesh* mMesh = nullptr;		 // the mesh
	Texture* mTexture = nullptr; // texture
	dmat4 mModelMat;			 // modeling matrix
	dvec4 mColor;				 // color vector

	// transfers modelViewMat to the GPU
	virtual void upload(dmat4 const& mModelViewMat) const;
};
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
	dvec3 position = dvec3();
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

class Hexagono : public Abs_Entity {
public:
	explicit Hexagono(GLdouble radio);
	virtual ~Hexagono() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update() {};
};
//-------------------------------------------------------------------------

/*
 * No usamos la clase EntityWithIndex porque como la unica diferencia entre esta clase y
 * Abs_Entity sería cambiar el Mesh* por IndexMesh*; y al ser Mesh la clase padre de IndexMesh,
 * un Mesh ya puede contener un IndexMesh, la clase se quedaría así:
 *
 * class EntityWithIndex: public Abs_Entity {};
 *
 * por lo que no es necesario (y sería más util hacer un using EntityWithIndex = Abs_Entity).
 * Además esto lo consultamos en clase contigo y nos diste permiso.
 */

class AnilloCuadrado : public Abs_Entity {
public:
	explicit AnilloCuadrado();
	virtual ~AnilloCuadrado() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update() {};
};
//-------------------------------------------------------------------------

class CuboIndexado : public Abs_Entity {
public:
	explicit CuboIndexado(GLdouble l);
	virtual ~CuboIndexado() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update() {};
};
//-------------------------------------------------------------------------

class CompoundEntity : public Abs_Entity {
public:
	explicit CompoundEntity() {};
	virtual ~CompoundEntity();
	std::vector<Abs_Entity*> gObjects;
	void addEntity(Abs_Entity* ae) { gObjects.emplace_back(ae); }
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update() {};
};
//-------------------------------------------------------------------------

class TIE : public CompoundEntity {
public:
	explicit TIE(std::vector<Texture*> gTextures);
	virtual ~TIE() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
private:
	std::vector<Abs_Entity*> gObjectsTrans;
};
//-------------------------------------------------------------------------

class GridCube : public CompoundEntity {
public:
	explicit GridCube(GLdouble lado, GLuint numDivisiones, std::vector<Texture*> gTextures);
	virtual ~GridCube() {};
};
//-------------------------------------------------------------------------

class Grid : public Abs_Entity {
public:
	Grid(GLdouble lado, GLuint numDivisiones);
	virtual ~Grid() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

// Entidades por revolucion (Mbr)

class Cono : public Abs_Entity {
public:
	explicit Cono(GLdouble height, GLdouble radius, GLuint n);
	virtual ~Cono() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

class Esfera : public Abs_Entity {
public:
	explicit Esfera(GLdouble radius, GLuint puntosPerfil, GLdouble rev);
	virtual ~Esfera() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

#endif //_H_Entities_H_