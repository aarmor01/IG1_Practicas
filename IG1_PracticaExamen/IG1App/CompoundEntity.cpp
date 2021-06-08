#include "CompoundEntity.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "QuadricEntity.h"

//------------------------------------------------------------------------

CompoundEntity::~CompoundEntity()
{
	for (Abs_Entity* el : gObjects) { delete el;  el = nullptr; }

	gObjects.clear();
}
//------------------------------------------------------------------------

void CompoundEntity::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
	upload(aMat);
	for (Abs_Entity* e : gObjects)
		e->render(aMat);
}
//------------------------------------------------------------------------

TIE::TIE(std::vector<Texture*> gTextures, GLfloat size, bool turnLight)
{
	if (turnLight) {
		light = new SpotLight();
		light->setDiff({ 1, 1, 1, 1 });
		light->setAmb({ 0, 0, 0, 1 });
		light->setSpec({ 0.5, 0.5, 0.5, 1 });
		light->setPosDir({ 0, 0, 0 });
		light->setSpot(glm::fvec3(0.0, -1.0, 0.0), 20, 80);
	}

	Hexagono* wingL = new Hexagono(size);
	wingL->setTexture(gTextures[5]);
	wingL->setColor(dvec4(0, 0.254, 0.415, 0));
	glm::dmat4 mAux = wingL->modelMat();
	mAux = rotate(mAux, radians(-90.0), dvec3(0.0, 1.0, 0));
	mAux = translate(mAux, dvec3(0, 0, size / 2.0));
	wingL->setModelMat(mAux);
	this->gObjectsTrans.push_back(wingL);

	Hexagono* wingR = new Hexagono(size);
	wingR->setTexture(gTextures[5]);
	wingR->setColor(dvec4(0, 0.254, 0.415, 0));
	mAux = wingR->modelMat();
	mAux = rotate(mAux, radians(-90.0), dvec3(0.0, 1.0, 0));
	mAux = translate(mAux, dvec3(0, 0, -size / 2.0));
	wingR->setModelMat(mAux);
	gObjectsTrans.push_back(wingR);

	Sphere* core = new Sphere(size / 3.0);
	core->setColor(dvec4(0, 0.254, 0.415, 0));
	gObjects.push_back(core);

	Cylinder* shaft = new Cylinder(size / 15.0, size / 15.0, size);
	shaft->setColor(dvec4(0, 0.254, 0.415, 0));
	mAux = shaft->modelMat();
	mAux = translate(mAux, dvec3(size / 2.0, 0, 0));
	mAux = rotate(mAux, radians(-90.0), dvec3(0.0, 1.0, 0));
	shaft->setModelMat(mAux);
	gObjects.push_back(shaft);

	CompoundEntity* front = new CompoundEntity();

	Cylinder* cono = new Cylinder(size / 6.0, size / 6.0, (2 * size) / 3.0);
	cono->setColor(dvec4(0, 0.254, 0.415, 0));
	mAux = cono->modelMat();
	mAux = translate(mAux, dvec3(0, 0, -size / 3.0));
	cono->setModelMat(mAux);
	front->gObjects.push_back(cono);

	//Disco tapar cilindro
	Disk* disk = new Disk(0, size / 6.0);
	disk->setColor(dvec4(0, 0.254, 0.415, 0));
	mAux = disk->modelMat();
	mAux = translate(mAux, dvec3(0, 0, size / 3.0));
	disk->setModelMat(mAux);
	front->gObjects.push_back(disk);

	disk = new Disk(0, size / 6.0);
	disk->setColor(dvec4(0, 0.254, 0.415, 0));
	mAux = disk->modelMat();
	mAux = translate(mAux, dvec3(0, 0, -size / 3.0));
	disk->setModelMat(mAux);
	front->gObjects.push_back(disk);

	gObjects.push_back(front);
}
//------------------------------------------------------------------------

TIE::~TIE()
{
	if (light != nullptr) { light->disable(); delete light; }
	for (Abs_Entity* el : gObjectsTrans) delete el;
}
//------------------------------------------------------------------------

void TIE::render(glm::dmat4 const& modelViewMat) const
{
	CompoundEntity::render(modelViewMat);
	dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
	if (light != nullptr) light->upload(aMat);
	upload(aMat);

	for (Abs_Entity* el : gObjectsTrans) {
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		el->render(aMat);

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}
}
//------------------------------------------------------------------------

GridCube::GridCube(GLdouble lado, GLuint numDivisiones, std::vector<Texture*> gTextures)
{
	dmat4 mat;
	//Laterales
	Grid* grid = new Grid(lado, numDivisiones);
	mat = grid->modelMat();
	mat = translate(mat, dvec3(lado / 2, 0, 0));
	mat = rotate(mat, radians(-90.0), dvec3(0.0, 0.0, 1.0));
	grid->setModelMat(mat);
	grid->setTexture(gTextures[7]);
	gObjects.push_back(grid);

	grid = new Grid(lado, numDivisiones);
	mat = grid->modelMat();
	mat = translate(mat, dvec3(-lado / 2, 0, 0));
	mat = rotate(mat, radians(180.0), dvec3(0.0, 1.0, 0.0));
	mat = rotate(mat, radians(-90.0), dvec3(0.0, 0.0, 1.0));
	grid->setModelMat(mat);
	grid->setTexture(gTextures[7]);
	gObjects.push_back(grid);

	//Frontal y trasera

	grid = new Grid(lado, numDivisiones);
	mat = grid->modelMat();
	mat = translate(mat, dvec3(0, 0, lado / 2));
	mat = rotate(mat, radians(180.0), dvec3(0.0, 1.0, 0.0));
	mat = rotate(mat, radians(-90.0), dvec3(0.0, 0.0, 1.0));
	mat = rotate(mat, radians(-90.0), dvec3(1.0, 0.0, 0.0));
	grid->setModelMat(mat);
	grid->setTexture(gTextures[7]);
	gObjects.push_back(grid);

	grid = new Grid(lado, numDivisiones);
	mat = grid->modelMat();
	mat = translate(mat, dvec3(0, 0, -lado / 2));
	mat = rotate(mat, radians(-90.0), dvec3(0.0, 0.0, 1.0));
	mat = rotate(mat, radians(-90.0), dvec3(1.0, 0.0, 0.0));
	grid->setModelMat(mat);
	grid->setTexture(gTextures[7]);
	gObjects.push_back(grid);

	//Superior e inferior

	grid = new Grid(lado, numDivisiones);
	mat = grid->modelMat();
	mat = translate(mat, dvec3(0, lado / 2, 0));
	grid->setModelMat(mat);
	grid->setTexture(gTextures[6]);
	gObjects.push_back(grid);

	grid = new Grid(lado, numDivisiones);
	mat = grid->modelMat();
	mat = translate(mat, dvec3(0, -lado / 2, 0));
	mat = rotate(mat, radians(180.0), dvec3(1.0, 0.0, 0));
	grid->setModelMat(mat);
	grid->setTexture(gTextures[6]);
	gObjects.push_back(grid);
}
//------------------------------------------------------------------------