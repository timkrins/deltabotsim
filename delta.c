#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glfw.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <math.h>

//define constants
const int window_width = 1300, window_height = 600;
const int fs_window_width = 1366, fs_window_height = 768;
#define PI 3.14159265

#define SIN60 0.866025404
#define SIN180 0
#define SIN300 -0.866025404
#define COS60 0.5
#define COS180 -1
#define COS300 0.5

#define TOP_PLAT_DIST 10.0f
#define TOP_PLAT_THICK 3.0f
#define TOP_CYL_WIDTH 5.0f

GLubyte rasters[24] = {
   0xc0, 0x00, 0xc0, 0x00, 0xc0, 0x00, 0xc0, 0x00, 0xc0, 0x00,
   0xff, 0x00, 0xff, 0x00, 0xc0, 0x00, 0xc0, 0x00, 0xc0, 0x00,
   0xff, 0xc0, 0xff, 0xc0};

//define variables
float squaresize = 0.3;
int xMouse, yMouse, h;
float yrot, xrot;
float speedmin  = 0.15f;
float speedmulti = 0.05f;
float c_red = 0.1f, c_green = 0.1f, c_blue = 0.1f;


GLUquadric *quadSphere;
GLUquadric *quadCylinder;
GLdouble radius = 1.0;
GLint slices, stacks;
GLint nsides, rings;

//define delta static vars

static float top_arm_len = 20.0f;
static float bot_arm_len = 30.0f;
static float bot_plat_dist = 10.0f; // the bottom platform distance from center point

float top_v_2[6][3] =
{
{TOP_PLAT_DIST*SIN180, 0.0f, COS180*TOP_PLAT_DIST},
{TOP_PLAT_DIST*SIN60,0.0f,COS60*TOP_PLAT_DIST},
{TOP_PLAT_DIST*SIN300,0.0f,COS300*TOP_PLAT_DIST},
{TOP_PLAT_DIST*SIN180, 0.0f-TOP_PLAT_THICK, COS180*TOP_PLAT_DIST},
{TOP_PLAT_DIST*SIN60,0.0f-TOP_PLAT_THICK,COS60*TOP_PLAT_DIST},
{TOP_PLAT_DIST*SIN300,0.0f-TOP_PLAT_THICK,COS300*TOP_PLAT_DIST}
};

//define delta dynamic vars
float servo_1 = 25.0f;
float servo_2 = 25.0f;
float servo_3 = 25.0f;


// from http://www.opengl.org.ru/docs/pg/0801.html
GLubyte space[] =
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
GLubyte letters[][13] = {
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
    {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
    {0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff}
};

GLuint fontOffset;

//protyping
void major_loop(void);
void quit(void);
void init_arrays(void);
void draw_top_plat(void);
void graphics_init(void);
void graphics_draw(void);

void makeRasterFont(void)
{
   GLuint i, j;
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 
   fontOffset = glGenLists (128);
   for (i = 0,j = 'A'; i < 26; i++,j++) {
      glNewList(fontOffset + j, GL_COMPILE);
      glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, letters[i]);
      glEndList();
   }
   glNewList(fontOffset + ' ', GL_COMPILE);
   glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, space);
   glEndList();
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
  glEnable (GL_BLEND);
  glEnable( GL_DEPTH_TEST ) ;
  //glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  glfwSetWindowTitle("Delta Robot Simulator by Tim K");
  glMatrixMode(GL_PROJECTION);
  glPolygonMode(GL_FRONT, GL_FILL);
//glPolygonMode(GL_BACK, GL_LINE);
  glLoadIdentity();
  float aspect_ratio = ((float)window_height) / window_width;
  glFrustum(.5, -.5, -.5 * aspect_ratio, .5 * aspect_ratio, 1, 300);
  glMatrixMode(GL_MODELVIEW);
  glClearColor( 0.05f, 0.05f, 0.05f, 0.0f ); // Preset color
  makeRasterFont();
  quadCylinder=gluNewQuadric();
  quadSphere=gluNewQuadric();
  //glfwSetMouseWheelCallback( wheelmoved );
}//end-of-graphics-initialise

void printString(char *s)
{
   glPushAttrib (GL_LIST_BIT);
   glListBase(fontOffset);
   glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *) s);
   glPopAttrib ();
}
void makeSphereAt(float height_position, float center_radius, float angle, float sphere_radius)
{
  // a 'rotate and make sphere' function
  
  glPushMatrix();
  glRotatef(angle,0,1,0);
  glTranslatef(0,height_position,center_radius);
  gluSphere(quadSphere, sphere_radius, 32, 16);
  glPopMatrix();
}

void makeHorizontalCylinderAt(float height_position, float center_radius, float angle, float cyl_radius, float cyl_width)
{
  // a 'rotate and make cylinders' function
  
  glPushMatrix();
  glRotatef(angle,0,1,0);
  glTranslatef(0,height_position,center_radius);
  glRotatef(90,0,1,0);
  glTranslatef(0,0,0-(cyl_width/2));
  gluCylinder(quadCylinder, cyl_radius, cyl_radius, cyl_width, 32, 16);
  glPopMatrix();
}

void makeHorizontalCapsuleAt(float height_position, float center_radius, float angle, float cyl_radius, float cyl_width)
{
  // a 'rotate and make capsule' function
  
  glPushMatrix();
  glRotatef(angle,0,1,0);
  glTranslatef(0,height_position,center_radius);
  glRotatef(90,0,1,0);
  glTranslatef(0,0,0-(cyl_width/2));
  gluCylinder(quadCylinder, cyl_radius, cyl_radius, cyl_width, 32, 16);
  glPopMatrix();
  float colors[4];
  glGetFloatv(GL_CURRENT_COLOR, colors);
  glColor3f(colors[0]*0.8, colors[1]*0.8, colors[2]*0.8);
  glPushMatrix();
  glRotatef(angle,0,1,0);
  glTranslatef(0,height_position,center_radius);
  glRotatef(90,0,1,0);
  glTranslatef(0,0,0-(cyl_width/2));
  gluSphere(quadSphere, cyl_radius*1.1, 32, 16);
  glTranslatef(0,0,cyl_width);
  gluSphere(quadSphere, cyl_radius*1.1, 32, 16);
  glPopMatrix();
}

void draw_top_plat(void) {  // Draws the top platform
glPushMatrix();
  glBegin(GL_TRIANGLES);
  {    //glColor3f(c_red, c_green, c_blue);
   glColor4f(0,0,0,0);
   glColor3f(.5f, .1f, .2f);
   glVertex3f(top_v_2[0][0],top_v_2[0][1],top_v_2[0][2]); //1,2,3
   glVertex3f(top_v_2[1][0],top_v_2[1][1],top_v_2[1][2]);
   glVertex3f(top_v_2[2][0],top_v_2[2][1],top_v_2[2][2]);
   glColor3f(.3f, .7f, .3f);
   glVertex3f(top_v_2[0][0],top_v_2[0][1],top_v_2[0][2]); //1,5,2
   glVertex3f(top_v_2[4][0],top_v_2[4][1],top_v_2[4][2]);
   glVertex3f(top_v_2[1][0],top_v_2[1][1],top_v_2[1][2]);
   glColor3f(.5f, .3f, .8f);
   glVertex3f(top_v_2[0][0],top_v_2[0][1],top_v_2[0][2]); //1,4,5
   glVertex3f(top_v_2[3][0],top_v_2[3][1],top_v_2[3][2]);
   glVertex3f(top_v_2[4][0],top_v_2[4][1],top_v_2[4][2]);
   glColor3f(.5f, .7f, .8f);
   glVertex3f(top_v_2[1][0],top_v_2[1][1],top_v_2[1][2]); //2,5,6
   glVertex3f(top_v_2[4][0],top_v_2[4][1],top_v_2[4][2]);
   glVertex3f(top_v_2[5][0],top_v_2[5][1],top_v_2[5][2]);
   glColor3f(.7f, .7f, .8f);
   glVertex3f(top_v_2[1][0],top_v_2[1][1],top_v_2[1][2]); //2,3,6
   glVertex3f(top_v_2[5][0],top_v_2[5][1],top_v_2[5][2]);
   glVertex3f(top_v_2[2][0],top_v_2[2][1],top_v_2[2][2]);
   glColor3f(.5f, .1f, .8f);
   glVertex3f(top_v_2[2][0],top_v_2[2][1],top_v_2[2][2]); //3,4,6
   glVertex3f(top_v_2[5][0],top_v_2[5][1],top_v_2[5][2]);
   glVertex3f(top_v_2[3][0],top_v_2[3][1],top_v_2[3][2]);
   glColor3f(.5f, .7f, .3f);
   glVertex3f(top_v_2[2][0],top_v_2[2][1],top_v_2[2][2]); //3,4,1
   glVertex3f(top_v_2[3][0],top_v_2[3][1],top_v_2[3][2]);
   glVertex3f(top_v_2[0][0],top_v_2[0][1],top_v_2[0][2]);
   glColor3f(.5f, .6f, .6f);
   glVertex3f(top_v_2[4][0],top_v_2[4][1],top_v_2[4][2]);
   glVertex3f(top_v_2[3][0],top_v_2[3][1],top_v_2[3][2]);
   glVertex3f(top_v_2[5][0],top_v_2[5][1],top_v_2[5][2]);
  }
  glEnd();
      float color = 0.9;
    glColor3f(color, color, color);
    glRasterPos3f(top_v_2[0][0],top_v_2[0][1]+0.5,top_v_2[0][2]);
    printString("ONE");
    glRasterPos3f(top_v_2[1][0],top_v_2[1][1]+0.5,top_v_2[1][2]);
    printString("TWO");
    glRasterPos3f(top_v_2[2][0],top_v_2[2][1]+0.5,top_v_2[2][2]);
    printString("THREE");
    
    glColor3f(0.3, 0.3, 1.0);
    glRasterPos3f(25,6,-5);
    printString("OPENGL DELTA SIMULATOR");
    glRasterPos3f(25,5,-5);
    printString("TWENTYTWELVE TIM KRINS");
    
    glColor3f(0.9, 0.9, 0.0);
    glRasterPos3f(-20,-8,-5);
    printString("ANGLE ONE");
    glRasterPos3f(-20,-9,-5);
    printString("ANGLE TWO");
    glRasterPos3f(-20,-10,-5);
    printString("ANGLE THREE");
  glPopMatrix();
  
  glColor3f(.1f, .9f, .1f);
  //makeHorizontalCylinderAt(0-(TOP_PLAT_THICK/2),TOP_PLAT_DIST, 300.0,(TOP_PLAT_THICK/1.9),TOP_CYL_WIDTH); 
  makeHorizontalCapsuleAt(0-(TOP_PLAT_THICK/2),TOP_PLAT_DIST, 300.0,(TOP_PLAT_THICK/1.9),TOP_CYL_WIDTH);
  //makeSphereAt(0-(TOP_PLAT_THICK/2),TOP_PLAT_DIST, 300.0,(TOP_PLAT_THICK/1.9)); 
  
  glColor3f(.1f, .9f, .1f);
  makeHorizontalCylinderAt(0-(TOP_PLAT_THICK/2),TOP_PLAT_DIST, 180.0,(TOP_PLAT_THICK/1.9),TOP_CYL_WIDTH); 
  //makeSphereAt(0-(TOP_PLAT_THICK/2),TOP_PLAT_DIST, 180.0,(TOP_PLAT_THICK/1.9)); 
  
  glColor3f(.1f, .1f, .9f);
  makeHorizontalCylinderAt(0-(TOP_PLAT_THICK/2),TOP_PLAT_DIST, 60.0,(TOP_PLAT_THICK/1.9),TOP_CYL_WIDTH); 
  //makeSphereAt(0-(TOP_PLAT_THICK/2),TOP_PLAT_DIST, 60.0,(TOP_PLAT_THICK/1.9)); 
  
  glColor3f(.9f, .1f, .9f);
  //makeHorizontalCylinderAt(0-(TOP_PLAT_THICK/2),TOP_PLAT_DIST, 60.0,(TOP_PLAT_THICK/1.9),TOP_CYL_WIDTH); 
  
}//end-of-draw-top-plat

void draw_machine(void) {  // Draws a square with a gradient color at coordinates 0, 10
  glBegin(GL_QUADS);
  {
    glColor3f(c_red, c_green, c_blue*3);
    glVertex3f(-10, -10, -1.5);
    glVertex3f(10, -10, -1.5);
    glColor3f(0.1,0.9,0.9);
    glVertex3f(10, 10, -1.5);
    glVertex3f(-10, 10, -1.5);
  }
  glEnd();
}//end-of-drawing-the-back

void graphics_draw(void) {
    glLoadIdentity();
   	glTranslatef(0, 0, (glfwGetMouseWheel()*3)-70);  // move view back a bit 
    glfwGetMousePos(&xMouse,&yMouse);
    gluLookAt(
    (double)(xMouse-(window_width/2))/20, ((double)(yMouse-(window_height/2))/20)+0, 12, // look from
		0, -5, 0, // look here
		0.0f, 1.0f, 0.0f); // up matrix
		
		
		//draw things here
    draw_top_plat();
    //draw_machine();
    //end drawing things
   
    glFlush();
    
   // glPushMatrix();
   // //draw next item
    //glTranslatef(1.2,1.2,0);
    ////draw_bot_plat(0.5f);
    ////end drawing next item
    //glPopMatrix();
}//end-of-graphics-draw

void major_loop(void) {
double old_time = glfwGetTime();
while(glfwGetWindowParam(GLFW_ACTIVE))
  {
  double current_time = glfwGetTime();
  glfwSleep(0.02);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  graphics_draw();
  glfwSwapBuffers();
  }
}//end-of-major-loop

int main(void)
{
  graphics_init();
  major_loop();
  quit();

}//end-of-main