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

// set up bitmap font

#include "bitmap.c"

void makeBitmapFonts(void){
// ########################
// Create the bitmap fonts.
// ########################
GLint i;
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
fontOffset = glGenLists(128);
for (i = 32; i < 127; i++) {
    glNewList(i+fontOffset, GL_COMPILE);
        glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, bitmap_font[i-32]);
    glEndList();
};
};

void printString(char *s){
// #############################################################
// Prints a string using bitmap fonts at current raster location
// #############################################################
glPushAttrib (GL_LIST_BIT);
glListBase(fontOffset);
glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *) s);
glPopAttrib();
};

int line_pos(int lines){
int margin = 6;
return (lines*13)+margin;
}

void left_align_text(int pos, char *charlies){
glRasterPos2f(6 ,line_pos(pos));   printString(charlies);
}

void right_align_text(int pos, char *charlies){
int index = 0;
for(index = 0; charlies[index] != 0; index++);
int text_width = index * 10;
int xpos = window_width - text_width;
glRasterPos2f(xpos - 6 ,line_pos(pos));   printString(charlies);
}

void centre_align_text(int pos, char *charlies){
int index = 0;
for(index = 0; charlies[index] != 0; index++);
int text_width = index * 10;
int xpos = (window_width/2) - (text_width/2);
glRasterPos2f(xpos ,line_pos(pos));   printString(charlies);
}

char * make_text(float number_float){
memset(sprinter, 0, sizeof(sprinter));
sprintf(sprinter, "%.4f", number_float);
return sprinter;
}

void draw_text_layers(void) {
glMatrixMode(GL_PROJECTION);
glPushMatrix();
glLoadIdentity();
glOrtho(0,window_width,0,window_height,0,1);
glMatrixMode(GL_MODELVIEW);
glPushMatrix();
glLoadIdentity();

/*
At this point, glVerte2i(x,y) or glRasterPos2i(x,y) will set the 
vertex/raster position to pixel (x,y) on the front plane (as an 
overlay). When you're done drawing in 2D onto the front plane, you can 
go back to whereever you were.
*/
    glColor3f(0.3, 0.9, 0.3);
    left_align_text(0, "OpenGL Delta Robot Simulator");
    
    glColor3f(0.5, 0.9, 0.5);
    right_align_text(0, "(c) 2012 TimKrins");
    
    glColor3f(0.7, 0.3, 0.3);
    centre_align_text(0, "W,A,S,D,Q,E,I,J,O,K,P,L");
    
    
    int current_line_height = 40;
    char text_str[50];
        
    glColor3f(0.8, 0.8, 0.8);
    right_align_text(current_line_height--, "First Angles:");
    glColor3f(0.1, 0.9, 0.1);

    strcat(text_str,"One ");
    strcat(text_str, make_text(robot_angles[0]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.1, 0.1, 0.9);
    strcat(text_str,"Two ");
    strcat(text_str, make_text(robot_angles[1]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.9, 0.1, 0.1);
    strcat(text_str,"Three ");
    strcat(text_str, make_text(robot_angles[2]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    current_line_height--;
    
    glColor3f(0.8, 0.8, 0.8);
    right_align_text(current_line_height--, "Second Angles:");
    glColor3f(0.1, 0.9, 0.1);

    strcat(text_str,"One ");
    strcat(text_str, make_text(robot_arm_first_angles[0]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.1, 0.1, 0.9);
    strcat(text_str,"Two ");
    strcat(text_str, make_text(robot_arm_first_angles[1]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.9, 0.1, 0.1);
    strcat(text_str,"Three ");
    strcat(text_str, make_text(robot_arm_first_angles[2]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
        current_line_height--;
    
    glColor3f(0.8, 0.8, 0.8);
    right_align_text(current_line_height--, "Third Angles:");
    glColor3f(0.1, 0.9, 0.1);

    strcat(text_str,"One ");
    strcat(text_str, make_text(robot_arm_second_angles[0]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.1, 0.1, 0.9);
    strcat(text_str,"Two ");
    strcat(text_str, make_text(robot_arm_second_angles[1]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.9, 0.1, 0.1);
    strcat(text_str,"Three ");
    strcat(text_str, make_text(robot_arm_second_angles[2]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    current_line_height--;
    
    glColor3f(0.8, 0.8, 0.8);
    right_align_text(current_line_height--, "Debug Variables:");
    
    glColor3f(0.5, 0.9, 0.5);
    strcat(text_str,"TopEqn = ");
    strcat(text_str, make_text(top_eqns[0]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.5, 0.5, 0.9);
    strcat(text_str,"TopEqn = ");
    strcat(text_str, make_text(top_eqns[1]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.9, 0.5, 0.5);
    strcat(text_str,"TopEqn = ");
    strcat(text_str, make_text(top_eqns[2]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
        glColor3f(0.5, 0.9, 0.5);
    strcat(text_str,"Top2ndEqn = ");
    strcat(text_str, make_text(top_2nd_eqn[0]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.5, 0.5, 0.9);
    strcat(text_str,"Top2ndEqn = ");
    strcat(text_str, make_text(top_2nd_eqn[1]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.9, 0.5, 0.5);
    strcat(text_str,"Top2ndEqn = ");
    strcat(text_str, make_text(top_2nd_eqn[2]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.5, 0.9, 0.5);
    strcat(text_str,"BtmEqn = ");
    strcat(text_str, make_text(btm_eqns[0]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.5, 0.5, 0.9);
    strcat(text_str,"BtmEqn = ");
    strcat(text_str, make_text(btm_eqns[1]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.9, 0.5, 0.5);
    strcat(text_str,"BtmEqn = ");
    strcat(text_str, make_text(btm_eqns[2]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.5, 0.9, 0.5);
    strcat(text_str,"Total1Eqn = ");
    strcat(text_str, make_text(entire_eqn[0]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.5, 0.5, 0.9);
    strcat(text_str,"Total1Eqn = ");
    strcat(text_str, make_text(entire_eqn[1]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.9, 0.5, 0.5);
    strcat(text_str,"Total1Eqn = ");
    strcat(text_str, make_text(entire_eqn[2]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
        glColor3f(0.5, 0.9, 0.5);
    strcat(text_str,"Total2Eqn = ");
    strcat(text_str, make_text(entire_2nd_eqn[0]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.5, 0.5, 0.9);
    strcat(text_str,"Total2Eqn = ");
    strcat(text_str, make_text(entire_2nd_eqn[1]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.9, 0.5, 0.5);
    strcat(text_str,"Total2Eqn = ");
    strcat(text_str, make_text(entire_2nd_eqn[2]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    
        current_line_height--;
        glColor3f(0.8, 0.8, 0.8);
    right_align_text(current_line_height--, make_text(frame));
    
    
    current_line_height = 43;
    
    glColor3f(0.1, 0.9, 0.1);
    strcat(text_str,"J (");
    strcat(text_str, make_text(j_x[0]));
    strcat(text_str,",");
    strcat(text_str, make_text(j_y[0]));
    strcat(text_str,",");
    strcat(text_str, make_text(j_z[0]));
    strcat(text_str,")");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.1, 0.1, 0.9);
    strcat(text_str,"J (");
    strcat(text_str, make_text(j_x[1]));
    strcat(text_str,",");
    strcat(text_str, make_text(j_y[1]));
    strcat(text_str,",");
    strcat(text_str, make_text(j_z[1]));
    strcat(text_str,")");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.9, 0.1, 0.1);
    strcat(text_str,"J (");
    strcat(text_str, make_text(j_x[2]));
    strcat(text_str,",");
    strcat(text_str, make_text(j_y[2]));
    strcat(text_str,",");
    strcat(text_str, make_text(j_z[2]));
    strcat(text_str,")");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    current_line_height--;
    
    glColor3f(0.8, 0.8, 0.8);
    strcat(text_str,"EE (");
    strcat(text_str, make_text(ee[0]));
    strcat(text_str,",");
    strcat(text_str, make_text(ee[1]));
    strcat(text_str,",");
    strcat(text_str, make_text(ee[2]));
    strcat(text_str,")");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    current_line_height--;
    //sinXj1x[currentslice]
    
    glColor3f(0.5, 0.9, 0.5);
    strcat(text_str,"ratioXnoY ");
    strcat(text_str, make_text(ratioXnoY[0]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.5, 0.5, 0.9);
    strcat(text_str,"ratioXnoY ");
    strcat(text_str, make_text(ratioXnoY[1]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.9, 0.5, 0.5);
    strcat(text_str,"ratioXnoY ");
    strcat(text_str, make_text(ratioXnoY[2]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    current_line_height--;
    //cosXj1y[currentslice]
    
    glColor3f(0.5, 0.9, 0.5);
    strcat(text_str,"ratioYnoX ");
    strcat(text_str, make_text(ratioYnoX[0]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.5, 0.5, 0.9);
    strcat(text_str,"ratioYnoX ");
    strcat(text_str, make_text(ratioYnoX[1]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.9, 0.5, 0.5);
    strcat(text_str,"ratioYnoX ");
    strcat(text_str, make_text(ratioYnoX[2]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    current_line_height--;
    //cosXj1y[currentslice]
    
    glColor3f(0.5, 0.9, 0.5);
    strcat(text_str,"j_x_times_x_ratio ");
    strcat(text_str, make_text(j_x_times_x_ratio[0]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.5, 0.5, 0.9);
    strcat(text_str,"j_x_times_x_ratio ");
    strcat(text_str, make_text(j_x_times_x_ratio[1]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.9, 0.5, 0.5);
    strcat(text_str,"j_x_times_x_ratio ");
    strcat(text_str, make_text(j_x_times_x_ratio[2]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    current_line_height--;
    glColor3f(0.5, 0.9, 0.5);
    strcat(text_str,"j_y_times_y_ratio ");
    strcat(text_str, make_text(j_y_times_y_ratio[0]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.5, 0.5, 0.9);
    strcat(text_str,"j_y_times_y_ratio ");
    strcat(text_str, make_text(j_y_times_y_ratio[1]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.9, 0.5, 0.5);
    strcat(text_str,"j_y_times_y_ratio ");
    strcat(text_str, make_text(j_y_times_y_ratio[2]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
        current_line_height--;
    glColor3f(0.5, 0.9, 0.5);
    strcat(text_str,"r_y_n_x/r_x_n_y ");
    strcat(text_str, make_text(ratioYnoX[0]/ratioXnoY[0]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.5, 0.5, 0.9);
    strcat(text_str,"r_y_n_x/r_x_n_y ");
    strcat(text_str, make_text(ratioYnoX[1]/ratioXnoY[1]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.9, 0.5, 0.5);
    strcat(text_str,"r_y_n_x/r_x_n_y ");
    strcat(text_str, make_text(ratioYnoX[2]/ratioXnoY[2]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
        glColor3f(0.5, 0.5, 0.9);
        strcat(text_str,"r_x_n_y/r_y_n_x ");
    strcat(text_str, make_text(ratioXnoY[1]/ratioYnoX[1]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.9, 0.5, 0.5);
        strcat(text_str,"r_x_n_y/r_y_n_x ");
    strcat(text_str, make_text(ratioXnoY[2]/ratioYnoX[2]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    
    current_line_height--;
    glColor3f(0.5, 0.9, 0.5);
    strcat(text_str,"j_y_t_y_r/j_x_t_x_r ");
    strcat(text_str, make_text(j_y_times_y_ratio[0]/j_x_times_x_ratio[0]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.5, 0.5, 0.9);
    strcat(text_str,"j_y_t_y_r/j_x_t_x_r ");
    strcat(text_str, make_text(j_y_times_y_ratio[1]/j_x_times_x_ratio[1]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    glColor3f(0.9, 0.5, 0.5);
    strcat(text_str,"j_y_t_y_r/j_x_t_x_r ");
    strcat(text_str, make_text(j_y_times_y_ratio[2]/j_x_times_x_ratio[2]));
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
 /* glColor3f(0.1, 0.9, 0.1);
    strcat(text_str,"TopEqn = ");
    strcat(text_str, make_text(top_eqns[0]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.1, 0.9, 0.1);
    strcat(text_str,"BtmEqn = ");
    strcat(text_str, make_text(btm_eqns[0]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.1, 0.9, 0.1);
    strcat(text_str,"TotalEqn = ");
    strcat(text_str, make_text(entire_eqn[0]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.1, 0.1, 0.9);
    strcat(text_str,"TopEqn = ");
    strcat(text_str, make_text(top_eqns[1]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.1, 0.1, 0.9);
    strcat(text_str,"BtmEqn = ");
    strcat(text_str, make_text(btm_eqns[1]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.1, 0.1, 0.9);
    strcat(text_str,"TotalEqn = ");
    strcat(text_str, make_text(entire_eqn[1]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.9, 0.1, 0.1);
    strcat(text_str,"TopEqn = ");
    strcat(text_str, make_text(top_eqns[2]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.9, 0.1, 0.1);
    strcat(text_str,"BtmEqn = ");
    strcat(text_str, make_text(btm_eqns[2]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    glColor3f(0.9, 0.1, 0.1);
    strcat(text_str,"TotalEqn = ");
    strcat(text_str, make_text(entire_eqn[2]));
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    
  */
    /*
    glColor3f(0.9, 0.1, 0.1);
    strcat(text_str,"Three ");
    strcat(text_str, make_text(robot_arm_first_angles[2]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    */

glMatrixMode(GL_PROJECTION);
glPopMatrix();
glMatrixMode(GL_MODELVIEW);
glPopMatrix();
};