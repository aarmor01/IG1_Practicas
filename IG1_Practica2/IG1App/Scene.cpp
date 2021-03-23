#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;

//-------------------------------------------------------------------------

void Scene::init()
{
	setGL();  // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	//Objetos con opacos

	// Graphics objects (entities) of the scene
	gObjects.push_back(new EjesRGB(400.0));

	if (mId == 0) {
		gObjects.push_back(new Poligono(3, 260)); //triangulo de tres lineas solo, sin relleno
		gObjects.back()->setColor(glm::dvec4(1.0, 1.0, 0.0, 1.0));

		gObjects.push_back(new Poligono(260, 260)); //circulo
		gObjects.back()->setColor(glm::dvec4(1.0, 0.07, 0.57, 1.0));

		gObjects.push_back(new Sierpinski(10000, 260)); //sierpinski
		gObjects.back()->setColor(dvec4(1.0, 1.0, 0.0, 1.0));

		gObjects.push_back(new TrianguloRGB(30)); //triangulo RGB
		/*gObjects.back()->setModelMat(translate(dmat4(1), dvec3(0.0, 280.0, 0.0)));
		gObjects.back()->setModelMat(rotate(gObjects.back()->modelMat(), radians(25.0), dvec3(0.0, 0.0, 1.0)));*/

		gObjects.push_back(new RectanguloRGB(900, 600)); //rectangulo RGB
		gObjects.back()->setModelMat(translate(dmat4(1), dvec3(0.0, 0.0, -100.0)));
	}
	else {
		/* Objetos opacos */
		Texture* t = new Texture();

		// Estrella
		t->load("..\\Bmps\\baldosaP.bmp");
		Estrella3D* estrella = new Estrella3D(30, 8, 55);
		gTextures.push_back(t);
		gObjects.push_back(estrella); //estrella 3d
		dvec3 position = dvec3(-125.0, 150.0, -125.0);
		estrella->setModelMat(translate(dmat4(1), position));
		estrella->setPosition(position);
		estrella->setTexture(t);

		// Caja
		CajaConFondo* c = new CajaConFondo(80);
		position = dvec3(-125.0, 40.0, -125.0);
		c->setPosition(position);
		c->translateAll();
		t = new Texture();
		t->load("..\\Bmps\\container.bmp");
		gTextures.push_back(t);
		Texture* t1 = new Texture();
		t1->load("..\\Bmps\\papelC.bmp");
		gTextures.push_back(t1);
		c->setTexture(t, t1);
		gObjects.push_back(c);

		// Suelo
		t = new Texture();
		t->load("..\\Bmps\\baldosaC.bmp");
		gTextures.push_back(t);
		gObjects.push_back(new Suelo(500, 500, 15, 15));
		gObjects.back()->setModelMat(rotate(gObjects.back()->modelMat(), radians(90.0), dvec3(1.0, 0.0, 0.0)));
		gObjects.back()->setColor(dvec4(0.75, 1.0, 0.75, 1.0));
		gObjects.back()->setTexture(t);

		// foto
		t = new Texture();
		gTextures.push_back(t);
		gObjects.push_back(new Foto(4 * 500/15, 3 * 500/15));
		gObjects.back()->setTexture(t);
		gObjects.back()->setModelMat(translate(dmat4(1), dvec3(0.0, 1.0, 0.0)));
		gObjects.back()->setModelMat(rotate(gObjects.back()->modelMat(), radians(-90.0), dvec3(1.0, 0.0, 0.0)));

		/* Objetos con transparencia */

		// Planta
		t = new Texture();
		t->load("..\\Bmps\\grass.bmp", u8vec3(0.0, 0.0, 0.0));
		gTextures.push_back(t);
		Planta* planta = new Planta(150, 150);
		gObjects.push_back(planta);
		planta->setTexture(t);
		planta->setModelMat(translate(planta->modelMat(), dvec3(150, 75, -150)));


		// Cristalera translucida
		t = new Texture();
		t->load("..\\Bmps\\windowV.bmp", 120);
		gTextures.push_back(t);
		Caja* caja = new Caja(500);
		gObjects.push_back(caja);
		caja->setTexture(t, t);
		caja->setModelMat(translate(caja->modelMat(), dvec3(0, 250, 0)));
	};
}
//-------------------------------------------------------------------------

void Scene::free()
{ // release memory and resources   

	for (Abs_Entity* el : gObjects) {
		delete el;  el = nullptr;
	}

	for (Texture* el : gTextures) {
		delete el;  el = nullptr;
	}

	gObjects.clear();
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
	for (Abs_Entity* obj : gObjects) {
		obj->update();
	}
}
//-------------------------------------------------------------------------

void Scene::render(Camera const& cam) const
{
	cam.upload();

	for (Abs_Entity* el : gObjects) {
		el->render(cam.viewMat());
	}
}
//-------------------------------------------------------------------------

void Scene::changeScene(int const id) {
	if (id != mId) {
		mId = id;
		free();
		resetGL();
		init();
	}
}
//-------------------------------------------------------------------------


