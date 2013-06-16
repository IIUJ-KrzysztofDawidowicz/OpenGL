
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>                // plik nag³ówkowy dodatkowych aplikacji bibliotek OpenGL
#include <windows.h>               // standardowy plik nag³ówkowy Windows
#include "kostka.h"
#include "freeglut.h"
#include "controls.h"


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
