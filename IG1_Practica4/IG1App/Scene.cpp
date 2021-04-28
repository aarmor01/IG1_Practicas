#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;

//-------------------------------------------------------------------------

const pair<std::string, int> Scene::bmps[NUM_TEXTURES] = {
		{"..\\Bmps\\baldosaP.bmp", 255}, {"..\\Bmps\\container.bmp", 255 }, {"..\\Bmps\\papelC.bmp", 255},
		{"..\\Bmps\\baldosaC.bmp", 255}, {"..\\Bmps\\windowV.bmp", 150}, {"..\\Bmps\\noche.bmp", 150}
};

void Scene::init(int mId)
{
	this->mId = mId;
	setGL();  // OpenGL settings
	
	// allocate memory and load resources
	// Lights
	// Textures
	loadTexture();

	// Graphics objects (entities) of the scene
	gObjectsOpaque.push_back(new EjesRGB(400.0));

//	if(this->mId == 0)
//	{
//		#pragma region case0
//		gObjectsOpaque.push_back(new Poligono(3, 260)); //triangulo de tres lineas solo, sin relleno
//		gObjectsOpaque.back()->setColor(glm::dvec4(1.0, 1.0, 0.0, 1.0));
//
//		gObjectsOpaque.push_back(new Poligono(260, 260)); //circulo
//		gObjectsOpaque.back()->setColor(glm::dvec4(1.0, 0.07, 0.57, 1.0));
//
//		gObjectsOpaque.push_back(new Sierpinski(10000, 260)); //sierpinski
//		gObjectsOpaque.back()->setColor(dvec4(1.0, 1.0, 0.0, 1.0));
//
//		gObjectsOpaque.push_back(new TrianguloRGB(30)); //triangulo RGB
//
//		gObjectsOpaque.push_back(new RectanguloRGB(900, 600)); //rectangulo RGB
//		gObjectsOpaque.back()->setModelMat(translate(dmat4(1), dvec3(0.0, 0.0, -100.0)));
//#pragma endregion
//	}
//	else if (this->mId == 1)
//	{
//		#pragma region case1 
//		/* Objetos opacos */
//		// Estrella
//		Estrella3D* estrella = new Estrella3D(30, 8, 55);
//		gObjectsOpaque.push_back(estrella); //estrella 3d
//		dvec3 position = dvec3(-125.0, 150.0, -125.0);
//		estrella->setModelMat(translate(dmat4(1), position));
//		estrella->setPosition(position);
//		estrella->setTexture(gTextures[0]);
//
//		// Caja
//		CajaConFondo* c = new CajaConFondo(80);
//		position = dvec3(-125.0, 40.0, -125.0);
//		c->setPosition(position);
//		c->translateAll();
//		c->setTexture(gTextures[1], gTextures[2]);
//		gObjectsOpaque.push_back(c);
//
//		// Suelo
//		gObjectsOpaque.push_back(new Suelo(500, 500, 15, 15));
//		gObjectsOpaque.back()->setModelMat(rotate(gObjectsOpaque.back()->modelMat(), radians(90.0), dvec3(1.0, 0.0, 0.0)));
//		gObjectsOpaque.back()->setColor(dvec4(0.75, 1.0, 0.75, 1.0));
//		gObjectsOpaque.back()->setTexture(gTextures[3]);
//
//		// Foto
//		Texture* t = new Texture();
//		gTextures.push_back(t);
//		gObjectsOpaque.push_back(new Foto(4 * 500 / 15, 3 * 500 / 15));
//		gObjectsOpaque.back()->setTexture(t);
//		gObjectsOpaque.back()->setModelMat(translate(dmat4(1), dvec3(0.0, 1.0, 0.0)));
//		gObjectsOpaque.back()->setModelMat(rotate(gObjectsOpaque.back()->modelMat(), radians(-90.0), dvec3(1.0, 0.0, 0.0)));
//
//		// Planta
//		Planta* planta = new Planta(150, 150);
//		gObjectsOpaque.push_back(planta);
//		planta->setTexture(gTextures[5]);
//		planta->setModelMat(translate(planta->modelMat(), dvec3(150, 75, -150)));
//
//		/* Objetos con transparencia */
//
//		// Cristalera translucida
//		Caja* caja = new Caja(500);
//		gObjectsTrans.push_back(caja);
//		caja->setTexture(gTextures[4], gTextures[4]);
//		caja->setModelMat(translate(caja->modelMat(), dvec3(0, 250, 0)));
//#pragma endregion
//	}
	if(this->mId == 0)
	{
		/*Sphere* esfera = new Sphere(100.0);
		esfera->setColor(dvec4(0.5, 1, 1, 0));
		gObjectsOpaque.push_back(esfera);

		Cylinder* cono = new Cylinder(50.0, 0, 100.0);
		cono->setColor(dvec4(1, 0.5, 0.7, 0));
		glm::dmat4 mAux = cono->modelMat();
		mAux = translate(mAux, dvec3(0, 85, 0));
		mAux = rotate(mAux, radians(-90.0), dvec3(1.0, 0, 0));
		cono->setModelMat(mAux);
		gObjectsOpaque.push_back(cono);*/

		//Disk* disk = new Disk(40, 100);
		//disk->setColor(dvec4(0.5, 1, 1, 0));
		//gObjectsOpaque.push_back(disk);

		//PartialDisk* disk2 = new PartialDisk(40, 100, 20, 90);
		//disk2->setColor(dvec4(0.5, 1, 1, 0));
		//glm::dmat4 mAux = disk2->modelMat();
		//mAux = translate(mAux, dvec3(0, 85, 0));
		//mAux = rotate(mAux, radians(-90.0), dvec3(1.0, 0, 0));
		//disk2->setModelMat(mAux);
		//gObjectsOpaque.push_back(disk2);

		Sphere* esfera = new Sphere(100.0);
		esfera->setColor(dvec4(0, 0.254, 0.415, 0));
		gObjectsOpaque.push_back(esfera);


		//Cilindro frontal
		Cylinder* cono = new Cylinder(50.0, 50.0, 200.0);
		cono->setColor(dvec4(0, 0.254, 0.415, 0));
		glm::dmat4 mAux = cono->modelMat();
		mAux = translate(mAux, dvec3(0, 0, -100));
		cono->setModelMat(mAux);
		gObjectsOpaque.push_back(cono);

		//Disco tapar cilindro
		Disk* disk = new Disk(0, 50);
		disk->setColor(dvec4(0, 0.254, 0.415, 0));
		mAux = disk->modelMat();
		mAux = translate(mAux, dvec3(0, 0, 100));
		disk->setModelMat(mAux);
		gObjectsOpaque.push_back(disk);

		disk = new Disk(0, 50);
		disk->setColor(dvec4(0, 0.254, 0.415, 0));
		mAux = disk->modelMat();
		mAux = translate(mAux, dvec3(0, 0, -100));
		disk->setModelMat(mAux);
		gObjectsOpaque.push_back(disk);

		//Cilindro alas
		cono = new Cylinder(20.0, 20.0, 300.0);
		cono->setColor(dvec4(0, 0.254, 0.415, 0));
		mAux = cono->modelMat();
		mAux = translate(mAux, dvec3(150, 0, 0));
		mAux = rotate(mAux, radians(-90.0), dvec3(0.0, 1.0, 0)); 
		cono->setModelMat(mAux);
		gObjectsOpaque.push_back(cono);

		Hexagono* hexagono = new Hexagono(300);
		hexagono->setTexture(gTextures[5]);
		hexagono->setColor(dvec4(0, 0.254, 0.415, 0));
		mAux = hexagono->modelMat();
		mAux = rotate(mAux, radians(-90.0), dvec3(0.0, 1.0, 0));
		mAux = translate(mAux, dvec3(0, 0, 150));
		hexagono->setModelMat(mAux);
		gObjectsTrans.push_back(hexagono);

		hexagono = new Hexagono(300);
		hexagono->setTexture(gTextures[5]);
		hexagono->setColor(dvec4(0, 0.254, 0.415, 0));
		mAux = hexagono->modelMat();
		mAux = rotate(mAux, radians(-90.0), dvec3(0.0, 1.0, 0));
		mAux = translate(mAux, dvec3(0, 0, -150));
		hexagono->setModelMat(mAux);
		gObjectsTrans.push_back(hexagono);
	}
	else if (this->mId == 1) {
		AnilloCuadrado* anillo = new AnilloCuadrado();
		gObjectsOpaque.push_back(anillo);
	}
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
	glClearColor(0.0, 0.0, 0.0, 0.0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test 
	glEnable(GL_TEXTURE_2D);  // enable Texture 

}
//-------------------------------------------------------------------------

void Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test 	
	glDisable(GL_TEXTURE_2D);  // disable Texture 	
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
	if(this->mId != 1)
		sceneDirLight(cam);
	else {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
	cam.upload();

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

void Scene::sceneDirLight(Camera const& cam) const
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glm::fvec4 posDir = { 1, 1, 1, 0 };
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(cam.viewMat()));
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(posDir));
	glm::fvec4 ambient = { 0, 0, 0, 1 };
	glm::fvec4 diffuse = { 1, 1, 1, 1 };
	glm::fvec4 specular = { 0.5, 0.5, 0.5, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, value_ptr(ambient));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, value_ptr(diffuse));
	glLightfv(GL_LIGHT0, GL_SPECULAR, value_ptr(specular));
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
