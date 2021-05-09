#include "Entity.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_access.hpp>
#include "../IG1App/Scene.h"
#include "IG1App.h"

//-------------------------------------------------------------------------

void Abs_Entity::upload(dmat4 const& modelViewMat) const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));  // transfers modelView matrix to the GPU
}
//-------------------------------------------------------------------------

EjesRGB::EjesRGB(GLdouble l) : Abs_Entity()
{
	mMesh = Mesh::createRGBAxes(l);
}
//-------------------------------------------------------------------------

void EjesRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glEnable(GL_COLOR_MATERIAL);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
		glDisable(GL_COLOR_MATERIAL);
	}
}
//-------------------------------------------------------------------------

Poligono::Poligono(GLuint numL, GLdouble rd) : Abs_Entity()
{
	mMesh = Mesh::generaPoligono(numL, rd);
}
//-------------------------------------------------------------------------

void Poligono::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glLineWidth(2);
		glColor3d(mColor.r, mColor.g, mColor.b);
		mMesh->render();

		glColor3d(1.0, 1.0, 1.0);
		glLineWidth(1);
	}
}
//-------------------------------------------------------------------------

Sierpinski::Sierpinski(GLuint numP, GLdouble rd) : Abs_Entity()
{
	mMesh = Mesh::generaSierpinski(numP, rd);
}
//-------------------------------------------------------------------------

void Sierpinski::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPointSize(2);
		glColor4dv(value_ptr(mColor)); //le pasa el puntero al atributo para no poner [r, g, b]
		mMesh->render();
		glColor4dv(value_ptr(dvec4(1)));
		glPointSize(1);
	}
}
//-------------------------------------------------------------------------

TrianguloRGB::TrianguloRGB(GLdouble rd) : Abs_Entity()
{
	mMesh = Mesh::generaTrianguloRGB(rd);
}
//-------------------------------------------------------------------------

void TrianguloRGB::update() {
	localAngle += 2;
	globalAngle++;

	setModelMat(translate(dmat4(1.0), dvec3(rad * cos(radians(globalAngle)), rad * sin(radians(globalAngle)), 0.0)));
	setModelMat(rotate(mModelMat, -radians(localAngle), dvec3(0.0, 0.0, 1.0)));
}
//-------------------------------------------------------------------------

void TrianguloRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_BACK, GL_LINE);
		glPolygonMode(GL_FRONT, GL_FILL);
		mMesh->render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
//-------------------------------------------------------------------------

RectanguloRGB::RectanguloRGB(GLdouble w, GLdouble h) : Abs_Entity()
{
	mMesh = Mesh::generaRectanguloRGB(w, h);
}
//-------------------------------------------------------------------------

void RectanguloRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_BACK, GL_LINE);
		glPolygonMode(GL_FRONT, GL_FILL);
		mMesh->render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
//-------------------------------------------------------------------------

Estrella3D::Estrella3D(GLdouble re, GLuint np, GLdouble h) : Abs_Entity()
{
	mMesh = Mesh::generaEstrellaTexCor(re, np, h);
}
//-------------------------------------------------------------------------

void Estrella3D::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		mTexture->bind(GL_REPLACE);
		mMesh->render();

		aMat = rotate(aMat, radians(180.0), dvec3(0.0, 1.0, 0.0)); // rotamos la matriz
		upload(aMat);
		mMesh->render();

		mTexture->unbind();
	}
}
//-------------------------------------------------------------------------

void Estrella3D::update() {
	zAngle++;
	yAngle++;

	//primero giramos en la 'y' y luego en la 'z' para obtener la rotacion deseada
	setModelMat(translate(dmat4(1), position));
	setModelMat(rotate(mModelMat, radians(yAngle), dvec3(0.0, 1.0, 0.0)));
	setModelMat(rotate(mModelMat, -radians(zAngle), dvec3(0.0, 0.0, 1.0)));
}
void Estrella3D::setPosition(const dvec3& position)
{
	this->position = position;
}
//-------------------------------------------------------------------------

Caja::Caja(GLdouble ld) : Abs_Entity()
{
	mMesh = Mesh::generaContCuboTexCor(ld);
}
//-------------------------------------------------------------------------

void Caja::setTexture(Texture* const& t, Texture* const& t2)
{
	Abs_Entity::setTexture(t);
	mText2 = t2;
}
//-------------------------------------------------------------------------

void Caja::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glEnable(GL_CULL_FACE);

		//Renderizamos primero las caras traseras para
		//evitar problemas con el blending, es decir, tenemos
		//que renderizar los objetos transparentes de más lejanos a más cercanos.
		glCullFace(GL_FRONT);
		mText2->bind(GL_REPLACE);
		mMesh->render();
		mText2->unbind();

		glCullFace(GL_BACK);
		mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();

		glDisable(GL_CULL_FACE);
	}
}
//-------------------------------------------------------------------------

CajaConFondo::CajaConFondo(GLdouble ld) : Caja(ld), ld_(ld), angle(0), position(1), modelMatFloor(1)
{
	meshFloor = Mesh::generaRectanguloTexCor(ld, ld, 1, 1);
}
//-------------------------------------------------------------------------

CajaConFondo::~CajaConFondo()
{
	delete meshFloor;
}
//-------------------------------------------------------------------------

void CajaConFondo::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr && meshFloor != nullptr) {

		glEnable(GL_CULL_FACE);
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication

		//Renderizamos primero las caras traseras para
		//evitar problemas con el blending, es decir, tenemos
		//que renderizar los objetos transparentes de más lejanos a más cercanos.
		upload(aMat);
		glCullFace(GL_FRONT);
		mText2->bind(GL_REPLACE);
		mMesh->render();
		upload(modelViewMat * modelMatFloor);
		meshFloor->render();
		mText2->unbind();

		upload(aMat);
		glCullFace(GL_BACK);
		mTexture->bind(GL_REPLACE);
		mMesh->render();
		upload(modelViewMat * modelMatFloor);
		meshFloor->render();
		mTexture->unbind();

		glDisable(GL_CULL_FACE);
	}
}
//-------------------------------------------------------------------------

void CajaConFondo::update()
{
	this->translateAll();
	angle++;
	angle = std::fmod(angle, 360.0);
}
//-------------------------------------------------------------------------

void CajaConFondo::setPosition(const dvec3& position)
{
	this->position = position;
}
//-------------------------------------------------------------------------

void CajaConFondo::translateAll()
{
	setModelMat(translate(dmat4(1), this->position));
	setModelMat(rotate(mModelMat, radians(-angle), dvec3(0, 0, 1)));
	modelMatFloor = translate(mModelMat, dvec3(0, -ld_ / 2, 0));
	modelMatFloor = rotate(modelMatFloor, radians(90.0), dvec3(1, 0, 0));
}
//-------------------------------------------------------------------------

Suelo::Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh) : Abs_Entity()
{
	mMesh = Mesh::generaRectanguloTexCor(w, h, rw, rh);
}
//-------------------------------------------------------------------------

void Suelo::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		mTexture->setWrap(GL_REPEAT);
		//mTexture->bind(GL_REPLACE);
		mTexture->bind(GL_MODULATE);
		glColor4dv(value_ptr(mColor));
		mMesh->render();
		glColor4dv(value_ptr(dvec4(1)));
		mTexture->unbind();
	}
}
//-------------------------------------------------------------------------

Foto::Foto(GLdouble w, GLdouble h) : Abs_Entity()
{
	mMesh = Mesh::generaRectanguloTexCor(w, h, 1, 1);
}
//-------------------------------------------------------------------------

void Foto::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		mTexture->setWrap(GL_REPEAT);
		mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
	}
}
//-------------------------------------------------------------------------

void Foto::update() {
	mTexture->loadColorBuffer(IG1App::s_ig1app.winWidth(), IG1App::s_ig1app.winHeight(), GL_FRONT);
}
//-------------------------------------------------------------------------

Planta::Planta(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generaRectanguloTexCor(w, h, 1, 1);
}
//-------------------------------------------------------------------------

void Planta::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0);

		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		mTexture->setWrap(GL_REPEAT);
		mTexture->bind(GL_REPLACE);
		mMesh->render();

		aMat = rotate(aMat, radians(60.0), dvec3(0.0, 1.0, 0.0)); // rotamos la matriz
		upload(aMat);
		mMesh->render();

		aMat = rotate(aMat, radians(60.0), dvec3(0.0, 1.0, 0.0)); // rotamos la matriz
		upload(aMat);
		mMesh->render();

		mTexture->unbind();

		glDisable(GL_ALPHA_TEST);
	}
}
//-------------------------------------------------------------------------

void Sphere::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// Aquí se puede fijar el color de la esfera así:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(mColor.r, mColor.g, mColor.b);
	// Aquí se puede fijar el modo de dibujar la esfera:
	gluQuadricDrawStyle(q, GLU_FILL);
	gluSphere(q, radius, 50, 50);
	// Aquí se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
}
//------------------------------------------------------------------------

Cylinder::Cylinder(GLdouble baseRadius, GLdouble topRadius, GLdouble height)
{
	this->baseRadius = baseRadius;
	this->topRadius = topRadius;
	this->height = height;
}
//------------------------------------------------------------------------

void Cylinder::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// Aquí se puede fijar el color de la esfera así:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(mColor.r, mColor.g, mColor.b);
	// Aquí se puede fijar el modo de dibujar la esfera:
	gluQuadricDrawStyle(q, GLU_FILL);
	gluCylinder(q, baseRadius, topRadius, height, 50, 50);
	// Aquí se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
}
//------------------------------------------------------------------------

Disk::Disk(GLdouble innerRadius, GLdouble outerRadius)
{
	this->innerRadius = innerRadius;
	this->outerRadius = outerRadius;
}
//------------------------------------------------------------------------

void Disk::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// Aquí se puede fijar el color de la esfera así:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(mColor.r, mColor.g, mColor.b);
	// Aquí se puede fijar el modo de dibujar la esfera:
	gluQuadricDrawStyle(q, GLU_FILL);
	gluDisk(q, innerRadius, outerRadius, 50, 50);
	// Aquí se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
}
//------------------------------------------------------------------------

PartialDisk::PartialDisk(GLdouble innerRadius, GLdouble outerRadius, GLdouble startAngle, GLdouble sweepAngle)
{
	this->innerRadius = innerRadius;
	this->outerRadius = outerRadius;
	this->startAngle = startAngle;
	this->sweepAngle = sweepAngle;
}
//------------------------------------------------------------------------

void PartialDisk::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// Aquí se puede fijar el color de la esfera así:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(mColor.r, mColor.g, mColor.b);
	// Aquí se puede fijar el modo de dibujar la esfera:
	gluQuadricDrawStyle(q, GLU_FILL);
	gluPartialDisk(q, innerRadius, outerRadius, 50, 50, startAngle, sweepAngle);
	// Aquí se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
}
//------------------------------------------------------------------------

Hexagono::Hexagono(GLdouble radio)
{
	mMesh = Mesh::generaHexagonoTexCor(radio);
}
//------------------------------------------------------------------------

void Hexagono::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
	}
}
//------------------------------------------------------------------------

AnilloCuadrado::AnilloCuadrado()
{
	mMesh = IndexMesh::generaAnilloCuadradoIndexado();
}
//------------------------------------------------------------------------

void AnilloCuadrado::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glEnable(GL_COLOR_MATERIAL);

		mMesh->render();

		glDisable(GL_COLOR_MATERIAL);
	}
}
//------------------------------------------------------------------------

CuboIndexado::CuboIndexado(GLdouble l)
{
	mMesh = IndexMesh::generaCuboConTapasIndexado(l);
}

void CuboIndexado::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glEnable(GL_COLOR_MATERIAL);
		glColor4d(0.0, 1.0, 0.0, 1.0);
		mMesh->render();
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glDisable(GL_COLOR_MATERIAL);
	}
}

CompoundEntity::~CompoundEntity()
{
	for (Abs_Entity* el : gObjects) {
		delete el;  el = nullptr;
	}

	gObjects.clear();
}

void CompoundEntity::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
	upload(aMat);
	for (Abs_Entity* e : gObjects) e->render(aMat);
}

TIE::TIE(std::vector<Texture*> gTextures)
{
	Hexagono* wingL = new Hexagono(300);
	wingL->setTexture(gTextures[5]);
	wingL->setColor(dvec4(0, 0.254, 0.415, 0));
	glm::dmat4 mAux = wingL->modelMat();
	mAux = rotate(mAux, radians(-90.0), dvec3(0.0, 1.0, 0));
	mAux = translate(mAux, dvec3(0, 0, 150));
	wingL->setModelMat(mAux);
	this->gObjectsTrans.push_back(wingL);

	Hexagono* wingR = new Hexagono(300);
	wingR = new Hexagono(300);
	wingR->setTexture(gTextures[5]);
	wingR->setColor(dvec4(0, 0.254, 0.415, 0));
	mAux = wingR->modelMat();
	mAux = rotate(mAux, radians(-90.0), dvec3(0.0, 1.0, 0));
	mAux = translate(mAux, dvec3(0, 0, -150));
	wingR->setModelMat(mAux);
	gObjectsTrans.push_back(wingR);

	Sphere* core = new Sphere(100.0);
	core->setColor(dvec4(0, 0.254, 0.415, 0));
	gObjects.push_back(core);

	Cylinder* shaft = new Cylinder(20.0, 20.0, 300.0);
	shaft->setColor(dvec4(0, 0.254, 0.415, 0));
	mAux = shaft->modelMat();
	mAux = translate(mAux, dvec3(150, 0, 0));
	mAux = rotate(mAux, radians(-90.0), dvec3(0.0, 1.0, 0));
	shaft->setModelMat(mAux);
	gObjects.push_back(shaft);

	CompoundEntity* front = new CompoundEntity();

	Cylinder* cono = new Cylinder(50.0, 50.0, 200.0);
	cono->setColor(dvec4(0, 0.254, 0.415, 0));
	mAux = cono->modelMat();
	mAux = translate(mAux, dvec3(0, 0, -100));
	cono->setModelMat(mAux);
	front->gObjects.push_back(cono);

	//Disco tapar cilindro
	Disk* disk = new Disk(0, 50);
	disk->setColor(dvec4(0, 0.254, 0.415, 0));
	mAux = disk->modelMat();
	mAux = translate(mAux, dvec3(0, 0, 100));
	disk->setModelMat(mAux);
	front->gObjects.push_back(disk);

	disk = new Disk(0, 50);
	disk->setColor(dvec4(0, 0.254, 0.415, 0));
	mAux = disk->modelMat();
	mAux = translate(mAux, dvec3(0, 0, -100));
	disk->setModelMat(mAux);
	front->gObjects.push_back(disk);

	gObjects.push_back(front);
}

void TIE::render(glm::dmat4 const& modelViewMat) const
{
	CompoundEntity::render(modelViewMat);

	for (Abs_Entity* el : gObjectsTrans) {
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		el->render(modelViewMat);

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}
}

Cono::Cono(GLdouble height, GLdouble radius, GLuint n)
{
	// h=altura del cono, r=radio de la base
	// n=número de muestras, m=número de puntos del perfil
	int m = 3;
	dvec3* perfil = new dvec3[m];
	perfil[0] = dvec3(0.5, 0.0, 0.0);
	perfil[1] = dvec3(radius, 0.0, 0.0);
	perfil[2] = dvec3(0.5, height, 0.0);
	this->mMesh = new MbR(m, n, perfil);
	mMesh = MbR::generaIndexMeshByRevolution(m,n,perfil);
}

void Cono::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		mMesh->render();
	}
}

Esfera::Esfera(GLdouble radius, GLuint puntosPerfil, GLdouble rev)
{
	dvec3* perfil = new dvec3[puntosPerfil];

	//////////////////

	GLfloat angle = 270; //angulo
	for (GLuint counter = 0; counter < puntosPerfil; counter++) {
		perfil[counter] = dvec3(radius * cos(radians(angle)), radius * sin(radians(angle)), 0.0);
		angle += 180.0 / (puntosPerfil-1); // incrementamos el angulo del siguiente vertice
	}

	/////////////

	this->mMesh = MbR::generaIndexMeshByRevolution(puntosPerfil, rev, perfil);
}

void Esfera::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		mMesh->render();
	}
}
