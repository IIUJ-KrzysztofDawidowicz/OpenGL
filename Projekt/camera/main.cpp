
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
#include "controls.h"


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




GLuint shader_program;
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



void error()
{
  GLenum err = glGetError();
  if(err != 0)
  {
      MessageBox(NULL, (const char*)gluErrorString(err), "B³¹d", MB_OK);
  }
}
// ----------------------------------------------------------------------------+
bool InitShaders() {

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  if (vertex_shader == 0 || fragment_shader == 0)
  {
    MessageBox(NULL, "Nie ma szejderow", NULL, MB_OK);
    return false;
  }

  //char *vs = ReadAllTextFromFile("shaders/basic.vert");
  //char *fs = ReadAllTextFromFile("shaders/basic.frag");
  char *vs = ReadAllTextFromFile("krzysztof.vert");
  char *fs = ReadAllTextFromFile("krzysztof.frag");

  if (vs == NULL) {
    MessageBox(NULL, "Nie mo¿e za³adowaæ vertex-szejdera", NULL, MB_OK);
    return false;
  }
  if (fs == NULL) {
    MessageBox(NULL, "Nie mo¿e za³adowaæ fragment-szejdera", NULL, MB_OK);
    return false;
  }

  const char * vv = vs;
  const char * ff = fs;

  glShaderSource(vertex_shader, 1, &vv, NULL);
  glShaderSource(fragment_shader, 1, &ff, NULL);

  delete [] vs;
  delete [] fs;

  glCompileShader(vertex_shader);
  glCompileShader(fragment_shader);

  GLint status;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    MessageBox(NULL, "B³¹d kompilacji vertex-szejdera", NULL, MB_OK);
    return false;
  }
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    MessageBox(NULL, "B³¹d kompilacji fragment-szejdera", NULL, MB_OK);
    return false;
  }
  
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);

  glLinkProgram(shader_program);
  glUseProgram(shader_program);

  GLuint location = glGetUniformLocation(shader_program, "texture1");
  glUniform1i(location, 0);
  location = glGetUniformLocation(shader_program, "texture2");
  glUniform1i(location, 1);
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
  glDeleteProgram(shader_program);
}

// ----------------------------------------------------------------------------+


void msgbox(const char *msg,char *title){

    MessageBoxA(NULL,msg,title,MB_OKCANCEL);
}


class ControlsGLUT: public Controls
{
public:
    ControlsGLUT()
    {
        upkey = downkey = leftkey = rightkey = altleftkey = altrightkey = wkey = skey = ctrlkey = altkey = akey = dkey = shiftkey = spacebar = rightButton =
            leftButton = pageupkey = pagedownkey = false;

     mouseX = mouseY = 0;
        przesuwanie = false;
        zmianaKoloru = true;
    }
// get mouse button state
void processMouse(int button, int state, int x, int y) {

    if (state == GLUT_DOWN){                                                    // if mouse button is down...
        if(button == GLUT_LEFT_BUTTON) leftButton = true;                       // obvious stuff
        if(button == GLUT_RIGHT_BUTTON) rightButton = true;                     // ...
    }else{
        leftButton = false;
        rightButton = false;
    }
}


// poll the shift, control, and alt keys
void getShiftStatus(int shift_state) {

    shiftkey = false;
    ctrlkey = false;
    altkey = false;
    switch(shift_state){
        case 0: break;
        case 1: shiftkey = true; break;
        case 2: ctrlkey = true; break;
        case 3: shiftkey = true; ctrlkey = true; break;
        case 4: altkey = true; break;
        case 5: shiftkey = true; altkey = true; break;
        case 6: ctrlkey = true; altkey = true; break;
        case 7: shiftkey = true; ctrlkey = true; altkey = true; break;
    }
}



// get key (down) states
void keydown(unsigned char key, int x, int y){

    switch (key) {
    //	case 'f': case 'F': freemove = !freemove; break;
        case 'w': case 'W': wkey=true; break;
        case 's': case 'S': skey=true; break;
        case 'd': case 'D': dkey=true; break;
        case 'a': case 'A': akey=true; break;
        case 'p': case 'P': przesuwanie = !przesuwanie; break;
        case 32: spacebar = true; zmianaKoloru=!zmianaKoloru; break;
        case 27: exit(0); break;
   }
}



// get key (up) states
void keyup(unsigned char key,int x, int y){

    switch(key){
        case 'w': case 'W': wkey=false; break;
        case 's': case 'S': skey=false; break;
        case 'd': case 'D': dkey=false; break;
        case 'a': case 'A': akey=false; break;
        case 38: upkey = false; break;
        case 40: downkey = false; break;
        case 32: spacebar = false; break;
        case 37:
            leftkey=false;
            altleftkey=false;
            break;
        case 39:
            altrightkey = false;
            rightkey = false;
            break;
    }
}



void specialDown(int key, int mx,int my){

    getShiftStatus(glutGetModifiers());
    switch(key){
        case GLUT_KEY_LEFT:
            if (altkey)altleftkey=true;
            else{leftkey=true;}
            break;
        case GLUT_KEY_RIGHT:
            if (altkey)altrightkey=true;
            else{rightkey=true;}
            break;
        case GLUT_KEY_UP: upkey=true; break;
        case GLUT_KEY_DOWN: downkey=true; break;
        case 104: pageupkey=true; break;
        case 105: pagedownkey=true; break;
    }
}



void specialKeyUp(int key, int mx, int my){

    switch(key){
        case GLUT_KEY_LEFT:
            leftkey=false;
            altleftkey=false;
            break;
        case GLUT_KEY_RIGHT:
            rightkey=false;
            altrightkey=false;
            break;
        case GLUT_KEY_UP: upkey=false; break;
        case GLUT_KEY_DOWN: downkey=false; break;
        case 104: pageupkey=false; break;
        case 105: pagedownkey=false; break;
    }
}



// adjust camera pitch and yaw (this function is called when the mouse is moved while a button is pressed)
void processMouseActiveMotion(int x, int y){

    mouseX = x;
    mouseY = y;
}



// adjust camera pitch and yaw (when mouse buttons are idle)
void processMousePassiveMotion(int x, int y){

    mouseX = x;
    mouseY = y;
}



};

ControlsGLUT* controls = new ControlsGLUT();

class Camera
{
// this function returns the angle of a point (hx, hy) in radians relative to the origin
double heading(double hx, double hy){

    static double sn, cs;

    dist = sqrt(hx * hx + hy * hy);                     // get distance of point to origin
    if (!dist) dist = .00000000001;                         // make sure d is non-zero (avoids /0 error)
    sn = hx / dist;                                     // get the sine (opposite over hypotenuse)
    cs = hy / dist;                                     // get the cosine (adjacent over hypotenuse)
    if(!cs) cs = .00000000001;                          // make sure cs is non-zero (avoids /0 error
    return cs > 0 ? atan(sn/cs) : pi + atan(sn/cs);     // return heading
    
}



// returns deviation from the equator in radians of a point relative to the origin
double elevation(double x, double y, double z){

    dist = sqrt(x * x + y * y + z * z);                // get distance of point from origin
    if(dist) return acos(z / dist);                     // if point is not coincidental with the origin, return elevation
    return 0;                                       // else return 0;
}

Controls* controls;

// Camera
public:
        double  eyeX,                                // Player location X
               eyeY,                                // Player location Y
               eyeZ,                                // Player location Z
               lookX,                               // X value of 'look-direction' vector
               lookY,                               // Y value of 'look-direction' vector
               lookZ,                               // Z value of 'look-direction' vector
               //sideX,
               //sideY,
               //sideZ,
               dist;                                // global variable for distance calcs
        Camera(Controls* c)
        {
            controls = c;
        }

    void doCamera(){

    static GLfloat bob,                         // controls bobbing
                   g,                           // gravity
                   base = -50,                  // bottom of universe
                   bob_height = 4,              // bob amplitude
                   accel = 0,                   // speed multiplier (for shift = run)
                   strafe_backforward_v = 0,    // forward velocity
                   strafe_lateral_v = 0,        // left / right velocity
                   strafe_updown_v = 0,         // vertical velocity
                   strafev = /*-2.5*/ -0.25,    // velocity
                   tv = -.015,                  // rotational velocity
                   p_drag=1.5,                  // look drag
                   strafe_drag = 1.5,           // movement drag
                   jumpheight = 100,            // jump height
                   jumpv,                       // jump speed
                   foot = -50,                  // initial vertical footing
                   yawVelocity = 0,             // Yaw velocity
                   pitchVelocity = 0,           // Pitch velocity
                   rollVelocity = 0,            // Roll velocity
                   yaw = 0,                     // Acutal Yaw
                   pitch = 0,                   // Actual Pitch
                   roll = 0,                    // Actual Roll
                   playerHeight = 100,          // Height of player's eye when freemove = false
                   gravity = .5,                // gravity
                   mouseSensitivity = 300;      // Mouse Sensitivity (less is more)

    static double p1,                           // holds heading
                  p2,                           // holds elevation
                  view[5][6];                   // holds camera vectors
                 
    static bool jumping,                        // is a jump in progress?
                grounded;                       // has a jump ended?

    grounded = false;                           // grounded = false


    if((controls->spacebar || controls->rightButton) && !jumping){  // if a jump is wanted... 
        jumping = true;                         // set jumping = true
        jumpv = jumpheight;                     // increase jump velocity
    }


    yawVelocity -= (screenWidth / 2 - controls->mouseX) / mouseSensitivity;
    pitchVelocity += (screenHeight / 2 - controls->mouseY) / mouseSensitivity;
    glutWarpPointer(screenWidth / 2, screenHeight / 2);                 // center the mouse

    if(::GetKeyState(VK_SHIFT)& 0x8000) controls->shiftkey = true;                // poll shift key
    else{controls->shiftkey = false;}
    if(controls->shiftkey) accel = 3;                                             // if shift key is pressed, set accel = 2
    else{accel = 1;}
    if (controls->upkey) pitchVelocity -= tv * accel;                             // decrement pitch velocity
    if (controls->downkey) pitchVelocity += tv * accel;                           // increment pitch velocity
    if (controls->leftkey) yawVelocity += tv * accel;                             // increment yaw velocity
    if (controls->rightkey) yawVelocity -= tv * accel;                            // decrement yaw velocity
    if (controls->wkey) strafe_backforward_v -= strafev * accel;                  // move forward
    if (controls->skey) strafe_backforward_v += strafev * accel;                  // move backward
    if (controls->altleftkey || controls->akey) strafe_lateral_v += strafev * accel;        // move left
    if (controls->altrightkey || controls->dkey) strafe_lateral_v -= strafev * accel;       // move right
    if (controls->pageupkey /*|| rightButton*/) strafe_updown_v += strafev * accel;   // move up
    if (controls->pagedownkey /*|| leftButton*/) strafe_updown_v -= strafev * accel;  // move down
    if(controls->rightButton) turn += tv;
    if(controls->leftButton) turn -= tv;



    view[1][1] = 0;                                                     // initialize camera vectors
    view[1][2] = -1;                                                    // ...
    view[1][3] = 0;                                                     // ...
    view[2][1] = 0;                                                     // ...
    view[2][2] = 0;                                                     // ...
    view[2][3] = -1;                                                    // ...
    view[3][1] = -1;                                                    // ...
    view[3][2] = 0;                                                     // ...
    view[3][3] = 0;                                                     // ...

    if((pitch + pitchVelocity > pi/2 && pitch <= pi/2) ||               // clamp vertical look to north / sout poles
       (pitch + pitchVelocity < -pi/2 && pitch >= -pi/2)) pitchVelocity = 0;  

    yaw += yawVelocity;                                                 // add yaw velocity to yaw
    pitch += pitchVelocity;                                             // add pitch velocity to pitch
    roll += rollVelocity;                                               // add roll velocity to roll

    for(int j = 1; j <= 3; ++j){                                        // loop
        p1 = heading(view[j][1], view[j][3]);                           // get heading
        view[j][1] = sin(p1 + roll) * dist;                             // adjust camera vector
        view[j][3] = cos(p1 + roll) * dist;                             // adjust camera vector
        p1 = heading(view[j][2], view[j][3]);                           // get heading
        view[j][2] = sin(p1 + pitch) * dist;                            // adjust camera vector
        view[j][3] = cos(p1 + pitch) * dist;                            // adjust camera vector
        p1 = heading(view[j][1], view[j][2]);                           // get heading
        view[j][1] = sin(p1 + yaw) * dist;                              // adjust camera vector
        view[j][2] = cos(p1 + yaw) * dist;                              // adjust camera vector
        view[j][4]=heading(view[j][1], view[j][2]);                     // adjust camera vector
        view[j][5]=elevation(view[j][1], view[j][2], view[j][3]);       // adjust camera vector
    }
    if(freemove){                                                               // if flying...
        eyeX += sin(view[1][4]) * sin(view[1][5]) * strafe_backforward_v +      // move eyex
                sin(view[2][4]) * sin(view[2][5]) * strafe_updown_v +
                sin(view[3][4]) * sin(view[3][5]) * strafe_lateral_v;
        eyeY += cos(view[1][4]) * sin(view[1][5]) * strafe_backforward_v +      // move eyey
                cos(view[2][4]) * sin(view[2][5]) * strafe_updown_v +
                cos(view[3][4]) * sin(view[3][5]) * strafe_lateral_v;
        eyeZ += cos(view[1][5]) * strafe_backforward_v +                        // move eyez
                cos(view[2][5]) * strafe_updown_v +
                cos(view[3][5]) * strafe_lateral_v;
    }else{                                                                      // else (not flying)
        eyeX += sin(view[1][4]) * strafe_backforward_v+                         // move eyex
                sin(view[3][4]) * strafe_lateral_v;
        eyeY += cos(view[1][4]) * strafe_backforward_v+                         // move eyey
                cos(view[3][4]) * strafe_lateral_v;
        if((controls->akey || controls->skey || controls->dkey || controls->wkey) && !jumping) bob += .24 * (1 + accel / 2);    // bob
        else{bob_height /= 1.1;}                                                // return bob to default when standing still
        if(jumpv > .1){                                                         // if jump is on the way up...
            foot += jumpv;                                                      // add jump velocity to the foot
            jumpv /= 1.25;                                                      // reduce jump velocity
        }
        if(foot > base && !grounded){                                           // if player isn't grounded...
            foot -= g;                                                          // apply gravity
            g += gravity;                                                       // ...
        }else{                                                                  // else
            g=0;                                                                // gravity = 0
            jumping = false;                                                    // set jumping = false
        }
        eyeZ = foot + sin(bob) * bob_height + playerHeight;                     // set eyez
    }

    lookX = eyeX + sin(view[1][4]) * sin(view[1][5]);                           // get look vector (X)
    lookY = eyeY + cos(view[1][4]) * sin(view[1][5]);                           // get look vector (Y)
    lookZ = eyeZ + cos(view[1][5]);                                             // get look vector (Z)

    //sideX = sin(turn);//lookX + sin(view[1][4]) * sin(view[1][5]);
    //sideY = sin(turn);//lookY + sin(view[1][4]) * sin(view[1][5]);
    //sideZ = cos(turn);//lookZ + sin(view[1][4]) * sin(view[1][5]);

    yawVelocity /= p_drag;                                                      // reduce yaw velocity
    pitchVelocity /= p_drag;                                                    // reduce pitch velocity
    rollVelocity /= p_drag;                                                     // reduce roll velocity

    strafe_lateral_v = strafe_lateral_v / strafe_drag;                          // reduce left / right velocity
    strafe_updown_v = strafe_updown_v / strafe_drag;                            // reduce up / down velocity
    strafe_backforward_v = strafe_backforward_v / strafe_drag;                  // reduce forward / back velocity
}



};


Camera camera = Camera(controls);

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

  GLuint location = glGetUniformLocation(shader_program,"rotation");
  glUniform1f(location, rotation );
  location = glGetUniformLocation(shader_program,"alpha");
  glUniform1f(location, alpha );
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
    InitShaders();
    generateScene();
    glutMainLoop();
    return 0;
}
