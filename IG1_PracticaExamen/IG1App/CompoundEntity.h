//#pragma once
#ifndef _H_CompoundEntity_H_
#define _H_CompoundEntity_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Entity.h"

#include "Mesh.h"
#include "Light.h"
#include "Texture.h"
#include "CheckML.h"

using namespace glm;
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
	explicit TIE(std::vector<Texture*> gTextures, GLfloat size = 300, bool turnLight = false);
	virtual ~TIE();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	SpotLight* getSpotLight() { return light; }
private:
	std::vector<Abs_Entity*> gObjectsTrans;
	SpotLight* light = nullptr;
};
//-------------------------------------------------------------------------

class GridCube : public CompoundEntity {
public:
	explicit GridCube(GLdouble lado, GLuint numDivisiones, std::vector<Texture*> gTextures);
	virtual ~GridCube() {};
};
//-------------------------------------------------------------------------

#endif //_H_CompoundEntity_H_