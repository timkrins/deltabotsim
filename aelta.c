/*|_|_|_ __ ___ | | ___ __|_|_ __  ___ 
| _|| | '_ ` _ \| |/ / '__|@|a'_ \/ __|
| |_| | | | | | |   <| |  |g|i| | \__ \
 \__|_|_| |_| |_|_|\_\_|  |m|l| |_|__*/
 
// This is a delta robot simulator.
// Implements OpenGL, GLFW and GLU.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glfw.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <math.h>

// protoype software functions

void quit(void);
void variables_init(void);
void graphics_init(void);
void graphics_draw(void);
void graphics_loop(void);
void handleMouseClick(int button, int action);
void handleKeypress(int theKey, int theAction);
int calculate_delta(float theta1, float theta2, float theta3);


// prototype various drawing functions

void draw_text_layers(void);
void draw_delta_robot(void);
  void draw_top_base(void);
    void draw_topbase_full_top(void);
    void draw_topbase_full_sides(void);
      void draw_topbase_onethird_sides(void);
    void draw_topbase_full_motors(void);
      void draw_topbase_onethird_motors(void);
    void draw_topbase_full_bottom(void);
  void draw_bottom_base(void);
    void draw_botbase_full_top(void);
    void draw_botbase_full_sides(void);
      void draw_botbase_onethird_sides(void);
    void draw_botbase_full_bottom(void);
  void draw_arms(void);
    void draw_arms_top_full_motor_joiner(void);
    void draw_arms_top_full_arm(void);
    void draw_arms_mid_full_joiner(void);
    void draw_arms_bottom_full_arms(void);
      void draw_arms_bottom_onehalf_arms(void);
    void draw_arms_bottom_full_joiner(void);

//  define constants

const int window_width = 1300, window_height = 600;
const int fs_window_width = 1366, fs_window_height = 768;



// set up software variables

int startxMouse, startyMouse, xMouse, yMouse, h;
float colors[4];
char sprinter[50];
int clicked = 0;
int holdingW, holdingS, holdingA, holdingD, holdingQ, holdingE, holdingI, holdingJ, holdingO, holdingK, holdingP, holdingL;
GLUquadric *quadSphere;
GLUquadric *quadCylinder;
GLUquadric *quadDisk;
GLint slices, stacks;
GLint nsides, rings;

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
float robot_top_motor_gapfromzero = 1.0f;
float robot_top_motor_platform_thickness = 1.0f;
float robot_top_arm_length = 10.0f;

float robot_top_arm_height = 0.9f;
float robot_top_arm_width = 0.4f;

float robot_top_arm_connector_radius = 0.5f;
float robot_top_arm_connector_width = 0.6f;

float robot_mid_connector_radius = 0.5f;
float robot_mid_connector_width = 5.0f;

float robot_bottom_arm_length = 20.0f;
float robot_bottom_arm_radius = 5.0f;

float robot_bottom_platform_triangle_radius = 10.0f;
float robot_bottom_platform_thickness = 3.0f;

float robot_angle_one = 20.0f;
float robot_angle_two = 40.0f;
float robot_angle_three = 60.0f;

float robot_top_platform_triangle_v1_x;
float robot_top_platform_triangle_v1_y;
float robot_top_platform_triangle_v1_z;
float robot_top_platform_triangle_v2_x;
float robot_top_platform_triangle_v2_y;
float robot_top_platform_triangle_v2_z;
float robot_top_platform_triangle_v3_x;
float robot_top_platform_triangle_v3_y;
float robot_top_platform_triangle_v3_z;
float robot_top_platform_triangle_sidelength;
float robot_top_platform_triangle_inscribed;

float startrotation = 0.0f;

int value[3];

float robot_end_effector_x;
float robot_end_effector_y;
float robot_end_effector_z;

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

// set up bitmap font

GLubyte bitmap_font[][13] = {
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}, 
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36}, 
{0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00}, 
{0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18}, 
{0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70}, 
{0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38}, 
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e}, 
{0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c}, 
{0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30}, 
{0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00}, 
{0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00}, 
{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03}, 
{0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c}, 
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18}, 
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e}, 
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e}, 
{0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c}, 
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff}, 
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e}, 
{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff}, 
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e}, 
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e}, 
{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06}, 
{0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60}, 
{0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e}, 
{0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18}, 
{0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe}, 
{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e}, 
{0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc}, 
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff}, 
{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff}, 
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e}, 
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, 
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e}, 
{0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06}, 
{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3}, 
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0}, 
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3}, 
{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3}, 
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e}, 
{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe}, 
{0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c}, 
{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe}, 
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e}, 
{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff}, 
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, 
{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, 
{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, 
{0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3}, 
{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3}, // Y
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff}, // Z
{0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c}, 
{0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60}, 
{0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c}, 
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18}, 
{0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70}, 
{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0}, 
{0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03}, 
{0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e}, 
{0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0}, 
{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00}, 
{0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00}, 
{0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0}, 
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78}, 
{0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00}, 
{0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00}, 
{0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x00}, 
{0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00}, 
{0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00}, 
{0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f}, 
{0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}, 
{0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0}, 
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00} 
};

void variables_init(void){

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

robot_top_platform_triangle_v1_x = robot_top_platform_triangle_radius*sin180;
robot_top_platform_triangle_v1_y = 0.0f;
robot_top_platform_triangle_v1_z = robot_top_platform_triangle_radius*cos180;

robot_top_platform_triangle_v2_x = robot_top_platform_triangle_radius*sin60;
robot_top_platform_triangle_v2_y = 0.0f;
robot_top_platform_triangle_v2_z = robot_top_platform_triangle_radius*cos60;

robot_top_platform_triangle_v3_x = robot_top_platform_triangle_radius*sin300;
robot_top_platform_triangle_v3_y = 0.0f;
robot_top_platform_triangle_v3_z = robot_top_platform_triangle_radius*cos300;

robot_top_platform_triangle_sidelength = robot_top_platform_triangle_radius / sqrt3on3;
robot_top_platform_triangle_inscribed = sqrt((robot_top_platform_triangle_radius*robot_top_platform_triangle_radius)-((robot_top_platform_triangle_sidelength/2)*(robot_top_platform_triangle_sidelength/2)));

};

GLint fontOffset;

void makeBitmapFonts(void)
{
    GLint i;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    fontOffset = glGenLists(128);
    for (i = 32; i < 127; i++) {
        glNewList(i+fontOffset, GL_COMPILE);
            glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, bitmap_font[i-32]);
        glEndList();
    }
}

int calculate_delta(float theta1, float theta2, float theta3) {
     float t = (robot_top_platform_triangle_radius-robot_bottom_platform_triangle_radius)*tan30/2.0; float dtr = pi/(float)180.0;
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
     robot_end_effector_x = xval0;
     robot_end_effector_y = yval0;
     robot_end_effector_z = zval0;
     return 0;
 }


void quit(void)
{
  glfwTerminate();
  exit(0);
}//end-of-quit

void graphics_init(void) {
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
  //glClearColor( 0.1f, 0.1f, 0.1f, 0.0f ); // Preset color
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); // Preset color
  makeBitmapFonts();
  quadCylinder=gluNewQuadric();
  quadSphere=gluNewQuadric();
  quadDisk=gluNewQuadric();
  glfwSetKeyCallback(handleKeypress);
  glfwSetMouseButtonCallback(handleMouseClick);
  
}//end-of-graphics-initialise

void printString(char *s)
{
   glPushAttrib (GL_LIST_BIT);
   glListBase(fontOffset);
   glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *) s);
   glPopAttrib();
}

void handleMouseClick(int button, int action){
if(action == GLFW_PRESS){
clicked = 1;
} else {
clicked = 0;
};
};

void handleKeypress(int theKey, int theAction)
{
    // If a key is pressed, toggle the relevant key-press flag
      if (theAction == GLFW_PRESS)
    {
        switch(theKey)
        {
        case 'W':
            holdingW = 1;
            break;
        case 'S':
            holdingS = 1;
            break;
        case 'A':
            holdingA = 1;
            break;
        case 'D':
            holdingD = 1;
            break;
        case 'Q':
            holdingQ = 1;
            break;
        case 'E':
            holdingE = 1;
            break;
        case 'P':
            holdingP = 1;
                    break;
        case 'L':
            holdingL = 1;
                    break;
        case 'O':
            holdingO = 1;
                    break;
        case 'K':
            holdingK = 1;
                    break;
        case 'I':
            holdingI = 1;
                    break;
        case 'J':
            holdingJ = 1;
                    break;
        default:
            // Do nothing...
            break;
        }
    }
    else // If a key is released, toggle the relevant key-release flag
    {
        switch(theKey)
        {
        case 'W':
            holdingW = 0;
            break;
        case 'S':
            holdingS = 0;
            break;
        case 'A':
            holdingA = 0;
            break;
        case 'D':
            holdingD = 0;
            break;
        case 'Q':
            holdingQ = 0;
            break;
        case 'E':
            holdingE = 0;
            break;
        case 'P':
            holdingP = 0;
                    break;
        case 'L':
            holdingL = 0;
                    break;
        case 'O':
            holdingO = 0;
                    break;
        case 'K':
            holdingK = 0;
                    break;
        case 'I':
            holdingI = 0;
                    break;
        case 'J':
            holdingJ = 0;
        default:
            // Do nothing...
            break;
        }
    }
}

void update_view(void){

    //use momentum
    
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

    if(view_distance_from_model < 10){
    view_momentum_z = 0;
    view_distance_from_model = 10;
    }
    view_distance_from_model += view_momentum_z;
    if((view_momentum_z > 0.01)||(view_momentum_z < -0.01)) {
    view_momentum_z = view_momentum_z/1.05;
    } else {
    view_momentum_z = 0.0;
    };


    if(holdingA) {
    view_momentum_x += 0.1;
    };
    if(holdingD) {
    view_momentum_x -= 0.1;
    };
    if(holdingS) {
    view_momentum_y += 0.1;
    //view_lookfrom_y_linear += 1;
    };
    if(holdingW) {
    view_momentum_y -= 0.1;
    //view_lookfrom_y_linear -= 1;
    };
    view_orientation = 1; 
    
    if((view_lookfrom_y_linear > 90)||(view_lookfrom_y_linear < -90)){
    view_orientation = -1;
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
   
    if(holdingQ) {
    //view_distance_from_model += 1;
    view_momentum_z +=0.1;
    };
    if(holdingE) {
    //view_distance_from_model -= 1;
    view_momentum_z -=0.1;
    };
   
    if(holdingP) {
    robot_angle_three +=0.5;
    };
    
    if(holdingL) {
    robot_angle_three -=0.5;
    };
    
    if(holdingO) {
    robot_angle_two +=0.5;
    };
   
    if(holdingK) {
    robot_angle_two -=0.5;
    };
    
    if(holdingI) {
    robot_angle_one +=0.5;
    };
    
    if(holdingJ) {
    robot_angle_one -=0.5;
    };

    // so 'linear' should go from -180 to 180, starting at zero.
    // it will generate a position about the origin

    view_lookfrom_x = cos(view_lookfrom_x_linear*pion180)*cos(view_lookfrom_y_linear*pion180)*view_distance_from_model;
    view_lookfrom_y = sin(view_lookfrom_y_linear*pion180)*view_distance_from_model;
    view_lookfrom_z = cos(view_lookfrom_y_linear*pion180)*sin(view_lookfrom_x_linear*pion180)*view_distance_from_model;
 
}

void graphics_draw(void) {
    glLoadIdentity();
    update_view();
   	glTranslatef(0, 0, 0);  // View scrolls with MouseWheel
    gluLookAt(
    view_lookfrom_x, view_lookfrom_y, view_lookfrom_z,
    view_lookat_x, view_lookat_y, view_lookat_z,
    0.0f, view_orientation, 0.0f
    );
		
		
		//draw things here
    //###################################################


    draw_delta_robot();
    draw_text_layers();




    //####################################################
    //end drawing things

}//end-of-graphics-draw

void graphics_loop(void) {
  while(glfwGetWindowParam(GLFW_ACTIVE))
  {
  glfwSleep(0.008);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  graphics_draw();
  glfwSwapBuffers();
  }
}//end-of-major-loop

int main(void)
{
  variables_init();
  graphics_init();
  graphics_loop();
  quit();
  
}//end-of-main

void draw_text_layers(void){
    glColor3f(0.3, 0.3, 0.3);
    glRasterPos3f(0,7,0);   printString("OpenGL Delta Robot Simulator");
    glColor3f(0.5, 0.5, 0.5);
    glRasterPos3f(0,6,0);   printString("(c) 2012 TimKrins");
    glColor3f(0.7, 0.7, 0.7);
    glRasterPos3f(0,5,0);   printString("UNFINISHED BUT PRETTY LOL");
    
    int dubX = (int)view_lookfrom_x_linear;
    int dubY = (int)view_lookfrom_y_linear;
    int dubD = (int)view_distance_from_model;
    
    float momentX = view_momentum_x;
    float momentY = view_momentum_y;
    float momentD = view_momentum_z;
    
    
    //memset(sprinter, 0, sizeof(sprinter));
    sprintf(sprinter, "XVIEW: %i, YVIEW: %i, DIST: %i", dubX , dubY , dubD);
    glColor3f(.9f, .9f, .9f);
    glRasterPos3f(0,4,0);
    printString(sprinter);
    memset(sprinter, 0, sizeof(sprinter));
    
    sprintf(sprinter, "XMomentum: %.2f, YMomentum: %.2f, DMomentum: %.2f", momentX , momentY , momentD);
    glRasterPos3f(0,4.5,0);
    printString(sprinter);
    memset(sprinter, 0, sizeof(sprinter));
};

void draw_delta_robot(void){
    draw_top_base();
    draw_bottom_base();
    draw_arms();
};

void draw_top_base(void){
    draw_topbase_full_top();
    draw_topbase_full_sides();
    draw_topbase_full_motors();
    draw_topbase_full_bottom();
};

void draw_topbase_full_top(void){
  glPushMatrix();
  glBegin(GL_TRIANGLES);
  {
   glColor3f(.7f, .7f, .7f);
   glVertex3f(robot_top_platform_triangle_v1_x,robot_top_platform_triangle_v1_y,robot_top_platform_triangle_v1_z);
   glVertex3f(robot_top_platform_triangle_v2_x,robot_top_platform_triangle_v2_y,robot_top_platform_triangle_v2_z);
   glVertex3f(robot_top_platform_triangle_v3_x,robot_top_platform_triangle_v3_y,robot_top_platform_triangle_v3_z);
   }
  glEnd();
  glPopMatrix();
};

void draw_topbase_full_sides(void){
  glPushMatrix();
  glRotatef(startrotation,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_inscribed);
  glRotatef(180,0,1,0);
  draw_topbase_onethird_sides();
  glPopMatrix();
  
  glPushMatrix();
  glRotatef(120+startrotation,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_inscribed);
  glRotatef(180,0,1,0);
  draw_topbase_onethird_sides();
  glPopMatrix();
  
  glPushMatrix();
  glRotatef(240+startrotation,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_inscribed);
  glRotatef(180,0,1,0);
  draw_topbase_onethird_sides();
  glPopMatrix();
  
};
//(robot_top_platform_triangle_sidelength/2)
void draw_topbase_onethird_sides(void){
  glBegin(GL_TRIANGLES);
  {
   //first square
   glColor3f(.22f, .22f, .22f);
   glVertex3f(0-(robot_top_platform_triangle_sidelength/2),0,0);                             //VERTEX1
   glVertex3f(0-(robot_top_platform_triangle_sidelength/2),-robot_top_platform_thickness,0); //VERTEX2
   glVertex3f(robot_top_motor_gapfromzero,0,0);                                              //VERTEX3
   glVertex3f(robot_top_motor_gapfromzero,0,0);                                              //VERTEX3
   glVertex3f(0-(robot_top_platform_triangle_sidelength/2),-robot_top_platform_thickness,0); //VERTEX2
   glVertex3f(robot_top_motor_gapfromzero,-robot_top_platform_thickness,0);                  //VERTEX4
   
   //second square (coming out from z)
   glColor3f(.38f, .38f, .38f);
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);                        //VERTEX3A
   glVertex3f(robot_top_motor_gapfromzero,-robot_top_platform_thickness,0);                                                            //VERTEX4
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);   //VERTEX5A
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);   //VERTEX5A
   glVertex3f(robot_top_motor_gapfromzero,-robot_top_platform_thickness,0);                                                            //VERTEX4
   glVertex3f(robot_top_motor_gapfromzero,-robot_top_platform_thickness,-robot_top_motor_width);                                       //VERTEX6
   
   //third square
   glColor3f(.46f, .46f, .46f);
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);   //VERTEX5A
   glVertex3f(robot_top_motor_gapfromzero,-robot_top_platform_thickness,-robot_top_motor_width); //VERTEX6
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);//VERTEX7A
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);//VERTEX7A
   glVertex3f(robot_top_motor_gapfromzero,-robot_top_platform_thickness,-robot_top_motor_width); //VERTEX6
   glVertex3f((robot_top_platform_triangle_sidelength/2),-robot_top_platform_thickness,-robot_top_motor_width); //VERTEX8
   
   
   //fourth square
   glColor3f(.53f, .53f, .53f);
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);//VERTEX7A
   glVertex3f((robot_top_platform_triangle_sidelength/2),-robot_top_platform_thickness,-robot_top_motor_width); //VERTEX8
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);          //VERTEX9A
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);          //VERTEX9A
   glVertex3f((robot_top_platform_triangle_sidelength/2),-robot_top_platform_thickness,-robot_top_motor_width); //VERTEX8
   glVertex3f((robot_top_platform_triangle_sidelength/2),-robot_top_platform_thickness,0); //VERTEX10
   
      //fifth square
   glColor3f(.61f, .61f, .61f);
   glVertex3f(robot_top_motor_gapfromzero,0,0);                                              //VERTEX3
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);                        //VERTEX3A
   glVertex3f((robot_top_platform_triangle_sidelength/2),0,0);          //VERTEX9
   glVertex3f((robot_top_platform_triangle_sidelength/2),0,0);          //VERTEX9
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);                        //VERTEX3A
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);          //VERTEX9A
   
   //draw tops
   glColor3f(.35f, .35f, .35f);
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);          //VERTEX9A
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);                        //VERTEX3A
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);   //VERTEX5A
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);          //VERTEX9A
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);   //VERTEX5A
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);//VERTEX7A
   
      //draw bottoms
   glColor3f(.23f, .23f, .23f);
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness,0);          //VERTEX
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness,0);                        //VERTEX
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness,-robot_top_motor_width);   //VERTEX
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness,0);          //VERTEX
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness,-robot_top_motor_width);   //VERTEX
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness,-robot_top_motor_width);//VERTEX
   
   }

  glEnd();
};

void draw_topbase_full_motors(void){
//including attach points for arm
// startrotation = 0.0f;
  glPushMatrix();
  glRotatef(startrotation,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_radius);
  glColor3f(.5f, .5f, .5f);
  glRotatef(180,0,1,0);
  draw_topbase_onethird_motors();
  glPopMatrix();
  
  glPushMatrix();
  glRotatef(120+startrotation,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_radius);
  glRotatef(180,0,1,0);
  draw_topbase_onethird_motors();
  glPopMatrix();
  
  glPushMatrix();
  glRotatef(240+startrotation,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_radius);
  glRotatef(180,0,1,0);
  draw_topbase_onethird_motors();
  glPopMatrix();
};

void draw_topbase_onethird_motors(void){
glRotatef(270,0,1,0);
glTranslatef(0,0,-(robot_top_arm_connector_width/2));
glColor3f(.9f, .9f, .9f);
gluCylinder(quadCylinder, robot_top_arm_connector_radius, robot_top_arm_connector_radius, robot_top_arm_connector_width, 32, 16);
gluDisk(quadDisk, 0, robot_top_arm_connector_radius, 32, 1);
glTranslatef(0,0,(robot_top_arm_connector_width));
gluDisk(quadDisk, 0, robot_top_arm_connector_radius, 32, 1);
//draw the actual motor and connector rod to center joint.
glTranslatef(0,0,-(robot_top_arm_connector_width/2));
glRotatef(180,0,1,0);
glColor3f(.58f, .58f, .58f);
gluCylinder(quadCylinder, robot_top_arm_connector_radius/2, robot_top_arm_connector_radius/2, robot_top_motor_gapfromzero, 32, 16);
glTranslatef(0,0,robot_top_motor_gapfromzero);
glColor3f(.158f, .158f, .158f);
gluDisk(quadDisk, 0, 1.5, 32, 1);
glColor3f(.1258f, .1258f, .1258f);
gluCylinder(quadCylinder, 1.5, 1.5, robot_top_motor_length, 32, 16);
glTranslatef(0,0,robot_top_motor_length);
glColor3f(.158f, .158f, .158f);
gluDisk(quadDisk, 0, 1.5, 32, 1);
};

void draw_topbase_full_bottom(void){
  glPushMatrix();
  glBegin(GL_TRIANGLES);
  {
   glColor3f(.7f, .7f, .7f);
   glVertex3f(robot_top_platform_triangle_v1_x,robot_top_platform_triangle_v1_y-robot_top_platform_thickness,robot_top_platform_triangle_v1_z);
   glVertex3f(robot_top_platform_triangle_v3_x,robot_top_platform_triangle_v3_y-robot_top_platform_thickness,robot_top_platform_triangle_v3_z);
   glVertex3f(robot_top_platform_triangle_v2_x,robot_top_platform_triangle_v2_y-robot_top_platform_thickness,robot_top_platform_triangle_v2_z);
   }
  glEnd();
  glPopMatrix();
};

void draw_bottom_base(void){


    draw_botbase_full_top();
    //draw_botbase_onethird_sides();
    //draw_botbase_full_bottom();
};

void draw_botbase_full_top(void){
glPushMatrix();
calculate_delta(robot_angle_one, robot_angle_two, robot_angle_three);
// Remember, our axes are different. Y is actually Z.
glTranslatef(robot_end_effector_x,robot_end_effector_z, 0-robot_end_effector_y);
gluSphere(quadSphere, 1, 32, 16);
glPopMatrix();
};

void draw_botbase_onethird_sides(void){

};

void draw_botbase_full_bottom(void){

};

void draw_arms(void){
    //draw_arms_top_full_motor_joiner();
    draw_arms_top_full_arm();
    draw_arms_mid_full_joiner();
    draw_arms_bottom_onehalf_arms();
    draw_arms_bottom_full_joiner();
};

void draw_arms_top_full_motor_joiner(void){

};

void draw_arms_top_full_arm(void){
  glPushMatrix();
  glRotatef(startrotation,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_radius);
  glColor3f(.0f, .9f, .0f);
  glRotatef(robot_angle_one,1,0,0);
  
  glBegin(GL_TRIANGLES);
  {
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),0); //V1
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),0); //V2
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),robot_top_arm_length); //V3
   
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),0); //V1
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),robot_top_arm_length); //V3
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V4
   
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),0); //V5
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),0); //V1
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V4
   
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),0); //V5
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V4
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V8
   
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V8
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),robot_top_arm_length); //V7
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),0); //V6
   
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V8
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),0); //V6
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),0); //V5
   
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),robot_top_arm_length); //V7
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),0); //V2
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),robot_top_arm_length); //V3
   
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),robot_top_arm_length); //V7
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),0); //V6
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),0); //V2
   }
  glEnd();
  
  glPopMatrix();
  
  glPushMatrix();
  glRotatef(startrotation+120,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_radius);
  glColor3f(.0f, .0f, .9f);
  glRotatef(robot_angle_two,1,0,0);
  glBegin(GL_TRIANGLES);
  {
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),0); //V1
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),0); //V2
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),robot_top_arm_length); //V3
   
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),0); //V1
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),robot_top_arm_length); //V3
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V4
   
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),0); //V5
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),0); //V1
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V4
   
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),0); //V5
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V4
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V8
   
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V8
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),robot_top_arm_length); //V7
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),0); //V6
   
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V8
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),0); //V6
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),0); //V5
   
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),robot_top_arm_length); //V7
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),0); //V2
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),robot_top_arm_length); //V3
   
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),robot_top_arm_length); //V7
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),0); //V6
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),0); //V2
   }
  glEnd();

  glPopMatrix();
  
  glPushMatrix();
  glRotatef(startrotation+240,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_radius);
  glColor3f(.9f, .0f, .0f);
  glRotatef(robot_angle_three,1,0,0);
  glBegin(GL_TRIANGLES);
  {
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),0); //V1
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),0); //V2
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),robot_top_arm_length); //V3
   
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),0); //V1
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),robot_top_arm_length); //V3
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V4
   
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),0); //V5
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),0); //V1
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V4
   
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),0); //V5
   glVertex3f((robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V4
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V8
   
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V8
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),robot_top_arm_length); //V7
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),0); //V6
   
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),robot_top_arm_length); //V8
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),0); //V6
   glVertex3f(-(robot_top_arm_width/2),(robot_top_arm_height/2),0); //V5
   
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),robot_top_arm_length); //V7
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),0); //V2
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),robot_top_arm_length); //V3
   
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),robot_top_arm_length); //V7
   glVertex3f(-(robot_top_arm_width/2),-(robot_top_arm_height/2),0); //V6
   glVertex3f((robot_top_arm_width/2),0-(robot_top_arm_height/2),0); //V2
   }
  glEnd();

  glPopMatrix();
};


void draw_arms_mid_full_joiner(void){

  glPushMatrix();
  glRotatef(startrotation,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_radius);
  glColor3f(.0f, .9f, .0f);
  glRotatef(robot_angle_one,1,0,0);
  glTranslatef(0,0,robot_top_arm_length);
  glRotatef(270,0,1,0);
  glTranslatef(0,0,-(robot_mid_connector_width/2));

glColor3f(.9f, .9f, .9f);
gluCylinder(quadCylinder, robot_mid_connector_radius, robot_mid_connector_radius, robot_mid_connector_width, 32, 16);
gluDisk(quadDisk, 0, robot_mid_connector_radius, 32, 1);
glTranslatef(0,0,(robot_mid_connector_width));
gluDisk(quadDisk, 0, robot_mid_connector_radius, 32, 1);

  glPopMatrix();
  
  glPushMatrix();
  glRotatef(startrotation+120,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_radius);
  glColor3f(.0f, .9f, .0f);
  glRotatef(robot_angle_two,1,0,0);
  glTranslatef(0,0,robot_top_arm_length);
  glRotatef(270,0,1,0);
  glTranslatef(0,0,-(robot_mid_connector_width/2));

glColor3f(.9f, .9f, .9f);
gluCylinder(quadCylinder, robot_mid_connector_radius, robot_mid_connector_radius, robot_mid_connector_width, 32, 16);
gluDisk(quadDisk, 0, robot_mid_connector_radius, 32, 1);
glTranslatef(0,0,(robot_mid_connector_width));
gluDisk(quadDisk, 0, robot_mid_connector_radius, 32, 1);

  glPopMatrix();
  
  glPushMatrix();
  glRotatef(startrotation+240,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_radius);
  glColor3f(.0f, .9f, .0f);
  glRotatef(robot_angle_three,1,0,0);
  glTranslatef(0,0,robot_top_arm_length);
  glRotatef(270,0,1,0);
  glTranslatef(0,0,-(robot_mid_connector_width/2));

glColor3f(.9f, .9f, .9f);
gluCylinder(quadCylinder, robot_mid_connector_radius, robot_mid_connector_radius, robot_mid_connector_width, 32, 16);
gluDisk(quadDisk, 0, robot_mid_connector_radius, 32, 1);
glTranslatef(0,0,(robot_mid_connector_width));
gluDisk(quadDisk, 0, robot_mid_connector_radius, 32, 1);

  glPopMatrix();
};

void draw_arms_bottom_onehalf_arms(void){

};

void draw_arms_bottom_full_joiner(void){

};