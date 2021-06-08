//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>
#include <vector>

#include "Camera.h"
#include "Entity.h"
#include "QuadricEntity.h"
#include "CompoundEntity.h"
#include "EntityWithMaterial.h"
#include "Texture.h"

using namespace std;
using namespace glm;
//-------------------------------------------------------------------------

class Scene
{
public:
	Scene() : dirLight(nullptr), posLight(nullptr), spotLight(nullptr) {};
	~Scene() { free(); resetGL(); clearLights(); };

	Scene(const Scene& s) = delete;				// no copy constructor
	Scene& operator=(const Scene& s) = delete;  // no copy assignment

	void init(int mId = 0);
	void update();
	void render(Camera const& cam) const;

	void changeScene(int const id);

	int getId() { return mId; };

	DirLight* getDirLight() { return dirLight; }
	PosLight* getPosLight() { return posLight; }
	SpotLight* getSpotLight() { return spotLight; }

	void defaultLighting();
	void darkScene();

	void TIEsLightsOn();
	void TIEsLightsOff();

	void orbita();
	void rota();

protected:
	void free();
	void setGL();
	void resetGL();

	vector<Abs_Entity*> gObjectsOpaque;    // Entities (graphic objects) of the scene
	vector<Abs_Entity*> gObjectsTrans;	   // Entities (graphic objects) translucents of the scene
	vector<Texture*> gTextures;			   // Textures for the gObjs

	int mId = 1;						   // id de la escena

	// boolean to only create lights once
	static bool lightsAreOn;					

	DirLight* dirLight;					   // luz direccional
	PosLight* posLight;					   // luz posicional
	SpotLight* spotLight;				   // luz de foco

	static const int NUM_TEXTURES = 8;
	static const pair<std::string, int> bmps[NUM_TEXTURES];

	CompoundEntity* tieGroup;
	GLdouble tieLocalAngle;

	void loadTexture();

	void createLights();				   // generates lights
	void disableAllLights();			   // disables all lights
	void clearLights();					   // clears all lights

	void primeraEscena2D();
	void primeraEscena3D();
	void tieFighter();
	void anilloCuadrado();
	void cuboIndexado();
	void dosEsferas();
	void gridCube();
	void tiesEsfera();
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_