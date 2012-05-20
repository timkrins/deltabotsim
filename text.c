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

// set up bitmap font

#include "bitmap.c"


#define red glColor3f(0.9, 0.1, 0.1)
#define green glColor3f(0.1, 0.9, 0.1)
#define blue glColor3f(0.1, 0.1, 0.9)


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
sprintf(sprinter, "%.1f", number_float);
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

    glColor3f(0.2, 0.9, 0.2);
    left_align_text(0, "OpenGL Delta Robot Simulator");
    
    glColor3f(0.5, 0.9, 0.5);
    right_align_text(0, "(c?) 2012 TimKrins. For personal use only.");
    
    int current_line_height = 40;
    
    char text_str[50];
        
    glColor3f(0.8, 0.8, 0.8);
    right_align_text(current_line_height--, "Motor Angles:");
    memset(text_str, 0, sizeof(text_str));

    red;
    strcat(text_str,"One ");
    strcat(text_str, make_text(robot_angles[0]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    green;
    strcat(text_str,"Two ");
    strcat(text_str, make_text(robot_angles[1]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    blue;
    strcat(text_str,"Three ");
    strcat(text_str, make_text(robot_angles[2]));
    strcat(text_str," degrees.");
    right_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    current_line_height = 15;
    
    glColor3f(0.8, 0.8, 0.8);
    right_align_text(current_line_height--, "[W,A,S,D] to orbit");
    right_align_text(current_line_height--, "[Q,E] to zoom");
    right_align_text(current_line_height--, "[Z,X] to change vertical orbit position");
    current_line_height--;
    right_align_text(current_line_height--, "[I,J] [O,K] [P,L] to adjust angles");
    current_line_height--;
    right_align_text(current_line_height--, "[0] to reset angles");
    right_align_text(current_line_height--, "[1,2,3] set to 70degrees");
    right_align_text(current_line_height--, "[4,5,6] set to 45degrees");
    right_align_text(current_line_height--, "[7,8,9] set to -50degrees");
    
    current_line_height--;
    right_align_text(current_line_height--, ":D Enjoy!");
 
    current_line_height = 40;
    
    glColor3f(0.8, 0.8, 0.8);
    left_align_text(current_line_height--, "End Effector Position:");
    
    glColor3f(0.8, 0.8, 0.8);
    strcat(text_str,"(");
    strcat(text_str, make_text(ee[0]));
    strcat(text_str,",");
    strcat(text_str, make_text(ee[1]));
    strcat(text_str,",");
    strcat(text_str, make_text(ee[2]));
    strcat(text_str,") mm");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
        current_line_height--;
    glColor3f(0.8, 0.8, 0.8);
    left_align_text(current_line_height--, "Base Joint Positions:");
    
    red;
    strcat(text_str,"J (");
    strcat(text_str, make_text(base_x[0]));
    strcat(text_str,",");
    strcat(text_str, make_text(base_y[0]));
    strcat(text_str,",");
    strcat(text_str, make_text(base_z[0]));
    strcat(text_str,") mm");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    green;
    strcat(text_str,"J (");
    strcat(text_str, make_text(base_x[1]));
    strcat(text_str,",");
    strcat(text_str, make_text(base_y[1]));
    strcat(text_str,",");
    strcat(text_str, make_text(base_z[1]));
    strcat(text_str,") mm");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    blue;
    strcat(text_str,"J (");
    strcat(text_str, make_text(base_x[2]));
    strcat(text_str,",");
    strcat(text_str, make_text(base_y[2]));
    strcat(text_str,",");
    strcat(text_str, make_text(base_z[2]));
    strcat(text_str,") mm");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    current_line_height--;
    glColor3f(0.8, 0.8, 0.8);
    left_align_text(current_line_height--, "Arm Joint Positions:");
    
    red;
    strcat(text_str,"J (");
    strcat(text_str, make_text(j_real_x[0]));
    strcat(text_str,",");
    strcat(text_str, make_text(j_real_y[0]));
    strcat(text_str,",");
    strcat(text_str, make_text(j_real_z[0]));
    strcat(text_str,") mm");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    green;
    strcat(text_str,"J (");
    strcat(text_str, make_text(j_real_x[1]));
    strcat(text_str,",");
    strcat(text_str, make_text(j_real_y[1]));
    strcat(text_str,",");
    strcat(text_str, make_text(j_real_z[1]));
    strcat(text_str,") mm");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    blue;
    strcat(text_str,"J (");
    strcat(text_str, make_text(j_real_x[2]));
    strcat(text_str,",");
    strcat(text_str, make_text(j_real_y[2]));
    strcat(text_str,",");
    strcat(text_str, make_text(j_real_z[2]));
    strcat(text_str,") mm");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    current_line_height--;
    glColor3f(0.8, 0.8, 0.8);
    left_align_text(current_line_height--, "End Effector Joint Positions:");
    
    red;
    strcat(text_str,"J (");
    strcat(text_str, make_text(eereal_x[0]));
    strcat(text_str,",");
    strcat(text_str, make_text(eereal_y[0]));
    strcat(text_str,",");
    strcat(text_str, make_text(eereal_z[0]));
    strcat(text_str,") mm");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    green;
    strcat(text_str,"J (");
    strcat(text_str, make_text(eereal_x[1]));
    strcat(text_str,",");
    strcat(text_str, make_text(eereal_y[1]));
    strcat(text_str,",");
    strcat(text_str, make_text(eereal_z[1]));
    strcat(text_str,") mm");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    blue;
    strcat(text_str,"J (");
    strcat(text_str, make_text(eereal_x[2]));
    strcat(text_str,",");
    strcat(text_str, make_text(eereal_y[2]));
    strcat(text_str,",");
    strcat(text_str, make_text(eereal_z[2]));
    strcat(text_str,") mm");
    left_align_text(current_line_height--, text_str);
    memset(text_str, 0, sizeof(text_str));
    
    current_line_height--;
    glColor3f(0.8, 0.8, 0.8);
    left_align_text(current_line_height--, "All these values are rounded");
    left_align_text(current_line_height--, "to 2 decimal places");
    left_align_text(current_line_height--, "but you can change it in code");
    
    current_line_height--;
    left_align_text(current_line_height--, "You should visually check");
    left_align_text(current_line_height--, "for collisions; this code does");
    left_align_text(current_line_height--, "not test for impossible positions.");
    
    
glMatrixMode(GL_PROJECTION);
glPopMatrix();
glMatrixMode(GL_MODELVIEW);
glPopMatrix();
};