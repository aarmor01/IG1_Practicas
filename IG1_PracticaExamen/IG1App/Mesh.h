////#pragma once
#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include <vector>

//-------------------------------------------------------------------------

class Mesh 
{
public:

	static Mesh* createRGBAxes(GLdouble l);											   // creates a new 3D-RGB axes mesh
	static Mesh* generaPoligono(GLuint numL, GLdouble rd);							   // creates a polygon of 'x' vertices
	static Mesh* generaSierpinski(GLuint numP, GLdouble rd);						   // creates a Sierpinski polygon
	static Mesh* generaTrianguloRGB(GLdouble rd);									   // creates an RBG triangle
	static Mesh* generaRectangulo(GLdouble w, GLdouble h);							   // creates a rectangle
	static Mesh* generaRectanguloRGB(GLdouble w, GLdouble h);						   // creates an RBG rectangle
	static Mesh* generaEstrella3D(GLdouble re, GLuint np, GLdouble h);				   // creates a 3d star
	static Mesh* generaContCubo(GLdouble ld);										   // creates a 3d cube
	static Mesh* generaRectanguloTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh); // creates a rectangle with texture vertices
	static Mesh* generaEstrellaTexCor(GLdouble re, GLuint np, GLdouble h);			   // creates a 3d star with texture
	static Mesh* generaContCuboTexCor(GLdouble nl);									   // creates a cube with texture
	static Mesh* generaHexagonoTexCor(GLdouble radio);								   // creates a cube with texture

	Mesh() {};
	virtual ~Mesh() {};

	Mesh(const Mesh & m) = delete;				// no copy constructor
	Mesh & operator=(const Mesh & m) = delete;  // no copy assignment
			
	virtual void render() const;
	
	GLuint size() const { return mNumVertices; };   // number of elements
	std::vector<glm::dvec3> const& vertices() const { return vVertices; };
	std::vector<glm::dvec4> const& colors() const { return vColors; };
		
protected:
	
	GLuint mPrimitive = GL_TRIANGLES;   // graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0;			// number of elements ( = vVertices.size())
	std::vector<glm::dvec3> vVertices;  // vertex array
	std::vector<glm::dvec4> vColors;    // color array
	std::vector<glm::dvec2> vTexCoords; // texture vertices
	std::vector<glm::dvec3> vNormals;   // tabla de normales

	virtual void draw() const;
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_