// Liam Kolber
// CSCI 4229
// CU Boulder Fall 2017
// Assignment 4
//------------------------------------------------------------------
//---------------------------Initializations------------------------
//------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
// OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//------------------------------------------------------------------
//-------------------------Global Variables-------------------------
//------------------------------------------------------------------
int th=0;         // Azimuth of view angle
int ph=0;         // Elevation of view angle
int axes=0;       // Display axes
int mode=0;       // What to display
double asp=1;     // Aspect ratio
double dim=20.0;  // Size of world
int fov=55;       // Field of view (for perspective)
// First-person specific
int fp = 0;       // Toggle
int rot = 0.0;    // Rotational angle
double Ex = 0;    // Eye coords
double Ey = 0;
double Ez = 5;
double Cx = 0;    // Centering coords
double Cz = 0; 
// Variables used for enabling/disabling objects during testing
int tf1=1;
int tf2=0;
int tf3=1;
int tf4=1;
int xw1=1;
int xw2=1;
int falc=1;
int death=1;
//  Cosine and Sine in degrees
float PI = 3.1415927;
#define Cos(x) (cos((x)*PI/180))
#define Sin(x) (sin((x)*PI/180))
//------------------------------------------------------------------
//---------------------------General Stuff--------------------------
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
   // Display characters one at a time at current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}
// Set projection
static void Project() {
   // Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   // Undo previous transformations
   glLoadIdentity();
   // Perspective transformation
   if(fp) {
      gluPerspective(fov,asp,dim/4,4*dim);
   }
   else {
      if (mode)
         gluPerspective(fov,asp,dim/4,4*dim);
      // Orthogonal projection
      else
         glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   } 
   // Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   // Undo previous transformations
   glLoadIdentity();
}
//------------------------------------------------------------------
//---------------------------Object Parts---------------------------
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
void sphere() {
  const int d=5;
  int th,ph;
  double r = 1;
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glScaled(r,r,r);

  //  Latitude bands
  for (ph=-90;ph<90;ph+=d) {
    glBegin(GL_QUAD_STRIP);
    glColor3ub(200,0,0);
    for (th=0;th<=360;th+=d) {
       glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
       glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
    }
    glEnd();
  }
  //  Undo transformations
  glPopMatrix();
}
// Window piece of TIE Fighter
void ellipse() {
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
  for (ph=-90;ph<90;ph+=d) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=d) {
     glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
     glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
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
  glColor3ub(150,50,0);
  GLfloat radius = 0.1;
  GLfloat height = 5;
  GLfloat x              = 0.0;
  GLfloat y              = 0.0;
  GLfloat angle          = 0.0;
  GLfloat angle_stepsize = 0.1;

  glColor3ub(100,0,0);
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
  glBegin(GL_POLYGON);
  angle = 0.0;
      while( angle < 2*PI ) {
          x = radius * cos(angle);
          y = radius * sin(angle);
          glVertex3f(x, y , 0);
          angle = angle + angle_stepsize;
      }
      glVertex3f(radius, 0.0, height);
  glEnd();
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
  glBegin(GL_POLYGON);
  angle = 0.0;
      while( angle < 2*PI ) {
          x = radius * cos(angle);
          y = radius * sin(angle);
          glVertex3f(x, y , 0);
          angle = angle + angle_stepsize;
      }
      glVertex3f(radius, 0.0, height);
  glEnd();
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
// Nose(s) of X-Wing and Falcon (hence color parameters)
void nose(r,g,b,r2,g2,b2) {
  int org = 0;
  float thick = 2;
  int length = 7;
  int h1 = 2;
  float h2 = 0.5;
  glColor3ub(r,g,b); //100
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
  glColor3ub(r2,g2,b2); //150
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
  glBegin(GL_POLYGON);
  float sides = 50;
  for(int i = 0; i < sides; ++i) {
    glColor3ub(0,50,50);
    glVertex3f(4*sin(i/sides*2*M_PI), 4*cos(i/sides*2*M_PI),0);
    glVertex3f(4*sin(i/sides*2*M_PI), 4*cos(i/sides*2*M_PI),1);
    glVertex3f(0,0,0.5);
    glVertex3f(0,0,-0.5);
    glColor3ub(0,150,150);
    glVertex3f(4*sin(i/sides*2*M_PI), 4*cos(i/sides*2*M_PI),0);
  }
  glEnd();
}
// Tunnel to cockpit of Falcon
void tunnel() {
  glColor3ub(0,125,125);
  GLfloat radius = 0.9;
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
// Cockpit of Falcon
void cockpit() {
  const int d=5;
  int th,ph;
  double r = 0.9;
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glScaled(r,r,r);

  //  Latitude bands
  for (ph=-90;ph<90;ph+=d) {
    glBegin(GL_QUAD_STRIP);
    glColor3ub(0,75,75);
    for (th=0;th<=360;th+=d) {
       glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
       glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
    }
    glEnd();
  }
  //  Undo transformations
  glPopMatrix();
}
// Death Star
void deathStar() {
  const int d=5;
  int th,ph;
  double r = 4;
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glScaled(r,r,r);

  //  Latitude bands
  for (ph=-90;ph<90;ph+=d) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=d) {
      glColor3ub(75,75,75);
      glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
      glColor3ub(25,25,25);
      glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
    }
    glEnd();
  }
  //  Undo transformations
  glPopMatrix();
}
//------------------------------------------------------------------
//--------------------------Object Assembly-------------------------
//------------------------------------------------------------------
// Assembles the TIE Fighter object 
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
// Assembles the X-Wing object
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
  nose(0,0,100,0,0,150);

  glColor3ub(255,0,0);
  glRotated(90,1,0,0);
  glRotated(90,0,1,0);
  glTranslated(-0.5,8.5,2);
  lasers();
  glTranslated(3,-14,4);
  lasers();
  glPopMatrix();
}
// Assembles Millenium Falcon object
void drawMilleniumFalcon() {
  glPushMatrix();
  falconBody();
  glRotated(180,0,0,0);
  glTranslated(0,0,-1);
  falconBody();

  glScaled(1,1.7,0.5);
  glTranslated(0,0.4,0);
  nose(0,100,100,0,50,50);
  glRotated(180,0,0,0);
  glTranslated(0,0.75,-2);
  nose(0,100,100,0,50,50);

  glScaled(1/1,1/1.7,1/0.5);
  glTranslated(2,4,0.5);
  glRotated(90,0,1,0);
  glRotated(110,1,0,0);
  tunnel();
  glRotated(70,1,0,0);
  glTranslated(0,0.5,-1.8);
  glScaled(1,1,0.5);
  tunnel();
  glTranslated(0,0,0);
  glScaled(1,1,2);
  glColor3ub(0,0,150);
  cockpit();

  glPopMatrix();
}
//------------------------------------------------------------------
//---------------------------Display Scene--------------------------
//------------------------------------------------------------------
// Display all the object in the viewer
void display() {
  const double len = 4;
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();

  if (fp) {
    Cx = +2*dim*Sin(rot); //Ajust the camera vector based on rot
    Cz = -2*dim*Cos(rot);
    gluLookAt(Ex,Ey,Ez, Cx+Ex,Ey,Cz+Ez, 0,1,0); // y is the up-axis
  }
  //  Perspective - set eye position
  else {
    //  Perspective - set eye position
    if (mode) {
      double px = -2*dim*Sin(th)*Cos(ph);
      double py = +2*dim        *Sin(ph);
      double pz = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(px,py,pz, 0,0,0, 0,Cos(ph),0);
    }
    //  Orthogonal - set world orientation
    else {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
    }
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
    glTranslated(0,12,0);
    glRotated(30,0,0,0);
    glScaled(1.5,1.5,1.5);
    drawMilleniumFalcon();
    glPopMatrix();
  }
  if (death == 1) {
    glPushMatrix();
    glTranslated(20,20,20);
    glScaled(3,3,3);
    deathStar();
    glRotated(90,1,0,0);
    deathStar();
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
  // Print the text string
  if (fp) {
    // Five pixels from the lower left corner of the window
    glWindowPos2i(5,5);
    Print("FP = On    View Angle = %d",rot);
    glWindowPos2i(5,150);
    Print("W = Forwards");
    glWindowPos2i(5,125);
    Print("S = Backwards");
    glWindowPos2i(5,100);
    Print("A = Look left");
    glWindowPos2i(5,75);
    Print("D = Look right");
  }
  else {
    // Five pixels from the lower left corner of the window
    glWindowPos2i(5,5);
    Print("Angle = %d,%d   Dim = %.1f   FOV = %d   Projection = %s   First Person = Off",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
    glWindowPos2i(5,175);
    Print("0 = Reset viewing angle");
    glWindowPos2i(5,150);
    Print("Arrow Keys = Rotate view");
    glWindowPos2i(5,125);
    Print("Page Up/Down = +/- dim");
    glWindowPos2i(5,100);
    Print("1/2 = +/- FOV");
    glWindowPos2i(5,75);
    Print("M = Change mode");
    glWindowPos2i(5,50);
    Print("9 = Toggle axes");
  }
  //  Render the scene
  glFlush();
  //  Make the rendered scene visible
  glutSwapBuffers();
}
//------------------------------------------------------------------
//-------------------------Special Function-------------------------
//------------------------------------------------------------------
// Called when an special key is pressed
void special(int key,int x,int y) {
   if(!fp) {
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
      else if (key == GLUT_KEY_PAGE_UP && dim>1)
         dim -= 0.1;
      //  PageDown key - decrease dim
      else if (key == GLUT_KEY_PAGE_DOWN)
         dim += 0.1;
      //  Keep angles to +/-360 degrees
      th %= 360;
      ph %= 360;
   }
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
//------------------------------------------------------------------
//---------------------------Key Function---------------------------
//------------------------------------------------------------------
// Called when a key is pressed
void key(unsigned char ch,int x,int y) {
  //  Exit on ESC
  if (ch == 27)
    exit(0);
  //  Toggle axes
  else if (ch == '9')
    axes = 1-axes;
  //  Toggle first person
  else if (ch == 'f' || ch == 'F') {
    fp = 1-fp;
  }
  //  Pause Light
  else if (ch == ' ') {
    move = 1-move;
  }
  //  Move light
  else if (ch == '<')
    zh += 3;
  else if (ch == '>')
    zh -= 3;
  // First Person Motion
  if (fp) {
    double dt = 0.05;
    if (ch == 'w' || ch == 'W') {
      Ex += Cx*dt; //Update the eye vector based on camera vector
      Ez += Cz*dt;
    }
    else if (ch == 'a' || ch == 'A'){
      rot -= 3;
    }
    else if (ch == 's' || ch == 'S'){
      Ex -= Cx*dt;
      Ez -= Cz*dt;
    }
    else if (ch == 'd' || ch == 'D'){
      rot += 3;
    }
    //  Keep angles to +/-360 degrees
    rot %= 360;
  }
  else {
    //  Reset view angle
    if (ch == '0')
       th = ph = 0;
    //  Switch display mode
    else if (ch == 'm' || ch == 'M')
       mode = 1-mode;
    //  Change field of view angle
    else if (ch == '1')
       fov--;
    else if (ch == '2')
       fov++;
  }
  //  Reproject
  Project();
  //  Animate if requested
  glutIdleFunc(move?idle:NULL);
  //  Tell GLUT it is necessary to redisplay the scene
  glutPostRedisplay();
}
//------------------------------------------------------------------
//-------------------------Reshape Function-------------------------
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
//---------------------------Main Function--------------------------
//------------------------------------------------------------------
// Does the thing
int main(int argc,char* argv[]) {
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering
   glutInitWindowSize(1200,900);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Liam Kolber: Assignment 4");
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
//--------------------------------End-------------------------------
//------------------------------------------------------------------