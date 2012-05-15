/*|_|_|_ __ ___ | | ___ __|_|_ __  ___ 
| _|| | '_ ` _ \| |/ / '__|@|a'_ \/ __|
| |_| | | | | | |   <| |  |g|i| | \__ \
 \__|_|_| |_| |_|_|\_\_|  |m|l| |_|__*/

// ###################################
// This is a delta robot simulator.
// Implements OpenGL, GLFW and GLU.
// Use the controls WASDQE for viewpoint.
// Use IJOKPL to change the angles.
// ###################################

void draw_sphere(float radius);
void draw_cuboid(float width, float height, float length);
void draw_closed_cylinder(float radius, float length);
void draw_centered_closed_cylinder(float radius, float length);

void draw_closed_cylinder(float radius, float length){
// ###################################
// Draws a closed cylinder from [zero] extending [length]
// ###################################
glPushMatrix();
color_darken();
gluCylinder(quadCylinder, radius, radius, length, 32, 16);
color_darken();
gluDisk(quadDisk, 0, radius, 32, 1);
glTranslatef(0,0,length);
color_darken();
gluDisk(quadDisk, 0, radius, 32, 1);
glPopMatrix();
};

void draw_centered_closed_cylinder(float radius, float length){
// ###################################
// Draws a [centered on zero] closed cylinder. Relys on closed_cylinder.
// ###################################
glPushMatrix();
glTranslatef(0,0,-(length/2));
draw_closed_cylinder(radius, length);
glPopMatrix();
}

void draw_cuboid(float width, float height, float length){
// ###################################
// Draws a [centered on zeros] cubic thing, extending from [zero].
// ###################################
glPushMatrix();
 glBegin(GL_TRIANGLES);
   glVertex3f((width/2),(height/2),0); //V1
   glVertex3f((width/2),0-(height/2),0); //V2
   glVertex3f((width/2),0-(height/2),length); //V3
   glVertex3f((width/2),(height/2),0); //V1
   glVertex3f((width/2),0-(height/2),length); //V3
   glVertex3f((width/2),(height/2),length); //V4
   glVertex3f(-(width/2),(height/2),0); //V5
   glVertex3f((width/2),(height/2),0); //V1
   glVertex3f((width/2),(height/2),length); //V4
   glVertex3f(-(width/2),(height/2),0); //V5
   glVertex3f((width/2),(height/2),length); //V4
   glVertex3f(-(width/2),(height/2),length); //V8
   glVertex3f(-(width/2),(height/2),length); //V8
   glVertex3f(-(width/2),-(height/2),length); //V7
   glVertex3f(-(width/2),-(height/2),0); //V6
   glVertex3f(-(width/2),(height/2),length); //V8
   glVertex3f(-(width/2),-(height/2),0); //V6
   glVertex3f(-(width/2),(height/2),0); //V5
   glVertex3f(-(width/2),-(height/2),length); //V7
   glVertex3f((width/2),0-(height/2),0); //V2
   glVertex3f((width/2),0-(height/2),length); //V3
   glVertex3f(-(width/2),-(height/2),length); //V7
   glVertex3f(-(width/2),-(height/2),0); //V6
   glVertex3f((width/2),0-(height/2),0); //V2
  glEnd();
  glPopMatrix();
}