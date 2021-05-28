#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;

bool Scene::lightsAreOn = false;
//-------------------------------------------------------------------------

const pair<std::string, int> Scene::bmps[NUM_TEXTURES] = {
		{"..\\Bmps\\baldosaP.bmp", 255}, {"..\\Bmps\\container.bmp", 255 }, {"..\\Bmps\\papelC.bmp", 255},
		{"..\\Bmps\\baldosaC.bmp", 255}, {"..\\Bmps\\windowV.bmp", 150}, {"..\\Bmps\\noche.bmp", 150},
		{"..\\Bmps\\checker.bmp", 255}, {"..\\Bmps\\stones.bmp", 255}
};
//-------------------------------------------------------------------------

void Scene::init(int mId)
{
	this->mId = mId;
	setGL();  // OpenGL settings

	// Lights
	if (!lightsAreOn)
		createLights();

	// Textures
	loadTexture();

	// Graphics objects (entities) of the scene
	if (mId != 6) gObjectsOpaque.push_back(new EjesRGB(400.0));

	if (this->mId == 1)
		tieFighter();
	else if (this->mId == 2)
		anilloCuadrado();
	else if (this->mId == 3)
		cuboIndexado();
	else if (this->mId == 4)
		dosEsferas();
	else if (this->mId == 5)
		gridCube();
	else if (this->mId == 6)
		tiesEsfera();
}
//-------------------------------------------------------------------------

void Scene::free()
{ // release memory and resources   

	for (Abs_Entity* el : gObjectsOpaque) {
		delete el;  el = nullptr;
	}

	for (Abs_Entity* el : gObjectsTrans) {
		delete el;  el = nullptr;
	}

	for (Texture* el : gTextures) {
		delete el;  el = nullptr;
	}

	gObjectsOpaque.clear();
	gObjectsTrans.clear();
	gTextures.clear();
}
//-------------------------------------------------------------------------

void Scene::setGL()
{
	// OpenGL basic setting
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test 
	glEnable(GL_TEXTURE_2D);  // enable Texture 
	glEnable(GL_LIGHTING);
}
//-------------------------------------------------------------------------

void Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test 	
	glDisable(GL_TEXTURE_2D);  // disable Texture 	
	glDisable(GL_LIGHTING);
}
//-------------------------------------------------------------------------

void Scene::update() {
	for (Abs_Entity* obj : gObjectsOpaque) {
		obj->update();
	}

	for (Abs_Entity* obj : gObjectsTrans) {
		obj->update();
	}
}
//-------------------------------------------------------------------------

void Scene::render(Camera const& cam) const
{
	cam.upload();
	dirLight->upload(cam.viewMat());
	posLight->upload(cam.viewMat());
	spotLight->upload(cam.viewMat());


	for (Abs_Entity* el : gObjectsOpaque) {
		el->render(cam.viewMat());
	}

	for (Abs_Entity* el : gObjectsTrans) {
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		el->render(cam.viewMat());

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}
}
//-------------------------------------------------------------------------

void Scene::changeScene(int const id) {
	if (id != mId) {
		free();
		resetGL();
		init(id);
	}

}
//-------------------------------------------------------------------------

void Scene::loadTexture()
{
	for (std::pair<std::string, int> s : bmps) {
		Texture* t = new Texture();
		t->load(s.first, s.second);
		gTextures.push_back(t);
	}

	//cargo la textura de la planta
	Texture* t = new Texture();
	t->load("..\\Bmps\\grass.bmp", u8vec3(0, 0, 0));
	gTextures.push_back(t);
}
//-------------------------------------------------------------------------

// Practica 1 Escena 2D
void Scene::primeraEscena2D()
{
	gObjectsOpaque.push_back(new Poligono(3, 260)); //triangulo de tres lineas solo, sin relleno
	gObjectsOpaque.back()->setColor(glm::dvec4(1.0, 1.0, 0.0, 1.0));

	gObjectsOpaque.push_back(new Poligono(260, 260)); //circulo
	gObjectsOpaque.back()->setColor(glm::dvec4(1.0, 0.07, 0.57, 1.0));

	gObjectsOpaque.push_back(new Sierpinski(10000, 260)); //sierpinski
	gObjectsOpaque.back()->setColor(dvec4(1.0, 1.0, 0.0, 1.0));

	gObjectsOpaque.push_back(new TrianguloRGB(30)); //triangulo RGB

	gObjectsOpaque.push_back(new RectanguloRGB(900, 600)); //rectangulo RGB
	gObjectsOpaque.back()->setModelMat(translate(dmat4(1), dvec3(0.0, 0.0, -100.0)));
}
//-------------------------------------------------------------------------

// Practica 1 Escena 3D
void Scene::primeraEscena3D()
{
	/* Objetos opacos */
	// Estrella
	Estrella3D* estrella = new Estrella3D(30, 8, 55);
	gObjectsOpaque.push_back(estrella);
	dvec3 position = dvec3(-125.0, 150.0, -125.0);
	estrella->setModelMat(translate(dmat4(1), position));
	estrella->setPosition(position);
	estrella->setTexture(gTextures[0]);

	// Caja
	CajaConFondo* c = new CajaConFondo(80);
	position = dvec3(-125.0, 40.0, -125.0);
	c->setPosition(position);
	c->translateAll();
	c->setTexture(gTextures[1], gTextures[2]);
	gObjectsOpaque.push_back(c);

	// Suelo
	gObjectsOpaque.push_back(new Suelo(500, 500, 15, 15));
	gObjectsOpaque.back()->setModelMat(rotate(gObjectsOpaque.back()->modelMat(), radians(90.0), dvec3(1.0, 0.0, 0.0)));
	gObjectsOpaque.back()->setColor(dvec4(0.75, 1.0, 0.75, 1.0));
	gObjectsOpaque.back()->setTexture(gTextures[3]);

	// Foto
	Texture* t = new Texture();
	gTextures.push_back(t);
	gObjectsOpaque.push_back(new Foto(4 * 500 / 15, 3 * 500 / 15));
	gObjectsOpaque.back()->setTexture(t);
	gObjectsOpaque.back()->setModelMat(translate(dmat4(1), dvec3(0.0, 1.0, 0.0)));
	gObjectsOpaque.back()->setModelMat(rotate(gObjectsOpaque.back()->modelMat(), radians(-90.0), dvec3(1.0, 0.0, 0.0)));

	// Planta
	Planta* planta = new Planta(150, 150);
	gObjectsOpaque.push_back(planta);
	planta->setTexture(gTextures[6]);
	planta->setModelMat(translate(planta->modelMat(), dvec3(150, 75, -150)));

	/* Objetos con transparencia */
	// Cristalera translucida
	Caja* caja = new Caja(500);
	gObjectsTrans.push_back(caja);
	caja->setTexture(gTextures[4], gTextures[4]);
	caja->setModelMat(translate(caja->modelMat(), dvec3(0, 250, 0)));
}
//-------------------------------------------------------------------------

// Practica 2 tie fighter
void Scene::tieFighter()
{
	TIE* tie = new TIE(gTextures);
	gObjectsOpaque.push_back(tie);
}
//-------------------------------------------------------------------------

// Practica 2 anillo cuadrado
void Scene::anilloCuadrado()
{
	AnilloCuadrado* anillo = new AnilloCuadrado();
	gObjectsOpaque.push_back(anillo);
}
//-------------------------------------------------------------------------

// Practica 2 cubo indexado
void Scene::cuboIndexado()
{
	CuboIndexado* cubo = new CuboIndexado(150);
	gObjectsOpaque.push_back(cubo);
}
//-------------------------------------------------------------------------

// Practica 2 esferas revoluciones
void Scene::dosEsferas()
{
	/*Cono* cono = new Cono(300, 100, 50);
	gObjectsOpaque.push_back(cono);*/

	Sphere* sphere = new Sphere(150);
	glm::dmat4 mAux = sphere->modelMat();
	mAux = translate(mAux, dvec3(200, 0, 0));
	sphere->setModelMat(mAux);

	Esfera* esfera = new Esfera(150, 50, 50);
	mAux = esfera->modelMat();
	mAux = translate(mAux, dvec3(-200, 0, 0));
	esfera->setModelMat(mAux);
	esfera->setColor(dvec4(0.431372f, 0.86274f, 0.8588, 1.0f));
	
	gObjectsOpaque.push_back(esfera);
	gObjectsOpaque.push_back(sphere);
}
//-------------------------------------------------------------------------

// Practica 2 cubo por grids
void Scene::gridCube()
{
	/*Grid* grid = new Grid(400, 10);
	grid->setTexture(gTextures[0]);
	gObjectsOpaque.push_back(grid);*/

	GridCube* gridCube = new GridCube(500, 150, gTextures);
	gObjectsOpaque.push_back(gridCube);
}
//-------------------------------------------------------------------------

// Practica 2 escena con ties y esfera
void Scene::tiesEsfera()
{
	GLdouble radioEsfera = 300;
	gObjectsOpaque.push_back(new EjesRGB(radioEsfera * 2));
	glm::dmat4 modelMat;
	Esfera* esfera = new Esfera(radioEsfera, 200, 400);
	esfera->setColor(dvec4(0.431372f, 0.86274f, 0.8588, 1.0f));
	Material* matl = new Material();
	matl->setCopper();
	esfera->setMaterial(matl);
	modelMat = esfera->modelMat();
	esfera->setModelMat(modelMat);
	gObjectsOpaque.push_back(esfera);

	tieGroup = new CompoundEntity();

	TIE* tie = new TIE(gTextures, radioEsfera / 8.0, true);
	modelMat = tie->modelMat();
	modelMat = translate(modelMat, dvec3(-radioEsfera / 5, -radioEsfera / 20, 0));
	modelMat = rotate(modelMat, radians(5.0), dvec3(1.0, 0.0, 1.0));
	tie->setModelMat(modelMat);
	tieGroup->addEntity(tie);

	tie = new TIE(gTextures, radioEsfera / 8.0, true);
	modelMat = tie->modelMat();
	modelMat = translate(modelMat, dvec3(0, 0, -radioEsfera / 5));
	modelMat = rotate(modelMat, radians(15.0), dvec3(1.0, 1.0, 0.0));
	tie->setModelMat(modelMat);
	tieGroup->addEntity(tie);

	tie = new TIE(gTextures, radioEsfera / 8.0, true);
	modelMat = tie->modelMat();
	modelMat = translate(modelMat, dvec3(radioEsfera / 5, -radioEsfera / 20, 0));
	modelMat = rotate(modelMat, radians(-7.0), dvec3(1.0, 0.0, 1.0));
	tie->setModelMat(modelMat);
	tieGroup->addEntity(tie);

	modelMat = tieGroup->modelMat();
	tieGroup->setModelMat(translate(modelMat, dvec3(0, radioEsfera * 1.2, 0)));
	gObjectsOpaque.push_back(tieGroup);
}
//-------------------------------------------------------------------------

void Scene::createLights()
{
	dirLight = new DirLight();
	dirLight->setDiff({ 1, 1, 1, 1 });
	dirLight->setAmb({ 0, 0, 0, 1 });
	dirLight->setSpec({ 0.5, 0.5, 0.5, 1 });
	dirLight->setPosDir({ 1, 1, 1 });

	posLight = new PosLight();
	posLight->setDiff({ 1, 1, 0, 1 });
	posLight->setAmb({ 0.2, 0.2, 0, 1 });
	posLight->setSpec({ 0.5, 0.5, 0.5, 1 });
	posLight->setPosDir({ 600, 200, 0 });

	spotLight = new SpotLight();
	spotLight->setDiff({ 1, 1, 1, 1 });
	spotLight->setAmb({ 0, 0, 0, 1 });
	spotLight->setSpec({ 0.5, 0.5, 0.5, 1 });
	spotLight->setPosDir({ 0, 0, 400 });
	spotLight->setSpot(glm::fvec3(0.0, 0.0, -1.0), 55, 0);

	lightsAreOn = true;
}
//-------------------------------------------------------------------------

void Scene::disableAllLights()
{
	dirLight->disable();
	posLight->disable();
	spotLight->disable();
}
//-------------------------------------------------------------------------

void Scene::clearLights()
{
	if (dirLight != nullptr) { delete dirLight; dirLight = nullptr; }
	if (spotLight != nullptr) { delete spotLight; spotLight = nullptr; }
	if (posLight != nullptr) { delete posLight; posLight = nullptr; }

	lightsAreOn = false;
}
//-------------------------------------------------------------------------

void Scene::darkScene()
{
	disableAllLights();

	GLfloat amb[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
}
//-------------------------------------------------------------------------

void Scene::defaultLighting()
{
	disableAllLights();

	GLfloat amb[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
}
//-------------------------------------------------------------------------

void Scene::TIEsLightsOn()
{
	for (Abs_Entity* tie : tieGroup->gObjects)
		static_cast<TIE*>(tie)->getSpotLight()->enable();
}
//-------------------------------------------------------------------------

void Scene::TIEsLightsOff()
{
	for (Abs_Entity* tie : tieGroup->gObjects)
		static_cast<TIE*>(tie)->getSpotLight()->disable();
}
//-------------------------------------------------------------------------

void Scene::orbita()
{
	dmat4 mat = tieGroup->modelMat();
	mat = translate(mat, dvec3(0.0, -370.0, 0.0));
	mat = rotate(mat, radians(1.0), dvec3(1, 0.0, 0));
	mat = translate(mat, dvec3(0.0, 370.0, 0.0));

	tieGroup->setModelMat(mat);
}
//-------------------------------------------------------------------------

void Scene::rota()
{
	dmat4 mat = tieGroup->modelMat();
	mat = rotate(mat, radians(tieLocalAngle), dvec3(0.0, 1.0, 0.0));

	tieLocalAngle += 0.1;
	tieGroup->setModelMat(mat);
}
//-------------------------------------------------------------------------