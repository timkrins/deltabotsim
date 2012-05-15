/*|_|_|_ __ ___ | | ___ __|_|_ __  ___ 
| _|| | '_ ` _ \| |/ / '__|@|a'_ \/ __|
| |_| | | | | | |   <| |  |g|i| | \__ \
 \__|_|_| |_| |_|_|\_\_|  |m|l| |_|__*/
 
// This is a delta robot simulator.
// Implements OpenGL, GLFW and GLU.
// Use the controls WASDQE for viewpoint.
// Use IJOKPL to change the angles.


//  define window constants

const int window_width = 1300, window_height = 600;
const int fs_window_width = 1366, fs_window_height = 768;

// set up software variables

float colors[4];
float current_position[16];
char sprinter[50];
int holdingW, holdingS, holdingA, holdingC, holdingD, holdingQ, holdingE, holdingI, holdingJ, holdingO, holdingK, holdingP, holdingL;
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

float robot_top_platform_triangle_radius = 10.0f;
float robot_top_platform_thickness = 3.0f;

float robot_top_motor_length = 5.0f;
float robot_top_motor_width = 7.0f;
float robot_top_motor_radius = 1.5f;
float robot_top_motor_gapfromzero = 1.0f;

float robot_top_motor_platform_thickness = 1.0f;
float robot_top_motor_platform_width = 5.0f;

float robot_top_arm_length = 10.0f;
float robot_top_arm_height = 0.9f;
float robot_top_arm_width = 0.4f;

float robot_top_arm_connector_radius = 0.5f;
float robot_top_arm_connector_width = 0.6f;

float robot_mid_connector_radius = 0.5f;
float robot_mid_connector_width = 5.0f;

float robot_bottom_arm_length = 40.0f;
float robot_bottom_arm_radius = 5.0f;

float robot_bottom_platform_triangle_radius = 5.0f;
//float robot_bottom_platform_triangle_radius = 0.0f;
float robot_bottom_platform_thickness = 3.0f;

float robot_angles[] = { 70.0f, 70.0f, 70.0f };
float robot_joint2[3];
float robot_arm_first_angles[] = {0,0,0}; // the side on rotation
float robot_arm_second_angles[] = {0,0,0}; // the forward facing rotation

float robot_end_effector[3];
//float robot_end_effector_x;
//float robot_end_effector_y;
//float robot_end_effector_z;

float multi = 1.0f;

// set view variables
float view_lookat_x = 0.0f;
float view_lookat_y = -5.0f;
float view_lookat_z = 0.0f;
float view_lookfrom_divide_x = -5.0f;
float view_lookfrom_divide_y = 5.0f;
float view_lookfrom_x = 0.0f;
float view_lookfrom_y = 0.0f;
float view_lookfrom_z = 12.0f;
float view_lookfrom_x_linear = 0.0f;
float view_lookfrom_y_linear = 0.0f;
float view_lookfrom_z_linear = 0.0f;
float view_distance_from_model = 50.0f;
float view_orientation = -1;

float view_momentum_x = 0.0f;
float view_momentum_y = 0.0f;
float view_momentum_z = 0.0f;

float color_red;
float color_green;
float color_blue;

// set up preset vertexes

float v_x[20];
float v_y[20];
float v_z[20];

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

float top_eqns[3];
float btm_eqns[3];
float entire_eqn[3];

float ee[3];
vec3d p1, p2, p3;
float  r1, r2, r3;
float rotation_2;

float robot_plat_effector[3][3];
//float robot_plat_effector_2[3];
//float robot_plat_effector_3[3];

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

void vertex_init(void){
// ###################################################################
// Calculate and store vertex positions to save processor computation.
// ###################################################################
v_x[0] = 0;
v_y[0] = 0;
v_z[0] = 0;

v_x[1] = robot_top_platform_triangle_radius*cos60;
v_y[1] = v_y[0];
v_z[1] = -sin60*robot_top_platform_triangle_radius;

v_x[2] = v_x[1];
v_y[2] = v_y[0];
v_z[2] = sin60*robot_top_platform_triangle_radius;

v_x[3] = v_x[1];
v_y[3] = -robot_top_platform_thickness;
v_z[3] = v_z[2];

v_x[4] = v_x[1];
v_y[4] = v_y[3];
v_z[4] = v_z[1];

v_x[5] = v_x[0];
v_y[5] = v_y[3];
v_z[5] = v_z[0];

v_x[6] = v_x[1];
v_y[6] = v_y[3];
v_z[6] = v_z[0] - robot_top_motor_gapfromzero;

v_x[7] = v_x[1];
v_y[7] = v_y[6] + robot_top_motor_platform_thickness;
v_z[7] = v_z[6];

v_x[8] = v_x[1];
v_y[8] = v_y[7];
v_z[8] = v_z[1];

v_x[9] = v_x[8] + robot_top_motor_platform_width;
v_y[9] = v_y[7];
v_z[9] = v_z[4];

v_x[10] = v_x[9];
v_y[10] = v_y[4];
v_z[10] = v_z[4];

v_x[11] = v_x[9];
v_y[11] = v_y[4];
v_z[11] = v_z[6];

v_x[12] = v_x[9];
v_y[12] = v_y[7];
v_z[12] = v_z[6];
/*
v_x[REPLACE] = REPLACE;
v_y[REPLACE] = REPLACE;
v_z[REPLACE] = REPLACE;

v_x[REPLACE] = REPLACE;
v_y[REPLACE] = REPLACE;
v_z[REPLACE] = REPLACE;
*/
}
