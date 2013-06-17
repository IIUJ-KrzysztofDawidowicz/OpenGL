#pragma once
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>                // plik nag³ówkowy dodatkowych aplikacji bibliotek OpenGL
#include "freeglut.h"
#include "controls.h"
#define _USE_MATH_DEFINES
#include <math.h>

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
	return cs > 0 ? atan(sn/cs) : M_PI + atan(sn/cs);     // return heading
	
}



// returns deviation from the equator in radians of a point relative to the origin
double elevation(double x, double y, double z){

	dist = sqrt(x * x + y * y + z * z);                // get distance of point from origin
	if(dist) return acos(z / dist);                     // if point is not coincidental with the origin, return elevation
	return 0;                                       // else return 0;
}

Controls* controls;
int *screenWidth, *screenHeight;
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
		Camera(Controls* c, int* screenWidth, int* screenHeight)
		{
			controls = c;
			this->screenWidth = screenWidth;
			this->screenHeight = screenHeight;
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


	yawVelocity -= (*screenWidth / 2 - controls->mouseX) / mouseSensitivity;
	pitchVelocity += (*screenHeight / 2 - controls->mouseY) / mouseSensitivity;
	glutWarpPointer(*screenWidth / 2, *screenHeight / 2);                 // center the mouse

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
	//if(controls->rightButton) turn += tv;
	//if(controls->leftButton) turn -= tv;



	view[1][1] = 0;                                                     // initialize camera vectors
	view[1][2] = -1;                                                    // ...
	view[1][3] = 0;                                                     // ...
	view[2][1] = 0;                                                     // ...
	view[2][2] = 0;                                                     // ...
	view[2][3] = -1;                                                    // ...
	view[3][1] = -1;                                                    // ...
	view[3][2] = 0;                                                     // ...
	view[3][3] = 0;                                                     // ...

	if((pitch + pitchVelocity > M_PI/2 && pitch <= M_PI/2) ||               // clamp vertical look to north / sout poles
	   (pitch + pitchVelocity < -M_PI/2 && pitch >= -M_PI/2)) pitchVelocity = 0;  

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
	if(/*freemove*/true){                                                               // if flying...
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

		gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, 0, 0, 1/*sideX, sideY, sideZ*/);  
}



};

