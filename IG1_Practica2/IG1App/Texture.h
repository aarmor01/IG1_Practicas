//#pragma once
#ifndef _H_Texture_H_
#define _H_Texture_H_

#include <GL/freeglut.h>
#include <glm.hpp>
#include "Pixmap32RGBA.h"
#include <string>

//-------------------------------------------------------------------------
class IG1App;
class Texture
{
public:
	Texture() {};
	~Texture();

	Texture(const Texture& tex) = delete;  // no copy constructor
	Texture& operator=(const Texture& tex) = delete;  // no copy assignment

	void load(const std::string& BMP_Name, GLubyte alpha = 255); // load from file and upload to GPU
	void load(const std::string& BMP_Name, glm::u8vec3 color, GLubyte alpha = 0); // load from file and upload to GPU
	void loadColorBuffer(GLuint width, GLuint height, GLuint buffer);
	void save(const std::string& BMP_Name);

	void bind(GLuint mixMode);   // GL_REPLACE, GL_MODULATE, GL_ADD, ...
	void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); };

	GLuint width() const { return mWidth; };
	GLuint height() const { return mHeight; };

	void setWrap(GLuint wp);  // GL_REPEAT, GL_CLAMP 

protected:

	void init();

	GLuint mWidth = 0;
	GLuint mHeight = 0;
	GLuint mId = 0;
};

//-------------------------------------------------------------------------

#endif //_H_Texture_H_

