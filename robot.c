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

#define vector_crossProduct(b,c) { (b.y*c.z-c.y*b.z) , (b.z*c.x-c.z*b.x) , (b.x*c.y-c.x*b.y) }
#define vector_minus(b,c) { (b.x-c.x), (b.y-c.y), (b.z-c.z) }
#define vector_dotProduct(b,c) ((b.x*c.x)+(b.y*c.y)+(b.z*c.z))
#define vector_magnitude(b) sqrt((b.x*b.x)+(b.y*b.y)+(b.z*b.z))
#define PI 3.14159265

void draw_delta_robot(void);
    void draw_slice(int current_slice);
        void draw_top(void);
            void draw_top_trisection(void);
            void draw_top_squaresection(void);
        void draw_motor(void);
        void draw_top_arm(int current_slice);
        void draw_arm_connector(void);
        void draw_bottom_arm(int current_slice);
        void draw_effector_slice(void);
    void draw_effector(void);
    
    
    void calculate(void);
    void simple_base(int i);
    void simple_top_arm(int i);
    void simple_bot_arm(int i);
    void simple_ee(int i); 
    
    void complex_base(int i);
    void complex_top_arm(int i);
    void complex_bot_arm(int i);
    void complex_ee(int i); 

void use_vertex(int vert);

void draw_delta_robot(void){
// ######################
// Draws the delta robot.
// ######################
glPushMatrix();
//draw_slice(0);
//draw_slice(1);
//draw_slice(2);
//draw_effector();

//make it simple first.

calculate();

//simple_base(0);
complex_base(0);
simple_base(1);
simple_base(2);

simple_top_arm(0);
simple_top_arm(1);
simple_top_arm(2);

simple_bot_arm(0);
simple_bot_arm(1);
simple_bot_arm(2);

simple_ee(0);
simple_ee(1);
simple_ee(2);


glPopMatrix();
}

void simple_base(int i){
glPushMatrix();
if(i == 0) { glColor3f(1, 0, 0); }
if(i == 1) { glColor3f(0, 1, 0); }
if(i == 2) { glColor3f(0, 0, 1); }
glBegin(GL_LINES);
glVertex3f(0, 0, 0); // origin of the line
glVertex3f(base_y[i], base_z[i], base_x[i]);
glEnd();
glPopMatrix();
}

void complex_base(int i){
glPushMatrix();
if(i == 0) { glColor3f(.8, .1, .1); }
if(i == 1) { glColor3f(.1, .8, .1); }
if(i == 2) { glColor3f(.1, .1, .8); }
vec3d a = {0, 0, 0};
vec3d b = {base_y[i], base_z[i], base_x[i]};
// This is the default direction for the cylinders to face in OpenGL
vec3d z = { 0 , 0 , 1 };
// Get diff between two points you want cylinder along
vec3d p = vector_minus(a, b);
// Get cross product (for the axis of rotation)
vec3d t = vector_crossProduct(z, p);
// Get dot product
int d = vector_dotProduct(z,p);
int m = vector_magnitude(p);
// Get angle. LENGTH is magnitude of the vector
double angle = (180/PI*acos(d/m));
if(holdingC == 1){
printf("P vector: x%f, y%f, z%f\n", p.x, p.y, p.z);
printf("P magnitude: %f\n", m);
}
glTranslated(b.x,b.y,b.z);
glRotated(angle,t.x,t.y,t.z);
draw_closed_cylinder(robot_top_arm_width,m);
glPopMatrix();
}

void simple_top_arm(int i){
glPushMatrix();
if(i == 0) { glColor3f(.8, .1, .1); }
if(i == 1) { glColor3f(.1, .8, .1); }
if(i == 2) { glColor3f(.1, .1, .8); }
glBegin(GL_LINES);
glVertex3f(base_y[i], base_z[i], base_x[i]); // origin of the line
glVertex3f(j_real_y[i], j_real_z[i], j_real_x[i]);
glEnd();
glPopMatrix();
}

void simple_bot_arm(int i){
glPushMatrix();
if(i == 0) { glColor3f(.6, .1, .1); }
if(i == 1) { glColor3f(.1, .6, .1); }
if(i == 2) { glColor3f(.1, .1, .6); }
glBegin(GL_LINES);

glVertex3f(j_real_y[i], j_real_z[i], j_real_x[i]);
glVertex3f(eereal_y[i], eereal_z[i], eereal_x[i]);
glEnd();
glPopMatrix();
}

void simple_ee(int i){
glPushMatrix();
if(i == 0) { glColor3f(.4, .1, .1); }
if(i == 1) { glColor3f(.1, .4, .1); }
if(i == 2) { glColor3f(.1, .1, .4); }
glBegin(GL_LINES);
glVertex3f(eereal_y[i], eereal_z[i], eereal_x[i]);
glVertex3f(ee[1], ee[2], ee[0]);
glEnd();
glPopMatrix();
}

void calculate(void){
calculate_delta();
}

void draw_slice(int current_slice){
// ######################################################################################
// Draws a slice.
// The first slice has no offset, the second is 120degrees, and the third is 240degrees.
//! For some reason, the drawing is offset by 30 degrees. Not sure why. I have compensated.
// I am going to Push() and Pop() the matrix excessively for simplicitys sake.
// ######################################################################################
glPushMatrix();
rotation = current_slice*120;
glRotatef(rotation,0,-1,0);
// COLORS

if(current_slice == 0){
color_red = 0;
color_green = 1;
color_blue = 0;
}
if(current_slice == 1){
color_red = 0;
color_green = 0;
color_blue = 1;
}
if(current_slice == 2){
color_red = 1;
color_green = 0;
color_blue = 0;
}

draw_top(); // ##DONE
glRotatef(90,0,1,0); // Compensate
draw_motor(); // ##DONE
draw_top_arm(current_slice); // ##DONE
//draw_arm_connector(); //! Do this later.

//draw_effector_slice(current_slice);

glPopMatrix();
draw_bottom_arm(current_slice); //! might not need a current slice variable.
}

void draw_top(void){
glPushMatrix();
draw_top_trisection(); //##DONE
draw_top_squaresection(); //##DONE
glPopMatrix();
}

void use_vertex(int vert){
// ########################################
// Use a vertex from the vertex definitions
// ########################################
glVertex3f(v_x[vert],v_y[vert],v_z[vert]);
}

void draw_top_trisection(void){
// ##########################################################################
// The top triangular section is an interesting unique shape. I may merge it.
// The vertexes are calculated in the variables file.
// ##########################################################################
glPushMatrix();
glColor3f(color_red, color_green, color_blue);
glBegin(GL_TRIANGLES);
float multi = 1.0f;
   use_vertex(0);   use_vertex(1);   use_vertex(2);
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(2);   use_vertex(1);   use_vertex(3);
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(3);   use_vertex(1);   use_vertex(4);
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(3);   use_vertex(4);   use_vertex(5);

glEnd();
glPopMatrix();
}

void draw_top_squaresection(void){
// ########################
// The top square platform.
// ########################
glPushMatrix();
glBegin(GL_TRIANGLES);
multi = 1.0f;
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(7);   use_vertex(12);   use_vertex(6);
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(12);   use_vertex(11);   use_vertex(6);
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(12);   use_vertex(9);   use_vertex(11);
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(9);   use_vertex(10);   use_vertex(11);
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(9);   use_vertex(8);   use_vertex(10);
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(8);   use_vertex(10);   use_vertex(4);
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(7);   use_vertex(8);   use_vertex(9);
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(9);   use_vertex(12);   use_vertex(7);
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(11);   use_vertex(10);   use_vertex(6);
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
   use_vertex(10);   use_vertex(4);   use_vertex(6);
multi = 1-(multi/1.2); glColor3f(color_red*multi, color_green*multi, color_blue*multi);

glEnd();
glPopMatrix();
}

void draw_motor(void){ 
// ########################
// The motor.
// ########################
glPushMatrix();
glTranslatef(0,0,robot_top_platform_triangle_radius); // jump out to the arm radius
glRotatef(90,0,1,0);
//glColor3f(.78f, .78f, .78f);
draw_closed_cylinder(robot_top_arm_connector_radius/2,robot_top_motor_gapfromzero); //motor axle
glPushMatrix();
glTranslatef(0,0,robot_top_motor_gapfromzero); // jump back to draw motor
//color_darken();
draw_closed_cylinder(robot_top_motor_radius,robot_top_motor_length); //motor
glPopMatrix();
// pop is in top arm section
}

void draw_top_arm(int current_slice){
// ########################
// The top arm and its motor joint.
// ########################
glPushMatrix();
glPushMatrix();
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
draw_centered_closed_cylinder(robot_top_arm_connector_radius,robot_top_arm_connector_width); //center joint
glPopMatrix();
glRotatef(-90,0,1,0); // turn back to face on
glRotatef(robot_angles[current_slice],1,0,0); // angle it down.
glPushMatrix();
draw_cuboid(robot_top_arm_width, robot_top_arm_height, robot_top_arm_length);
//! maybe draw a sphere now? ##########################################################################
glPopMatrix();
//glTranslatef(0, 0, robot_top_arm_length); // jump out to the joint
//glRotatef(-robot_angles[current_slice],1,0,0); // DONT angle it back to zero.
glPopMatrix();
//the pop from the motor section
glPopMatrix();
}

void draw_effector(void){
// #########################
// Draw the end effector dot
// #########################
glPushMatrix();
calculate_delta();
// Remember, our axes are different. Y is actually Z.
glTranslatef(ee[0],ee[2], 0-ee[1]);
glRotatef(90,1,0,0);
glColor3f(0.9, 0.9, 0.0);
draw_closed_cylinder(robot_bottom_platform_triangle_radius,robot_bottom_platform_thickness); //disc
glPopMatrix();
};

void draw_bottom_arm(int currentslice){
// ####################
// Draws the bottom arm
// ####################


// MOVE THESE CALCULATIONS OUTTA HERE!
/* I think these calculations are flawed... All of them...
top_eqns[currentslice] = ((j_x[currentslice]-ee[0])*cos((currentslice*120.0)*pion180))+((j_y[currentslice]-ee[1])*sin((currentslice*120.0)*pion180));
top_2nd_eqn[currentslice] = ((j_x[currentslice]-ee[0])*sin((currentslice*120.0)*pion180))-((j_y[currentslice]-ee[1])*cos((currentslice*120.0)*pion180));
btm_eqns[currentslice] = (j_z[currentslice]-ee[2]);
btm_2nd_eqns[currentslice] = (j_z[currentslice]-ee[2]);
entire_eqn[currentslice] = top_eqns[currentslice]/btm_eqns[currentslice];
entire_2nd_eqn[currentslice] = top_2nd_eqn[currentslice]/btm_2nd_eqns[currentslice];
robot_arm_first_angles[currentslice] =  atan(entire_eqn[currentslice])/pion180;
robot_arm_second_angles[currentslice] =  atan(entire_2nd_eqn[currentslice])/pion180;
*/

//robot_fourth_angles[currentslice] = atan((j_real_x[currentslice]-eereal_x[currentslice]) / (j_real_y[currentslice]-eereal_y[currentslice]))/pion180;
//robot_fourth_angles[currentslice] = atan((j_real_x[currentslice]-eereal_x[currentslice])*sin((currentslice*120.0)*pion180) / (j_real_y[currentslice]-eereal_y[currentslice])*cos((currentslice*120.0)*pion180))/pion180;



/*robot_fourth_angles[currentslice] = atan(
                                        ((eereal_y[currentslice]-j_real_y[currentslice]) *cos((currentslice*120.0)*pion180))
                                        /
                                        ((j_real_x[currentslice]-eereal_x[currentslice]) *sin((currentslice*120.0)*pion180))
                                        )/pion180;
*/

// ### NOTE: The Fourth angles need to take the Second or Third Angles into account. (ie. implement something with the Z axis)
/*
robot_fourth_angles[0] = atan(
                                        ((eereal_y[0]*sin((0*120.0)*pion180)-j_real_y[0]*cos((0*120.0)*pion180)))
                                        /
                                        ((j_real_x[0]*cos((0*120.0)*pion180)-eereal_x[0]*sin((0*120.0)*pion180)))
                                        )/pion180;*/
                                        
//robot_fourth_angles[currentslice] = acos((((j_real_z[currentslice])-eereal_z[currentslice])/cos(robot_arm_first_angles[currentslice]*pion180))/robot_bottom_arm_length)/(pion180);
/*
robot_fourth_angles[0] =
fabs(atan(
(((
(eereal_y[0]*cos(0*pion180))
-
(eereal_x[0]*sin(0*pion180))
)-(
(j_real_y[0]*cos(0*pion180))
-
(j_real_x[0]*sin(0*pion180))
))/((
(j_real_x[0]*cos(0*pion180))
+
(j_real_y[0]*sin(0*pion180))
)-(
(eereal_x[0]*cos(0*pion180))
+
(eereal_y[0]*sin(0*pion180))
))))
/
(pion180)) + (0);

robot_fourth_angles[1] =
(atan(
(((
(eereal_y[1]*cos(30*pion180))
-
(eereal_x[1]*sin(30*pion180))
)-(
(j_real_y[1]*cos(30*pion180))
-
(j_real_x[1]*sin(30*pion180))
))/((
(j_real_x[1]*cos(30*pion180))
+
(j_real_y[1]*sin(30*pion180))
)-(
(eereal_x[1]*cos(30*pion180))
+
(eereal_y[1]*sin(30*pion180))
))))
/
(pion180)) + (0);
*/
//this is a side length maybe.
//robot_fourth_angles[1] = sqrt(pow(ee[0],2)+pow(ee[1],2))*(sin(((0-(atan(ee[1]/ee[0])/(pion180)))*(pion180))));
// This is only one side length.

//realjz - eerealz = 34.592

//### = ((j_real_z[currentslice] - eereal_z[currentslice]) / cos(robot_angles[2]*pion180))

//robot_fourth_angles[2] = tan(sqrt(pow(ee[0],2)+pow(ee[1],2))*(sin(((60-(atan(ee[1]/ee[0])/(pion180)))*(pion180))))/(((fabs(eereal_z[2]) - fabs(j_real_z[2])) / cos(robot_angles[2]*pion180))))/(pion180);

//robot_fourth_angles[1] = tan(-sqrt(pow(ee[0],2)+pow(ee[1],2))*(sin(((60+(atan(ee[1]/ee[0])/(pion180)))*(pion180))))/(((fabs(eereal_z[1]) - fabs(j_real_z[1])) / cos(robot_angles[1]*pion180))))/(pion180);

//robot_fourth_angles[0] = tan(sqrt(pow(ee[0],2)+pow(ee[1],2))*(sin(((0+(atan(ee[1]/ee[0])/(pion180)))*(pion180))))/(((fabs(eereal_z[0]) - fabs(j_real_z[0])) / cos(robot_angles[0]*pion180))))/(pion180);



//robot_fourth_angles[2] =  ((fabs(eereal_z[2]) - fabs(j_real_z[2])) / cos(robot_angles[2]*pion180));
//FIRSTPART = ((fabs(eereal_z[2]) - fabs(j_real_z[2])) / cos(robot_angles[2]*pion180))
//SECONDPART = sqrt(pow(ee[0],2)+pow(ee[1],2))*(sin(((60-(atan(ee[1]/ee[0])/(pion180)))*(pion180))));



/*
len_a_a[currentslice] = j_real_y[currentslice] - eereal_y[currentslice];
len_a_b[currentslice] = j_real_x[currentslice] - eereal_x[currentslice];
len_a[currentslice] = sqrt(pow(len_a_a[currentslice],2)+pow(len_a_b[currentslice],2));

len_b_a[currentslice] = base_y[currentslice] - j_real_y[currentslice];
len_b_b[currentslice] = base_x[currentslice] - j_real_x[currentslice];
len_b[currentslice] = sqrt(pow(len_b_a[currentslice],2)+pow(len_b_b[currentslice],2));

len_c_a[currentslice] = base_y[currentslice] - eereal_y[currentslice];
len_c_b[currentslice] = base_x[currentslice] - eereal_x[currentslice];
len_c[currentslice] = sqrt(pow(len_c_a[currentslice],2)+pow(len_c_b[currentslice],2));
*/


//if (currentslice == 1) {
//if(holdingC == 1){
//printf("sqrt(%f^2+%f^2)*(sin(((60-(atan(%f/%f)/(pi/180)))*(pi/180))))\n", ee[0], ee[1], ee[1], ee[0]);
//}
//}
//angle_a[currentslice] = acos((pow(len_a[currentslice],2)+pow(len_b[currentslice],2)-pow(len_c[currentslice],2))/(2*(len_a[currentslice])*(len_b[currentslice])))/(pion180);
//robot_fourth_angles[currentslice] = angle_a[currentslice];




//float angle_b = ;
//float angle_c = ;

//float newangle = acos((pow(35.748,2)+pow(3.42,2)-pow(33.56,2))/(2*(35.748)*(3.42)))/(pion180);


multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);

//draw a sphere at the joint.
//gluSphere(quadSphere, 1, 32, 16);

multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);

// Draw a simple line instead.

glBegin(GL_LINES);
//remember, the Z is the X, and the Y is the Z
//glVertex3f(j_real_x[currentslice], j_real_y[currentslice], j_real_z[currentslice]); // origin of the line
//glVertex3f(eereal_x[currentslice], eereal_y[currentslice], eereal_z[currentslice]); // ending point of the line

//glVertex3f(j_real_x[currentslice], j_real_y[currentslice], j_real_z[currentslice]); // origin of the line
glVertex3f(j_real_y[currentslice], j_real_z[currentslice], j_real_x[currentslice]); // origin of the line
//glVertex3f(eereal_x[currentslice], eereal_z[currentslice], eereal_y[currentslice]); // ending point of the line
glVertex3f(ee[0], ee[2], ee[1]); // ending point of the line
glEnd( );




//draw 
//glRotatef(90,1,0,0); // angle it down.
//glRotatef(0-robot_fourth_angles[currentslice],0,0,1); // ROTATE ON Z FIRST A CERTAIN AMOUNT
//glRotatef(-90,0,0,1); // ROTATE ON Z FIRST A CERTAIN AMOUNT
//if(holdingF == 1) {
//glRotatef(30,1,0,0); 
//}//rotate on y
//if(holdingG == 1) {
//glRotatef(30,0,1,0);
//} // rotate on z

//if(holdingH == 1){
//glRotatef(30,0,0,1); // rotate on z
//}
//glRotatef(robot_fourth_angles[currentslice],0,0,1); // ROTATE ON Z FIRST A CERTAIN AMOUNT
//glRotatef(90,1,0,0); // angle it down.
//glRotatef(robot_arm_first_angles[currentslice],1,0,0); // angle it down.

//! I Need to determine the axis that the item is rotated on... it is not _just_ rotated in the Y axis

//glRotatef(robot_arm_second_angles[currentslice],0,0,1); // ROTATE ON Z FIRST A CERTAIN AMOUNT
//glRotatef(robot_arm_second_angles[currentslice],0,1,0); // angle it sideways.
//glRotatef(90,0,1,0); // angle it sideways.

//draw_cuboid(robot_bottom_arm_width, robot_bottom_arm_height, robot_bottom_arm_length);
}