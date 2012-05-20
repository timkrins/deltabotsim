/*|_|_|_ __ _(c)|2| ___ __|_|_ __  ___ 
| _|| | '_ ` _ \|0|/ / '__|@|a'_ \/ __|
| |_| | | | | | |1  <| |  |g|i| | \__ \
 \__|_|_| |_| | |2|\ \ |  |m|l| |_|__*/
 
// This is a delta robot simulator.
// Implements OpenGL, GLFW and GLU.
// Use the controls WASDQE for viewpoint.
// Use IJOKPL to change the angles.


//  define window constants

const int window_width = 1300, window_height = 600;
const int fs_window_width = 1366, fs_window_height = 768;

// set up software variables

float colors[4];
int frame = 0;
char sprinter[50];
int holdingA, holdingB, holdingC, holdingD, holdingE, holdingF, holdingG, holdingH, holdingI, holdingJ, holdingK, holdingL, holdingM, holdingN, holdingO, holdingP, holdingQ, holdingR, holdingS, holdingT, holdingU, holdingV, holdingW, holdingX, holdingY, holdingZ;
int holding0, holding1, holding2, holding3, holding4, holding5, holding6, holding7, holding8, holding9, holding0;
float rotation = 0;
GLUquadric *quadSphere;
GLUquadric *quadCylinder;
GLUquadric *quadDisk;
GLint slices, stacks;
GLint nsides, rings;
GLint fontOffset;

// trigonometric constants

float sqrt3;
float pi;
float sin120;   
float cos120;        
float tan60;
float sin30;
float tan30;
float pion180;
float sin60;
float sin180;
float sin300;
float cos60;
float cos180;
float cos300;
float sqrt3on3;
float sqrt3on6;

// set up robot variables

float robot_top_platform_radius = 10.0f;
float robot_top_platform_thickness = 3.0f;

float robot_top_motor_length = 5.0f;
float robot_top_motor_radius = 1.5f;

float robot_top_motor_axle = 1.0f;

float robot_top_arm_radius = 0.4f;

float robot_top_arm_length = 10.0f;
float robot_top_arm_connector_radius = 0.5f;
float robot_top_arm_connector_width = 0.6f;

float robot_mid_connector_radius = 0.5f;
float robot_mid_connector_width = 5.0f;

float robot_bottom_arm_length = 20.0f;
float robot_bottom_arm_radius = 1.0f;

float robot_ee_radius = 5.0f;
float robot_ee_thickness = 0.5f;

float robot_angles[] = { 10.0f, 10.0f, 10.0f };

//float multi = 1.0f;
float y_view = 10;

// set view variables
float view_lookat_x = 0.0f;
float view_lookat_y = 0;
float view_lookat_z = 0.0f;
float view_lookfrom_divide_x = -5.0f;
float view_lookfrom_divide_y = 5.0f;
float view_lookfrom_x = 0.0f;
float view_lookfrom_y = 0.0f;
float view_lookfrom_z = 12.0f;
float view_lookfrom_x_linear = 0.0f;
float view_lookfrom_y_linear = 20.0f;
float view_lookfrom_z_linear = 0.0f;
float view_distance_from_model = 90.0f;
float view_orientation = -1;

float view_momentum_x = 0.0f;
float view_momentum_y = 0.0f;
float view_momentum_z = 0.0f;

//for calculations
// j is the centroid of the spheres. (the elbow connectors)
float j_x[3];
float j_y[3];
float j_z[3];

float j_real_x[3];
float j_real_y[3];
float j_real_z[3];

float base_x[3];
float base_y[3];
float base_z[3];

float base_calc_x[3];
float base_calc_y[3];
float base_calc_z[3];

float end_x[3];
float end_y[3];
float end_z[3];

float eereal_x[3];
float eereal_y[3];
float eereal_z[3];


float ee[3];
vec3d p1, p2, p3;
float  r1, r2, r3;

void variables_init(void){
// #######################################################
// Initialise all the variables that require calculations.
// #######################################################
sqrt3 = sqrt(3.0);
pi = 3.141592653;
sin120 = sqrt3/2.0;   
cos120 = -0.5;        
tan60 = sqrt3;
sin30 = 0.5;
tan30 = 1/sqrt3;
pion180 = (pi/180.0);
sin60 = sin(60*pion180);
sin180 =sin(180*pion180);
sin300 =sin(300*pion180);
cos60 = cos(60*pion180);
cos180 =cos(180*pion180);
cos300 =cos(300*pion180);
sqrt3on3 = sqrt(3)/3;
sqrt3on6 = sqrt(3)/6;
};