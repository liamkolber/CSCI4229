// Liam Kolber and Eric Speaker
// CSCI 4229
// CU Boulder Fall 2017
// Final Project
//------------------------------------------------------------------
//---------------------------Initializations------------------------
//------------------------------------------------------------------
#include "CSCIx229.h"
//------------------------------------------------------------------
//--------------------------Global Variables------------------------
//------------------------------------------------------------------
  // General views
  int th        =   0;  // Azimuth of view angle
  int ph        =  20;  // Elevation of view angle
  int axes      =   0;  // Display axes
  int mode      =   2;  // What to display
  double asp    =   1;  // Aspect ratio
  double dim    = 1e4;  // Size of world
  int fov       =  55;  // Field of view (for perspective)
  double Ox     =   0;  // Look at location
  double Oy     =   0;  // Look at location
  double Oz     =   0;  // Look at location
  int X,Y;              // Last mouse location
  float z[65][65];      // DEM data
  float zmin    =+1e8;  // DEM lowest location
  float zmax    =-1e8;  // DEM highest location
  float zmag    =   1;  // DEM magnification
  int shiftx    =-7e3;  // Shift used to adjust eye and ball position in x
  int shifty    =1000;  // Shift used to adjust eye and ball position in y
  int shiftz    =4000;  // Shift used to adjust eye and ball position in z

  // First-person specific
  int fp        =   0;  // Toggle
  int rot       =90.0;  // Rotational angle
  double Ex     =  -4;  // Eye coords
  double Ey     =   0;
  double Ez     =   0;
  double Cx     =   0;  // Centering coords
  double Cz     =   0; 
  int move      =   0;  // Movement

  // Light values
  int one       =   1;  // Unit value
  int distance  =3000;  // Light distance
  int inc       =  10;  // Ball increment
  int smooth    =   1;  // Smooth/Flat shading
  int local     =   0;  // Local Viewer Model
  int emission  =   0;  // Emission intensity (%)
  int ambient   =  30;  // Ambient intensity (%)
  int diffuse   = 100;  // Diffuse intensity (%)
  int specular  =   0;  // Specular intensity (%)
  int shininess =   0;  // Shininess (power of two)
  float shinyvec[1];    // Shininess (value)
  int zh        =  90;  // Light azimuth
  int qh        =   0;
  float ylight  =   0;  // Elevation of light

  unsigned int texEngine;
  unsigned int texSmoke;
  unsigned int texBody;
  unsigned int texWheel;
  unsigned int texPlain;
  unsigned int texFront;
  unsigned int texTracks;
  unsigned int texGround;
  unsigned int texPitch;

  // ALL THE PIE!
  float PI = 3.1415927;

//------------------------------------------------------------------
//---------------------------General Stuff--------------------------
//------------------------------------------------------------------
  // Draw vertex in polar
  static void Vertex(double th,double ph) {
    double x = Sin(th)*Cos(ph);
    double y = Cos(th)*Cos(ph);
    double z =         Sin(ph);
    //  For a sphere at the origin, the position
    //  and normal vectors are the same
    glNormal3d(x,y,z);
    glVertex3d(x,y,z);
  }
  //Draw ball that'l be used for light
  static void ball(double x,double y,double z,double r) {
    int th,ph;
    float yellow[] = {1.0,1.0,0.0,1.0};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    //glTranslated(-7000,1000,4000);
    glTranslated(x,y,z);
    glScaled(r,r,r);
    //  White ball
    glColor3f(1,1,1);
    glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
    glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
    //  Bands of latitude
    for (ph=-90;ph<90;ph+=inc) {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc) {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
    }
    //  Undo transofrmations
    glPopMatrix();
  }
  
static void flyingobject(double x,double y,double z,double r,double chx, double chy, double chz)
{
   //int th,ph;
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   //float yellow[] = {1.0,1.0,0.0,1.0};
   //float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x+chx,y+chy,z+chz);
   glScaled(r,r,r);
   //  White ball
   glColor3ub(255,182,193);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT,GL_EMISSION,black);
   //  Bands of latitude
    glBegin(GL_QUADS);
  // Front
    glColor3ub(255,182,193);
  glNormal3f( 0, 0, 1);
  glVertex3f(-1,-1, 1); // Bottom
  glVertex3f(+1,-1, 1); // Bottom
  glVertex3f(+1,+1, 1); // Top
  glVertex3f(-1,+1, 1); // Top
  // Back
  glNormal3f( 0, 0,-1);
  glVertex3f(+1,-1,-1); // Bottom
  glVertex3f(-1,-1,-1); // Bottom
  glVertex3f(-1,+1,-1); // Top
  glVertex3f(+1,+1,-1); // Top
  // Right
  glNormal3f(+1, 0, 0);
  glVertex3f(+1,-1,+1); // Bottom
  glVertex3f(+1,-1,-1); // Bottom
  glVertex3f(+1,+1,-1); // Top
  glVertex3f(+1,+1,+1); // Top
  // Left
  glNormal3f(-1, 0, 0);
  glVertex3f(-1,-1,-1); // Bottom
  glVertex3f(-1,-1,+1); // Bottom
  glVertex3f(-1,+1,+1); // Top
  glVertex3f(-1,+1,-1); // Top
  //bottom
  glNormal3f(0, -1, 0);
  glVertex3f(-1,-1,-1); // Bottom
  glVertex3f(+1,-1,-1); // Bottom
  glVertex3f(+1,-1,+1); // Bottom
  glVertex3f(-1,-1,+1); // Bottom
  //top
  glNormal3f(0,+1,0);
  glVertex3f(-1,1,-1);
  glVertex3f(1,1,-1);
  glVertex3f(1,1,1);
  glVertex3f(-1,1,1);
      //  Undo transofrmations
  glEnd();
  glPopMatrix();
}

  // Set projection
  static void oldProject() {
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
//---------------------------Liam's Objects-------------------------
//------------------------------------------------------------------
  // Draw cylinder
  void drawCylinder(double x,double y,double z, double dx,double dy,double dz, double th) {
    double radius = 0.2, height = 0.2;
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glColor3ub(0,150,150);
    glPushMatrix();
    //  Transform cylinder
    glTranslated(x,y,z);
    glRotated(th,0,0,1);
    glScaled(dx,dy,dz);

    // Set Textures
    //glEnable(GL_TEXTURE_2D);
    //glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    //glBindTexture(GL_TEXTURE_2D,texture);

    //ADD TEXTURE TO PARAMETERS

    int j;
    glBegin(GL_TRIANGLE_STRIP);
    for (j = 0; j <= 360; j++) {
      //const float tc = (j / (float) 360);
      double x = radius * Cos(j);
      double y = height;
      double z = radius * Sin(j);
      glNormal3d(Cos(j),0,Sin(j));
      //glTexCoord2f(-tc,1.0);
      glVertex3d(x, y, z);
      //glTexCoord2f(-tc,0.0);
      glVertex3d(x, -y, z);
    }
    glEnd();

    double i;
    //glBindTexture(GL_TEXTURE_2D,texture);
    glNormal3d(0,1,0);
    /* Top of Cylinder */
    glBegin(GL_TRIANGLE_FAN);
      //glTexCoord2f(0.5,0.5); 
      glVertex3d(0.0, height, 0.0);
      for(i = 0.0; i < 360; i+=.125) {
        //glTexCoord2f(-0.5 * cos(i)+0.5, 0.5 * sin(i)+0.5);
        glVertex3d(radius * cos(i), height, radius * sin(i));
      }
    glEnd();
    glNormal3d(0,-1,0);

    /* Bottom of Cylinder */
    glBegin(GL_TRIANGLE_FAN);
      //glTexCoord2f(0.5,0.5);
      glVertex3d(0.0, -height, 0.0);
      for(i = 0.0; i < 360; i+=.125) {
        //glTexCoord2f(-0.5 * cos(i)+0.5, 0.5 * sin(i)+0.5);
        glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
    glEnd();
    glPopMatrix(); 
  }
  // Wings of TIE Fighter
  void TIEwing() {
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);


    glColor3ub(0,0,150);
    int org = 0;
    double l = 5;
    double w = 7;
    double h = 0.1;
    glBegin(GL_QUADS);  
    // Back
    glColor3ub(0,0,150);
    glNormal3d(-1,0,0);
    glVertex3f(org,org,org);
    glVertex3f(org,w,org);
    glVertex3f(org,w,h);
    glVertex3f(org,org,h);
    // Front
    glColor3ub(0,0,150);
    glNormal3d(1,0,0);
    glVertex3f(l,org,org);
    glVertex3f(l,w,org);
    glVertex3f(l,w,h);
    glVertex3f(l,org,h);
    glColor3ub(0,0,100);
    // Side2
    glColor3ub(0,0,150);
    glNormal3d(0,-1,0);
    glVertex3f(org,org,org);
    glVertex3f(l,org,org);
    glVertex3f(l,org,h);
    glVertex3f(org,org,h);
    // Side 2
    glColor3ub(0,0,150);
    glNormal3d(0,1,0);
    glVertex3f(org,w,org);
    glVertex3f(l,w,org);
    glVertex3f(l,w,h);
    glVertex3f(org,w,h);
    // Top
    glColor3ub(0,0,150);
    glNormal3d(0,0,-1);
    glColor3ub(0,0,150);
    glVertex3f(org,org,org);
    glVertex3f(org,w,org);
    glVertex3f(l,w,org);
    glVertex3f(l,org,org);
    //Bottom
    glColor3ub(0,0,150);
    glNormal3d(0,0,1);
    glVertex3f(org,org,h);
    glVertex3f(org,w,h);
    glVertex3f(l,w,h);
    glVertex3f(l,org,h);

    glEnd();
  }
  // Body of TIE Fighter
  void sphere() {

    //  Set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

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
        glNormal3d(Sin(th),0,Cos(th));
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

    //  Set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

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
        glNormal3d(Sin(th),0,Cos(th));
        glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
        glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
      }
      glEnd();
    }
    //  Undo transformations
    glPopMatrix();
  }
  // Assemble the TIE Fighter
  void drawTieFighter() {
    glPushMatrix();

    glTranslated(-2.5,3.5,0.4);
    TIEwing();
    glTranslated(2.5,3.5,-0.4);

    glRotated(90,0,1,0);
    glTranslated(-1.5,0,0);
    drawCylinder(0,0,0, 2,5,2, 90);
    glTranslated(1.5,0,0);
    glRotated(-90,0,1,0);

    glTranslated(0,0,3);
    sphere();
    glTranslated(0.17,0,0);

    ellipse();
    glTranslated(-0.17,0,0);
    glTranslated(0,0,3);
    glRotated(180,0,0,0);
    glTranslated(-2.5,-3.5,0.4);
    TIEwing();
    glTranslated(2.5,3.5,-0.4);

    glRotated(90,0,1,0);
    glTranslated(-1.5,0,0);
    drawCylinder(0,0,0, 2,5,2, 90);
    glTranslated(1.5,0,0);
    glRotated(-90,0,1,0);

    glColor3ub(0,255,0);
    glRotated(90,1,0,0);
    glRotated(90,0,1,0);
    glTranslated(0,2,1);

    glPopMatrix();
  }
  // Wings of X-Wing
  void Xwings() {
    int org = 0;
    float thick = 0.2;
    int length = 7;
    int h1 = 2;
    int h2 = 1;
    glColor3ub(0,100,50);
    glBegin(GL_QUADS);
    //side1
    glNormal3d(0,0,-1);
    glVertex3f(org,org,org);
    glVertex3f(length,org,org);
    glVertex3f(length,h2,org);
    glVertex3f(org,h1,org);
    //side2
    glNormal3d(0,0,1);
    glVertex3f(org,org,thick);
    glVertex3f(length,org,thick);
    glVertex3f(length,h2,thick);
    glVertex3f(org,h1,thick);
    //big
    glNormal3d(0,0,0);
    glVertex3f(org,org,org);
    glVertex3f(org,h1,org);
    glVertex3f(org,h1,thick);
    glVertex3f(org,org,thick);
    //small
    glNormal3d(0,-1,0);
    glColor3ub(100,0,0);
    glVertex3f(org,org,org);
    glVertex3f(org,org,thick);
    glVertex3f(length,org,thick);
    glVertex3f(length,org,org);
    //bottom
    glNormal3d(0,1,0);
    glVertex3f(length,org,org);
    glVertex3f(length,org,thick);
    glVertex3f(length,h2,thick);
    glVertex3f(length,h2,org);
    //tip
    glNormal3d(0,1,0);
    glVertex3f(org,org,org);
    glVertex3f(org,org,thick);
    glVertex3f(org,h1,thick);
    glVertex3f(org,h1,org);
    //top
    glNormal3d(0,1,0);
    glVertex3f(org,h1,org);
    glVertex3f(org,h1,thick);
    glVertex3f(length,h2,thick);
    glVertex3f(length,h2,org);

    glEnd();

    glTranslated(7,3,0.1);
    glRotated(90,0,0,0);

    //Rockets
    glPushMatrix();
    glTranslated(-5,0,0);
    glRotated(90,1,0,0);
    drawCylinder(0,3,0, 2.2,10,2.2, 0);
    //Guns
    glTranslated(5,0,0);
    drawCylinder(0,1,0, 0.5,10,0.5, 0);
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
    glNormal3d(-1,0,0);
    glVertex3f(org,org,org);
    glVertex3f(org,w,org);
    glVertex3f(org,w,h);
    glVertex3f(org,org,h);
    // Front
    glNormal3d(0,0,0);
    glVertex3f(l,org,org);
    glVertex3f(l,w,org);
    glVertex3f(l,w,h);
    glVertex3f(l,org,h);
    glColor3ub(0,0,100);
    // Side2
    glNormal3d(0,-1,0);
    glVertex3f(org,org,org);
    glVertex3f(l,org,org);
    glVertex3f(l,org,h);
    glVertex3f(org,org,h);
    // Side 2
    glNormal3d(0,1,0);
    glVertex3f(org,w,org);
    glVertex3f(l,w,org);
    glVertex3f(l,w,h);
    glVertex3f(org,w,h);
    // Bottom
    glNormal3d(0,0,-1);
    glColor3ub(0,0,150);
    glVertex3f(org,org,org);
    glVertex3f(org,w,org);
    glVertex3f(l,w,org);
    glVertex3f(l,org,org);
    //Top
    glNormal3d(0,0,1);
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
    glNormal3d(0,0,-1);
    glVertex3f(org,org,org);
    glVertex3f(length,org,org);
    glVertex3f(length,h2,org);
    glVertex3f(org,h1,org);
    //side2
    glNormal3d(0,0,1);
    glVertex3f(org,org,thick);
    glVertex3f(length,org,thick);
    glVertex3f(length,h2,thick);
    glVertex3f(org,h1,thick);
    //big
    glNormal3d(0,-1,0);
    glVertex3f(org,org,org);
    glVertex3f(org,h1,org);
    glVertex3f(org,h1,thick);
    glVertex3f(org,org,thick);
    //small
    glNormal3d(0,-1,0);
    glColor3ub(r2,g2,b2); //150
    glVertex3f(org,org,org);
    glVertex3f(org,org,thick);
    glVertex3f(length,org,thick);
    glVertex3f(length,org,org);
    //tip
    glNormal3d(1,0,0);
    glVertex3f(length,org,org);
    glVertex3f(length,org,thick);
    glVertex3f(length,h2,thick);
    glVertex3f(length,h2,org);
    //back
    glNormal3d(0,0,0);
    glColor3ub(255,255,255);
    glVertex3f(org,org,org);
    glVertex3f(org,org,thick);
    glVertex3f(org,h1,thick);
    glVertex3f(org,h1,org);
    glColor3ub(r2,g2,b2);
    //top
    glNormal3d(0,1,0);
    glVertex3f(org,h1,org);
    glVertex3f(org,h1,thick);
    glVertex3f(length,h2,thick);
    glVertex3f(length,h2,org);

    glEnd();
  }
  // Assemble the X-Wing object
  void drawXWing() {
    glPushMatrix();
    Xwings();
    glRotated(180,0,1,-1);
    glTranslated(8,-3,-0.2);
    Xwings();
    glPopMatrix();

    glPushMatrix();
    glRotated(30,0,1,0);
    Xwings();
    glRotated(180,0,1,-1);
    glTranslated(8,-3,0.2);
    Xwings();
    glPopMatrix();

    glPushMatrix();
    glRotated(90,0,0,1);
    glRotated(12,1,0,0);
    glTranslated(-0.5,-0.5,-1);
    body();
    glRotated(90,1,0,0);
    glTranslated(4,0,-2);
    nose(0,0,100,0,0,150);

    glPopMatrix();
  }
  // Function for ground borrowed from ex17.c
  void DEM() {
     int i,j;
     double z0 = (zmin+zmax)/2;
     //  Show DEM wire frame
     if (mode==1) {
        glColor3f(1,1,0);
        for (i=0;i<64;i++)
           for (j=0;j<64;j++)
           {
              float x=16*i-512;
              float y=16*j-512;
              glBegin(GL_LINE_LOOP);
              glVertex3d(x+ 0,y+ 0,zmag*(z[i+0][j+0]-z0));
              glVertex3d(x+16,y+ 0,zmag*(z[i+1][j+0]-z0));
              glVertex3d(x+16,y+16,zmag*(z[i+1][j+1]-z0));
              glVertex3d(x+ 0,y+16,zmag*(z[i+0][j+1]-z0));
              glEnd();
           }
     }
     //  Apply texture to DEM wireframe
     else {
        glColor3f(1,1,1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texGround);
        //glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        for (i=0;i<64;i++)
           for (j=0;j<64;j++)
           {
              float x=16*i-512;
              float y=16*j-512;
              glBegin(GL_QUADS);
              glTexCoord2f((i+0)/64.,(j+0)/64.); glVertex3d(x+ 0,y+ 0,zmag*(z[i+0][j+0]-z0));
              glTexCoord2f((i+1)/64.,(j+0)/64.); glVertex3d(x+16,y+ 0,zmag*(z[i+1][j+0]-z0));
              glTexCoord2f((i+1)/64.,(j+1)/64.); glVertex3d(x+16,y+16,zmag*(z[i+1][j+1]-z0));
              glTexCoord2f((i+0)/64.,(j+1)/64.); glVertex3d(x+ 0,y+16,zmag*(z[i+0][j+1]-z0));
              glEnd();
           }
        //glDisable(GL_CULL_FACE);
        //glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);
     }
  }
  // Draw train cylinder
  void trainCylinder(double x,double y,double z, double dx,double dy,double dz, double th, char lcol, char scol, unsigned int texture) {
     double radius = 0.3, height = 0.2;

     //  Set specular color to white
     float white[] = {1,1,1,1};
     float black[] = {0,0,0,1};
     glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
     glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
     glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

     glPushMatrix();
     //  Transform cylinder
     glTranslated(x,y,z);
     glRotated(th,0,0,1);
     glScaled(dx,dy,dz);

     // Set Textures
     glEnable(GL_TEXTURE_2D);
     glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

     //Set colors
     if (scol == 'r') {
        glColor3f(0.3, 0.3, 0.3); //Set color to red
     } else if (scol == 'g') {
        glColor3f(0.1, 0.1, 0.1); //Set color to green
     } else if (scol == 'b') {
        glColor3f(0.0, 0.0, 0.0); //Set color to blue
     } else if (scol == 'w') {
        glColor3f(1.0, 1.0, 1.0); //Set color to white
     } else {
        glColor3f(0.5, 0.5, 0.5); //Default color to gray
     }

     int flag = 0;
     if (texture == texWheel) {
        texture = texPlain;
        flag = 1;
     }
     else if (texture == texEngine) {
        flag = 2;
     }   
     else if (texture == texTracks) {
        texture = texPlain;
        flag = 3;
     }

     glBindTexture(GL_TEXTURE_2D,texture);

     int j;
     glBegin(GL_TRIANGLE_STRIP);
     for (j = 0; j <= 360; j++) {
        const float tc = (j / (float) 360);
        double x = radius * Cos(j);
        double y = height;
        double z = radius * Sin(j);
        glNormal3d(Cos(j),0,Sin(j));
        glTexCoord2f(-tc,1.0);
        glVertex3d(x, y, z);
        glTexCoord2f(-tc,0.0);
        glVertex3d(x, -y, z);
     }
     glEnd();
     //Set colors
     if (lcol == 'r'){
        glColor3f(0.3, 0.3, 0.3); //Set color to red
     } else if (lcol == 'g') {
        glColor3f(0.1, 0.1, 0.1); //Set color to green
     } else if (lcol == 'b') {
        glColor3f(0.0, 0.0, 0.0); //Set color to blue
     } else if (lcol == 'w') {
        glColor3f(1.0, 1.0, 1.0); //Set color to white
     } else {
        glColor3f(0.8, 0.8, 0.8); //Default color to gray
     }


     double i; 
     if (flag == 1) {
        texture = texWheel;
     }
     else if (flag == 2) {
        texture = texFront;
     }
     else if (flag == 3) {
        texture = texTracks;
     }
     else {
        texture = texPlain;
     }
     glBindTexture(GL_TEXTURE_2D,texture);
     glNormal3d(0,1,0);
     // Top of Cylinder 
     glBegin(GL_TRIANGLE_FAN);
     glTexCoord2f(0.5,0.5); 
     glVertex3d(0.0, height, 0.0);
     for(i = 0.0; i < 360; i+=.125) {
        glTexCoord2f(-0.5 * cos(i)+0.5, 0.5 * sin(i)+0.5);
        glVertex3d(radius * cos(i), height, radius * sin(i));
     }
     glEnd();


     glNormal3d(0,-1,0);

     // Bottom of Cylinder 
     glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.5,0.5); 
        glVertex3d(0.0, -height, 0.0);
        for(i = 0.0; i < 360; i+=.125) {
           glTexCoord2f(-0.5 * cos(i)+0.5, 0.5 * sin(i)+0.5);
           glVertex3d(radius * cos(i), -height, radius * sin(i));
        }
     glEnd();
     glPopMatrix(); 
     glDisable(GL_TEXTURE_2D);
  }

//------------------------------------------------------------------
//---------------------------Eric's Objects-------------------------
//------------------------------------------------------------------
  // Dude's got some SERIOUS efficiency issues  ;)
  //-------------------------------Pitch------------------------------
  static void drawField(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize, ySize-12, zSize);
    //glRotated(th, 0, 1, 0);
    glScaled(dx*40, 10*dy, dz*24);

    // Set Textures
    //glEnable(GL_TEXTURE_2D);
   // glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    //glBindTexture(GL_TEXTURE_2D,texPitch);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(32,162,3);
    glNormal3f( 0, 0, 1);
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(-1,-1, 1); // Bottom
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(+1,-1, 1); // Bottom
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(+1,+1, 1); // Top
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(-1,+1, 1); // Top
    // Back
    glNormal3f( 0, 0,-1);
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(+1,-1,-1); // Bottom
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(-1,-1,-1); // Bottom
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(-1,+1,-1); // Top
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(+1,+1,-1); // Top
    // Right
    glNormal3f(+1, 0, 0);
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(+1,-1,+1); // Bottom
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(+1,-1,-1); // Bottom
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(+1,+1,-1); // Top
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(+1,+1,+1); // Top
    // Left
    glNormal3f(-1, 0, 0);
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(-1,-1,-1); // Bottom
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(-1,-1,+1); // Bottom
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(-1,+1,+1); // Top
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(-1,-1,-1); // Bottom
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(+1,-1,-1); // Bottom
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(+1,-1,+1); // Bottom
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(-1,1,-1);
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(1,1,-1);
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(1,1,1);
    //glTexCoord2f(0.5,0.5); 
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  void drawFieldLine(double xSize, double ySize, double zSize){
    glPushMatrix();

    glRotatef(90, 1, 0, 0); 
    glTranslated(xSize-2, ySize-1, zSize+10);  //z is up down....+ is down
    glScaled(xSize*9, ySize*6, zSize);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP); //GL_LINE_LOOP draws a connected group of line segments from one vector to another
    glColor3ub(255,255,255);  
    for(int i=0; i<=300; i++){
      double angle = 2*3.1415*i/300;
      double x=cos(angle);
      double y=sin(angle);  
      glVertex2d(x,y);
    }
    glEnd();  
    glPopMatrix();
  }
  void goalzone1(double xSize, double ySize, double zSize){
    //this is a replication of drawFieldLine but with some of the following changed: angle,color,position,scale
    //with the rotation for this object and viewing from botton:
      //x -- left/right
      //y -- up/down
      //z -- in/out of frame (+ into frame / - out of frame)
      
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslated(xSize+14, ySize-1, zSize+10); 
    glScaled(xSize*1.25, ySize*3.5, zSize);
    glBegin(GL_LINE_LOOP);
    glColor3ub(255,255,255);  
    for(int i=0; i<=300; i++){
      double angle = 2*3.1415*i/300;
      double x=cos(angle);
      double y=sin(angle);  
      glVertex2d(x,y);
    }
    glEnd();  

    glPopMatrix();
  }
  void goalzone2(double xSize, double ySize, double zSize){
    //this is a replication of drawFieldLine but with some of the following changed: angle,color,position,scale
    //with the rotation for this object and viewing from botton:
      //x -- left/right
      //y -- up/down
      //z -- in/out of frame (+ into frame / - out of frame)
      
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslated(xSize-16, ySize-1, zSize+10); 
    glScaled(xSize*1.25, ySize*3.5, zSize);
    glBegin(GL_LINE_LOOP);
    glColor3ub(255,255,255);  
    for(int i=0; i<=300; i++){
      double angle = 2*3.1415*i/300;
      double x=cos(angle);
      double y=sin(angle);  
      glVertex2d(x,y);
    }
    glEnd();  

    glPopMatrix();
  }
  void halffield(double xSize, double ySize, double zSize){
    //this is a replication of drawFieldLine but with some of the following changed: angle,color,position,scale
    glPushMatrix();
    glTranslated(xSize-1, ySize-12, zSize-7); 

    glScaled(xSize, ySize, zSize);
    glBegin(GL_LINES);
    glColor3ub(255,255,255);
    glVertex3f(0,0,0);
    glVertex3f(0,0,12);
    glEnd();  

    glPopMatrix();
  }
  void centercircle(double xSize, double ySize, double zSize){
    //this is a replication of drawFieldLine but with some of the following changed: angle,color,position,scale
    //with the rotation for this object and viewing from botton:
      //x -- left/right
      //y -- up/down
      //z -- in/out of frame (+ into frame / - out of frame)
      
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslated(xSize-2, ySize-2, zSize+9); 
    glScaled(xSize*1.5, ySize*1.5, zSize);
    glBegin(GL_LINE_LOOP);
    glColor3ub(255,255,255);  
    for(int i=0; i<=300; i++){
      double angle = 2*3.1415*i/300;
      double x=cos(angle);
      double y=sin(angle);  
      glVertex2d(x,y);
    }
    glEnd();  

    glPopMatrix();
  }
  //-----------------------------Goal Posts---------------------------
  static void drawGPost1(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize+15, ySize-9, zSize);
    //glRotated(th, 0, 1, 0);
    glScaled(dx/2, dy*3, dz/2);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(189,164,4);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void drawGPost2(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize+15, ySize-9, zSize+2);
    //glRotated(th, 0, 1, 0);
    glScaled(dx/2, dy*3, dz/2);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(189,164,4);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void drawGPost3(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize+15, ySize-9, zSize-2);
    //glRotated(th, 0, 1, 0);
    glScaled(dx/2, dy*3, dz/2);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(189,164,4);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void drawGPost4(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize-15, ySize-9, zSize);
    //glRotated(th, 0, 1, 0);
    glScaled(dx/2, dy*3, dz/2);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(189,164,4);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void drawGPost5(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize-15, ySize-9, zSize+2);
    //glRotated(th, 0, 1, 0);
    glScaled(dx/2, dy*3, dz/2);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(189,164,4);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void drawGPost6(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize-15, ySize-9, zSize-2);
    //glRotated(th, 0, 1, 0);
    glScaled(dx/2, dy*3, dz/2);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(189,164,4);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  //-------------------------------Stands-----------------------------
  static void stand1(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize, ySize-8, zSize+9);
    //glRotated(th, 0, 1, 0);
    glScaled(dx, dy*4, dz);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(255,0,0);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void stand2(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize-7.5, ySize-8, zSize+8);
    //glRotated(th, 0, 1, 0);
    glScaled(dx, dy*4, dz);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(255,255,0);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void stand3(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize-15, ySize-8, zSize+6);
    //glRotated(th, 0, 1, 0);
    glScaled(dx, dy*4, dz);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(0,0,255);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void stand4(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize-20, ySize-8, zSize);
    //glRotated(th, 0, 1, 0);
    glScaled(dx, dy*4, dz);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(255,255,0);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void stand5(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize-15, ySize-8, zSize-6);
    //glRotated(th, 0, 1, 0);
    glScaled(dx, dy*4, dz);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(255,0,0);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void stand6(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize-7.5, ySize-8, zSize-8);
    //glRotated(th, 0, 1, 0);
    glScaled(dx, dy*4, dz);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(0,255,0);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void stand7(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize, ySize-8, zSize-9);
    //glRotated(th, 0, 1, 0);
    glScaled(dx, dy*4, dz);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(0,0,255);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void stand8(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize+7.5, ySize-8, zSize-8);
    //glRotated(th, 0, 1, 0);
    glScaled(dx, dy*4, dz);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(255,255,0);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void stand9(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize+15, ySize-8, zSize-6);
    //glRotated(th, 0, 1, 0);
    glScaled(dx, dy*4, dz);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(255,0,0);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void stand10(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize+20, ySize-8, zSize);
    //glRotated(th, 0, 1, 0);
    glScaled(dx, dy*4, dz);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(0,255,0);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void stand11(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize+15, ySize-8, zSize+6);
    //glRotated(th, 0, 1, 0);
    glScaled(dx, dy*4, dz);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(0,0,255);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  static void stand12(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize+7.5, ySize-8, zSize+8);
    //glRotated(th, 0, 1, 0);
    glScaled(dx, dy*4, dz);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(0,255,0);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  //--------------------------Object Locations------------------------
  static void hut(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th){
    //set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    // Translation
    glTranslated(xSize+33, ySize-9, zSize+18); //y is height, z is near or far
    //glRotated(th, 0, 1, 0);
    glScaled(dx*4, dy*2.5, dz*4);
    //start drawling of the the field
      glBegin(GL_QUADS);
    // Front
      glColor3ub(255,255,255);
      glNormal3f( 0, 0, 1);
    glVertex3f(-1,-1, 1); // Bottom
    glVertex3f(+1,-1, 1); // Bottom
    glVertex3f(+1,+1, 1); // Top
    glVertex3f(-1,+1, 1); // Top
    // Back
      glNormal3f( 0, 0,-1);
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,+1,-1); // Top
    glVertex3f(+1,+1,-1); // Top
    // Right
      glNormal3f(+1, 0, 0);
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,+1,-1); // Top
    glVertex3f(+1,+1,+1); // Top
    // Left
      glNormal3f(-1, 0, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    glVertex3f(-1,+1,+1); // Top
    glVertex3f(-1,+1,-1); // Top
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-1,-1,-1); // Bottom
    glVertex3f(+1,-1,-1); // Bottom
    glVertex3f(+1,-1,+1); // Bottom
    glVertex3f(-1,-1,+1); // Bottom
    //top
    glNormal3f(0,+1,0);
    glVertex3f(-1,1,-1);
    glVertex3f(1,1,-1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);

      glEnd(); // End drawing main body
    glPopMatrix(); //undo transformations
  }
  void object1 (double xSize, double ySize, double zSize){
    //replication of drawpost1 function but with a change in some of the following: scale,position,color and angle
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glPushMatrix();
    glTranslated(xSize+3, ySize+5, zSize+1); 

    glScaled(xSize, ySize, zSize);
    glBegin(GL_QUADS);
    //front
    glNormal3f( 0, 0, 1);
    glColor3ub(144,144,144); //this is a different way to 'color' an object and uses the html color valules to do so
    glVertex3f(-1,-1,1);
    glVertex3f(1,-1,1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);
    //back
    //glColor3ub(232,45,7);
    glNormal3f( 0, 0, -1);
    glVertex3f(+1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    // Right
    glNormal3f( 1, 0, 0);
    glVertex3f(+1,-1,1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+1,1);
    // Left
    glNormal3f( -1, 0, 0);  
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1,+1);
    glVertex3f(-1,+1,+1);
    glVertex3f(-1,+1,-1);
    //top
    glNormal3f( 0, 1, 0); 
    glVertex3f(-1,-1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(-1,+1,-1);
    //bottom -- not needed
    glEnd();
    glPopMatrix();
  }
  void object2 (double xSize, double ySize, double zSize){
    //replication of drawpost1 function but with a change in some of the following: scale,position,color and angle
    glPushMatrix();
    glTranslated(xSize-5, ySize+5, zSize+1); 

    glScaled(xSize, ySize, zSize);
    glBegin(GL_QUADS);
    //front
    glNormal3f( 0, 0, 1);
    glColor3ub(144,144,144); //this is a different way to 'color' an object and uses the html color valules to do so
    glVertex3f(-1,-1,1);
    glVertex3f(1,-1,1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);
    //back
    //glColor3ub(232,45,7);
    glNormal3f( 0, 0, -1);
    glVertex3f(+1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    // Right
    glNormal3f( 1, 0, 0);
    glVertex3f(+1,-1,1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+1,1);
    // Left
    glNormal3f( -1, 0, 0);  
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1,+1);
    glVertex3f(-1,+1,+1);
    glVertex3f(-1,+1,-1);
    //top
    glNormal3f( 0, 1, 0); 
    glVertex3f(-1,-1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(-1,+1,-1);
    //bottom -- not needed
    glEnd();
    glPopMatrix();
  }
  void object3 (double xSize, double ySize, double zSize){
    //replication of drawpost1 function but with a change in some of the following: scale,position,color and angle
    glPushMatrix();
    glTranslated(xSize+3, ySize+5, zSize-5); 

    glScaled(xSize, ySize, zSize);
    glBegin(GL_QUADS);
    //front
    glNormal3f( 0, 0, 1);
    glColor3ub(144,144,144); //this is a different way to 'color' an object and uses the html color valules to do so
    glVertex3f(-1,-1,1);
    glVertex3f(1,-1,1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);
    //back
    //glColor3ub(232,45,7);
    glNormal3f( 0, 0, -1);
    glVertex3f(+1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    // Right
    glNormal3f( 1, 0, 0);
    glVertex3f(+1,-1,1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+1,1);
    // Left
    glNormal3f( -1, 0, 0);  
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1,+1);
    glVertex3f(-1,+1,+1);
    glVertex3f(-1,+1,-1);
    //top
    glNormal3f( 0, 1, 0); 
    glVertex3f(-1,-1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(-1,+1,-1);
    //bottom -- not needed
    glEnd();
    glPopMatrix();
  }
  void object4 (double xSize, double ySize, double zSize){
    //replication of drawpost1 function but with a change in some of the following: scale,position,color and angle
    glPushMatrix();
    glTranslated(xSize-5, ySize+5, zSize-5); 

    glScaled(xSize, ySize, zSize);
    glBegin(GL_QUADS);
    //front
    glNormal3f( 0, 0, 1);
    glColor3ub(144,144,144); //this is a different way to 'color' an object and uses the html color valules to do so
    glVertex3f(-1,-1,1);
    glVertex3f(1,-1,1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);
    //back
    //glColor3ub(232,45,7);
    glNormal3f( 0, 0, -1);
    glVertex3f(+1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    // Right
    glNormal3f( 1, 0, 0);
    glVertex3f(+1,-1,1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+1,1);
    // Left
    glNormal3f( -1, 0, 0);  
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1,+1);
    glVertex3f(-1,+1,+1);
    glVertex3f(-1,+1,-1);
    //top
    glNormal3f( 0, 1, 0); 
    glVertex3f(-1,-1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(-1,+1,-1);
    //bottom -- not needed
    glEnd();
    glPopMatrix();
  }
  void object5 (double xSize, double ySize, double zSize) {
    //replication of drawpost1 function but with a change in some of the following: scale,position,color and angle
    glPushMatrix();
    glTranslated(xSize-1, ySize+5, zSize-2); 

    glScaled(xSize, ySize, zSize);
    glBegin(GL_QUADS);
    //front
    glNormal3f( 0, 0, 1);
    glColor3ub(144,144,144); //this is a different way to 'color' an object and uses the html color valules to do so
    glVertex3f(-1,-1,1);
    glVertex3f(1,-1,1);
    glVertex3f(1,1,1);
    glVertex3f(-1,1,1);
    //back
    //glColor3ub(232,45,7);
    glNormal3f( 0, 0, -1);
    glVertex3f(+1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    // Right
    glNormal3f( 1, 0, 0);
    glVertex3f(+1,-1,1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+1,1);
    // Left
    glNormal3f( -1, 0, 0);  
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1,+1);
    glVertex3f(-1,+1,+1);
    glVertex3f(-1,+1,-1);
    //top
    glNormal3f( 0, 1, 0); 
    glVertex3f(-1,-1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(-1,+1,-1);
    //bottom -- not needed
    glEnd();
    glPopMatrix();
  }

//------------------------------------------------------------------
//---------------------------Load Textures--------------------------
//------------------------------------------------------------------
  void loadTextures() {
    texEngine = LoadTexBMP("textures/engine.bmp");
    texSmoke = LoadTexBMP("textures/smokestack.bmp");
    texBody = LoadTexBMP("textures/body.bmp");
    texWheel = LoadTexBMP("textures/wheel.bmp");
    texPlain = LoadTexBMP("textures/plain.bmp");
    texFront = LoadTexBMP("textures/front.bmp");
    texTracks = LoadTexBMP("textures/tracks.bmp");
    texGround = LoadTexBMP("saddleback.bmp");
    //texPitch = LoadTexBMP("textures/pitch.bmp");
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
      double px = -2*dim*Sin(th)*Cos(ph)+shiftx;
      double py = +2*dim        *Sin(ph)+shifty;
      double pz = +2*dim*Cos(th)*Cos(ph)+shiftz;
      gluLookAt(px,py,pz, shiftx,shifty,shiftz, 0,Cos(ph),0);
      //gluLookAt(px,py,pz, 1,0,0, 0,Cos(ph),0);
    }

    glShadeModel(GL_SMOOTH);
    //  Translate intensity to color vectors
    float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
    float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
    float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
    //  Light position
    float Position[]  = {distance*Cos(zh)+shiftx,distance*Sin(qh)+shifty,distance*Sin(zh)+shiftz,1};
    //  Draw light position as ball (still no lighting here)
    glColor3f(1,1,1);
    ball(Position[0],Position[1],Position[2],100);
  //glTranslated(2000,5000,0);
flyingobject(Position[0],Position[1],Position[2],100,2000,-1000,0);
    //  OpenGL should normalize normal vectors
    glEnable(GL_NORMALIZE);
    //  Enable lighting
    glEnable(GL_LIGHTING);
    //  Location of viewer for specular calculations
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
    //  glColor sets ambient and diffuse color materials
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //  Enable light 0
    glEnable(GL_LIGHT0);
    //  Set ambient, diffuse, specular components and position of light 0
    glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
    glLightfv(GL_LIGHT0,GL_POSITION,Position);

    glRotated(-90,1,0,0);

    // The fighters
    glScaled(10,10,10);
    glTranslated(-800,-500,50);
    glRotated(90,0,1,0);
    drawTieFighter();
    glRotated(-90,0,1,0);

    glTranslated(0,-2,-2);
    glRotated(-90,0,1,0);
    drawXWing();
    glRotated(90,0,1,0);
    glTranslated(800,500,-50);

    // Hogwarts Express
    // Body
    glTranslated(0,0,500);
    glScaled(10,10,10);
    glRotated(90,1,0,0);
    glRotated(90,0,1,0);
    trainCylinder(0,0,0, 2,7,2, 90,'w','q',texEngine);
    trainCylinder(2,0.2,0, 3,4,3, 0,'g','w',texBody);
    trainCylinder(-1,0.7,0, 1,2,1, 0,'g','w',texSmoke);
    trainCylinder(2,1,0, 2,3,2, 0,'g','w',texSmoke);

    // Wheels and axels
    glRotated(90,0,1,0);
    trainCylinder(0,-0.6,-1, 0.2,5.5,0.2, 90,'r','r',texPlain);
    trainCylinder(0,-0.6,2, 0.2,5.5,0.2, 90,'r','r',texPlain);
    trainCylinder(0,-0.6,0.5, 0.2,5.5,0.2, 90,'r','r',texPlain);

    trainCylinder(1,-0.6,-1, 2,0.3,2, 90,'r','r',texWheel);
    trainCylinder(-1,-0.6,-1, 2,0.3,2, 90,'r','r',texWheel);
    trainCylinder(1,-0.6,2, 2,0.3,2, 90,'r','r',texWheel);
    trainCylinder(-1,-0.6,2, 2,0.3,2, 90,'r','r',texWheel);
    trainCylinder(1,-0.6,0.5, 2,0.3,2, 90,'r','r',texWheel);
    trainCylinder(-1,-0.6,0.5, 2,0.3,2, 90,'r','r',texWheel);
    glRotated(180,0,-1,0);
    glRotated(90,-1,0,0);
    glScaled(0.1,0.1,0.1);
    glScaled(0.5,0.5,0.5);

    // The ground
    glTranslated(0,0,-1000);
    glScaled(10,10,10);
    DEM();

    // Eric's field
    glScaled(2,2,2);
    glRotated(90,1,0,0);
    glTranslated(-75,0,40);
    glTranslated(0,-9,0);
    drawField(0,0,0,1,1,1,0);
    glTranslated(0,9,0);
    drawGPost1(0,0,0,1,1,1,0);
    drawGPost2(0,0,0,1,1,1,0);
    drawGPost3(0,0,0,1,1,1,0);
    drawGPost4(0,0,0,1,1,1,0);
    drawGPost5(0,0,0,1,1,1,0);
    drawGPost6(0,0,0,1,1,1,0);
    stand1(0,0,0,1,1,1,0);
    stand2(0,0,0,1,1,1,0);
    stand3(0,0,0,1,1,1,0);
    stand4(0,0,0,1,1,1,0);
    stand5(0,0,0,1,1,1,0);
    stand6(0,0,0,1,1,1,0);
    stand7(0,0,0,1,1,1,0);
    stand8(0,0,0,1,1,1,0);
    stand9(0,0,0,1,1,1,0);
    stand10(0,0,0,1,1,1,0);
    stand11(0,0,0,1,1,1,0);
    stand12(0,0,0,1,1,1,0);
    drawFieldLine(2,1,1);
    goalzone1(1,1,1);
    goalzone2(1,1,1);
    halffield(1,1,1);
    centercircle(2,2,2);
    hut(0,0,0,1,1,1,0);
    object1(1,1,1);
    object2(1,1,1);
    object3(1,1,1);
    object4(1,1,1);
    object5(1,1,1);


    //  Draw axes - no lighting from here on
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
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
      Print("Angle = %d,%d   Dim = %.1f   FOV = %d   Projection = %s   First Person = Off   Mode = %d",th,ph,dim,fov,mode?"Perpective":"Orthogonal",mode);
      glWindowPos2i(5,250);
      Print("0 = Reset viewing angle");
      glWindowPos2i(5,225);
      Print("SPACE = Toggle auto light movement");
      glWindowPos2i(5,200);
      Print("U/I = Vertical light movement");
      glWindowPos2i(5,175);
      Print("</> = Horizontal light movement");
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
     //  Render the scene and make it visible
     ErrCheck("display");
     glFlush();
     glutSwapBuffers();
  }

//------------------------------------------------------------------
//--------------------------Idle Function---------------------------
//------------------------------------------------------------------
  // Called when the window is idling
  void idle() {
    //  Elapsed time in seconds
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh = fmod(90*t,360.0);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
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
         dim -= 500;
      //  PageDown key - decrease dim
      else if (key == GLUT_KEY_PAGE_DOWN)
         dim += 500;
      //  Keep angles to +/-360 degrees
      th %= 360;
      ph %= 360;
    }
    //  Update projection
    oldProject();
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
    //  Pause Light
    else if (ch == ' ')
      move = 1-move;
    //  Move light
    else if (ch == '<')
      zh += 3;
    else if (ch == '>')
      zh -= 3;
    else if (ch == 'u' || ch == 'U')
      qh += 3;
    else if (ch == 'i' || ch == 'I')
      qh -= 3;
    //  Toggle first person
    else if (ch == 'f' || ch == 'F')
      fp = 1-fp;

    if (fp) {
      double dt = 0.05;
      if (ch == 'w' || ch == 'W'){
         Ex += Cx*dt; //Update the eye vector based on the camera vector
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
         mode = 2;
         //mode = (mode+1)%3;
      //  Change field of view angle
      else if (ch == '1')
         fov--;
      else if (ch == '2')
         fov++;
    }

    //  Reproject
    oldProject();
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
    oldProject();
  }

//------------------------------------------------------------------
//-------------------------Motion Function--------------------------
//------------------------------------------------------------------
  //GLUT calls this routine when a mouse is moved
  void motion(int x,int y) {
     //  Do only when move is set
     //  WARNING:  this only works because by coincidence 1m = 1pixel
     if (move) {
        // Left/right movement
        Ox += X-x;
        // Near/far or Up/down movement
        if (move<0)
           Oy -= Y-y;
        else
           Oz += Y-y;
        // Remember location
        X = x;
        Y = y;
        glutPostRedisplay();
     }
  }

//------------------------------------------------------------------
//--------------------------Mouse Function--------------------------
//------------------------------------------------------------------
  //GLUT calls this routine when a mouse button is pressed or released
  void mouse(int key,int status,int x,int y) {
    //  On button down, set 'move' and remember location
    if (status==GLUT_DOWN) {
      move = (key==GLUT_LEFT_BUTTON) ? 1 : -1;
      X = x;
      Y = y;
    }
    //  On button up, unset move
    else if (status==GLUT_UP)
      move = 0;
  }

//------------------------------------------------------------------
//-------------------------ReadDEM Function-------------------------
//------------------------------------------------------------------
  //Read DEM from file
  void ReadDEM(char* file) {
    int i,j;
    FILE* f = fopen(file,"r");
    if (!f) Fatal("Cannot open file %s\n",file);
    for (j=0;j<=64;j++)
      for (i=0;i<=64;i++) {
        if (fscanf(f,"%f",&z[i][j])!=1) Fatal("Error reading saddleback.dem\n");
        if (z[i][j] < zmin) zmin = z[i][j];
        if (z[i][j] > zmax) zmax = z[i][j];
      }
    fclose(f);
  }

//------------------------------------------------------------------
//---------------------------Main Function--------------------------
//------------------------------------------------------------------
  // Does the thing
  int main(int argc,char* argv[]) {
    //  Initialize GLUT and process user parameters
    glutInit(&argc,argv);
    glutInitWindowSize(1200,900);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Liam Kolber: Assignment 5");
    //  Tell GLUT to call "display" when the scene should be drawn
    glutDisplayFunc(display);
    //  Tell GLUT to call "reshape" when the window is resized
    glutReshapeFunc(reshape);
    //  Tell GLUT to call "special" when an arrow key is pressed
    glutSpecialFunc(special);
    //  Tell GLUT to call "key" when a key is pressed
    glutKeyboardFunc(key);
    //  Tell GLUT to call "mouse" when a mouse button is pressed
    glutMouseFunc(mouse);
    //  Tell GLUT to call "motion" when the mouse is moved
    glutMotionFunc(motion);
    //  Load DEM
    ReadDEM("saddleback.dem");
    //  Pass control to GLUT so it can interact with the user
    ErrCheck("init");
    // Tell GLUT what to do when idle
    glutIdleFunc(idle);
    //  Textures
    loadTextures();
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
    return 0;
  }

//------------------------------------------------------------------
//--------------------------------End-------------------------------
//------------------------------------------------------------------