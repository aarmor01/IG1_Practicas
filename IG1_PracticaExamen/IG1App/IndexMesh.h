////#pragma once
#ifndef _H_IndexMesh_H_
#define _H_IndexMesh_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include <vector>
#include "Mesh.h"

using namespace std;
using namespace glm;
//-------------------------------------------------------------------------

class IndexMesh : public Mesh {
public:

	static IndexMesh* generaAnilloCuadradoIndexado();
	static IndexMesh* generaCuboConTapasIndexado(GLdouble l);
	static IndexMesh* generaGrid(GLdouble lado, GLuint nDiv);

	IndexMesh() { mPrimitive = GL_TRIANGLES; }
	virtual ~IndexMesh() { delete[] vIndices; }
	virtual void render() const;

protected:
	GLuint* vIndices = nullptr; // tabla de índices
	GLuint nNumIndices = 0;

	virtual void draw() const;
	void buildNormalVectors();
};
//-------------------------------------------------------------------------

class MbR : public IndexMesh {
public:
	static MbR* generaIndexMeshByRevolution(int mm, int nn, glm::dvec3* perfil);

	MbR(int m, int n, glm::dvec3* perfil);
	virtual ~MbR() { delete[] perfil; }

protected:
	int n;
	int m;
	glm::dvec3* perfil;
};
//-------------------------------------------------------------------------

#endif //_H_IndexMesh_H_