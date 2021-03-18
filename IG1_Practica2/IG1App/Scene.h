//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Camera.h"
#include "Entity.h"
#include "Texture.h"

#include <vector>

using namespace std;

//-------------------------------------------------------------------------

class Scene
{
public:
	Scene() {};
	~Scene() { free(); resetGL(); };

	Scene(const Scene& s) = delete;  // no copy constructor
	Scene& operator=(const Scene& s) = delete;  // no copy assignment

	void init();
	void update();
	void render(Camera const& cam) const;
	void changeScene(int const id);

protected:
	void free();
	void setGL();
	void resetGL();

	vector<Abs_Entity*> gObjects;  // Entities (graphic objects) of the scene
	vector<Texture*> gTextures;  // Textures for the gObjs

	int mId = 1;
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

