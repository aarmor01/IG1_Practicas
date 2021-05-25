#include "IG1App.h"
#include "CheckML.h"
#include <iostream>

using namespace std;

//-------------------------------------------------------------------------
// static single instance (singleton pattern)

IG1App IG1App::s_ig1app;  // default constructor (constructor with no parameters)

//-------------------------------------------------------------------------

void IG1App::close()
{
	if (!mStop) {  // if main loop has not stopped
		cout << "Closing glut...\n";
		glutLeaveMainLoop();  // stops main loop and destroy the OpenGL context
		mStop = true;   // main loop stopped  
	}
	free();
}
//-------------------------------------------------------------------------

void IG1App::run()   // enters the main event processing loop
{
	if (mWinId == 0) { // if not initialized
		init();       // initialize the application 
		glutMainLoop();      // enters the main event processing loop 
		mStop = true;  // main loop has stopped  
	}
}
//-------------------------------------------------------------------------

void IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();

	// create the scene after creating the context 
	// allocate memory and resources
	mViewPort = new Viewport(mWinW, mWinH); //glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
	mCamera = new Camera(mViewPort);
	mCamera2 = new Camera(mViewPort);
	mScene = new Scene;
	mScene2 = new Scene;

	mCamera->set2D();
	mCamera2->set2D();
	mScene->init(1);
	mScene2->init(1);
}
//-------------------------------------------------------------------------

void IG1App::iniWinOpenGL()
{  // Initialization
	cout << "Starting glut...\n";
	int argc = 0;
	glutInit(&argc, nullptr);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);  // GLUT_CORE_PROFILE
	glutInitContextFlags(GLUT_DEBUG);					 // GLUT_FORWARD_COMPATIBLE

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(mWinW, mWinH);   // window size
	//glutInitWindowPosition (140, 140);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH /*| GLUT_STENCIL*/); // RGBA colors, double buffer, depth buffer and stencil buffer   

	mWinId = glutCreateWindow("IG1App");  // with its associated OpenGL context, return window's identifier 

	// Callback registration
	glutReshapeFunc(s_resize);
	glutKeyboardFunc(s_key);
	glutSpecialFunc(s_specialKey);
	glutDisplayFunc(s_display);
	glutIdleFunc(s_update);
	glutMouseFunc(s_mouse);
	glutMotionFunc(s_motion);
	glutMouseWheelFunc(s_mouseWheel);

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}
//-------------------------------------------------------------------------

void IG1App::free()
{  // release memory and resources
	delete mScene; mScene = nullptr;
	delete mScene2; mScene2 = nullptr;
	delete mCamera; mCamera = nullptr;
	delete mCamera2; mCamera2 = nullptr;
	delete mViewPort; mViewPort = nullptr;
}
//-------------------------------------------------------------------------

void IG1App::display() const
{  // double buffering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clears the back buffer

	if (!m2Vistas && !m2Escenas) mScene->render(*mCamera); // uploads the viewport and camera to the GPU
	else if (m2Escenas)			 display2Escenas();
	else if (m2Vistas)			 display2Vistas();


	glutSwapBuffers();									 // swaps the front and back buffer
}
//-------------------------------------------------------------------------

void IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth; mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
}
//-------------------------------------------------------------------------

void IG1App::key(unsigned char key, int x, int y)
{
	bool need_redisplay = true;

	if (key >= '0' && key <= '9')
	{
		getCamera(mCoord.x)->set2D();
		getScene(mCoord.x)->changeScene(key-48);
	}

	switch (key) {
	case 27:					   // Escape key 
		glutLeaveMainLoop();	   // stops main loop and destroy the OpenGL context
	case '+':
		getCamera(mCoord.x)->setScale(+0.01);  // zoom in  (increases the scale)
		break;
	case '-':
		getCamera(mCoord.x)->setScale(-0.01);  // zoom out (decreases the scale)
		break;
	case 'l':
		getCamera(mCoord.x)->set3D();
		break;
	case 'k':
		if (!m2Escenas) m2Vistas = !m2Vistas;
		break;
	case 'j':
		if (!m2Vistas)  m2Escenas = !m2Escenas;
		break;
	case 'o':
		getCamera(mCoord.x)->set2D();
		break;
	case 'u':
		getScene(mCoord.x) == mScene ? idleAnim = !idleAnim : idleAnim2 = !idleAnim2;
		break;
	case 'f':
		save();
		break;
	case '<':
		getCamera(mCoord.x)->orbit(1);
		break;
	case 'p':
		getCamera(mCoord.x)->changePrj();
		break;
	case 'q':
		getScene(mCoord.x)->getDirLight()->enable();
		break;
	case 'w':
		getScene(mCoord.x)->getDirLight()->disable();
		break;
	case 'a':
		getScene(mCoord.x)->getPosLight()->enable();
		break;
	case 's':
		getScene(mCoord.x)->getPosLight()->disable();
		break;
	case 'z':
		getScene(mCoord.x)->getSpotLight()->enable();
		break;
	case 'c':
		getScene(mCoord.x)->getSpotLight()->disable();
		break;
	case 'e':
		getScene(mCoord.x)->getDirLight()->enable();
		break;
	case 'r':
		getScene(mCoord.x)->darkScene();
		break;
	case 'b':
		getScene(mCoord.x)->rota();
		break;
	case 'y':
		getScene(mCoord.x)->orbita();
		break;
	case 't':
		getScene(mCoord.x)->TIEsLightsOn();
		break;
	case 'g':
		getScene(mCoord.x)->TIEsLightsOff();
		break;
	default:
		need_redisplay = false;
		break;
	} //switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
}
//-------------------------------------------------------------------------

void IG1App::specialKey(int key, int x, int y)
{
	bool need_redisplay = true;
	int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)

	switch (key) {
	case GLUT_KEY_RIGHT:
		if (mdf == GLUT_ACTIVE_CTRL)
			mCamera->moveFB(5);
		//mCamera->pitch(-1);   // rotates -1 on the X axis-
		else
			mCamera->moveLR(1);
		//mCamera->pitch(1);    // rotates 1 on the X axis
		break;
	case GLUT_KEY_LEFT:
		if (mdf == GLUT_ACTIVE_CTRL)
			mCamera->moveFB(-5);
		//mCamera->yaw(1);      // rotates 1 on the Y axis 
		else
			mCamera->moveLR(-1);
		//mCamera->yaw(-1);     // rotate -1 on the Y axis 
		break;
	case GLUT_KEY_UP:
		mCamera->moveUD(1);
		//mCamera->roll(1);		  // rotates 1 on the Z axis
		break;
	case GLUT_KEY_DOWN:
		mCamera->moveUD(-1);
		//mCamera->roll(-1);		  // rotates -1 on the Z axis
		break;
	default:
		need_redisplay = false;
		break;
	}//switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
}
//-------------------------------------------------------------------------

void IG1App::update()
{
	if (glutGet(GLUT_ELAPSED_TIME) - mLastUpdateTime >= refreshTimeRate) {
		mLastUpdateTime = glutGet(GLUT_ELAPSED_TIME);
		if (idleAnim)  mScene->update();
		if (idleAnim2) mScene2->update();
		glutPostRedisplay();
	}
}
//-------------------------------------------------------------------------

void IG1App::save()
{
	Texture* t = new Texture();
	t->loadColorBuffer(mWinW, mWinH, GL_FRONT);
	t->save("..\\Bmps\\foto.bmp");
	delete t;
}
//-------------------------------------------------------------------------

void IG1App::mouse(int button, int state, int x, int y)
{
	int _y = glutGet(GLUT_WINDOW_HEIGHT) - y;
	mCoord = dvec2(x, _y);
	mBot = button;
}
//-------------------------------------------------------------------------

void IG1App::motion(int x, int y)
{
	int _y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	dvec2 difference = mCoord - dvec2(x, _y);
	mCoord = dvec2(x, _y);


	if (mBot == GLUT_LEFT_BUTTON)
		getCamera(x)->orbit(difference.x * 0.05, difference.y);
	else if (mBot == GLUT_RIGHT_BUTTON)
	{
		getCamera(x)->moveLR(difference.x);
		getCamera(x)->moveUD(difference.y);
	}

	glutPostRedisplay();
}
//-------------------------------------------------------------------------

void IG1App::mouseWheel(int wheelButtonNumber, int direction, int x, int y)
{
	int _y = glutGet(GLUT_WINDOW_HEIGHT) - y;
	int modifiers = glutGetModifiers();

	if (modifiers > 0 && GLUT_ACTIVE_CTRL) getCamera(x)->setScale(direction);
	else								   getCamera(x)->moveFB(direction);

	glutPostRedisplay();
}
//-------------------------------------------------------------------------

void IG1App::display2Escenas() const {

	// el puerto de vista auxiliar para restaurarlo
	Viewport auxVP = *mViewPort;
	// tamaño de los 2 puertos de vista
	mViewPort->setSize(mWinW / 2, mWinH);

	// vista usuario
	mCamera->setSize(mWinW / 2.0, mViewPort->height());
	mViewPort->setPos(0, 0);
	mScene->render(*mCamera);
	mCamera->setSize(mWinW, mViewPort->height());

	mCamera2->setSize(mWinW / 2.0, mViewPort->height());
	mViewPort->setPos(mWinW / 2, 0);
	mScene2->render(*mCamera2);

	*mViewPort = auxVP;  // restaurar el puerto de vista
}

void IG1App::display2Vistas() const
{
	//  camara auxiliar
	Camera auxCam = *mCamera;
	// el puerto de vista auxiliar para restaurarlo
	Viewport auxVP = *mViewPort;
	// tama�o de los 2 puertos de vista
	mViewPort->setSize(mWinW / 2, mWinH);

	auxCam.setSize(mViewPort->width(), mViewPort->height());

	// vista usuario
	mViewPort->setPos(0, 0);
	mScene->render(auxCam);

	//vista cenital
	mViewPort->setPos(mWinW / 2, 0);
	auxCam.setCenital();
	mScene->render(auxCam);

	*mViewPort = auxVP;  // restaurar el puerto de vista
}
//-------------------------------------------------------------------------