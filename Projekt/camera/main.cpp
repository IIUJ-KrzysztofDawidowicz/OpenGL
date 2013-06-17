
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





GLuint tekstura[3];


char *ReadAllTextFromFile(char *fname) {
    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fname == NULL) return NULL;

    fp = fopen(fname,"rt");

    if (fp == NULL) return NULL;
    
    fseek(fp, 0, SEEK_END);
    count = ftell(fp);
    rewind(fp);

    if (count > 0) {
        content = new char[sizeof(char) * (count+1)];
        count = fread(content,sizeof(char), count, fp);
        content[count] = '\0';
    }
    fclose(fp);

    return content;
}

class Shader
{
public:
    GLuint shader_program;

    void LinkShader(GLuint vertex_shader, GLuint fragment_shader)
{
      shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);

  glLinkProgram(shader_program);
  glUseProgram(shader_program);
}

    void BindValue(const GLchar* name, const float value)
    {
        GLuint location = glGetUniformLocation(shader_program, name);
        glUniform1f(location, value);
    }

    void BindValue(const GLchar* name, const int value)
    {
        GLuint location = glGetUniformLocation(shader_program, name);
        glUniform1i(location, value);
    }
};

Shader* shader = new Shader();

void error()
{
  GLenum err = glGetError();
  if(err != 0)
  {
      MessageBox(NULL, (const char*)gluErrorString(err), "B³¹d", MB_OK);
  }
}
// ----------------------------------------------------------------------------+

bool MakeShader(GLuint shader, char* name)
{
    if (shader == 0)
  {
    MessageBox(NULL, "Nie ma szejderow", NULL, MB_OK);
    return false;
  }
  char *text = ReadAllTextFromFile(name);
  if (text == NULL) {
    MessageBox(NULL, "Nie mo¿e za³adowaæ vertex-szejdera", NULL, MB_OK);
    return 0;
  }
  const char * temp = text;
  glShaderSource(shader, 1, &temp, NULL);
  delete [] text;
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    MessageBox(NULL, "B³¹d kompilacji vertex-szejdera", NULL, MB_OK);
    return 0;
  }
}

GLuint MakeVertexShader(char* name)
{
      GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
      MakeShader(vertex_shader, name);
  
  return vertex_shader;
}


GLuint MakeFragmentShader(char* name)
{
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
      MakeShader(fragment_shader, name);
  
  return fragment_shader;
}

bool InitShader() {

  GLint status;
  GLuint vertex_shader = MakeVertexShader("krzysztof.vert");/*glCreateShader(GL_VERTEX_SHADER);

  if (vertex_shader == 0)
  {
    MessageBox(NULL, "Nie ma szejderow", NULL, MB_OK);
    return false;
  }
  char *vs = ReadAllTextFromFile("krzysztof.vert");
  if (vs == NULL) {
    MessageBox(NULL, "Nie mo¿e za³adowaæ vertex-szejdera", NULL, MB_OK);
    return false;
  }
  const char * vv = vs;
  glShaderSource(vertex_shader, 1, &vv, NULL);
  delete [] vs;
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    MessageBox(NULL, "B³¹d kompilacji vertex-szejdera", NULL, MB_OK);
    return false;
  }*/

  
  GLuint fragment_shader = MakeFragmentShader("krzysztof.frag");/*glCreateShader(GL_FRAGMENT_SHADER);
  if (fragment_shader == 0)
  {
    MessageBox(NULL, "Nie ma szejderow", NULL, MB_OK);
    return false;
  }

  //char *vs = ReadAllTextFromFile("shaders/basic.vert");
  //char *fs = ReadAllTextFromFile("shaders/basic.frag");
  char *fs = ReadAllTextFromFile("krzysztof.frag");

  if (fs == NULL) {
    MessageBox(NULL, "Nie mo¿e za³adowaæ fragment-szejdera", NULL, MB_OK);
    return false;
  }

  const char * ff = fs;

  glShaderSource(fragment_shader, 1, &ff, NULL);

  delete [] fs;

  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    MessageBox(NULL, "B³¹d kompilacji fragment-szejdera", NULL, MB_OK);
    return false;
  }*/
  
  shader->LinkShader(vertex_shader, fragment_shader);

  //GLuint location = glGetUniformLocation(shader->shader_program, "texture1");
  //glUniform1i(location, 0);
  //location = glGetUniformLocation(shader->shader_program, "texture2");
  //glUniform1i(location, 1);
  shader->BindValue("texture1", 0);
  shader->BindValue("texture2", 1);
  glActiveTextureARB(GL_TEXTURE0_ARB);
  glBindTexture(GL_TEXTURE_2D, tekstura[0]);
  glActiveTextureARB(GL_TEXTURE1_ARB);
  glBindTexture(GL_TEXTURE_2D, tekstura[1]);
  glEnable(GL_TEXTURE_2D);
glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);

  error();

  return true;
}

// ----------------------------------------------------------------------------+
void CleanUpShaders() {
  glUseProgram(0);
  glDeleteProgram(shader->shader_program);
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
    //if (state == GLUT_DOWN){                                                    // if mouse button is down...
    //	if(button == GLUT_LEFT_BUTTON) controls->leftButton = true;                       // obvious stuff
    //	if(button == GLUT_RIGHT_BUTTON) controls->rightButton = true;                     // ...
    //}else{
    //	controls->leftButton = false;
    //	controls->rightButton = false;
    //}
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
    //controls->shiftkey = false;
    //controls->ctrlkey = false;
    //controls->altkey = false;
    //switch(shift_state){
    //	case 0: break;
    //	case 1: controls->shiftkey = true; break;
    //	case 2: controls->ctrlkey = true; break;
    //	case 3: controls->shiftkey = true; controls->ctrlkey = true; break;
    //	case 4: controls->altkey = true; break;
    //	case 5: controls->shiftkey = true; controls->altkey = true; break;
    //	case 6: controls->ctrlkey = true; controls->altkey = true; break;
    //	case 7: controls->shiftkey = true; controls->ctrlkey = true; controls->altkey = true; break;
    //}
}



// get key (down) states
void keydown(unsigned char key, int x, int y){

    controls->keydown(key, x, y);
    //switch (key) {
    ////	case 'f': case 'F': freemove = !freemove; break;
    //	case 'w': case 'W': controls->wkey=true; break;
    //	case 's': case 'S': controls->skey=true; break;
    //	case 'd': case 'D': controls->dkey=true; break;
    //	case 'a': case 'A': controls->akey=true; break;
    //	case 'p': case 'P': przesuwanie = !przesuwanie; break;
    //	case 32: controls->spacebar = true; zmianaKoloru=!zmianaKoloru; break;
    //	case 27: exit(0); break;
 //  }
}



// get key (up) states
void keyup(unsigned char key,int x, int y){

    controls->keyup(key, x, y);
    //switch(key){
    //	case 'w': case 'W': controls->wkey=false; break;
    //	case 's': case 'S': controls->skey=false; break;
    //	case 'd': case 'D': controls->dkey=false; break;
    //	case 'a': case 'A': controls->akey=false; break;
    //	case 38: controls->upkey = false; break;
    //	case 40: controls->downkey = false; break;
    //	case 32: controls->spacebar = false; break;
    //	case 37:
    //		controls->leftkey=false;
    //		controls->altleftkey=false;
    //		break;
    //	case 39:
    //		controls->altrightkey = false;
    //		controls->rightkey = false;
    //		break;
    //}
}



void specialDown(int key, int mx,int my){

    controls->specialDown(key, mx, my);
    //getShiftStatus(glutGetModifiers());
    //switch(key){
    //	case GLUT_KEY_LEFT:
    //		if (controls->altkey)controls->altleftkey=true;
    //		else{controls->leftkey=true;}
    //		break;
    //	case GLUT_KEY_RIGHT:
    //		if (controls->altkey)controls->altrightkey=true;
    //		else{controls->rightkey=true;}
    //		break;
    //	case GLUT_KEY_UP: controls->upkey=true; break;
    //	case GLUT_KEY_DOWN: controls->downkey=true; break;
    //	case 104: controls->pageupkey=true; break;
    //	case 105: controls->pagedownkey=true; break;
    //}
}



void specialKeyUp(int key, int mx, int my){

    controls->specialKeyUp(key, mx, my);
    //switch(key){
    //	case GLUT_KEY_LEFT:
    //		controls->leftkey=false;
    //		controls->altleftkey=false;
    //		break;
    //	case GLUT_KEY_RIGHT:
    //		controls->rightkey=false;
    //		controls->altrightkey=false;
    //		break;
    //	case GLUT_KEY_UP: controls->upkey=false; break;
    //	case GLUT_KEY_DOWN: controls->downkey=false; break;
    //	case 104: controls->pageupkey=false; break;
    //	case 105: controls->pagedownkey=false; break;
    //}
}



// adjust camera pitch and yaw (this function is called when the mouse is moved while a button is pressed)
void processMouseActiveMotion(int x, int y){

    controls->processMouseActiveMotion(x, y);
    //controls->mouseX = x;
    //controls->mouseY = y;
}



// adjust camera pitch and yaw (when mouse buttons are idle)
void processMousePassiveMotion(int x, int y){

    controls->processMousePassiveMotion(x,y);
    //controls->mouseX = x;
    //controls->mouseY = y;
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
  tekstura[1] = Tekstury::LoadGLTexture("mosaicwindowgp9.bmp");
  tekstura[0] = Tekstury::LoadGLTexture("earth_sphere.bmp");
    InitShader();
    generateScene();
    glutMainLoop();
    return 0;
}
