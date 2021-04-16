#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
using namespace std;
using namespace glm;

//-------------------------------------------------------------------------

void Mesh::draw() const
{
	glDrawArrays(mPrimitive, 0, size()); // primitive graphic, first index and number of elements to be rendered
}
//-------------------------------------------------------------------------

void Mesh::render() const
{
	if (vVertices.size() > 0) { // transfer data
								// transfer the coordinates of the vertices

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());   // number of coordinates per vertex, type of each coordinate, stride, pointer 
		if (vColors.size() > 0) {    // transfer colors
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_DOUBLE, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
		}
		if (vTexCoords.size() > 0) { // transfer texture
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
		}

		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}
//-------------------------------------------------------------------------

Mesh* Mesh::createRGBAxes(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaPoligono(GLuint numL, GLdouble rd) {
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINE_LOOP;

	mesh->mNumVertices = numL;
	mesh->vVertices.reserve(mesh->mNumVertices);
	GLdouble angle = 90; //angulo

	for (GLuint counter = 0; counter != numL; counter++) {
		mesh->vVertices.emplace_back(rd * cos(radians(angle)), rd * sin(radians(angle)), 0.0);
		angle += 360.0 / numL; // incrementamos el angulo del siguiente vertice
	}

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaSierpinski(GLuint numP, GLdouble rd) {
	Mesh* triangle = generaPoligono(3, rd);
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_POINTS;

	mesh->mNumVertices = numP;
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vVertices.emplace_back(triangle->vVertices[rand() % 3]);
	glm::dvec3 pAux;

	for (GLuint counter = 1; counter != numP; counter++) {
		pAux = triangle->vVertices[rand() % 3];
		mesh->vVertices.emplace_back((mesh->vVertices.back() + pAux) / 2.0);
	}

	delete triangle; triangle = nullptr;

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaTrianguloRGB(GLdouble rd) {
	Mesh* mesh = generaPoligono(3, rd);

	mesh->mPrimitive = GL_TRIANGLES;

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaRectangulo(GLdouble w, GLdouble h) {
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;

	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(-w / 2.0, h / 2.0, 0.0);  // v0 arriba izquierda
	mesh->vVertices.emplace_back(-w / 2.0, -h / 2.0, 0.0); // v1 abajo izquierda
	mesh->vVertices.emplace_back(w / 2.0, h / 2.0, 0.0);   // v2 arriba derecha
	mesh->vVertices.emplace_back(w / 2.0, -h / 2.0, 0.0);  // v3 abajo derecha

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaRectanguloRGB(GLdouble w, GLdouble h) {
	Mesh* mesh = generaRectangulo(w, h);

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(1.0, 0.0, 0.75, 1.0); // rosa
	mesh->vColors.emplace_back(0.5, 1.0, 0.25, 1.0); // azul claro
	mesh->vColors.emplace_back(0.25, 0.5, 1.0, 1.0); // verde muy claro
	mesh->vColors.emplace_back(1.0, 0.8, 0.0, 1.0);  // dorado

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaEstrella3D(GLdouble re, GLuint np, GLdouble h) {
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_FAN;

	mesh->mNumVertices = 2 * np + 2;
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);

	GLdouble angle = 90.0;

	for (int i = 0; i < mesh->mNumVertices - 1; i++) {
		if (i % 2 == 0)
			mesh->vVertices.emplace_back(re * cos(radians(angle)), re * sin(radians(angle)), h);
		else
			mesh->vVertices.emplace_back(re / 2 * cos(radians(angle)), re / 2 * sin(radians(angle)), h);

		angle += 360.0 / (np * 2.0);
	}

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaContCubo(GLdouble ld) {
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;

	mesh->mNumVertices = 10;
	mesh->vVertices.reserve(mesh->mNumVertices);

	GLdouble x, y, z;

	for (int i = 0; i < mesh->mNumVertices; i++) {
		if (i > 1 && i < 6) x = ld / 2;
		else x = -ld / 2;

		if (i % 2 == 0) y = ld / 2;
		else y = -ld / 2;

		if (i > 3 && i < 8) z = -ld / 2;
		else z = ld / 2;

		mesh->vVertices.emplace_back(x, y, z);
	}

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaRectanguloTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh) {
	Mesh* mesh = generaRectangulo(w, h);

	mesh->vTexCoords.reserve(mesh->mNumVertices);
	mesh->vTexCoords.emplace_back(0.0, rh);  // v0
	mesh->vTexCoords.emplace_back(0.0, 0.0); // v1
	mesh->vTexCoords.emplace_back(rw, rh);   // v2
	mesh->vTexCoords.emplace_back(rw, 0.0);  // v3

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaEstrellaTexCor(GLdouble re, GLuint np, GLdouble h) {
	Mesh* mesh = generaEstrella3D(re, np, h);

	mesh->vTexCoords.reserve(mesh->mNumVertices);
	mesh->vTexCoords.emplace_back(0.5, 0.5);

	GLdouble angle = 90.0;

	for (int i = 0; i < mesh->mNumVertices - 1; i++) {
		mesh->vTexCoords.emplace_back(0.5 + 0.5 * cos(radians(angle)), 0.5 + 0.5 * sin(radians(angle)));
		angle += 360.0 / (np * 2.0);
	}

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaContCuboTexCor(GLdouble nl) {
	Mesh* mesh = generaContCubo(nl);

	mesh->vTexCoords.reserve(mesh->mNumVertices);

	for (int i = 0; i < mesh->mNumVertices; i++) {
		mesh->vTexCoords.emplace_back(GLdouble(i / 2), i % 2 == 0 ? 1.0 : 0.0);
	}

	return mesh;
}
Mesh* Mesh::generaHexagonoTexCor(GLdouble radio)
{
	Mesh* auxMesh = Mesh::generaPoligono(6, radio);
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(0, 0, 0);

	for (int i = 5; i >= 0; i--) mesh->vVertices.emplace_back(auxMesh->vVertices[i]);

	mesh->vVertices.emplace_back(auxMesh->vVertices[5]);

	mesh->vTexCoords.reserve(mesh->mNumVertices);
	mesh->vTexCoords.emplace_back(0.5, 0.5);

	GLdouble angle = 90.0;

	for (int i = 0; i < mesh->mNumVertices - 1; i++) {
		mesh->vTexCoords.emplace_back(0.5 + 0.5 * cos(radians(angle)), 0.5 + 0.5 * sin(radians(angle)));
		angle += 360.0 / 6;
	}

	return mesh;
}
//-------------------------------------------------------------------------