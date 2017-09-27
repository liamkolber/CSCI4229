//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
// Global Variables
int th=90;        //  Azimuth of view angle
int ph=-5;        //  Elevation of view angle
double zh=0;      //  Rotation of teapot
int axes=1;       //  Display axes
int mode=0;       //  What to display
int FPS=60;
int tf1=1;
int tf2=1;
int tf3=1;
int tf4=1;

//  Cosine and Sine in degrees
//#define Cos(x) (cos((x)*3.1415927/180))
//#define Sin(x) (sin((x)*3.1415927/180))
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
// Text output
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...) {
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------  
// Wings of TIE Fighter
void hexagon() {
  glColor3ub(0,0,150);
  glBegin(GL_POLYGON);
  for(int i = 0; i < 6; ++i) {
    glVertex3f(4*sin(i/6.0*2*M_PI), 4*cos(i/6.0*2*M_PI),0);
    glVertex3f(0,0,0.5);
    glVertex3f(4*sin(i/6.0*2*M_PI), 4*cos(i/6.0*2*M_PI),0);
  }
  glEnd();
}
// Body of TIE Fighter
void sphere() {
  glColor3ub(200,0,0);
  glutSolidSphere(1,20,20);
}
// Window piece of TIE Fighter
void ellipse() {
  glColor3ub(0,150,0);
  glutSolidSphere(0.9,20,20);
}
// Arm pieces of TIE Fighter
void cylinder() {
  glColor3ub(150,150,150);
  gluCylinder(gluNewQuadric(),0.1,0.5,3,20,20);
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
// Draws the TIE Fighter object 
void drawTieFighter() {
  glPushMatrix();

  hexagon();
  cylinder();
  glTranslated(0,0,3);
  sphere();
  glTranslated(0.17,0,0);
  ellipse();
  glTranslated(-0.17,0,0);
  glTranslated(0,0,3);
  glRotated(180,0,0,0);
  hexagon();
  cylinder();

  glPopMatrix();
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
// Display all the object in the viewer
void display() {
  const double len = 4;

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();
  glOrtho(-5,5,-5,5,-10,10);

  glRotated(ph,1,0,0);
  glRotated(th,0,1,0);
//------------------------------------------------------------------
  if (tf1==1){
    glPushMatrix();
    glRotated(15,0,-1,0);
    glTranslated(0,0,-14.5);
    drawTieFighter();
    glPopMatrix();
  }
  if (tf2 == 1) {
    glPushMatrix();
    glScaled(1.1,1.1,1.1);
    glRotated(15,0,-1,0);
    glTranslated(0,0,-7);
    drawTieFighter();
    glPopMatrix();
  }
  if (tf3 == 1) {
    glPushMatrix();
    glScaled(1.3,1.3,1.3);
    glRotated(-15,0,-1,0);
    glRotated(45,0,0,1);
    glTranslated(0,0,1);
    drawTieFighter();
    glPopMatrix();
  }
  if (tf4 == 1) {
    glPushMatrix();
    glScaled(1.5,1.5,1.5);
    glRotated(-15,0,-1,0);
    glRotated(45,0,0,-1);
    glRotated(15,1,0,0);
    glTranslated(0,0,8.5);
    drawTieFighter();
    glPopMatrix();
  }
//------------------------------------------------------------------
  glColor3f(1,1,1);
  //  Draw axes
  if (axes) {
    glBegin(GL_LINES);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(len,0.0,0.0);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(0.0,len,0.0);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(0.0,0.0,len);
    glEnd();
    //  Label axes
    glRasterPos3d(len,0.0,0.0);
    Print("X");
    glRasterPos3d(0.0,len,0.0);
    Print("Y");
    glRasterPos3d(0.0,0.0,len);
    Print("Z");
  }
  //  Five pixels from the lower left corner of the window
  glWindowPos2i(5,5);
  //  Print the text string
  Print("View Angle=%d,%d",th,ph);
  //  Render the scene
  glFlush();
  //  Make the rendered scene visible
  glutSwapBuffers();
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y) {
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y) {
  //  Exit on ESC
  if (ch == 27)
    exit(0);
  //  Reset view angle
  else if (ch == '0'){
    th = 0;
    ph = 5;
  }
  //  Toggle axes
  else if (ch == 'a' || ch == 'A')
    axes = 1-axes;
  //else if (ch == 'i' || ch == 'I'){
  //  reshape()
  //}
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height) {
   const double dim=2.5;
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection
   glOrtho(-w2h*dim,+w2h*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[]) {
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(1200,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Liam Kolber: Assignment 3");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------