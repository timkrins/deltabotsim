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

float startrotation = 0.0f; //marked for removal

int value[3]; //marked for removal


void variables_init(void){

// mark alll this for deletion
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

void handleMouseClick(int button, int action){
if(action == GLFW_PRESS){
clicked = 1;
} else {
clicked = 0;
};
};











// Redo the text drawing into a seperate identity.

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

//draw_triangular_platform(x1,y1,z1,x2,y2,z2,x3,y3,z3,thickness);


//(robot_top_platform_triangle_sidelength/2)

void draw_topbase_full_motors(void){
//including attach points for arm
// startrotation = 0.0f;
//remember to pop and push.

for (int i = -1; i < 2; i++) {
  glPushMatrix(); //reset to zero
  glRotatef(startrotation+(120*i),0,1,0); //rotate to desired angle
  glTranslatef(0,0,robot_top_platform_triangle_radius); // jump out to radius
  glColor3f(.5f, .5f, .5f); //set color
  glRotatef(180,0,1,0); //flip
  draw_topbase_onethird_motors(); // draw joint and motors
  glPopMatrix(); //pop
  };
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





void draw_arms_mid_full_joiner(void){
for (int i = 0; i < 3; i++) {
  glPushMatrix();
  rotation = startrotation+(i*120);
  color_red = sin((rotation+10)*pion180);
  color_green = sin((rotation+130)*pion180);
  color_blue = sin((rotation+250)*pion180);
  glColor3f(color_red, color_green, color_blue);
  glRotatef(rotation,0,1,0);
  glTranslatef(0,0,robot_top_platform_triangle_radius);
  if(i == 0){  glRotatef(robot_angle_one,1,0,0);  }
  if(i == 1){  glRotatef(robot_angle_two,1,0,0);  }
  if(i == 2){  glRotatef(robot_angle_three,1,0,0);  }
  glTranslatef(0,0,robot_top_arm_length);
  glRotatef(270,0,1,0);
  color_darken();
  draw_centered_closed_cylinder(robot_mid_connector_radius,robot_mid_connector_width); //center joint
  glPopMatrix();
  };
};

void draw_arms_bottom_onehalf_arms(void){

};

void draw_arms_bottom_full_joiner(void){

};