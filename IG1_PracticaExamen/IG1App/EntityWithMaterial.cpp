#include "EntityWithMaterial.h"

//------------------------------------------------------------------------

Esfera::Esfera(GLdouble radius, GLuint puntosPerfil, GLdouble rev)
{
	dvec3* perfil = new dvec3[puntosPerfil];

	GLfloat angle = 270; //angulo
	for (GLuint counter = 0; counter < puntosPerfil; counter++) {
		perfil[counter] = dvec3(radius * cos(radians(angle)), radius * sin(radians(angle)), 0.0);
		angle += 180.0 / (puntosPerfil - 1); // incrementamos el angulo del siguiente vertice
	}

	this->mMesh = MbR::generaIndexMeshByRevolution(puntosPerfil, rev, perfil);
}
//------------------------------------------------------------------------

void Esfera::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		if (material != nullptr) {
			material->upload();
			mMesh->render();
		}
		else {
			glEnable(GL_COLOR_MATERIAL);
			glColor3f(mColor.r, mColor.g, mColor.b);
			mMesh->render();
			glColor3f(1.0, 1.0, 1.0);
			glDisable(GL_COLOR_MATERIAL);
		}
	}
}
//------------------------------------------------------------------------