#include "IndexMesh.h"
#include "CheckML.h"
#include <fstream>

//-------------------------------------------------------------------------

IndexMesh* IndexMesh::generaGrid(GLdouble lado, GLuint numDiv)
{
	// Grid cuadrado de lado*lado, centrado en el plano Y=0,
	// dividido en numDiv*numDiv celdas (cada celda son 2 triángulos)
	IndexMesh* indexMesh = new IndexMesh();
	GLdouble incr = lado / numDiv; // incremento para la coordenada x, y la c. z
	GLuint numFC = numDiv + 1;	   // número de vértices por filas y columnas

	// Generación de vértices
	indexMesh->mNumVertices = numFC * numFC;
	indexMesh->vVertices.reserve(indexMesh->mNumVertices);

	for (int i = 0; i < indexMesh->mNumVertices; i++)
		indexMesh->vVertices.emplace_back(dvec3());

	double z = -lado / 2;
	double x = -lado / 2;
	for (int f = 0; f < numFC; f++) {
		for (int c = 0; c < numFC; c++) {
			int indice = f * numFC + c;
			indexMesh->vVertices[indice] = dvec3(x + c * incr, 0, z + f * incr);
		}
	}

	// generacion de texturas
	indexMesh->vTexCoords.reserve(indexMesh->mNumVertices);

	for (int i = 0; i < indexMesh->mNumVertices; i++)
		indexMesh->vTexCoords.emplace_back(dvec2());

	GLuint s = 0;
	GLuint t = 1;
	for (int f = 0; f < numFC; f++) {
		for (int c = 0; c < numFC; c++) {
			int indice = f * numFC + c;
			indexMesh->vTexCoords[indice] = dvec2(s + (double)c / numDiv, t - (double)f / numDiv);
		}
	}

	// Generación de índices
	indexMesh->nNumIndices = numDiv * numDiv * 6;
	indexMesh->vIndices = new GLuint[indexMesh->nNumIndices];

	GLuint i = 0;
	for (int h = 0; h < numDiv; h++) {
		for (int k = 0; k < numDiv; k++) {
			int iv = h * numFC + k;
			indexMesh->vIndices[i++] = iv;
			indexMesh->vIndices[i++] = iv + numFC;
			indexMesh->vIndices[i++] = iv + 1;

			indexMesh->vIndices[i++] = iv + 1;
			indexMesh->vIndices[i++] = iv + numFC;
			indexMesh->vIndices[i++] = iv + numFC + 1;
		}
	}

	// normales
	indexMesh->buildNormalVectors();
	return indexMesh;
}
//-------------------------------------------------------------------------

void IndexMesh::render() const
{
	if (vVertices.size() > 0) { // transfer data
								// transfer the coordinates of the vertices
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());   // number of coordinates per vertex, type of each coordinate, stride, pointer 
		if (vColors.size() > 0) {    // transfer colors
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_DOUBLE, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
		}
		if (vTexCoords.size() > 0) { // transfer texture
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
		}
		if (vNormals.size() > 0) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_DOUBLE, 0, vNormals.data());
		}
		if (vIndices != nullptr) { // transfer texture
			glEnableClientState(GL_INDEX_ARRAY);
			glIndexPointer(GL_UNSIGNED_INT, 0, vIndices);
		}

		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
	}
}
//-------------------------------------------------------------------------

void IndexMesh::draw() const
{
	glDrawElements(mPrimitive, nNumIndices, GL_UNSIGNED_INT, vIndices);
}
//-------------------------------------------------------------------------

void IndexMesh::buildNormalVectors()
{
	vNormals.reserve(mNumVertices);

	for (int i = 0; i < mNumVertices; i++)
		vNormals.emplace_back(0, 0, 0);

	for (int i = 0; i < nNumIndices; i += 3)
	{
		dvec3 a, b, c, n;
		a = vVertices[vIndices[i]];
		b = vVertices[vIndices[i + 1]];
		c = vVertices[vIndices[i + 2]];
		n = cross(b - a, c - a);

		vNormals[vIndices[i]] += n;
		vNormals[vIndices[i + 1]] += n;
		vNormals[vIndices[i + 2]] += n;
	}

	for (int i = 0; i < mNumVertices; i++)
		vNormals[i] = normalize(vNormals[i]);
}
//-------------------------------------------------------------------------

IndexMesh* IndexMesh::generaAnilloCuadradoIndexado()
{
	IndexMesh* iMesh = new IndexMesh();

	iMesh->mPrimitive = GL_TRIANGLE_STRIP;

	iMesh->mNumVertices = 8;
	iMesh->vVertices.reserve(iMesh->mNumVertices);

	iMesh->vVertices.emplace_back(30.0, 30.0, 0.0);
	iMesh->vVertices.emplace_back(10.0, 10.0, 0.0);
	iMesh->vVertices.emplace_back(70.0, 30.0, 0.0);
	iMesh->vVertices.emplace_back(90.0, 10.0, 0.0);
	iMesh->vVertices.emplace_back(70.0, 70.0, 0.0);
	iMesh->vVertices.emplace_back(90.0, 90.0, 0.0);
	iMesh->vVertices.emplace_back(30.0, 70.0, 0.0);
	iMesh->vVertices.emplace_back(10.0, 90.0, 0.0);

	iMesh->vColors.reserve(iMesh->mNumVertices);

	iMesh->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
	iMesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	iMesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	iMesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	iMesh->vColors.emplace_back(1.0, 1.0, 0.0, 1.0);
	iMesh->vColors.emplace_back(1.0, 0.0, 1.0, 1.0);
	iMesh->vColors.emplace_back(0.0, 1.0, 1.0, 1.0);
	iMesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);

	iMesh->nNumIndices = 10;
	iMesh->vIndices = new GLuint[iMesh->nNumIndices];
	for (int i = 0; i < iMesh->nNumIndices; i++)
		iMesh->vIndices[i] = i % 8;

	//Normales
	iMesh->vNormals.reserve(iMesh->mNumVertices);

	for (int i = 0; i < iMesh->mNumVertices; i++)
		iMesh->vNormals.emplace_back(0, 0, 0);

	dvec3 a, b, c, n;
	a = iMesh->vVertices[iMesh->vIndices[0]];
	b = iMesh->vVertices[iMesh->vIndices[1]];
	c = iMesh->vVertices[iMesh->vIndices[2]];

	n = cross(b - a, c - a);

	for (int i = 0; i < iMesh->mNumVertices; i++)
		iMesh->vNormals[i] = normalize(n);

	return iMesh;
}
//-------------------------------------------------------------------------

IndexMesh* IndexMesh::generaCuboConTapasIndexado(GLdouble l)
{
	IndexMesh* iMesh = new IndexMesh();

	iMesh->mPrimitive = GL_TRIANGLES;

	iMesh->mNumVertices = 8;
	iMesh->vVertices.reserve(iMesh->mNumVertices);

	GLdouble x, y, z;

	for (int i = 0; i < iMesh->mNumVertices; i++) {
		if (i > 1 && i < 6) x = l / 2;
		else x = -l / 2;

		if (i % 2 == 0) y = l / 2;
		else y = -l / 2;

		if (i > 3 && i < 8) z = -l / 2;
		else z = l / 2;

		iMesh->vVertices.emplace_back(x, y, z);
	}

	//Indices
	iMesh->nNumIndices = 36;
	iMesh->vIndices = new GLuint[iMesh->nNumIndices]{ 0,3,2,	  //Cara delantera
									 0,1,3,
									 2,3,4,	  //Cara derecha
									 4,3,5,
									 4,5,6,	  //Cara trasera
									 6,5,7,
									 6,7,1,	  //Cara izquierda
									 1,0,6,
									 6,0,4,	  //Cara arriba
									 4,0,2,
									 7,1,5,	  //Cara abajo
									 5,1,3 };


	//Normales
	iMesh->buildNormalVectors();

	return iMesh;
}
//-------------------------------------------------------------------------

MbR::MbR(int m, int n, glm::dvec3* perfil)
{
	this->m = m;
	this->n = n;
	this->perfil = perfil;
}
//-------------------------------------------------------------------------

MbR* MbR::generaIndexMeshByRevolution(int mm, int nn, glm::dvec3* perfil)
{
	MbR* mesh = new MbR(mm, nn, perfil);

	// Definir la primitiva como GL_TRIANGLES
	mesh->mPrimitive = GL_TRIANGLES;
	// Definir el número de vértices como nn*mm
	mesh->mNumVertices = nn * mm;
	// Reservamos espacio
	mesh->vVertices.reserve(mesh->mNumVertices);

	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vVertices.emplace_back(0, 0, 0);

	for (int i = 0; i < nn; i++)
	{
		// Generar la muestra i-ésima de vértices
		GLdouble theta = i * 360 / nn;
		GLdouble c = cos(radians(theta));
		GLdouble s = sin(radians(theta));

		// R_y(theta) es la matriz de rotación alrededor del eje Y
		for (int j = 0; j < mm; j++) {
			int indice = i * mm + j;
			GLdouble x = c * perfil[j].x + s * perfil[j].z;
			GLdouble z = -s * perfil[j].x + c * perfil[j].z;
			mesh->vVertices[indice] = dvec3(x, perfil[j].y, z);
		}
	}

	//Determinar indices caras cuadrangulares
	mesh->nNumIndices = 6 * nn * mm - 6 * nn;
	mesh->vIndices = new GLuint[mesh->nNumIndices];
	// El contador i recorre las muestras alrededor del eje Y
	int indiceMayor = 0;
	for (int i = 0; i < nn; i++)
	{
		// El contador j recorre los vértices del perfil,
		// de abajo arriba. Las caras cuadrangulares resultan
		// al unir la muestra i-ésima con la (i+1)%nn-ésima
		for (int j = 0; j < mm - 1; j++)
		{
			// El contador indice sirve para llevar cuenta
			// de los índices generados hasta ahora. Se recorre
			// la cara desde la esquina inferior izquierda
			int indice = i * mm + j;
			// Los cuatro índices son entonces:
			//indice, (indice + mm) % (nn * mm), (indice + mm + 1) % (nn * mm), indice + 1
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm) % (nn * mm);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;

			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = indice + 1;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
		}
	}

	// Vectores normales
	mesh->buildNormalVectors();
	return mesh;
}
//-------------------------------------------------------------------------