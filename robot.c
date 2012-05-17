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

void use_vertex(int vert);

void draw_delta_robot(void){
// ######################
// Draws the delta robot.
// ######################
glPushMatrix();
draw_slice(0);
draw_slice(1);
draw_slice(2);
glPopMatrix();
draw_effector();
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
glRotatef(rotation,0,1,0);
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


/*
color_red = sin((rotation)*pion180);
color_green = sin((rotation+120)*pion180);
color_blue = sin((rotation+240)*pion180);
*/

draw_top(); // ##DONE
glRotatef(90,0,1,0); // Compensate
draw_motor(); // ##DONE
draw_top_arm(current_slice); // ##DONE
//draw_arm_connector(); //! Do this later.
draw_bottom_arm(current_slice); //! might not need a current slice variable.
//draw_effector_slice(current_slice);
glPopMatrix();
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
glTranslatef(0,0,robot_top_platform_triangle_radius); // jump out to the arm radius
glRotatef(90,0,1,0);
//glColor3f(.78f, .78f, .78f);
draw_closed_cylinder(robot_top_arm_connector_radius/2,robot_top_motor_gapfromzero); //motor axle
glPushMatrix();
glTranslatef(0,0,robot_top_motor_gapfromzero); // jump back to draw motor
//color_darken();
draw_closed_cylinder(robot_top_motor_radius,robot_top_motor_length); //motor
glPopMatrix();
}

void draw_top_arm(int current_slice){
// ########################
// The top arm and its motor joint.
// ########################
glPushMatrix();
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
//glRotatef(90,0,1,0); //turn to the side
draw_centered_closed_cylinder(robot_top_arm_connector_radius,robot_top_arm_connector_width); //center joint
glPopMatrix();
glRotatef(-90,0,1,0); // turn back to face on
glRotatef(robot_angles[current_slice],1,0,0); // angle it down.
glPushMatrix();
draw_cuboid(robot_top_arm_width, robot_top_arm_height, robot_top_arm_length);
//! maybe draw a sphere now? ##########################################################################
glPopMatrix();
glTranslatef(0, 0, robot_top_arm_length); // jump out to the joint
glRotatef(-robot_angles[current_slice],1,0,0); // angle it back to zero.
}

void draw_effector(void){
// #########################
// Draw the end effector dot
// #########################
glPushMatrix();
calculate_delta();
// Remember, our axes are different. Y is actually Z.
glTranslatef(ee[0],ee[2], 0-ee[1]);
//gluSphere(quadSphere, 5, 32, 16);
glRotatef(90,1,0,0);
glColor3f(0.9, 0.9, 0.0);
draw_closed_cylinder(robot_bottom_platform_triangle_radius,1); //disc
glPopMatrix();
};

void draw_bottom_arm(int currentslice){
//
// Draws the bottom arm
//
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
//Calculate the angles required.
/*
robot_joint2[0] = ((((robot_top_arm_length)*cos(robot_angles[currentslice]*pion180))+robot_top_platform_triangle_radius)*cos((rotation*pion180))+30); //X
robot_joint2[1] = ((robot_top_arm_length)*sin(robot_angles[currentslice]*pion180)); //Y
robot_joint2[2] = ((((robot_top_arm_length)*cos(robot_angles[currentslice]*pion180))+robot_top_platform_triangle_radius)*sin((rotation*pion180))+30); //Z
*/
robot_joint2[0] = j_x[currentslice]; //X
robot_joint2[1] = j_y[currentslice]; //Y
robot_joint2[2] = j_z[currentslice]; //Z

//robot_joint2[0] = robot_plat_effector[currentslice][0]; //X
//robot_joint2[1] = robot_plat_effector[currentslice][1]; //Y
//robot_joint2[2] = robot_plat_effector[currentslice][2]; //Z

//glPopMatrix();

//glTranslatef(robot_joint2[0], robot_joint2[2], robot_joint2[3]);
//gluSphere(quadSphere, .7, 32, 16);

//glPushMatrix();


// this is the angle on the side #########################
//robot_arm_first_angles[currentslice] = atan((robot_joint2[1]-robot_plat_effector[currentslice][1])/(robot_joint2[0]-robot_plat_effector[currentslice][0]))/pion180;

// Something is different about the X angles.
//top_eqns[currentslice] = ((j_x[currentslice]-ee[0])/cos((currentslice*120.0)*pion180));

//!!!!! satisfies direct translations! but funny with sides...
//top_eqns[currentslice] = fabs((j_x[currentslice]-ee[0])*cos((currentslice*120.0)*pion180)) + fabs((j_y[currentslice]-ee[1])*sin((currentslice*120.0)*pion180));

//!!!!! no fabs! Correct!
top_eqns[currentslice] = ((j_x[currentslice]-ee[0])*cos((currentslice*120.0)*pion180)) + ((j_y[currentslice]-ee[1])*sin((currentslice*120.0)*pion180));

//Angle 1 is correct!
//But Angle 2 and Angle 3 should also equal zero when they are the same.
//top_2nd_eqn[currentslice] = ((j_x[currentslice]-ee[0])*sin((currentslice*120.0)*pion180)) + ((j_y[currentslice]-ee[1])*cos((currentslice*120.0)*pion180));
//                                 


top_2nd_eqn[currentslice] = ((j_x[currentslice]-ee[0])*sin((currentslice*120.0)*pion180))
                            -
                            ((j_y[currentslice]-ee[1])*cos((currentslice*120.0)*pion180));



ratioXnoY[currentslice] = cos((currentslice*120.0)*pion180);
ratioYnoX[currentslice] = sin((currentslice*120.0)*pion180);





j_x_times_x_ratio[currentslice] = ratioXnoY[currentslice] * j_x[currentslice];
j_y_times_y_ratio[currentslice] = ratioYnoX[currentslice] * j_y[currentslice];
jonez[currentslice] = j_z[currentslice];



btm_eqns[currentslice] = (fabs(ee[2])-fabs(j_z[currentslice]));
btm_2nd_eqns[currentslice] = ((j_z[currentslice])-ee[2]);

entire_eqn[currentslice] = top_eqns[currentslice]/btm_eqns[currentslice];

entire_2nd_eqn[currentslice] = top_2nd_eqn[currentslice]/btm_2nd_eqns[currentslice];

robot_arm_first_angles[currentslice] =  90+atan(entire_eqn[currentslice])/pion180;

robot_arm_second_angles[currentslice] =  atan(entire_2nd_eqn[currentslice])/pion180;


//robot_arm_second_angles[currentslice] = atan((((ee[1]-j_y[currentslice])/sin((currentslice*120.0)*pion180)))/(fabs(ee[2])-fabs(j_z[currentslice])))/pion180;

//#robot_arm_first_angles[currentslice] =  atan((((j_x[currentslice]-ee[0])/cos((currentslice*120.0)*pion180)))/(fabs(ee[2])-fabs(j_z[currentslice])))/pion180;
//#robot_arm_second_angles[currentslice] = atan((((ee[1]-j_y[currentslice])/sin((currentslice*120.0)*pion180)))/(fabs(ee[2])-fabs(j_z[currentslice])))/pion180;

//height = (robot_joint2[1] - robot_end_effector[1])
//length = robot_joint2[0]-robot_end_effector[0] //make sure this is relevant to the corrdinates.



//robot_arm_second_angles[currentslice] = atan((robot_joint2[0]-robot_end_effector[0])/(robot_end_effector[1]-robot_joint2[2]))/pion180;


multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
//glTranslatef(0, 0, robot_top_arm_length);
gluSphere(quadSphere, 1, 32, 16);

glRotatef(robot_arm_first_angles[currentslice],1,0,0); // angle it down.
//glRotatef(robot_arm_first_angles[currentslice]+90,1,0,0); // angle it down.
multi = multi/1.2; glColor3f(color_red*multi, color_green*multi, color_blue*multi);
glRotatef(robot_arm_second_angles[currentslice],0,1,0); // angle it sideways.
draw_cuboid(robot_top_arm_width, robot_top_arm_height, robot_bottom_arm_length);
}