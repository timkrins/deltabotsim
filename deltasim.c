/*|_|_|_ __ _(c)|2| ___ __|_|_ __  ___ 
| _|| | '_ ` _ \|0|/ / '__|@|a'_ \/ __|
| |_| | | | | | |1  <| |  |g|i| | \__ \
 \__|_|_| |_| | |2|\ \ |  |m|l| |_|__*/

// ###################################
// This is a delta robot simulator.
// Implements OpenGL, GLFW and GLU.
// Use the controls WASDQE for viewpoint.
// Use IJOKPL to change the angles.
// ###################################

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glfw.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <math.h>
#define   MAXZERO  0.0

// protoype software functions

void quit(void);
void variables_init(void);
void graphics_init(void);
void graphics_draw(void);
void graphics_loop(void);
void handleMouseClick(int button, int action);
void handleKeypress(int theKey, int theAction);
int calculate_delta(void);

// prototype the drawing functions

void draw_text_layers(void);
void draw_delta_robot(void);

//build structs

typedef struct vec3d    vec3d;
struct vec3d {
        float  x;
        float  y;
        float  z;
};

//##################################################
// Bring in external source files.
// I have orginised these to make it easier to code.
// #################################################
#include "constants.c"
#include "shapes.c"
#include "robot.c"
#include "text.c"

vec3d vdiff(const vec3d vector1, const vec3d vector2){
/* Return the difference of two vectors, (vector1 - vector2). */
        vec3d v;
        v.x = vector1.x - vector2.x;
        v.y = vector1.y - vector2.y;
        v.z = vector1.z - vector2.z;
        return v;
}
 

vec3d vsum(const vec3d vector1, const vec3d vector2){
/* Return the sum of two vectors. */
        vec3d v;
        v.x = vector1.x + vector2.x;
        v.y = vector1.y + vector2.y;
        v.z = vector1.z + vector2.z;
        return v;
}
 

vec3d vmul(const vec3d vector, const double n){
/* Multiply vector by a number. */
        vec3d v;
        v.x = vector.x * n;
        v.y = vector.y * n;
        v.z = vector.z * n;
        return v;
}
 

vec3d vdiv(const vec3d vector, const double n){
/* Divide vector by a number. */
        vec3d v;
        v.x = vector.x / n;
        v.y = vector.y / n;
        v.z = vector.z / n;
        return v;
}
 
double vnorm(const vec3d vector){
/* Return the Euclidean norm. */
        return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}
 
double dot(const vec3d vector1, const vec3d vector2){
/* Return the dot product of two vectors. */
        return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}
 

vec3d cross(const vec3d vector1, const vec3d vector2){
/* Replace vector with its cross product with another vector. */
        vec3d v;
        v.x = vector1.y * vector2.z - vector1.z * vector2.y;
        v.y = vector1.z * vector2.x - vector1.x * vector2.z;
        v.z = vector1.x * vector2.y - vector1.y * vector2.x;
        return v;
}

int trilateration(vec3d *const result1, vec3d *const result2,
                  const vec3d p1, const double r1,
                  const vec3d p2, const double r2,
                  const vec3d p3, const double r3,
                  const double maxzero){
/* Return zero if successful, negative error otherwise.
 * The last parameter is the largest nonnegative number considered zero;
 * it is somewhat analoguous to machine epsilon (but inclusive).
*/
                  
        vec3d   ex, ey, ez, t1, t2;
        double  h, i, j, x, y, z, t;
 
        /* h = |p2 - p1|, ex = (p2 - p1) / |p2 - p1| */
        ex = vdiff(p2, p1);
        h = vnorm(ex);
        if (h <= maxzero) {
                /* p1 and p2 are concentric. */
                return -1;
        }
        ex = vdiv(ex, h);
 
        /* t1 = p3 - p1, t2 = ex (ex . (p3 - p1)) */
        t1 = vdiff(p3, p1);
        i = dot(ex, t1);
        t2 = vmul(ex, i);
 
        /* ey = (t1 - t2), t = |t1 - t2| */
        ey = vdiff(t1, t2);
        t = vnorm(ey);
        if (t > maxzero) {
                /* ey = (t1 - t2) / |t1 - t2| */
                ey = vdiv(ey, t);
 
                /* j = ey . (p3 - p1) */
                j = dot(ey, t1);
        } else
                j = 0.0;
 
        /* Note: t <= maxzero implies j = 0.0. */
        if (fabs(j) <= maxzero) {
                /* p1, p2 and p3 are colinear. */
 
                /* Is point p1 + (r1 along the axis) the intersection? */
                t2 = vsum(p1, vmul(ex, r1));
                if (fabs(vnorm(vdiff(p2, t2)) - r2) <= maxzero &&
                    fabs(vnorm(vdiff(p3, t2)) - r3) <= maxzero) {
                        /* Yes, t2 is the only intersection point. */
                        if (result1)
                                *result1 = t2;
                        if (result2)
                                *result2 = t2;
                        return 0;
                }
 
                /* Is point p1 - (r1 along the axis) the intersection? */
                t2 = vsum(p1, vmul(ex, -r1));
                if (fabs(vnorm(vdiff(p2, t2)) - r2) <= maxzero &&
                    fabs(vnorm(vdiff(p3, t2)) - r3) <= maxzero) {
                        /* Yes, t2 is the only intersection point. */
                        if (result1)
                                *result1 = t2;
                        if (result2)
                                *result2 = t2;
                        return 0;
                }
 
                return -2;
        }
 
        /* ez = ex x ey */
        ez = cross(ex, ey);
 
        x = (r1*r1 - r2*r2) / (2*h) + h / 2;
        y = (r1*r1 - r3*r3 + i*i) / (2*j) + j / 2 - x * i / j;
        z = r1*r1 - x*x - y*y;
        if (z < -maxzero) {
                /* The solution is invalid. */
                return -3;
        } else
        if (z > 0.0)
                z = sqrt(z);
        else
                z = 0.0;
 
        /* t2 = p1 + x ex + y ey */
        t2 = vsum(p1, vmul(ex, x));
        t2 = vsum(t2, vmul(ey, y));
 
        /* result1 = p1 + x ex + y ey + z ez */
        if (result1)
                *result1 = vsum(t2, vmul(ez, z));
 
        /* result1 = p1 + x ex + y ey - z ez */
        if (result2)
                *result2 = vsum(t2, vmul(ez, -z));
 
        return 0;
}

vec3d   o1, o2;

int calculate_vectors(vec3d p1, vec3d p2, vec3d p3, double r1, double r2, double r3){
        int     result;
                result = trilateration(&o1, &o2, p1, r1, p2, r2, p3, r3, MAXZERO);
                if (result){
                        printf("No solution. Please check your robot dimensions.\n", result);
                        quit();
                };
};

int calculate_delta(void) {
// ##################################################################
// Calculate the position of the end effector using the angles given.
// This technique is called 'Forward Kinematics'.
// This code was borrowed from:
// http://forums.trossenrobotics.com/tutorials/introduction-129/delta-robot-kinematics-3276/
// ##################################################################
/*

THIS CODE WAS ACTUALLY RUBBISH LOL

float t = (robot_top_platform_radius-robot_ee_radius)*tan30/2.0; float dtr = pi/(float)180.0;
float y1 = -(t + robot_top_arm_length*cos(theta1*dtr));float z1 = -robot_top_arm_length*sin(theta1*dtr);
float y2 = (t + robot_top_arm_length*cos(theta2*dtr))*sin30;float x2 = y2*tan60;
float z2 = -robot_top_arm_length*sin(theta2*dtr);float y3 = (t + robot_top_arm_length*cos(theta3*dtr))*sin30;
float x3 = -y3*tan60;float z3 = -robot_top_arm_length*sin(theta3*dtr);
float dnm = (y2-y1)*x3-(y3-y1)*x2;float w1 = y1*y1 + z1*z1;
float w2 = x2*x2 + y2*y2 + z2*z2;float w3 = x3*x3 + y3*y3 + z3*z3;
float a1 = (z2-z1)*(y3-y1)-(z3-z1)*(y2-y1);float b1 = -((w2-w1)*(y3-y1)-(w3-w1)*(y2-y1))/2.0;
float a2 = -(z2-z1)*x3+(z3-z1)*x2;float b2 = ((w2-w1)*x3 - (w3-w1)*x2)/2.0;
float a = a1*a1 + a2*a2 + dnm*dnm;float b = 2*(a1*b1 + a2*(b2-y1*dnm) - z1*dnm*dnm);
float c = (b2-y1*dnm)*(b2-y1*dnm) + b1*b1 + dnm*dnm*(z1*z1 - robot_bottom_arm_length*robot_bottom_arm_length);
float d = b*b - (float)4.0*a*c;if (d < 0) return -1;
float zval0 = -(float)0.5*(b+sqrt(d))/a;float xval0 = (a1*zval0 + b1)/dnm;
float yval0 = (a2*zval0 + b2)/dnm;

*/

// ########################################################################
// Calculate the values for the base, base_xyz, the joint positions, j_xyz.
// ########################################################################
for (int i=0; i < 3; i++){ // run through each slice
rotation = i*120;

//these real positions are only used for information.
base_x[i] = (cos(rotation*pion180)) * ((robot_top_platform_radius));
base_y[i] = (sin(rotation*pion180)) * ((robot_top_platform_radius));
base_z[i] = 0;

//these are 'fake' positions, used for calculations.
base_calc_x[i] = (cos(rotation*pion180)) * ((robot_top_platform_radius-robot_ee_radius));
base_calc_y[i] = (sin(rotation*pion180)) * ((robot_top_platform_radius-robot_ee_radius));
base_calc_z[i] = 0;

//these are 'fake' positions, used for calculations.
j_x[i] = (((cos(rotation*pion180)))*(robot_top_arm_length*cos(robot_angles[i]*pion180)))+base_calc_x[i];
j_y[i] = (((sin(rotation*pion180)))*(robot_top_arm_length*cos(robot_angles[i]*pion180)))+base_calc_y[i];
j_z[i] = 0 - (robot_top_arm_length*sin(robot_angles[i]*pion180));


// These real position are not used for calculations, only for information.
j_real_x[i] = (((cos(rotation*pion180)))*(robot_top_arm_length*cos(robot_angles[i]*pion180)))+base_x[i];
j_real_y[i] = (((sin(rotation*pion180)))*(robot_top_arm_length*cos(robot_angles[i]*pion180)))+base_y[i];
j_real_z[i] = 0 - (robot_top_arm_length*sin(robot_angles[i]*pion180));
};

// For use with the vector calculator.
p1.x = j_x[0];
p1.y = j_y[0];
p1.z = j_z[0];
r1 = robot_bottom_arm_length;
p2.x = j_x[1];
p2.y = j_y[1];
p2.z = j_z[1];
r2 = robot_bottom_arm_length;
p3.x = j_x[2];
p3.y = j_y[2];
p3.z = j_z[2];
r3 = robot_bottom_arm_length;

//
// Calculate the vector positions.
// Changes o1 and o2 vectors only.
//
calculate_vectors(p1, p2, p3, r1, r2, r3);


//
// Determines the lowest Z value of the vector calculations, and sets the End Effector positions.
//
if (o1.z < 0){
ee[0] = o1.x;
ee[1] = o1.y;
ee[2] = o1.z;
} else {
ee[0] = o2.x;
ee[1] = o2.y;
ee[2] = o2.z;
}

for (int i=0; i < 3; i++){
rotation = i*120;

//
// Calculates the real end-effector joint positions, used for calculating the elusive 4th angle.
//
eereal_x[i] = ((cos(rotation*(3.1415/180.0)))*(robot_ee_radius))+ee[0];
eereal_y[i] = ((sin(rotation*(3.1415/180.0)))*(robot_ee_radius))+ee[1];
eereal_z[i] = ee[2];
}

return 0;
};
 
void quit(void){
// #####################
// Quits the application
// #####################
glfwTerminate();
exit(0);
}

void graphics_init(void) {
// ###############################
// Initialise the graphics window.
// ###############################
if (glfwInit() != GL_TRUE) {
  quit();
}
glfwOpenWindowHint( GLFW_FSAA_SAMPLES, 4);
if(glfwOpenWindow( window_width,window_height,8,8,8,8,24,8,GLFW_WINDOW ) != GL_TRUE) {
  quit();
}
glfwOpenWindowHint( GLFW_FSAA_SAMPLES, 4);
glEnable( GL_MULTISAMPLE );
glEnable( GL_BLEND );
glEnable( GL_DEPTH_TEST );
glfwSetWindowTitle("Delta Robot Simulator by Tim K");
glMatrixMode(GL_PROJECTION);
glPolygonMode(GL_FRONT, GL_FILL);
glPolygonMode(GL_BACK, GL_FILL);
glLoadIdentity();
float aspect_ratio = ((float)window_height) / window_width;

glFrustum(.5, -.5, -.5 * aspect_ratio, .5 * aspect_ratio, 1, 500);

glMatrixMode(GL_MODELVIEW);
glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); // Preset color
makeBitmapFonts();
quadCylinder=gluNewQuadric();
quadSphere=gluNewQuadric();
quadDisk=gluNewQuadric();
glfwSetKeyCallback(handleKeypress);
};


void handleKeypress(int theKey, int theAction){
// #################################################
// Process the keyboard callback and set key states.
// #################################################
if (theAction == GLFW_PRESS){
    switch(theKey){
    case 'A': holdingA = 1; break;
    case 'B': holdingB = 1; break;
    case 'C': holdingC = 1; break;
    case 'D': holdingD = 1; break;
    case 'E': holdingE = 1; break;
    case 'F': holdingF = 1; break;
    case 'G': holdingG = 1; break;
    case 'H': holdingH = 1; break;
    case 'I': holdingI = 1; break;
    case 'J': holdingJ = 1; break;
    case 'K': holdingK = 1; break;
    case 'L': holdingL = 1; break;
    case 'M': holdingM = 1; break;
    case 'N': holdingN = 1; break;
    case 'O': holdingO = 1; break;
    case 'P': holdingP = 1; break;
    case 'Q': holdingQ = 1; break;
    case 'R': holdingR = 1; break;
    case 'S': holdingS = 1; break;
    case 'T': holdingT = 1; break;
    case 'U': holdingU = 1; break;
    case 'V': holdingV = 1; break;
    case 'W': holdingW = 1; break;
    case 'X': holdingX = 1; break;
    case 'Y': holdingY = 1; break;
    case 'Z': holdingZ = 1; break;
    case '1': holding1 = 1; break;
    case '2': holding2 = 1; break;
    case '3': holding3 = 1; break;
    case '4': holding4 = 1; break;
    case '5': holding5 = 1; break;
    case '6': holding6 = 1; break;
    case '7': holding7 = 1; break;
    case '8': holding8 = 1; break;
    case '9': holding9 = 1; break;
    case '0': holding0 = 1; break;
    
    default: break;
    }
  } else {
    switch(theKey){
    case 'A': holdingA = 0; break;
    case 'B': holdingB = 0; break;
    case 'C': holdingC = 0; break;
    case 'D': holdingD = 0; break;
    case 'E': holdingE = 0; break;
    case 'F': holdingF = 0; break;
    case 'G': holdingG = 0; break;
    case 'H': holdingH = 0; break;
    case 'I': holdingI = 0; break;
    case 'J': holdingJ = 0; break;
    case 'K': holdingK = 0; break;
    case 'L': holdingL = 0; break;
    case 'M': holdingM = 0; break;
    case 'N': holdingN = 0; break;
    case 'O': holdingO = 0; break;
    case 'P': holdingP = 0; break;
    case 'Q': holdingQ = 0; break;
    case 'R': holdingR = 0; break;
    case 'S': holdingS = 0; break;
    case 'T': holdingT = 0; break;
    case 'U': holdingU = 0; break;
    case 'V': holdingV = 0; break;
    case 'W': holdingW = 0; break;
    case 'X': holdingX = 0; break;
    case 'Y': holdingY = 0; break;
    case 'Z': holdingZ = 0; break;
    case '1': holding1 = 0; break;
    case '2': holding2 = 0; break;
    case '3': holding3 = 0; break;
    case '4': holding4 = 0; break;
    case '5': holding5 = 0; break;
    case '6': holding6 = 0; break;
    case '7': holding7 = 0; break;
    case '8': holding8 = 0; break;
    case '9': holding9 = 0; break;
    case '0': holding0 = 0; break;
    
    default: break;
    };
  };
};

void update_view(void){
// #############################################################
// These functions implement the momentum of the camera.
//
// In a perfect world where I have infinity time and motivation,
// I would like to have used the Boost Preprocessor library
// and create these functions using a macro.
// but for now, and for simplicitys sake, copy and paste yeah!
// #############################################################
view_lookfrom_x_linear += view_momentum_x;
if((view_momentum_x > 0.01)||(view_momentum_x < -0.01)) {
  view_momentum_x = view_momentum_x/1.05;
} else {
  view_momentum_x = 0.0;
};

view_lookfrom_y_linear += view_momentum_y;
if((view_momentum_y > 0.01)||(view_momentum_y < -0.01)) {
  view_momentum_y = view_momentum_y/1.05;
} else {
  view_momentum_y = 0.0;
};

view_distance_from_model += view_momentum_z;
if((view_momentum_z > 0.01)||(view_momentum_z < -0.01)) {
  view_momentum_z = view_momentum_z/1.05;
} else {
  view_momentum_z = 0.0;
};
    
// ##########################
// Don't get too close now...
// ##########################
    
if(view_distance_from_model < 10){
  view_momentum_z = 0;
  view_distance_from_model = 10;
};
// #######################
// These keys add momentum
// #######################
if(holdingA) { view_momentum_x += 0.1; };
if(holdingD) { view_momentum_x -= 0.1; };
if(holdingS) { view_momentum_y += 0.1; };
if(holdingW) { view_momentum_y -= 0.1; };
if(holdingQ) { view_momentum_z +=0.1; };
if(holdingE) { view_momentum_z -=0.1; };
    
// ##########################################################
// This code here flips the up direction so the model doesn't
// spaz out when you pass directly over it.
// ##########################################################
if((view_lookfrom_y_linear > 90)||(view_lookfrom_y_linear < -90)){
  view_orientation = -1;
} else {
  view_orientation = 1;
};

if(view_lookfrom_y_linear > 180){
  view_orientation = -1;
  view_lookfrom_y_linear = -179.9;
} else {
  if(view_lookfrom_y_linear < -180){
    view_orientation = -1;
    view_lookfrom_y_linear = 179.9;
  };
};
if(view_lookfrom_x_linear > 180){
  view_lookfrom_x_linear = -179.9;
} else {
  if(view_lookfrom_x_linear < -180){
    view_lookfrom_x_linear = 179.9;
  };
};
   
// #############################
// Changes the angles with keys.
// #############################
int maxangle = 95;
int minangle = -60;

if(holdingI) { if(robot_angles[0] < maxangle) {robot_angles[0] +=0.5; }; };
if(holdingJ) { if(robot_angles[0] > minangle) {robot_angles[0] -=0.5; }; };

if(holdingO) { if(robot_angles[1] < maxangle) {robot_angles[1] +=0.5; }; };
if(holdingK) { if(robot_angles[1] > minangle) {robot_angles[1] -=0.5; }; };

if(holdingP) { if(robot_angles[2] < maxangle) {robot_angles[2] +=0.5; };};
if(holdingL) { if(robot_angles[2] > minangle) {robot_angles[2] -=0.5; }; };

if(holding0) {robot_angles[0] = 0; robot_angles[1] = 0; robot_angles[2] = 0;};

if(holding1) {robot_angles[0] = 70;};
if(holding2) {robot_angles[1] = 70;};
if(holding3) {robot_angles[2] = 70;};

if(holding4) {robot_angles[0] = 45;};
if(holding5) {robot_angles[1] = 45;};
if(holding6) {robot_angles[2] = 45;};

if(holding7) {robot_angles[0] = -50;};
if(holding8) {robot_angles[1] = -50;};
if(holding9) {robot_angles[2] = -50;};

if(holdingB) {robot_angles[0] = -15;};
if(holdingN) {robot_angles[1] = -10;};
if(holdingM) {robot_angles[2] = -5;};

if(holdingZ) {y_view += 0.5;};
if(holdingX) {y_view -= 0.5;};

// ###################################
// Calculate the camera's coordinates.
// ###################################
view_lookfrom_x = cos(view_lookfrom_x_linear*pion180)*cos(view_lookfrom_y_linear*pion180)*view_distance_from_model;
view_lookfrom_y = sin(view_lookfrom_y_linear*pion180)*view_distance_from_model;
view_lookfrom_z = cos(view_lookfrom_y_linear*pion180)*sin(view_lookfrom_x_linear*pion180)*view_distance_from_model;
};

void graphics_draw(void) {
// ###############################################
// Positions the camera and renders to the buffer.
// ###############################################
glLoadIdentity();
update_view();
//view_lookat_y -= robot_top_arm_length;
//view_lookfrom_y -= robot_top_arm_length;
gluLookAt(view_lookfrom_x, view_lookfrom_y - y_view, view_lookfrom_z,view_lookat_x, view_lookat_y - y_view, view_lookat_z,0.0f, view_orientation, 0.0f);

// !!!!!!!!!!!!!!!!!!!!!!
// Draw all things here.
// !!!!!!!!!!!!!!!!!!!!!!

draw_delta_robot();
draw_text_layers();

// !!!!!!!!!!!!!!!!!!!!!!!!!
// Stop drawing things here.
// !!!!!!!!!!!!!!!!!!!!!!!!!

};

void graphics_loop(void) {
// #############################################################
// This is the loop that swaps the buffers and draws the screen.
// #############################################################
while(glfwGetWindowParam(GLFW_OPENED)){
  glfwSleep(0.01);
  if(glfwGetWindowParam(GLFW_ACTIVE) == GL_FALSE){
  glfwSleep(0.5);
  };
  frame++;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  graphics_draw();
  glfwSwapBuffers();
  };
};

int main(void){
// ######################################################
// This is the main loop, the entry point of the program.
// ######################################################
variables_init();
graphics_init();
graphics_loop();
quit();
};


