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
int ph=-15;        //  Elevation of view angle
double zh=0;      //  Rotation of teapot
int axes=0;       //  Display axes
int mode=0;       //  What to display
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
int fov=55;       //  Field of view (for perspective)
int FPS=60;
int tf1=1;
int tf2=0;
int tf3=1;
int tf4=1;
int xw1=1;
int xw2=1;
int falc=0;
float PI = 3.1415962;
//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))
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
/*
 *  Set projection
 */
static void Project() {
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/10,10*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
// FRICKIN' LASER BEAMS
void lasers() {
  //glColor3ub(0,150,0);
      GLfloat radius = 0.1;
    GLfloat height = 5;
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    /** Draw the tube */
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
}
// Wings of TIE Fighter
void hexagon() {
  glColor3ub(0,0,150);
  glBegin(GL_POLYGON);
  for(int i = 0; i < 6; ++i) {
    glColor3ub(0,0,150);
    glVertex3f(4*sin(i/6.0*2*M_PI), 4*cos(i/6.0*2*M_PI),0);
    glVertex3f(0,0,0.5);
    glVertex3f(0,0,-0.5);
    glColor3ub(0,0,50);
    glVertex3f(4*sin(i/6.0*2*M_PI), 4*cos(i/6.0*2*M_PI),0);
  }
  glEnd();
}
// Body of TIE Fighter
static void Vertex1(double th,double ph) {
   //glColor4f(1,1,1,1);
   glColor3ub(200,0,0);
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}
void sphere() {
  glColor3ub(200,0,0);
   const int d=5;
   int th,ph;
   double r = 1;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glColor3ub(200,0,0);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      glColor3ub(200,0,0);
      for (th=0;th<=360;th+=d)
      {
         Vertex1(th,ph);
         Vertex1(th,ph+d);
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}
// Window piece of TIE Fighter
static void Vertex2(double th,double ph) {
   //glColor4f(1,1,1,1);
   glColor3ub(0,100,50);
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}
void ellipse() {
  glColor3ub(0,100,50);
     const int d=5;
   int th,ph;
   double r = 0.9;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glColor3ub(0,100,50);
   glTranslated(0.05,0,0);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex2(th,ph);
         Vertex2(th,ph+d);
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}
// Arm pieces of TIE Fighter
void cylinder() {
  //0.1,0.5
  glColor3ub(100,100,100);
      GLfloat radius = 0.5;
    GLfloat height = 5;
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    /** Draw the tube */
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
}
// Gunns for X-Wing wings
void guns() {
  //0.1,0.2
  glColor3ub(150,50,0);
    GLfloat radius = 0.1;
    GLfloat height = 5;
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    /** Draw the tube */
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
  glPushMatrix();
  glTranslated(0,0,1);
  //0,0.4
  glColor3ub(100,0,0);
  //gluDisk(gluNewQuadric(),0,0.4,32,32);
  glPopMatrix();
  //glTranslated(0,0,3);
  //gluDisk(gluNewQuadric(),0,0.1,32,32);
}
// Rockets for X-Wing inner
void rocket() {
  glColor3ub(150,0,0);
  //0.4,0.4
    GLfloat radius = 0.4;
    GLfloat height = 5;
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    /** Draw the tube */
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
  //gluCylinder(gluNewQuadric(),0.4,0.4,5,32,32);
  //glColor3ub(50,0,0);
  //gluDisk(gluNewQuadric(),0,0.4,32,32);
  //glTranslated(0,0,5);
  //gluDisk(gluNewQuadric(),0,0.4,32,32);
}
// Wings of X-Wing
void wings() {
  int org = 0;
  float thick = 0.2;
  int length = 7;
  int h1 = 2;
  int h2 = 1;
  glColor3ub(0,100,50);
  glBegin(GL_QUADS);
  //side1
  glVertex3f(org,org,org);
  glVertex3f(length,org,org);
  glVertex3f(length,h2,org);
  glVertex3f(org,h1,org);
  //side2
  glVertex3f(org,org,thick);
  glVertex3f(length,org,thick);
  glVertex3f(length,h2,thick);
  glVertex3f(org,h1,thick);
  //big
  glVertex3f(org,org,org);
  glVertex3f(org,h1,org);
  glVertex3f(org,h1,thick);
  glVertex3f(org,org,thick);
  //small
  glColor3ub(100,0,0);
  glVertex3f(org,org,org);
  glVertex3f(org,org,thick);
  glVertex3f(length,org,thick);
  glVertex3f(length,org,org);
  //bottom
  glVertex3f(length,org,org);
  glVertex3f(length,org,thick);
  glVertex3f(length,h2,thick);
  glVertex3f(length,h2,org);
  //tip
  glVertex3f(org,org,org);
  glVertex3f(org,org,thick);
  glVertex3f(org,h1,thick);
  glVertex3f(org,h1,org);
  //top
  glVertex3f(org,h1,org);
  glVertex3f(org,h1,thick);
  glVertex3f(length,h2,thick);
  glVertex3f(length,h2,org);

  glEnd();

  glTranslated(7,3,0.1);
  glRotated(90,0,0,0);
  guns();

  glPushMatrix();
  glTranslated(-5,0,0);
  rocket();
  glPopMatrix();
}
// Fuselage of X-Wing
void body() {
  int org = 0;
  int l = 4;
  int w = 2;
  int h = 2;
  glColor3ub(0,0,125);
  glBegin(GL_QUADS);  
  // Back
  glVertex3f(org,org,org);
  glVertex3f(org,w,org);
  glVertex3f(org,w,h);
  glVertex3f(org,org,h);
  // Front
  glVertex3f(l,org,org);
  glVertex3f(l,w,org);
  glVertex3f(l,w,h);
  glVertex3f(l,org,h);
  glColor3ub(0,0,100);
  // Side2
  glVertex3f(org,org,org);
  glVertex3f(l,org,org);
  glVertex3f(l,org,h);
  glVertex3f(org,org,h);
  // Side 2
  glVertex3f(org,w,org);
  glVertex3f(l,w,org);
  glVertex3f(l,w,h);
  glVertex3f(org,w,h);
  // Top
  glColor3ub(0,0,150);
  glVertex3f(org,org,org);
  glVertex3f(org,w,org);
  glVertex3f(l,w,org);
  glVertex3f(l,org,org);
  //Bottom
  glVertex3f(org,org,h);
  glVertex3f(org,w,h);
  glVertex3f(l,w,h);
  glVertex3f(l,org,h);

  glEnd();
}
// Nose of X-Wing
void nose() {
  int org = 0;
  float thick = 2;
  int length = 7;
  int h1 = 2;
  float h2 = 0.5;
  glColor3ub(0,0,100);
  glBegin(GL_QUADS);
  //side1
  glVertex3f(org,org,org);
  glVertex3f(length,org,org);
  glVertex3f(length,h2,org);
  glVertex3f(org,h1,org);
  //side2
  glVertex3f(org,org,thick);
  glVertex3f(length,org,thick);
  glVertex3f(length,h2,thick);
  glVertex3f(org,h1,thick);
  //big
  glVertex3f(org,org,org);
  glVertex3f(org,h1,org);
  glVertex3f(org,h1,thick);
  glVertex3f(org,org,thick);
  //small
  glColor3ub(0,0,150);
  glVertex3f(org,org,org);
  glVertex3f(org,org,thick);
  glVertex3f(length,org,thick);
  glVertex3f(length,org,org);
  //bottom
  glVertex3f(length,org,org);
  glVertex3f(length,org,thick);
  glVertex3f(length,h2,thick);
  glVertex3f(length,h2,org);
  //tip
  //glColor3ub(0,0,125);
  glVertex3f(org,org,org);
  glVertex3f(org,org,thick);
  glVertex3f(org,h1,thick);
  glVertex3f(org,h1,org);
  //top
  glVertex3f(org,h1,org);
  glVertex3f(org,h1,thick);
  glVertex3f(length,h2,thick);
  glVertex3f(length,h2,org);

  glEnd();
}
// Body of Falcon
void falconBody() {
  glColor3ub(0,0,150);
  glBegin(GL_POLYGON);
  float sides = 50;
  for(int i = 0; i < sides; ++i) {
    glColor3ub(0,0,150);
    glVertex3f(4*sin(i/sides*2*M_PI), 4*cos(i/sides*2*M_PI),0);
    glVertex3f(4*sin(i/sides*2*M_PI), 4*cos(i/sides*2*M_PI),1);
    glVertex3f(0,0,0.5);
    glVertex3f(0,0,-0.5);
    glColor3ub(0,0,50);
    glVertex3f(4*sin(i/sides*2*M_PI), 4*cos(i/sides*2*M_PI),0);
  }
  glEnd();
}
// Cockpit of Falcon
void cockpit() {
  glColor3ub(0,0,125);
  //gluCylinder(gluNewQuadric(),0.9,0.9,5,32,32);
  //glColor3ub(0,0,100);
  //gluDisk(gluNewQuadric(),0,0.9,32,32);
  //glTranslated(0,0,5);
  //gluDisk(gluNewQuadric(),0,0.9,32,32);
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
  glColor3ub(0,255,0);
  glRotated(90,1,0,0);
  glRotated(90,0,1,0);
  glTranslated(0,2,1);
  lasers();
  glTranslated(0,2,4);
  lasers();

  glPopMatrix();
}
// Draws the X-Wing object
void drawXWing() {
  glPushMatrix();
  wings();
  glRotated(180,0,1,-1);
  glTranslated(8,-3,-0.2);
  wings();
  glPopMatrix();

  glPushMatrix();
  glRotated(30,0,1,0);
  wings();
  glRotated(180,0,1,-1);
  glTranslated(8,-3,0.2);
  wings();
  glPopMatrix();

  glPushMatrix();
  glRotated(90,0,0,1);
  glRotated(12,1,0,0);
  glTranslated(-0.5,-0.5,-1);
  body();
  glRotated(90,1,0,0);
  glTranslated(4,0,-2);
  nose();

  glColor3ub(255,0,0);
  glRotated(90,1,0,0);
  glRotated(90,0,1,0);
  glTranslated(-0.5,8.5,2);
  lasers();
  glTranslated(3,-14,4);
  lasers();
  glPopMatrix();
}
// Draws Millenium Falcon
void drawMilleniumFalcon() {
  glPushMatrix();
  falconBody();
  glRotated(180,0,0,0);
  glTranslated(0,0,-1);
  falconBody();

  glScaled(1,1.7,0.5);
  glTranslated(0,0.4,0);
  nose();
  glRotated(180,0,0,0);
  glTranslated(0,0.75,-2);
  nose();

  glScaled(1/1,1/1.7,1/0.5);
  glTranslated(2,4,0.5);
  glRotated(90,0,1,0);
  glRotated(110,1,0,0);
  cockpit();
  glRotated(70,1,0,0);
  glTranslated(0,-4.12,-3.5);
  glScaled(1,1,0.5);
  cockpit();
  glTranslated(0,0,-5);
  glScaled(1,1,2);
  glColor3ub(0,0,150);
  //glutSolidSphere(0.9,32,32);

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
  //glOrtho(-5,5,-5,5,-10,10);
  //glRotated(ph,1,0,0);
  //glRotated(th,0,1,0);
  //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }


  if (tf1==1){
    glPushMatrix();
    glRotated(45,0,-1,0);
    glRotated(25,0,0,-1);
    glTranslated(0,-5,-18.5);
    drawTieFighter();
    glPopMatrix();
  }
  if (tf2 == 1) {
    glPushMatrix();
    glScaled(1,1,1);
    glRotated(15,0,-1,0);
    glTranslated(0,5,-7);
    drawTieFighter();
    glPopMatrix();
  }
  if (tf3 == 1) {
    glPushMatrix();
    glScaled(1.3,1.3,1.3);
    glRotated(-15,0,-1,0);
    glRotated(45,0,0,1);
    glTranslated(-10,0,1);
    drawTieFighter();
    glPopMatrix();
  }
  if (tf4 == 1) {
    glPushMatrix();
    glScaled(1,1,1);
    glRotated(-15,0,-1,0);
    glRotated(45,0,0,-1);
    glRotated(15,1,0,0);
    glTranslated(0,10,8.5);
    drawTieFighter();
    glPopMatrix();
  }
  if (xw1 == 1) {
    glPushMatrix();
    glScaled(0.7,0.7,0.7);
    glRotated(230,0,-1,-1);
    glTranslated(8,-4,0);
    drawXWing();
    glPopMatrix();
  }
  if (xw2 == 1) {
    glPushMatrix();
    glScaled(0.7,0.7,0.7);
    glRotated(230,-1,-1,0);
    glTranslated(6,-20,14);
    drawXWing();
    glPopMatrix();
  }
  if (falc == 1) {
    glPushMatrix();
    glTranslated(-15,-10,-15);
    glRotated(90,0,1,0);
    glScaled(1.5,1.5,1.5);
    drawMilleniumFalcon();
    glPopMatrix();
  }


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
// Called when an arrow key is pressed
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
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
// Called when an arrow key is pressed
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
   else if (ch == 'm' || ch == 'M')
     mode = 1-mode;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;

   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
// Called when window is resized
void reshape(int width,int height) {
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
// Does the thing
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