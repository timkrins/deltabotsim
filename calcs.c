#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glfw.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <math.h> 
 
 const float e = 24.0;     // end effector radius
 const float f = 75.0;     // base radius
 const float re = 300.0; // length of bottom arm
 const float rf = 100.0; //length of upper arm
  // trigonometric constants
 float sqrt3;
 float pi;    // PI
 float sin120;   
 float cos120;        
 float tan60;
 float sin30;
 float tan30;
 
 // forward kinematics: (theta1, theta2, theta3) -> (x0, y0, z0)
 // returned status: 0=OK, -1=non-existing position
 
 int delta_calcForward(float theta1, float theta2, float theta3) {
     float t = (f-e)*tan30/2.0;
     float dtr = pi/(float)180.0;
  
     float y1 = -(t + rf*cos(theta1*dtr));
     float z1 = -rf*sin(theta1*dtr);

     float y2 = (t + rf*cos(theta2*dtr))*sin30;
     float x2 = y2*tan60;
     float z2 = -rf*sin(theta2*dtr);

     float y3 = (t + rf*cos(theta3*dtr))*sin30;
     float x3 = -y3*tan60;
     float z3 = -rf*sin(theta3*dtr);
 
     float dnm = (y2-y1)*x3-(y3-y1)*x2;
 
     float w1 = y1*y1 + z1*z1;
     float w2 = x2*x2 + y2*y2 + z2*z2;
     float w3 = x3*x3 + y3*y3 + z3*z3;
     
     // x = (a1*z + b1)/dnm
     float a1 = (z2-z1)*(y3-y1)-(z3-z1)*(y2-y1);
     float b1 = -((w2-w1)*(y3-y1)-(w3-w1)*(y2-y1))/2.0;
 
     // y = (a2*z + b2)/dnm;
     float a2 = -(z2-z1)*x3+(z3-z1)*x2;
     float b2 = ((w2-w1)*x3 - (w3-w1)*x2)/2.0;
 
     // a*z^2 + b*z + c = 0
     float a = a1*a1 + a2*a2 + dnm*dnm;
     float b = 2*(a1*b1 + a2*(b2-y1*dnm) - z1*dnm*dnm);
     float c = (b2-y1*dnm)*(b2-y1*dnm) + b1*b1 + dnm*dnm*(z1*z1 - re*re);
  
     // discriminant
     float d = b*b - (float)4.0*a*c;
     if (d < 0) return -1; // non-existing point
 
     float zval0 = -(float)0.5*(b+sqrt(d))/a;
     float xval0 = (a1*zval0 + b1)/dnm;
     float yval0 = (a2*zval0 + b2)/dnm;

     printf("\nresult! x(%f) y(%f) z(%f) mm\n\n", xval0, yval0, zval0);
     return 0;
 }

int main(void){
 sqrt3 = sqrt(3.0);
 pi = 3.141592653;    // PI
 sin120 = sqrt3/2.0;   
 cos120 = -0.5;        
 tan60 = sqrt3;
 sin30 = 0.5;
 tan30 = 1/sqrt3;
  
 float t1 = 5.0f;
 float t2 = 10.0f;
 float t3 = 15.0f;
 
printf("\n\nrunning the calculation! 1(%f) 2(%f) 3(%f) degrees", t1, t2, t3);
delta_calcForward(t1, t2, t3);
}