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
		gObjects.push_back(new Estrella3D(150, 7, 300)); //estrella 3d

		//gObjects.push_back(new Caja(200));
		gObjects.push_back(new CajaConFondo(200));

		gObjects.push_back(new Suelo(600, 400, 15, 10));
		gObjects.back()->setModelMat(translate(dmat4(1), dvec3(0.0, -100.0, 0.0)));
		gObjects.back()->setModelMat(rotate(gObjects.back()->modelMat(), radians(-90.0), dvec3(1.0, 0.0, 0.0)));
		gObjects.back()->setColor(dvec4(0.75, 1.0, 0.75, 1.0));
		Texture* t = new Texture();
		gTextures.push_back(t);
		t->load("..\\Bmps\\baldosaC.bmp");
		gObjects.back()->setTexture(t);
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


