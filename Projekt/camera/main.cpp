
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

using std::string;


// global doubles for camera
const double pi = 3.141592653589793233297;        // pi


static int screenWidth, screenHeight;



GLUquadricObj *g_texturedObject   = NULL;
bool freemove = true;    // Fly?
float alpha=0.5;
int phase=-1;
SYSTEMTIME LastTimeGot;
float time=0;

float rotation = 0.0f;
double turn = 0;
int /*screenWidth, screenHeight,*//* mouseX, mouseY,*/ board;

// global doubles for camera
//double pi = 3.141592653589793233297;        // pi



class Tekstury
{
public:
	GLuint tekstura;
	Tekstury(void);
	Tekstury(char* plik)
	{
		tekstura = LoadGLTexture(plik);
	}
	~Tekstury(void);

	static int LoadGLTexture(char *plik);
};
Tekstury::Tekstury(void)
{
}


Tekstury::~Tekstury(void)
{
}
int Tekstury::LoadGLTexture(char *plik)
{
  unsigned int Texture;
  FILE* img = NULL;

  img = fopen(plik,"rb");
  if (img==NULL)
  {
	MessageBox(NULL, "Nie mo¿na otworzyæ pliku z tekstur¹", "B³¹d", MB_OK);
	return -1;
  }
  unsigned long bWidth = 0;	
  unsigned long bHeight = 0;	
  DWORD size = 0;	

  fseek(img,18,SEEK_SET);
  fread(&bWidth,4,1,img);
  fread(&bHeight,4,1,img);
  fseek(img,0,SEEK_END);
  size = ftell(img) - 54;

  unsigned char *data = (unsigned char*)malloc(size);
  if (data==NULL)
  {
	MessageBox(NULL, "Nie mo¿na zaalokowaæ pamiêci na teksturê", "B³¹d", MB_OK);
	return -1;
  }

  fseek(img,54,SEEK_SET);	// image data
  fread(data,size,1,img);
  fclose(img);

  glGenTextures(1, &Texture);
  glBindTexture(GL_TEXTURE_2D, Texture);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bWidth, bHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  
  free(data);
  return Texture;
}

Tekstury tekstury[3];


//GLuint tekstura[3];


Shader* shader;

void error()
{
  GLenum err = glGetError();
  if(err != 0)
  {
	  MessageBox(NULL, (const char*)gluErrorString(err), "B³¹d", MB_OK);
  }
}
// ----------------------------------------------------------------------------+


bool InitShader() 
{

	shader = new Shader("krzysztof.vert", "krzysztof.frag");
  shader->BindValue("texture1", 0);
  shader->BindValue("texture2", 1);
  glActiveTextureARB(GL_TEXTURE0_ARB);
  //glBindTexture(GL_TEXTURE_2D, tekstura[0]);
  glBindTexture(GL_TEXTURE_2D, tekstury[0].tekstura);
  glActiveTextureARB(GL_TEXTURE1_ARB);
  //glBindTexture(GL_TEXTURE_2D, tekstura[1]);
  glBindTexture(GL_TEXTURE_2D, tekstury[1].tekstura);
  glEnable(GL_TEXTURE_2D);
glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);

  error();

  return true;
}

// ----------------------------------------------------------------------------+
void CleanUpShaders() {
  glUseProgram(0);
  shader->Delete();
}

// ----------------------------------------------------------------------------+


void msgbox(const char *msg,char *title){

	MessageBoxA(NULL,msg,title,MB_OKCANCEL);
}


ControlsGLUT* controls = new ControlsGLUT();


Camera camera = Camera(controls, &screenWidth, &screenHeight);

// get mouse button state
void processMouse(int button, int state, int x, int y) {
	controls->processMouse(button, state,x, y);
}


int getTime()
{
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


void drawScene(){
		int timeInterval = getTime();
	time+=timeInterval;

  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);  // kolor tla
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  glLoadIdentity();
  
  glLightfv (GL_LIGHT0, GL_SPECULAR, SpecularLight); 
  glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); 
  glLightfv (GL_LIGHT0, GL_AMBIENT, AmbientLight); 
  glLightfv (GL_LIGHT0, GL_POSITION, LightPosition);

  if(controls->przesuwanie) 
	  rotation = rotation + 0.05*timeInterval;   // zwieksza licznik kata obrotu
  if (controls->zmianaKoloru)
  {
	  alpha += timeInterval*0.05*phase;
	  if(alpha>2 || alpha<-1)
		  phase=-phase; 
  }
  /*if (rotation >= 360.0f)
	rotation = 0.0f;*/

  //GLuint location = glGetUniformLocation(shader->shader_program,"rotation");
  //glUniform1f(location, rotation );
  shader->BindValue("rotation", rotation);
  //location = glGetUniformLocation(shader->shader_program,"alpha");
  //glUniform1f(location, alpha );
  shader->BindValue("alpha", alpha);
	glPushMatrix();
	gluLookAt(camera.eyeX, camera.eyeY, camera.eyeZ, camera.lookX, camera.lookY, camera.lookZ, 0, 0, 1/*sideX, sideY, sideZ*/);      // move and aim camera
		//glTranslatef(0, 0, -50);
	//glRotatef(r*10, lookX, lookY, lookZ);
		glCallList(board);
		//glTranslatef(0, 0, 200);
		//glCallList(board);
	glPopMatrix();
}



// this function is called by GLUT once per frame
void display(){

	// Clear the screen
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// process the camera
	camera.doCamera();

	// draw the scene
	drawScene();

	// swap the buffers and tell GLUT to call the display function again
	glutSwapBuffers();
	glutPostRedisplay();
}



// reshape is called (once) when the window is created or resized
// it defines the shape of the viewport and initializes the graphics matrices
void reshape (int w, int h){

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
}
// adjust camera pitch and yaw (when mouse buttons are idle)
void processMousePassiveMotion(int x, int y){

	controls->processMousePassiveMotion(x,y);
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
  g_texturedObject = gluNewQuadric();
  gluQuadricTexture(g_texturedObject, GL_TRUE);
	  // wczytujemy i ustawiamy tekstury
  //tekstura[1] = Tekstury::LoadGLTexture("mosaicwindowgp9.bmp");
  tekstury[1] = Tekstury("mosaicwindowgp9.bmp");
  //tekstura[0] = Tekstury::LoadGLTexture("earth_sphere.bmp");
  tekstury[0] = Tekstury("earth_sphere.bmp");
	InitShader();
	generateScene();
	glutMainLoop();
	return 0;
}
