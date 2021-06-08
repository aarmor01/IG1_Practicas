//#pragma once
#ifndef _H_EntityWithMaterial_H_
#define _H_EntityWithMaterial_H_

#include "Entity.h"

#include "Mesh.h"
#include "Light.h"
#include "Texture.h"
#include "CheckML.h"

using namespace glm;
//-------------------------------------------------------------------------

class EntityWithMaterial : public Abs_Entity {
public:
	EntityWithMaterial() : Abs_Entity() { };
	virtual ~EntityWithMaterial() { };
	void setMaterial(Material* matl) { material = matl; };
protected:
	Material* material = nullptr;
};
//-------------------------------------------------------------------------

class Esfera : public EntityWithMaterial {
public:
	explicit Esfera(GLdouble radius, GLuint puntosPerfil, GLdouble rev);
	virtual ~Esfera() {};
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------

#endif //_H_EntityWithMaterial_H_