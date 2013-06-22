
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>                // plik nag³ówkowy dodatkowych aplikacji bibliotek OpenGL
#include <windows.h>               // standardowy plik nag³ówkowy Windows
#include "kostka.h"
#include "freeglut.h"
#include <iostream>
#include <math.h>
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")
using namespace std;
#include "Tekstury.h"
#include "camera.h"
//#include "controls.h"
#include "ControlsGLUT.h"
#include <string>
#include "Shader.h"
#include "ManagerTekstur.h"
#include "ShaderManager.h"

using std::string;


// global doubles for camera
const double pi = 3.141592653589793233297;        // pi


static int screenWidth, screenHeight;

const bool QUIT_ON_ERROR = false;


//GLUquadricObj *g_texturedObject   = NULL;
bool freemove = true;    // Fly?
float alpha=0.5;
int phase=-1;
float time=0;

float rotation = 0.0f;
double turn = 0;
int /*screenWidth, screenHeight,*//* mouseX, mouseY,*/ board;

// global doubles for camera
//double pi = 3.141592653589793233297;        // pi





//Tekstura tekstury[3];
ManagerTekstur tekstury = ManagerTekstur::getInstance();
ShaderManager shadery = ShaderManager::getInstance();
ControlsGLUT* controls = new ControlsGLUT(&screenWidth, &screenHeight);;
Camera camera = Camera(controls, &screenWidth, &screenHeight);

//GLuint tekstura[3];


//Shader* shader;

void error()
{
  GLenum err = glGetError();
  if(err != 0)
  {
	  MessageBox(NULL, (const char*)gluErrorString(err), "B³¹d", MB_OK);
	  if(QUIT_ON_ERROR) exit(-1);
  }
}
// ----------------------------------------------------------------------------+

void InitTekstury()
{
	tekstury.Dodaj(Tekstura("earth_sphere.bmp", GL_TEXTURE0), "earth_sphere");
	tekstury.Dodaj(Tekstura("mosaicwindowgp9.bmp", GL_TEXTURE1), "mosaicwindow");
	tekstury[0].Bind();
	tekstury[1].Bind();
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
}

GLuint fbo, fbo_texture, rbo_depth;
Shader postProcShader;


bool InitShader() 
{
	Shader main = Shader("krzysztof.vert", "krzysztof.frag");
	shadery.Dodaj(main, "main");
	main.Activate();
	main.BindValue("texture1", 0);
	main.BindValue("texture2", 1);
	postProcShader = Shader("blurShader.vert", "blurShader.frag");
	//postProcShader.BindValue("texture", fbo_texture);
  //error();
	//shadery.Dodaj(postproc1, "postproc1");
	main.Activate();

  return true;
}

// ----------------------------------------------------------------------------+
void CleanUpShaders() {
  glUseProgram(0);
  shadery.UsunWszystko();
  //shader->Delete();
  //shadery["main"].Delete();
}

// ----------------------------------------------------------------------------+


void msgbox(const char *msg,char *title){

	MessageBoxA(NULL,msg,title,MB_OKCANCEL);
}


// get mouse button state
void processMouse(int button, int state, int x, int y) {
	controls->processMouse(button, state,x, y);
}


int getTime()
{
	static SYSTEMTIME LastTimeGot;
	SYSTEMTIME newTime;
	GetSystemTime(&newTime);
	FILETIME fNew, fLast;
	SystemTimeToFileTime(&newTime,&fNew);
	SystemTimeToFileTime(&LastTimeGot,&fLast);
	CompareFileTime(&fNew,&fLast);
	int wynik = CompareFileTime(&fNew,&fLast);
	LastTimeGot=newTime;
	return wynik;
}

void SetLightning()
{  
	glLightfv (GL_LIGHT0, GL_SPECULAR, SpecularLight); 
	glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); 
	glLightfv (GL_LIGHT0, GL_AMBIENT, AmbientLight); 
	glLightfv (GL_LIGHT0, GL_POSITION, LightPosition);
}

void AnimationGlobals()
{
  int timeInterval = getTime();
	time+=timeInterval;

  if(controls->przesuwanie) 
	  rotation = rotation + 0.05*timeInterval;   // zwieksza licznik kata obrotu
  if (controls->zmianaKoloru)
  {
	  alpha += timeInterval*0.05*phase;
	  if(alpha>2 || alpha<-1)
		  phase=-phase; 
  }
}

void drawTheCube()
{
	//Set up
	AnimationGlobals();
	glLoadIdentity();
	SetLightning();
	//int shaderId;
	const string shaderName = "main";
	Shader cubeShader = shadery[shaderName];
	//glGetIntegerv(GL_CURRENT_PROGRAM, &shaderId);
	cubeShader.Activate();
	//glGetIntegerv(GL_CURRENT_PROGRAM, &shaderId);
	//glUseProgram(cubeShader.getId());
	//glGetIntegerv(GL_CURRENT_PROGRAM, &shaderId);
	cubeShader.BindValue("rotation", rotation);
	cubeShader.BindValue("alpha", alpha);
	tekstury[0].Bind();
	tekstury[1].Bind();
	glPushMatrix();

	//Draw
	camera.doCamera();
	glCallList(board);

	//Clean up
	glPopMatrix();
	//shadery["main"].Deactivate();
	//glGetIntegerv(GL_CURRENT_PROGRAM, &shaderId);
	tekstury.UnbindAll();
}

void printText(string text)
{
	static void* font = GLUT_BITMAP_TIMES_ROMAN_24;

	int length = text.length();
	for(int i = 0; i<length; ++i)
	{
		glutBitmapCharacter(font, text[i]);
	}
}

// this function is called by GLUT once per frame
void display(){

	// Clear the screen
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);  // kolor tla
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// process the camera
	//camera.doCamera();

	// draw the scene
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	drawTheCube();
	//printText("Test");
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		//Wylaczenie stanow
//glDisable(GL_DEPTH_TEST);
////glDepthMask(false);
//glDisable(GL_CULL_FACE);
//
////Wylaczenie macierzy
//glMatrixMode(GL_PROJECTION);
//glLoadIdentity();
//glMatrixMode(GL_MODELVIEW);
//glLoadIdentity();
//	glClearColor(0.0, 0.0, 0.0, 1.0);
//  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
// 
//  postProcShader.Activate();
//glActiveTexture( GL_TEXTURE2 );
//  glBindTexture(GL_TEXTURE_2D, fbo_texture);
//  postProcShader.BindValue("texture", fbo_texture);
//  postProcShader.BindValue("rotation", rotation);
//glBegin(GL_QUADS);
//
//glVertex3f(-1.0f,-1.0f, -1.0f);
//glVertex3f( 1.0f,-1.0f, -1.0f);
//glVertex3f( 1.0f, 1.0f, -1.0f);
//glVertex3f(-1.0f, 1.0f, -1.0f);
//
//glEnd();
	// swap the buffers and tell GLUT to call the display function again
	glutSwapBuffers();
	glutPostRedisplay();
}



// reshape is called (once) when the window is created or resized
// it defines the shape of the viewport and initializes the graphics matrices
void reshape (int w, int h){
	// Rescale FBO and RBO as well
  glBindTexture(GL_TEXTURE_2D, fbo_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
 
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rbo_depth);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT16, screenWidth, screenHeight);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	glViewport(0, 0, (GLint) w, (GLint) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-.9, .9, -.56, .56, .6, 1000000.0);
	glTranslated(0,0,-.1);
	glMatrixMode(GL_MODELVIEW);
}

// poll the shift, control, and alt keys
void getShiftStatus(int shift_state) {

	controls->getShiftStatus(shift_state);
}
// get key (down) states
void keydown(unsigned char key, int x, int y){

	controls->keydown(key, x, y);
}
// get key (up) states
void keyup(unsigned char key,int x, int y){

	controls->keyup(key, x, y);
}
void specialDown(int key, int mx,int my){

	controls->specialDown(key, mx, my);
}
void specialKeyUp(int key, int mx, int my){

	controls->specialKeyUp(key, mx, my);
}
// adjust camera pitch and yaw (this function is called when the mouse is moved while a button is pressed)
void processMouseActiveMotion(int x, int y){

	controls->processMouseActiveMotion(x, y);
	//glutWarpPointer(screenWidth / 2, screenHeight / 2);
}
// adjust camera pitch and yaw (when mouse buttons are idle)
void processMousePassiveMotion(int x, int y){

	controls->processMousePassiveMotion(x,y);
	//glutWarpPointer(screenWidth / 2, screenHeight / 2);
}




void generateScene(){

	static GLfloat squareSize = 100;
	static bool checker = false;

	board = glGenLists(1);
	glNewList(board, GL_COMPILE);
	glDrawCube();
		// draw squares
		//glBegin(GL_QUADS);
		//    for(int i = -50; i < 50; ++i){
		//        checker = !checker;
		//        for(int j = -50; j < 50; ++j){
		//            checker = !checker;
		//            if(checker) glColor3f(1, .5, .5);
		//            else {glColor3f(.5, .5, 1);}
		//            glVertex3f(-squareSize / 2 + j * squareSize, -squareSize / 2 + i * squareSize, 0);
		//            glVertex3f(squareSize / 2 + j * squareSize, -squareSize / 2 + i * squareSize, 0);
		//            glVertex3f(squareSize / 2 + j * squareSize, squareSize / 2 + i * squareSize, 0);
		//            glVertex3f(-squareSize / 2 + j * squareSize, squareSize / 2 + i * squareSize, 0);
		//        }
		//    }
		//glEnd();
	glEndList();
}



int main(int argc, char** argv){

	// Initialze GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow (argv[0]);
	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);                            // kill the cursor
	screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
	glutWarpPointer(screenWidth / 2, screenHeight / 2);         // center the mouse


	glutReshapeFunc(reshape);
	glutKeyboardFunc (keydown);
	glutKeyboardUpFunc (keyup);                                 // function is called when a 'normal' key is let up
	glutSpecialFunc(specialDown);                               // function is called when special keys are pressed
	glutSpecialUpFunc(specialKeyUp);                            // function is called when special keys are release
	glutMouseFunc(processMouse);                                // function is called when a mouse button is pressed
	glutMotionFunc(processMouseActiveMotion);                   // called when the mouse moves & a button is pressed
	glutPassiveMotionFunc(processMousePassiveMotion);           // function is called when the mouse moves
	glutDisplayFunc(display);
  glEnable(GL_DEPTH_TEST);
  glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //// tworzy obiekt powierzchni generuj¹cy wspó³rzêdne tekstury
  static GLUquadricObj *g_texturedObject = gluNewQuadric();
  gluQuadricTexture(g_texturedObject, GL_TRUE);
	  // wczytujemy i ustawiamy tekstury
  //tekstura[1] = Tekstury::LoadGLTexture("mosaicwindowgp9.bmp");
  //tekstura[0] = Tekstury::LoadGLTexture("earth_sphere.bmp");
  //tekstury[0] = Tekstura("earth_sphere.bmp");
  //tekstury[1] = Tekstura("mosaicwindowgp9.bmp");
	//InitFramebuffer();
	InitShader();
	InitTekstury();
	generateScene();
	glutMainLoop();
	return 0;
}
