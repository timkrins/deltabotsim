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

//! these are excessive, seeing as I already have vector functions defined.
#define vector_crossProduct(b,c) { (b.y*c.z-c.y*b.z) , (b.z*c.x-c.z*b.x) , (b.x*c.y-c.x*b.y) }
#define vector_minus(b,c) { (b.x-c.x), (b.y-c.y), (b.z-c.z) }
#define vector_dotProduct(b,c) ((b.x*c.x)+(b.y*c.y)+(b.z*c.z))
#define vector_magnitude(b) sqrt((b.x*b.x)+(b.y*b.y)+(b.z*b.z))
#define PI 3.14159265
#define calculate_chunk vec3d z = { 0 , 0 , 1 }; \
                        vec3d p = vector_minus(a, b); \
                        vec3d t = vector_crossProduct(z, p);\
                        float d = vector_dotProduct(z,p);\
                        float m = vector_magnitude(p);\
                        double angle = (180/PI*acos(d/m));\
                        glTranslated(b.x,b.y,b.z)
                        
#define color_chunk(c) if(i == 0) { glColor3f(c, 0, 0); } \
                       if(i == 1) { glColor3f(0, c, 0); } \
                       if(i == 2) { glColor3f(0, 0, c); }
                       
#define grey(c) glColor3f(c, c, c)

void draw_delta_robot(void);

    void calculate(void);
    void simple_base(int i);
    void simple_top_arm(int i);
    void simple_bot_arm(int i);
    void simple_ee(int i); 
    
    void complex_base(int i);
    void complex_top_arm(int i);
    void complex_bot_arm(int i);
    void complex_ee(void);

void draw_delta_robot(void){
// ######################
// Draws the delta robot.
// ######################
glPushMatrix();

calculate();

complex_base(0);
complex_base(1);
complex_base(2);

complex_top_arm(0);
complex_top_arm(1);
complex_top_arm(2);

complex_bot_arm(0);
complex_bot_arm(1);
complex_bot_arm(2);

complex_ee();

glPopMatrix();
}

void complex_base(int i){
glPushMatrix();

color_chunk(0.8);

vec3d a = {0, 0, 0};
vec3d b = {base_y[i], base_z[i], base_x[i]};

calculate_chunk;

glPushMatrix(); // draw joint and motor.
glRotatef(-((i*-120)+90),0,1,0);
draw_centered_closed_cylinder(robot_top_arm_radius*1.1, robot_top_arm_radius*2.1);
draw_closed_cylinder(robot_top_arm_radius/2,robot_top_motor_axle);
glTranslated(0,0,robot_top_motor_axle);
draw_closed_cylinder(robot_top_motor_radius,robot_top_motor_length);
glPopMatrix(); // end of joint and motor.

glRotated(angle,t.x,t.y,t.z);

glTranslated(3,0,0);
grey(0.3);
draw_closed_cylinder(robot_top_arm_radius,15);

glPopMatrix();
}

void complex_top_arm(int i){
glPushMatrix();
color_chunk(0.7);
vec3d a = {base_y[i], base_z[i], base_x[i]};
vec3d b = {j_real_y[i], j_real_z[i], j_real_x[i]};
calculate_chunk;
glPushMatrix(); // draw joint.
draw_sphere(robot_top_arm_radius*2);
glPopMatrix(); // end of joint.
glRotated(angle,t.x,t.y,t.z);
draw_closed_cylinder(robot_top_arm_radius,m);//draws back towards the origin.
glPopMatrix();
}

void complex_bot_arm(int i){
glPushMatrix();
//draw arm one

vec3d a = {j_real_y[i], j_real_z[i], j_real_x[i]};
vec3d b = {eereal_y[i], eereal_z[i], eereal_x[i]};
calculate_chunk;

glPushMatrix();
color_chunk(0.6);

glPushMatrix(); // draw joint.
draw_sphere(robot_top_arm_radius*2);
glPopMatrix(); // end of joint.

glRotated(angle,t.x,t.y,t.z);//draws back towards the origin.
draw_closed_cylinder(robot_top_arm_radius,m);
glPopMatrix();

glPopMatrix();
}

void complex_ee(void){
glPushMatrix();
grey(0.5);
vec3d a = {ee[1], ee[2]-robot_ee_thickness, ee[0]};
vec3d b = {ee[1], ee[2], ee[0]};

calculate_chunk;

glRotated(angle,t.x,t.y,t.z);//draws back towards the origin.
draw_closed_cylinder(robot_ee_radius,m);
glPopMatrix();
}

void calculate(void){
calculate_delta();
}