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

#define PION180 0.017453292519943295769
#define SIN60 0.866025404
#define SIN180 0
#define SIN300 -0.866025404
#define COS60 0.5
#define COS180 -1
#define COS300 0.5
#define SQRT3ON3 0.577350269
#define SQRT3ON6 0.288675135

// set up software variables

int startxMouse, startyMouse, xMouse, yMouse, h;
float colors[4];
char sprinter[15];
int clicked = 0;
int holdingForward, holdingBackward, holdingLeftStrafe, holdingRightStrafe, holdingZoomIn, holdingZoomOut;
GLUquadric *quadSphere;
GLUquadric *quadCylinder;
GLint slices, stacks;
GLint nsides, rings;

// set up robot variables

float robot_top_platform_triangle_radius = 10.0f;
float robot_top_platform_thickness = 3.0f;
//float robot_top_motor_height = 10.0f;
float robot_top_motor_width = 10.0f;
float robot_top_motor_gapfromzero = 1.0f;
float robot_top_motor_platform_thickness = 1.0f;
float robot_top_arm_length = 20.0f;
float robot_top_arm_height = 5.0f;
float robot_top_arm_width = 5.0f;
float robot_bottom_arm_length = 20.0f;
float robot_bottom_arm_radius = 5.0f;
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
robot_top_platform_triangle_v1_x = robot_top_platform_triangle_radius*SIN180;
robot_top_platform_triangle_v1_y = 0.0f;
robot_top_platform_triangle_v1_z = robot_top_platform_triangle_radius*COS180;

robot_top_platform_triangle_v2_x = robot_top_platform_triangle_radius*SIN60;
robot_top_platform_triangle_v2_y = 0.0f;
robot_top_platform_triangle_v2_z = robot_top_platform_triangle_radius*COS60;

robot_top_platform_triangle_v3_x = robot_top_platform_triangle_radius*SIN300;
robot_top_platform_triangle_v3_y = 0.0f;
robot_top_platform_triangle_v3_z = robot_top_platform_triangle_radius*COS300;

robot_top_platform_triangle_sidelength = robot_top_platform_triangle_radius / SQRT3ON3;
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
  glClearColor( 0.1f, 0.1f, 0.1f, 0.0f ); // Preset color
  makeBitmapFonts();
  quadCylinder=gluNewQuadric();
  quadSphere=gluNewQuadric();
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
            holdingForward = 1;
            break;
 
        case 'S':
            holdingBackward = 1;
            break;
 
        case 'A':
            holdingLeftStrafe = 1;
            break;
 
        case 'D':
            holdingRightStrafe = 1;
            break;
            
        case 'Q':
            holdingZoomIn = 1;
            break;
 
        case 'E':
            holdingZoomOut = 1;
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
            holdingForward = 0;
            break;
 
        case 'S':
            holdingBackward = 0;
            break;
 
        case 'A':
            holdingLeftStrafe = 0;
            break;
 
        case 'D':
            holdingRightStrafe = 0;
            break;
            
        case 'Q':
            holdingZoomIn = 0;
            break;
 
        case 'E':
            holdingZoomOut = 0;
            break;
            
        default:
            // Do nothing...
            break;
        }
    }
    }

void update_view(void){
    if(holdingLeftStrafe) {
    if(view_lookfrom_x_linear < 179){
    view_lookfrom_x_linear += 1;
    } else {
    view_lookfrom_x_linear = -179;
    }
    };
    if(holdingRightStrafe) {
    if(view_lookfrom_x_linear > -179){
    view_lookfrom_x_linear -= 1;
    } else {
    view_lookfrom_x_linear = 179;
    }
    };
    if(holdingBackward) {
    if(view_lookfrom_y_linear < 88){
    view_lookfrom_y_linear += 1;
    }
    };
    if(holdingForward) {
    if(view_lookfrom_y_linear > -88){
    view_lookfrom_y_linear -= 1;
    }
    };
    if(holdingZoomIn) {
    view_distance_from_model += 1;
    };
    if(holdingZoomOut) {
    view_distance_from_model -= 1;
    };

    // so 'linear' should go from -180 to 180, starting at zero.
    // it will generate a position about the origin
    //view_lookfrom_x = (view_distance_from_model*cos(view_lookfrom_x_linear*PION180))+(view_distance_from_model*sin(view_lookfrom_y_linear*PION180));
    view_lookfrom_x = cos(view_lookfrom_x_linear*PION180)*cos(view_lookfrom_y_linear*PION180)*view_distance_from_model;
    //view_lookfrom_y = (100)*cos(view_lookfrom_x_linear*PION180) + (100)*cos(view_lookfrom_y_linear*PION180);
    view_lookfrom_y = sin(view_lookfrom_y_linear*PION180)*view_distance_from_model;
    view_lookfrom_z = cos(view_lookfrom_y_linear*PION180)*sin(view_lookfrom_x_linear*PION180)*view_distance_from_model;
    //view_lookfrom_z = (view_distance_from_model*sin(view_lookfrom_x_linear*PION180))+(view_distance_from_model*cos(view_lookfrom_y_linear*PION180));
    
    //view_distance_from_model
    
    glfwGetMousePos(&startxMouse,&startyMouse); // get the mouse position
    glfwGetMousePos(&xMouse,&yMouse);
    //view_lookfrom_x = (double)(((xMouse-(window_width/2))/view_lookfrom_divide_x));
    //view_lookfrom_y = (double)(((yMouse-(window_height/2))/view_lookfrom_divide_y));
    //view_lookfrom_z = ((((glfwGetMouseWheel()*glfwGetMouseWheel())/5)*(glfwGetMouseWheel()*0.1))-70);

}

void graphics_draw(void) {
    glLoadIdentity();
    update_view();
   	glTranslatef(0, 0, 0);  // View scrolls with MouseWheel
    gluLookAt(
    view_lookfrom_x, view_lookfrom_y, view_lookfrom_z,
    view_lookat_x, view_lookat_y, view_lookat_z,
    0.0f, 1.0f, 0.0f
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
  glfwSleep(0.01);
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
    glRasterPos3f(0,5,0);   printString("UNFINISHED BUT PRETTY");
};

void draw_delta_robot(void){
    draw_top_base();
    //draw_bottom_base();
    //draw_arms();
};

void draw_top_base(void){
    draw_topbase_full_top();
    draw_topbase_full_sides();
    draw_topbase_full_motors();
    //draw_topbase_full_bottom();
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
   glColor3f(.2f, .2f, .2f);
   glVertex3f(0-(robot_top_platform_triangle_sidelength/2),0,0);                             //VERTEX1
   glVertex3f(0-(robot_top_platform_triangle_sidelength/2),-robot_top_platform_thickness,0); //VERTEX2
   glVertex3f(robot_top_motor_gapfromzero,0,0);                                              //VERTEX3
   glVertex3f(robot_top_motor_gapfromzero,0,0);                                              //VERTEX3
   glVertex3f(0-(robot_top_platform_triangle_sidelength/2),-robot_top_platform_thickness,0); //VERTEX2
   glVertex3f(robot_top_motor_gapfromzero,-robot_top_platform_thickness,0);                  //VERTEX4
   
   //second square (coming out from z)
   glColor3f(.3f, .3f, .3f);
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);                        //VERTEX3A
   glVertex3f(robot_top_motor_gapfromzero,-robot_top_platform_thickness,0);                                                            //VERTEX4
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);   //VERTEX5A
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);   //VERTEX5A
   glVertex3f(robot_top_motor_gapfromzero,-robot_top_platform_thickness,0);                                                            //VERTEX4
   glVertex3f(robot_top_motor_gapfromzero,-robot_top_platform_thickness,-robot_top_motor_width);                                       //VERTEX6
   
   //third square
   glColor3f(.4f, .4f, .4f);
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);   //VERTEX5A
   glVertex3f(robot_top_motor_gapfromzero,-robot_top_platform_thickness,-robot_top_motor_width); //VERTEX6
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);//VERTEX7A
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);//VERTEX7A
   glVertex3f(robot_top_motor_gapfromzero,-robot_top_platform_thickness,-robot_top_motor_width); //VERTEX6
   glVertex3f((robot_top_platform_triangle_sidelength/2),-robot_top_platform_thickness,-robot_top_motor_width); //VERTEX8
   
   
   //fourth square
   glColor3f(.5f, .5f, .5f);
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);//VERTEX7A
   glVertex3f((robot_top_platform_triangle_sidelength/2),-robot_top_platform_thickness,-robot_top_motor_width); //VERTEX8
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);          //VERTEX9A
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);          //VERTEX9A
   glVertex3f((robot_top_platform_triangle_sidelength/2),-robot_top_platform_thickness,-robot_top_motor_width); //VERTEX8
   glVertex3f((robot_top_platform_triangle_sidelength/2),-robot_top_platform_thickness,0); //VERTEX10
   
      //fifth square
   glColor3f(.6f, .6f, .6f);
   glVertex3f(robot_top_motor_gapfromzero,0,0);                                              //VERTEX3
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);                        //VERTEX3A
   glVertex3f((robot_top_platform_triangle_sidelength/2),0,0);          //VERTEX9
   glVertex3f((robot_top_platform_triangle_sidelength/2),0,0);          //VERTEX9
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);                        //VERTEX3A
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);          //VERTEX9A
   
   //draw tops
   glColor3f(.3f, .3f, .3f);
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);          //VERTEX9A
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);                        //VERTEX3A
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);   //VERTEX5A
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,0);          //VERTEX9A
   glVertex3f(robot_top_motor_gapfromzero,0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);   //VERTEX5A
   glVertex3f((robot_top_platform_triangle_sidelength/2),0-robot_top_platform_thickness+robot_top_motor_platform_thickness,-robot_top_motor_width);//VERTEX7A
   
   
   
   
  // glVertex3f((robot_top_platform_triangle_sidelength/2),0,0);          //VERTEX9
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
gluSphere(quadSphere, 0.5, 32, 16);


};

void draw_topbase_full_bottom(void){

};

void draw_bottom_base(void){
    draw_botbase_full_top();
    draw_botbase_onethird_sides();
    draw_botbase_full_bottom();
};

void draw_botbase_full_top(void){

};

void draw_botbase_onethird_sides(void){

};

void draw_botbase_full_bottom(void){

};

void draw_arms(void){
    draw_arms_top_full_motor_joiner();
    draw_arms_top_full_arm();
    draw_arms_mid_full_joiner();
    draw_arms_bottom_onehalf_arms();
    draw_arms_bottom_full_joiner();
};

void draw_arms_top_full_motor_joiner(void){

};

void draw_arms_top_full_arm(void){

};

void draw_arms_mid_full_joiner(void){

};

void draw_arms_bottom_onehalf_arms(void){

};

void draw_arms_bottom_full_joiner(void){

};