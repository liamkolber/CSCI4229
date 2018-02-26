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
  int ph        =   0;  // Elevation of view angle
  int mode      =   2;  // What to display
  double asp    =   1;  // Aspect ratio
  int aspectW   =1200;
  int aspectH   = 700;
  double dim    = 1e5;  // Size of world
  int fov       =  55;  // Field of view (for perspective)
  double Ox     =   0;  // Look at location
  double Oy     =   0;  // Look at location
  double Oz     =   0;  // Look at location
  int X,Y;              // Last mouse location
  float z[65][65];      // DEM data
  float zmin    =+1e8;  // DEM lowest location
  float zmax    =-1e8;  // DEM highest location
  float zmag    =   1;  // DEM magnification
  int trainX    =-7e3;  // Shift used to adjust train position in x
  int trainY    =1000;  // Shift used to adjust train position in y
  int trainZ    =4000;  // Shift used to adjust train position in z
  int zhTrain   =   0;  // Train azimuth
  int zhTIE     =   0;
  int zhX_W     =   0;
  int zhFalc    =   0;
  int zhTIEf1   =   0;
  int zhXWf1    =   0;
  int zhStarDes = -80;
  int zhTARDIS  =   0;
  int zhSnitch  =   0;
  double elipA  = 2.8;
  double elipB  = 1.5;
  int distTrain = 9e2;
  int planet    =   0;
  int quaffle   =   0;
  int goalFlag  =   0;
  int stopQuaf  =   0;
  int LASERSHOT =   0;

  // First-person specific
  int fp        =   0;  // Toggle
  int rot       =  90;  // Rotational angle
  int rotup     = -10;
  double Ex     =-500;  // Eye coords
  double Ey     =-500;
  double Ez     =   0;
  double Cx     = 1e5;  // Centering coords
  double Cy     =   0;
  double Cz     =   0; 
  double quafX  =   0;
  double quafY  =   0;
  double quafZ  =   0;
  double saveEx =   0;  // Eye coords
  double saveEy =   0;
  double saveEz =   0;
  double saveCx =   0;
  double saveCy =   0;
  double saveCz =   0;
  int move      =   1;  // Movement

  // Light values
  int one       =   1;  // Unit value
  int distance  = 9e2;  // Light distance 9e4
  int inc       =  10;  // Ball increment
  int smooth    =   1;  // Smooth/Flat shading
  int local     =   0;  // Local Viewer Model
  int emission  =   0;  // Emission intensity (%)
  int ambient   =  50;  // Ambient intensity (%)
  int diffuse   = 100;  // Diffuse intensity (%)
  int specular  =   0;  // Specular intensity (%)
  int shininess =   0;  // Shininess (power of two)
  float shinyvec[1];    // Shininess (value)
  int zh        = 120;  // Light azimuth
  int qh        =  20;
  float ylight  =1000;  // Elevation of light

  unsigned int texEngine, texSmoke, texBody, texWheel, texPlain, texFront, texTracks;
  unsigned int texHUD,texGoal;
  unsigned int texPitch, texPitchSide;
  unsigned int texSky[2];
  unsigned int texNight[2];
  unsigned int texStand;
  unsigned int texTIEBody,texTIEWing,texTIEArm;
  unsigned int texXWING;
  unsigned int texTARDIS;
  unsigned int texMars, texNeptune, texSun;

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
    glTranslated(x,y,z);
    glScaled(r,r,r);
    //  White ball
    glColor3f(1.0f,1.0f,1.0f);
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
  // Set projection
  static void oldProject() {
    // Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    // Undo previous transformations
    glLoadIdentity();
    // Perspective transformation
    if(fp) {
      gluPerspective(fov,asp,dim/16000,4*dim);
    }
    else
      gluPerspective(fov,asp,dim/16000,4*dim);
    // Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    // Undo previous transformations
    glLoadIdentity();
  }
  void DrawHud(unsigned int texture) {
    glPushMatrix();
    glColor3ub(255,255,255);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture);
      glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);   glVertex3f(0.0f,1.0f,0.0f);
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(0.0f,0.0f,0.0f);
        glTexCoord2f(1.0f, 0.0f);   glVertex3f(1.0f,0.0f,0.0f);
        glTexCoord2f(1.0f, 1.0f);   glVertex3f(1.0f,1.0f,0.0f);
      glEnd();
    glPopMatrix();
  }
  void projectHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0,2.0, 0.0,6.0, -1.0,1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslated(0.5,0,0);

    DrawHud(texHUD);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
  void goalScored() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0,3.0, 0.0,11.0, -1.0,1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslated(1.0,5.0,0.0);
    DrawHud(texGoal);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
  void reticle() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0,41.0, 0.0,41.0, -1.0,1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslated(20,20,0.0);
    DrawHud(texTIEWing);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }

//------------------------------------------------------------------
//---------------------------Liam's Objects-------------------------
//------------------------------------------------------------------
  // Draw cylinder
  void drawCylinder(double x,double y,double z, double dx,double dy,double dz, double th, unsigned int texture) {
    double radius = 0.2, height = 0.2;
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glColor3ub(255,255,255);
    glPushMatrix();
    //  Transform cylinder
    glTranslated(x,y,z);
    glRotated(th,0,0,1);
    glScaled(dx,dy,dz);

    // Set Textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

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

    double i;
    glBindTexture(GL_TEXTURE_2D,texture);
    glNormal3d(0,1,0);
    /* Top of Cylinder */
    glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5); 
      glVertex3d(0.0, height, 0.0);
      for(i = 0.0; i < 360; i+=.125) {
        glTexCoord2f(-0.5 * cos(i)+0.5, 0.5 * sin(i)+0.5);
        glVertex3d(radius * cos(i), height, radius * sin(i));
      }
    glEnd();
    glNormal3d(0,-1,0);

    /* Bottom of Cylinder */
    glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3d(0.0, -height, 0.0);
      for(i = 0.0; i < 360; i+=.125) {
        glTexCoord2f(-0.5 * cos(i)+0.5, 0.5 * sin(i)+0.5);
        glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
    glEnd();
    glPopMatrix(); 
  }
  // Wings of TIE Fighter
  void TIEwing() {
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D,texTIEWing);
    glColor3ub(255,255,255);
    int org = 0;
    double l = 5;
    double w = 7;
    double h = 0.1;
    glBegin(GL_QUADS);  
    // Back
    glNormal3d(-1,0,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,w,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(org,w,h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,org,h);
    // Front
    glNormal3d(1,0,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(l,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(l,w,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(l,w,h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(l,org,h);
    // Side2
    glNormal3d(0,-1,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(l,org,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(l,org,h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,org,h);
    // Side 2
    glNormal3d(0,1,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,w,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(l,w,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(l,w,h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,w,h);
    // Top
    glNormal3d(0,0,-1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,w,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(l,w,org);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(l,org,org);
    //Bottom
    glNormal3d(0,0,1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,h);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,w,h);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(l,w,h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(l,org,h);

    glEnd();
    glPopMatrix();
  }
  // Body of TIE Fighter
  void sphere(unsigned int texture) {
    const int d=5;
    int th,ph;
    double r = 1;
    //  Save transformation
    glPushMatrix();
    //  Offset and scale
    glScaled(r,r,r);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D,texture);

    //  Latitude bands
    for (ph=-90;ph<90;ph+=d) {
      glBegin(GL_QUAD_STRIP);
      glColor3ub(255,255,255);
      for (th=0;th<=360;th+=d) {
        glNormal3d(-Sin(th),0,-Cos(th));
        glTexCoord2f(-cos(ph),sin(ph));
        glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
        glTexCoord2f(cos(ph),-sin(ph));
        glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
      }
      glEnd();
    }
    //  Undo transformations
    glPopMatrix();
  }
  // Window piece of TIE Fighter
  void ellipse(unsigned int texture) {
    const int d=5;
    int th,ph;
    double r = 0.9;
    //  Save transformation
    glPushMatrix();
    //  Offset and scale
    glColor3ub(10,10,10);
    glTranslated(0.05,0,0);
    glScaled(r,r,r);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D,texture);

    //  Latitude bands
    for (ph=-90;ph<90;ph+=d) {
      glBegin(GL_QUAD_STRIP);
      //glColor3ub(200,0,0);
      for (th=0;th<=360;th+=d) {
        glNormal3d(Sin(th),0,Cos(th));
        glTexCoord2f(-cos(ph),sin(ph));
        glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
        glTexCoord2f(cos(ph),-sin(ph));
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
    glColor3ub(255,255,255);
    TIEwing();
    glTranslated(2.5,3.5,-0.4);

    glRotated(90,0,1,0);
    glTranslated(-1.5,0,0);
    drawCylinder(0,0,0, 2,5,2, 90,texTIEArm);
    glTranslated(1.5,0,0);
    glRotated(-90,0,1,0);

    glTranslated(0,0,3);
    sphere(texTIEBody);
    glTranslated(0.12,0,0);
    ellipse(texPlain);
    glTranslated(-0.17,0,0);

    glTranslated(0,0,3);
    glRotated(180,0,0,0);
    glTranslated(-2.5,-3.5,0.4);
    glColor3ub(255,255,255);
    TIEwing();
    glTranslated(2.5,3.5,-0.4);

    glRotated(90,0,1,0);
    glTranslated(-1.5,0,0);
    drawCylinder(0,0,0, 2,5,2, 90,texTIEArm);

    glPopMatrix();
  }
  // Wings of X-Wing
  void Xwings() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D,texXWING);
    int org = 0;
    float thick = 0.2;
    int length = 7;
    int h1 = 2;
    int h2 = 1;
    glColor3ub(255,255,255);
    glBegin(GL_QUADS);
    //side1
    glNormal3d(0,0,-1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(length,org,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length,h2,org);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,h1,org);
    //side2
    glNormal3d(0,0,1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,thick);
    glTexCoord2f(0.0f, 1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(length,org,thick);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length,h2,thick);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,h1,thick);
    //big
    glNormal3d(0,0,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,h1,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(org,h1,thick);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,org,thick);
    //small
    glNormal3d(0,-1,0);
    glColor3ub(100,0,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,org,thick);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length,org,thick);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(length,org,org);
    //bottom
    glNormal3d(0,1,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(length,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(length,org,thick);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length,h2,thick);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(length,h2,org);
    //tip
    glNormal3d(0,1,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,org,thick);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(org,h1,thick);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,h1,org);
    //top
    glNormal3d(0,1,0);
    glTexCoord2f(0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(org,h1,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,h1,thick);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length,h2,thick);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(length,h2,org);

    glEnd();

    glTranslated(7,3,0.1);
    glRotated(90,0,0,0);

    //Rockets
    glPushMatrix();
    glTranslated(-5,0,0);
    glRotated(90,1,0,0);
    drawCylinder(0,3,0, 2.2,10,2.2, 0,texXWING);
    //Guns
    glTranslated(5,0,0);
    drawCylinder(0,1,0, 0.5,10,0.5, 0,texXWING);
    glPopMatrix();
  }
  // Fuselage of X-Wing
  void body() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D,texXWING);
    int org = 0;
    int l = 4;
    int w = 2;
    int h = 2;
    glColor3ub(255,255,255);
    glPushMatrix();
    glBegin(GL_QUADS);  
    // Back
    glNormal3d(-1,0,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,w,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(org,w,h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,org,h);
    // Front
    glNormal3d(0,0,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(l,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(l,w,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(l,w,h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(l,org,h);
    glColor3ub(150,0,0);
    // Side2
    glNormal3d(0,-1,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(l,org,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(l,org,h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,org,h);
    // Side 2
    glNormal3d(0,1,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,w,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(l,w,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(l,w,h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,w,h);
    // Bottom
    glNormal3d(0,0,-1);
    glColor3ub(255,255,255);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,w,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(l,w,org);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(l,org,org);
    //Top
    glNormal3d(0,0,1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,h);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,w,h);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(l,w,h);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(l,org,h);

    glEnd();
    glPopMatrix();
  }
  // Nose(s) of X-Wing and Falcon (hence color parameters)
  void nose(int r,int g,int b,int r2,int g2,int b2) {
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D,texXWING);
    int org = 0;
    float thick = 2;
    int length = 7;
    int h1 = 2;
    float h2 = 0.5;
    glColor3ub(r,g,b); //100
    glBegin(GL_QUADS);
    //side1
    glNormal3d(0,0,-1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(length,org,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length,h2,org);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,h1,org);
    //side2
    glNormal3d(0,0,1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,thick);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(length,org,thick);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length,h2,thick);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,h1,thick);
    //big
    glNormal3d(0,-1,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,h1,org);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(org,h1,thick);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,org,thick);
    //small
    glNormal3d(0,-1,0);
    glColor3ub(r2,g2,b2); //150
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,org,thick);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length,org,thick);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(length,org,org);
    //tip
    glNormal3d(1,0,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(length,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(length,org,thick);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length,h2,thick);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(length,h2,org);
    //back
    glNormal3d(0,0,0);
    glColor3ub(255,255,255);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,org,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,org,thick);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(org,h1,thick);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(org,h1,org);
    glColor3ub(r2,g2,b2);
    //top
    glNormal3d(0,1,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(org,h1,org);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(org,h1,thick);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length,h2,thick);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(length,h2,org);

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
    nose(150,0,0,255,255,255);

    glPopMatrix();
  }
  // Draw sky texture borrowed from ex25.c
  static void Sky(double D) {
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);

    //  Sides
    glBindTexture(GL_TEXTURE_2D,texNight[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
    glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
    glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
    glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

    glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
    glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
    glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
    glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

    glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
    glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
    glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
    glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

    glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
    glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
    glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
    glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
    glEnd();

    //  Top and bottom
    glBindTexture(GL_TEXTURE_2D,texNight[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
    glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
    glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
    glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);

    glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
    glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
    glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
    glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
    glEnd();

    glDisable(GL_TEXTURE_2D);
  }
  // Move TIE Fighter
  static void moveTIE(double x,double y,double z,double r) {
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glRotated(zhTIE+90,0,-1,0);
    drawTieFighter();
    //  Undo transofrmations
    glPopMatrix();
  }
  // Move X-Wing
  static void moveX_W(double x,double y,double z,double r) {
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(r,r,r);

    glRotated(90,-1,0,0);
    glRotated(180,0,0,1);
    if (2*zhX_W >= 0 && zhX_W < 90) 
      glRotated(2*zhX_W,0,0,-1);
    else if (2*zhX_W >= 90 && zhX_W < 270)
      glRotated(2*zhX_W,0,0,1);
    else if (2*zhX_W >= 270 && zhX_W < 360)
      glRotated(2*zhX_W,0,0,-1);
    drawXWing();
    //  Undo transofrmations
    glPopMatrix();
  }
  // Draw body of Millenium Falcon
  void falconCylinder(double x,double y,double z, double dx,double dy,double dz, double th, char lcol, char scol) {
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
        glColor3ub(150, 0, 0); //Set color to red
     } else if (scol == 'g') {
        glColor3ub(0, 150, 0); //Set color to green
     } else if (scol == 'b') {
        glColor3ub(0, 0, 255); //Set color to blue
     } else if (scol == 'w') {
        glColor3ub(255, 255, 255); //Set color to white
     } else if (scol == 'y') {
        glColor3ub(255, 215, 0); //Set color to yellow
     } else if (scol == 'n') {
        glColor3ub(139, 69, 19); //Set color to brown
     } else {
        glColor3ub(150, 150, 150); //Default color to gray
     }

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
        glColor3ub(150, 0, 0); //Set color to red
     } else if (lcol == 'g') {
        glColor3ub(0, 150, 0); //Set color to green
     } else if (lcol == 'b') {
        glColor3ub(0, 0, 255); //Set color to blue
     } else if (lcol == 'w') {
        glColor3ub(255, 255, 255); //Set color to white
     } else if (lcol == 'y') {
        glColor3ub(255, 215, 0); //Set color to yellow
     } else if (lcol == 'n') {
        glColor3ub(139, 69, 19); //Set color to brown
     } else {
        glColor3ub(150, 150, 150); //Default color to gray
     }

     double i; 
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
  void cockpit(double r) {
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    const int d=5;
    int th,ph;
    //  Save transformation
    glPushMatrix();
    //  Offset and scale
    glScaled(r,r,r);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D,texPlain);

    //  Latitude bands
    for (ph=-90;ph<90;ph+=d) {
      glBegin(GL_QUAD_STRIP);
      glColor3ub(150,0,0);
      for (th=0;th<=360;th+=d) {
        glNormal3d(Sin(th),0,Cos(th));
        glTexCoord2f(-cos(ph),sin(ph));
        glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
        glTexCoord2f(cos(ph),-sin(ph));
        glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
      }
      glEnd();
    }
    //  Undo transformations
    glPopMatrix();
  }
  // Assemble pieces of falcon
  void buildFalcon() {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,texPlain);
    falconCylinder(0,-1,0, 40,5,40, 0,':','r');
    glRotated(90,1,0,0);
    glTranslated(-9.7,9,1);
    falconCylinder(0,-1,0, 4,18,4, 0,'r','r');
    glTranslated(0,2.5,0);
    cockpit(1.2);
    glRotated(90,0,0,1);
    glTranslated(-3,-8,-1);
    glScaled(1.5,3,1);
    nose(150,150,150, 150,0,0);
    glTranslated(0,-1.2,2);
    glRotated(180,1,0,0);
    nose(150,150,150, 150,0,0);
    glPopMatrix();
  }
  // Move Falcon
  static void moveFalc(double x,double y,double z,double r) {
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glRotated(90,1,0,0);
    glRotated(90,0,1,0);
    glRotated(zhFalc+90,-1,0,0);
    glRotated(zhFalc+90,0,0,1);
    buildFalcon();
    //  Undo transofrmations
    glPopMatrix();
  }
  // Make snitch
  void buildSnitch() {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,texPlain);
    falconCylinder(0,-1,0, 40,5,40, 0,'y','y');
    glRotated(90,1,0,0);
    glTranslated(-9.7,9,1);
    falconCylinder(0,-1,0, 4,18,4, 0,'y','y');
    glTranslated(0,2.5,0);
    cockpit(1.2);
    glRotated(90,0,0,1);
    glTranslated(-3,-8,-1);
    glScaled(1.5,3,1);
    nose(255,255,0, 255,255,0);
    glTranslated(0,-1.2,2);
    glRotated(180,1,0,0);
    nose(255,255,0, 255,255,0);
    glPopMatrix();
  }
  // Move Snitch
  static void moveSnitch(double x,double y,double z,double r) {
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glRotated(90,-1,0,0);
    glRotated(90,0,-1,0);
    glRotated(90,0,0,1);
    buildSnitch();
    glPopMatrix();
  }
  // Fleet of TIE Fighters
  void TIEfleet(double x,double y,double z,double r) {
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glRotated(90,1,0,0);
    glRotated(zhTIEf1+90,0,0,1);
    drawTieFighter();
    glTranslated(-10,0,5);
    drawTieFighter();
    glTranslated(0,0,-10);
    drawTieFighter();
    glTranslated(-10,0,15);
    drawTieFighter();
    glTranslated(0,0,-10);
    drawTieFighter();
    glTranslated(0,0,-10);
    drawTieFighter();
    glTranslated(-10,0,-5);
    drawTieFighter();
    glTranslated(0,0,10);
    drawTieFighter();
    glTranslated(0,0,10);
    drawTieFighter();
    glTranslated(0,0,10);
    drawTieFighter();
    glPopMatrix();
  }
  // Fleet of X-Wings
  void XWfleet(double x,double y,double z,double r) {
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glRotated(90,0,0,-1);
    glRotated(zhXWf1+90,1,0,0);
    drawXWing();
    glTranslated(-10,20,5);
    drawXWing();
    glTranslated(0,0,-10);
    drawXWing();
    glTranslated(-10,0,15);
    drawXWing();
    glTranslated(0,20,-10);
    drawXWing();
    glTranslated(0,-20,-10);
    drawXWing();
    glTranslated(-10,-20,-5);
    drawXWing();
    glTranslated(0,20,10);
    drawXWing();
    glTranslated(0,0,10);
    drawXWing();
    glTranslated(0,-20,10);
    drawXWing();
    glPopMatrix();
  }
  // Body of TIE Fighter
  void drawQuaffle(double x,double y,double z,double r) {
    const int d=5;
    int th,ph;
    //  Save transformation
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(r,r,r);
    //  Offset and scale
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D,texPlain);

    //  Latitude bands
    for (ph=-90;ph<90;ph+=d) {
      glBegin(GL_QUAD_STRIP);
      glColor3ub(100,0,0);
      for (th=0;th<=360;th+=d) {
        glNormal3d(Sin(th),0,Cos(th));
        glTexCoord2f(-cos(ph),sin(ph));
        glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
        glTexCoord2f(cos(ph),-sin(ph));
        glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
      }
      glEnd();
    }
    //  Undo transformations
    glPopMatrix();
  }
  void LASERS(int r, int g, int b) {
    glPushMatrix();
    glColor3ub(r,g,b);
    drawCylinder(0,0,0, 0.05,1,0.05, 90,texPlain);
    glPopMatrix();
  }
  void LASERSmove () {
    glPushMatrix();
    glTranslated(-0.1*LASERSHOT,0,1.5);
    LASERS(255,0,255);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.1*LASERSHOT,-0.1*LASERSHOT,1.5);
    glRotated(30,0,0,1);
    LASERS(255,0,255);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.1*LASERSHOT,0.1*LASERSHOT,1.5);
    glRotated(-30,0,0,1);
    LASERS(255,0,255);
    glPopMatrix();
  }

//------------------------------------------------------------------
//---------------------------Eric's Objects-------------------------
//------------------------------------------------------------------
  //--------------------------------Pitch-----------------------------
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
      glScaled(dx*25, 2*dy, dz*15);

      // Set Textures
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

      glBindTexture(GL_TEXTURE_2D,texPitch);
      //start drawling of the the field
      glColor3f(1.0f,1.0f,1.0f);
      glBegin(GL_QUADS);        // start drawing the cube.
        // Top Face
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
      glEnd();

      glBindTexture(GL_TEXTURE_2D,texPitchSide);
      glBegin(GL_QUADS);
        // Front Face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
       
        // Back Face
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
       
        // Bottom Face
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
       
        // Right face
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
       
        // Left Face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
      glEnd();

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
  //--------------------------------Other-----------------------------
    // The towers and functions that texture them
    void drawTower(double x,double y,double z, double dx,double dy,double dz, double th, char lcol, char scol) {
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
          glColor3ub(150, 0, 0); //Set color to red
       } else if (scol == 'g') {
          glColor3ub(0, 150, 0); //Set color to green
       } else if (scol == 'b') {
          glColor3ub(0, 0, 255); //Set color to blue
       } else if (scol == 'w') {
          glColor3ub(255, 255, 255); //Set color to white
       } else if (scol == 'y') {
          glColor3ub(255, 215, 0); //Set color to yellow
       } else if (scol == 'n') {
          glColor3ub(139, 69, 19); //Set color to brown
       } else if (lcol == 'j') {
          glColor3ub(0, 255, 255); //Set color to brown
       } else if (lcol == 'k') {
          glColor3ub(255, 0, 45); //Set color to brown
       } else {
          glColor3ub(150, 150, 150); //Default color to gray
       }

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
          glColor3ub(150, 0, 0); //Set color to red
       } else if (lcol == 'g') {
          glColor3ub(0, 150, 0); //Set color to green
       } else if (lcol == 'b') {
          glColor3ub(0, 0, 255); //Set color to blue
       } else if (lcol == 'w') {
          glColor3ub(255, 255, 255); //Set color to white
       } else if (lcol == 'y') {
          glColor3ub(255, 215, 0); //Set color to yellow
       } else if (lcol == 'n') {
          glColor3ub(139, 69, 19); //Set color to brown
       } else if (lcol == 'j') {
          glColor3ub(0, 255, 255); //Set color to brown
       } else if (lcol == 'k') {
          glColor3ub(255, 0, 45); //Set color to brown
       } else {
          glColor3ub(150, 150, 150); //Default color to gray
       }

       double i; 
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
    void drawStandGrf(){
      glPushMatrix();
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawTower(0,-1,0, 4,20,4, 0,'r','r');
      glPopMatrix();
    }
    void drawStandRvn(){
      glPushMatrix();
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawTower(0,-1,0, 4,20,4, 0,'b','b');
      glPopMatrix();
    }
    void drawStandSly(){
      glPushMatrix();
      glBindTexture(GL_TEXTURE_2D,texStand);  
      drawTower(0,-1,0, 4,20,4, 0,'g','g');
      glPopMatrix();
    }
    void drawStandHuf(){
      glPushMatrix();
      glBindTexture(GL_TEXTURE_2D,texStand);  
      drawTower(0,-1,0, 4,20,4, 0,'y','y'); 
      glPopMatrix(); 
    }
    // Draw torus to use for hoops
    static void torus(int numc, int numt, int flag) {
      glPushMatrix();
      int i, j, k;
      double s, t, x, y, z, twopi;
      glBindTexture(GL_TEXTURE_2D,texPlain);
      
      if (flag)
        glColor3ub(255, 0, 45);
      else
        glColor3ub(0, 255, 255);
      glScaled(0.5,0.5,0.5);
      glTranslated(0,3.5,0);

      twopi = 2 * PI;
      for (i = 0; i < numc; i++) {
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= numt; j++) {
          for (k = 1; k >= 0; k--) {
            s = (i + k) % numc + 0.5;
            t = j % numt;

            x = (1+.1*cos(s*twopi/numc))*cos(t*twopi/numt);
            y = (1+.1*cos(s*twopi/numc))*sin(t*twopi/numt);
            z = .1 * sin(s * twopi / numc);
            glVertex3f(x, y, z);
          }
        }
        glEnd();
      }
      glPopMatrix();
    }
    void drawGoalPost(int flag){
      glPushMatrix();
      glRotated(90,0,1,0);

      torus(8,25,flag);
      char color;
      if (flag)
        color = 'k';
      else
        color = 'j';
      drawTower(0,-1,0, 0.2,12,0.2, 0,color,color);
      glPopMatrix();
    }
    static void drawWall(double xSize, double ySize, double zSize, double dx, double dy, double dz, double th, char wcol){
      //set specular color to white
      float white[] = {1,1,1,1};
      float black[] = {0,0,0,1};
      glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
      glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

      glPushMatrix();
      // Translation
      glTranslated(xSize, ySize, zSize);
      //glRotated(th, 0, 1, 0);
      glScaled(dx,dy,dz);

      // Set Textures
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

      //start drawling of the the field

      if(wcol == 'r'){
        glColor3f(1.0,0.0,0.0); //red
      } else if(wcol == 'g'){
        glColor3f(0.0,1.0,0.0);  //green
      } else if(wcol == 'b'){
        glColor3f(0.0,0.0,1.0); //blue
      } else if(wcol == 'y'){
        glColor3f(1.0,1.0,0); //yellow
      } else {
        glColor3f(1.0,1.0,1.0); //white
      } 

      glBegin(GL_QUADS);        // start drawing the cube.
     
      // Front Face
      glNormal3f( 0, 0, 1); 
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad

      // Top Face
      glNormal3f(0, 1, 0); 
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
     
      // Right face
      glNormal3f(1, 0, 0); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
      glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad

      // left face
      glNormal3f( 0, 0, 1); 
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad

      //bottom face
      glNormal3f(0, 1, 0); 
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
      

      //back face
      glNormal3f(1, 0, 0); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad

      glEnd();

      glPopMatrix(); //undo transformations
    }
  //---------------------------------BG-------------------------------
    static void drawbasesd(){
      glBegin( GL_TRIANGLES );
      glNormal3f(0,2,0);
      glColor3ub(187,187,187); glVertex3f(0,0,0);
      glColor3ub(187,187,187); glVertex3f(0,0,10);
      glColor3ub(187,187,187); glVertex3f(20,0,5);

      glColor3ub(187,187,187); glVertex3f(0,2,0);
      glColor3ub(187,187,187); glVertex3f(0,2,10);
      glColor3ub(187,187,187); glVertex3f(20,2,5);
      glEnd();

      glBegin(GL_QUADS);
      glColor3ub(9,82,77);
      glNormal3f(0,0,1);
      glVertex3f(0,0,0); //br
      glVertex3f(0,0,10); //bl
      glVertex3f(0,2,10); //tl
      glVertex3f(0,2,0); //tr
      glEnd();

      glBegin(GL_QUADS);
      glColor3ub(9,82,77);
      glNormal3f(-1,0,0);
      glVertex3f(0,0,10); //br
      glVertex3f(20,0,5); //bl
      glVertex3f(20,2,5); //tl
      glVertex3f(0,2,10); //tr
      glEnd();

      glBegin(GL_QUADS);
      glNormal3f(1,0,0);
      glColor3ub(9,82,77);
      glVertex3f(20,0,5); //br
      glVertex3f(0,0,0); //bl
      glVertex3f(0,2,0); //tl
      glVertex3f(20,2,5); //tr
      glEnd();
    }
    static void drawblock1(){
      glBegin(GL_QUADS);
      // Front
        glColor3ub(9,82,77);
        glNormal3f( 0, 0, 1);
      glVertex3f(1.5,2,2); //br
      glVertex3f(6,2,2); //bl
      glVertex3f(6,4,2); //tl
      glVertex3f(1.5,4,2); //tr
      // Back
        glNormal3f( 0, 0,-1);
      glVertex3f(1.5,2,8); // Bottom
      glVertex3f(6,2,8); // Bottom
      glVertex3f(6,4,8); // Top
      glVertex3f(1.5,4,8); // Top
      // Right  
      glNormal3f(+1, 0, 0);
      glVertex3f(1.5,2,8); // Bottom
      glVertex3f(1.5,2,2); // Bottom
      glVertex3f(1.5,4,2); // Top
      glVertex3f(1.5,4,8); // Top
      // Left 
        glColor3ub(187,187,187);
      glNormal3f(-1, 0, 0);
      glVertex3f(6,2,2); // Bottom
      glVertex3f(6,2,8); // Bottom
      glVertex3f(6,4,8); // Top
      glVertex3f(6,4,2); // Top
      //bottom
      glNormal3f(0, -1, 0);
      glVertex3f(1.5,2,8); // Bottom
      glVertex3f(6,2,8); // Bottom
      glVertex3f(6,2,2); // Bottom
      glVertex3f(1.5,2,2); // Bottom
      //top
      glNormal3f(0,+1,0);
      glVertex3f(1.5,4,2);
      glVertex3f(6,4,2);
      glVertex3f(6,4,8);
      glVertex3f(1.5,4,8);

      glEnd();
    }
    static void drawblock2(){
      glBegin(GL_QUADS);
      // Front
        glColor3ub(9,82,77);
        glNormal3f( 0, 0, 1);
      glVertex3f(1.5,4,3.5); //br
      glVertex3f(4,4,3.5); //bl
      glVertex3f(4,5.5,3.5); //tl
      glVertex3f(1.5,5.5,3.5); //tr
      // Back
        glNormal3f( 0, 0,-1);
      glVertex3f(1.5,4,6.5); // Bottom
      glVertex3f(4,4,6.5); // Bottom
      glVertex3f(4,5.5,6.5); // Top
      glVertex3f(1.5,5.5,6.5); // Top
      // Right
        glNormal3f(+1, 0, 0);
      glVertex3f(1.5,4,6.5); // Bottom
      glVertex3f(1.5,4,3.5); // Bottom
      glVertex3f(1.5,5.5,3.5); // Top
      glVertex3f(1.5,5.5,6.5); // Top
      // Left
        glColor3ub(187,187,187);
        glNormal3f(-1, 0, 0);
      glVertex3f(4,4,3.5); // Bottom
      glVertex3f(4,4,6.5); // Bottom
      glVertex3f(4,5.5,6.5); // Top
      glVertex3f(4,5.5,3.5); // Top
      //bottom
      glNormal3f(0, -1, 0);
      glVertex3f(1.5,4,6.5); // Bottom
      glVertex3f(4,4,6.5); // Bottom
      glVertex3f(4,4,3.5); // Bottom
      glVertex3f(1.5,4,3.5); // Bottom
      //top
      glNormal3f(0,+1,0);
      glVertex3f(1.5,5.5,3.5);
      glVertex3f(4,5.5,3.5);
      glVertex3f(4,5.5,6.5);
      glVertex3f(1.5,5.5,6.5);

      glEnd();
    }
    static void drawblock3(){
      glBegin(GL_QUADS);
      // Front
        glColor3ub(9,82,77);
        glNormal3f( 0, 0, 1);
      glVertex3f(2,5.5,4); //br
      glVertex3f(3,5.5,4); //bl
      glVertex3f(3,5.5,6); //tl
      glVertex3f(2,5.5,6); //tr
      // Back
        glNormal3f( 0, 0,-1);
      glVertex3f(2,5.5,6); // Bottom
      glVertex3f(3,5.5,6); // Bottom
      glVertex3f(3,8,6); // Top
      glVertex3f(2,8,6); // Top
      // Right
        glNormal3f(+1, 0, 0);
      glVertex3f(2,5.5,6); // Bottom
      glVertex3f(2,5.5,4); // Bottom
      glVertex3f(2,8,4); // Top
      glVertex3f(2,8,6); // Top
      // Left
        glColor3ub(187,187,187);
        glNormal3f(-1, 0, 0);
      glVertex3f(3,5.5,6); // Bottom
      glVertex3f(3,5.5,4); // Bottom
      glVertex3f(3,8,4); // Top
      glVertex3f(3,8,6); // Top
      //bottom
      glNormal3f(0, -1, 0);
      glVertex3f(2,5.5,4); // Bottom
      glVertex3f(3,5.5,4); // Bottom
      glVertex3f(3,8,4); // Bottom
      glVertex3f(2,8,4); // Bottom
      //top
      glNormal3f(0,+1,0);
      glVertex3f(2,8,4);
      glVertex3f(3,8,4);
      glVertex3f(3,8,6);
      glVertex3f(2,8,6);

      glEnd();
    }
    static void drawblock4(){
      glBegin(GL_QUADS);
      // Front
        glColor3ub(9,82,77);
        glNormal3f( 0, 0, 1);
      glVertex3f(2,7.5,2); //br
      glVertex3f(3.5,7.5,2); //bl
      glVertex3f(3.5,9.5,2); //tl
      glVertex3f(2,9.5,2); //tr
      // Back
        glNormal3f( 0, 0,-1);
      glVertex3f(2,7.5,8); // Bottom
      glVertex3f(3.5,7.5,8); // Bottom
      glVertex3f(3.5,9.5,8); // Top
      glVertex3f(2,9.5,8); // Top
      // Right
        glNormal3f(+1, 0, 0);
      glVertex3f(2,7.5,8); // Bottom
      glVertex3f(2,7.5,2); // Bottom
      glVertex3f(2,9.5,2); // Top
      glVertex3f(2,9.5,8); // Top*/
      // Left
        glColor3ub(187,187,187);
        glNormal3f(-1, 0, 0);
      glVertex3f(3.5,7.5,8); // Bottom
      glVertex3f(3.5,7.5,2); // Bottom
      glVertex3f(3.5,9.5,2); // Top
      glVertex3f(3.5,9.5,8); // Top
      //bottom
      glNormal3f(0, -1, 0);
      glVertex3f(2,7.5,8); // Bottom
      glVertex3f(2,7.5,2); // Bottom
      glVertex3f(3.5,7.5,2); // Bottom
      glVertex3f(3.5,7.5,8); // Bottom
      //top
      glNormal3f(0,+1,0);
      glVertex3f(2,9.5,8);
      glVertex3f(2,9.5,2);
      glVertex3f(3.5,9.5,2);
      glVertex3f(3.5,9.5,8);

      glEnd();
    }
    static void drawball1(){
      const int d=5;
      int th,ph;
      double r = 0.75;
      //  Save transformation
      glPushMatrix();
      //  Offset and scale
      glScaled(r,r,r);

      //  Latitude bands
      for (ph=-90;ph<90;ph+=d) {
        glBegin(GL_QUAD_STRIP);
        glColor3ub(255,255,255);
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
    void drawstardestroyer(double x,double y,double z,double r){
      glPushMatrix();
      glTranslated(x,y,z);
      glScaled(r,r,r);
      glRotated(zhStarDes-90,0,-1,0);
      glRotated(45,-1,4,0);
      drawbasesd();
      drawblock1();
      drawblock2();
      drawblock3();
      drawblock4();
      glTranslated(2.75,10,3.5);
      drawball1();
      glTranslated(0,0,3);
      drawball1();
      glPopMatrix();
    }
    void drawTardis(double x,double y,double z,double r){
      glPushMatrix();
      glTranslated(x,y,z);
      glRotated(4*zhTARDIS,0,1,0);
      glScaled(r,r,r);
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

      glBindTexture(GL_TEXTURE_2D,texTARDIS);
      glBegin(GL_QUADS);
        // Front
          glColor3ub(255,255,255);
          glNormal3f(0,0,-1);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0,0,0); //br
        glTexCoord2f(0.0f, 1.0f); glVertex3f(2,0,0); //bl
        glTexCoord2f(1.0f, 1.0f); glVertex3f(2,4,0); //tl
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0,4,0); //tr
        // Back
          glNormal3f(0,0,1);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0,0,2); // Bottom
        glTexCoord2f(0.0f, 1.0f); glVertex3f(2,0,2); // Bottom
        glTexCoord2f(1.0f, 1.0f); glVertex3f(2,4,2); // Top
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0,4,2); // Top
        // Right
          glNormal3f(1,0,0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(2,0,0); // Bottom
        glTexCoord2f(0.0f, 1.0f); glVertex3f(2,0,2); // Bottom
        glTexCoord2f(1.0f, 1.0f); glVertex3f(2,4,2); // Top
        glTexCoord2f(1.0f, 0.0f); glVertex3f(2,4,0); // Top
        // Left
          glNormal3f(-1,0,0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0,0,0); // Bottom
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0,0,2); // Bottom
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0,4,2); // Top
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0,4,0); // Top
        //bottom
        //glBindTexture(GL_TEXTURE_2D,texPlain);
        glNormal3f(0,1,0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0,0,0); // Bottom
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0,0,2); // Bottom
        glTexCoord2f(1.0f, 1.0f); glVertex3f(2,0,2); // Bottom
        glTexCoord2f(1.0f, 0.0f); glVertex3f(2,0,0); // Bottom
        //top
        glColor3ub(0,0,100);
        glNormal3f(0,1,0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(2,4,0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0,4,0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0,4,2);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(2,4,2);
      glEnd();

      glBegin(GL_QUADS);
        glColor3ub(0,0,255);
        glNormal3f(0,0,1);
        glVertex3f(2.5,-.5,2.5); //br
        glVertex3f(-.5,-.5,2.5); //bl
        glVertex3f(-.5,0,2.5); //tl
        glVertex3f(2.5,0,2.5); //tr
        
        glNormal3f(1,0,0);
        glVertex3f(2.5,-.5,2.5); // Bottom
        glVertex3f(2.5,-.5,-0.5); // Bottom
        glVertex3f(2.5,0,-0.5); // Top
        glVertex3f(2.5,0,2.5); // Top
        
        glNormal3f(-1,0,0);
        glVertex3f(-.5,0,-.5); // Bottom
        glVertex3f(-.5,-.5,-.5); // Bottom
        glVertex3f(-.5,-.5,2.5); // Top
        glVertex3f(-.5,0,2.5); // Top
        
        glNormal3f(0,0,-1);
        glVertex3f(2.5,0,-0.5); // Bottom
        glVertex3f(-.5,0,-0.5); // Bottom
        glVertex3f(-.5,-0.5,-0.5); // Top
        glVertex3f(2.5,-0.5,-0.5); // Top
        
        glNormal3f(0,-1,0);
        glVertex3f(2.5,-.5,-.5); // Bottom
        glVertex3f(-.5,-.5,-.5); // Bottom
        glVertex3f(-.5,-.5,2.5); // Bottom
        glVertex3f(2.5,-.5,2.5); // Bottom

        glNormal3f(0,1,0);
        glVertex3f(2.5,0,-.5);
        glVertex3f(-.5,0,-.5);
        glVertex3f(-.5,0,2.5);
        glVertex3f(2.5,0,2.5);
      glEnd();
      glPopMatrix();
    }
    static void drawbaseship1(){
      glBegin( GL_TRIANGLES );
      glNormal3f(0,2,0);
      glColor3ub(187,187,187); glVertex3f(8,0,0);
      glColor3ub(187,187,187); glVertex3f(8,0,10);
      glColor3ub(187,187,187); glVertex3f(20,0,5);

      glColor3ub(187,187,187); glVertex3f(8,2,0);
      glColor3ub(187,187,187); glVertex3f(8,2,10);
      glColor3ub(187,187,187); glVertex3f(20,2,5);
      glEnd();

      glBegin(GL_QUADS);
      glColor3ub(82,9,9);
      glNormal3f(0,0,1);
      glVertex3f(8,0,0); //br
      glVertex3f(8,0,10); //bl
      glVertex3f(8,2,10); //tl
      glVertex3f(8,2,0); //tr
      glEnd();

      glBegin(GL_QUADS);
      glColor3ub(82,9,9);
      glNormal3f(-1,0,0);
      glVertex3f(20,0,5); //br
      glVertex3f(8,0,0); //bl
      glVertex3f(8,2,0); //tl
      glVertex3f(20,2,5); //tr
      glEnd();

      glBegin(GL_QUADS);
      glNormal3f(1,0,0);
      glColor3ub(82,9,9);
      glVertex3f(8,0,10); //br
      glVertex3f(20,0,5); //bl
      glVertex3f(20,2,5); //tl
      glVertex3f(8,2,10); //tr
      glEnd();
    }
    static void drawbaseship2() {
        glBegin(GL_QUADS);
        //front
        glNormal3f(0,0,1);
        glColor3ub(187,187,187);
        glVertex3f(8,0,0); //br
        glVertex3f(8,0,10); //bl
        glVertex3f(8,2,10); //tl
        glVertex3f(8,2,0); //tr
        //back
        glNormal3f(0,0,-1);
        glColor3ub(187,187,187);
        glVertex3f(6,0,1.5); //br
        glVertex3f(6,0,8.5); //bl
        glVertex3f(6,2,8.5); //tl
        glVertex3f(6,2,1.5); //tr
        //left
        glNormal3f(-1,0,0);
        glColor3ub(82,9,9);
        glVertex3f(8,0,0); //br
        glVertex3f(6,0,1.5); //bl
        glVertex3f(6,2,1.5); //tl
        glVertex3f(8,2,0); //tr
        //right
        glNormal3f(1,0,0);
        glColor3ub(82,9,9);
        glVertex3f(6,0,8.5); //br
        glVertex3f(8,0,10); //bl
        glVertex3f(8,2,10); //tl
        glVertex3f(6,2,8.5); //tr
        //top
        glNormal3f(0,1,0);
        glColor3ub(187,187,187);
        glVertex3f(8,2,0); //br
        glVertex3f(6,2,1.5); //bl
        glVertex3f(6,2,8.5); //tl
        glVertex3f(8,2,10); //tr
        //bottom
        glNormal3f(0,-1,0);
        glColor3ub(187,187,187);
        glVertex3f(8,0,0); //br
        glVertex3f(6,0,1.5); //bl
        glVertex3f(6,0,8.5); //tl
        glVertex3f(8,0,10); //tr

        glEnd();
    }
    static void drawbaseship3(){
      glBegin(GL_QUADS);
      //front
      glNormal3f(0,0,1);
      glColor3ub(187,187,187);
      glVertex3f(6,0,1.5); //br
      glVertex3f(6,0,8.5); //bl
      glVertex3f(6,2,8.5); //tl
      glVertex3f(6,2,1.5); //tr
      //back
      glNormal3f(0,0,-1);
      glColor3ub(82,9,9);
      glVertex3f(0,0,-2); //br
      glVertex3f(0,0,12); //bl
      glVertex3f(0,2,12); //tl
      glVertex3f(0,2,-2); //tr
      //left
      glNormal3f(-1,0,0);
      glColor3ub(82,9,9);
      glVertex3f(6,0,1.5); //br
      glVertex3f(0,0,-2); //bl
      glVertex3f(0,2,-2); //tl
      glVertex3f(6,2,1.5); //tr
      //right
      glNormal3f(1,0,0);
      glColor3ub(82,9,9);
      glVertex3f(0,0,12); //br
      glVertex3f(6,0,8.5); //bl
      glVertex3f(6,2,8.5); //tl
      glVertex3f(0,2,12); //tr
      //top
      glNormal3f(0,1,0);
      glColor3ub(187,187,187);
      glVertex3f(6,2,1.5); //br
      glVertex3f(6,2,8.5); //bl
      glVertex3f(0,2,12); //tl
      glVertex3f(0,2,-2); //tr
      //bottom
      glNormal3f(0,-1,0);
      glColor3ub(187,187,187);
      glVertex3f(6,0,1.5); //br
      glVertex3f(6,0,8.5); //bl
      glVertex3f(0,0,12); //tl
      glVertex3f(0,2,-2); //tr

      glEnd();
    }
    static void drawtower(){
      glBegin(GL_QUADS);
      //front
      glNormal3f(0,0,1);
      glColor3ub(187,187,187);
      glVertex3f(5,2,7); //br
      glVertex3f(5,2,3); //bl
      glVertex3f(3,6,3); //tl
      glVertex3f(3,6,7); //tr
      //back
      glNormal3f(0,0,-1);
      glColor3ub(82,9,9);
      glVertex3f(1,2,7); //br
      glVertex3f(1,2,3); //bl
      glVertex3f(1,6,3); //tl
      glVertex3f(1,6,7); //tr
      //left
      glNormal3f(-1,0,0);
      glColor3ub(82,9,9);
      glVertex3f(1,2,3); //br
      glVertex3f(5,2,3); //bl
      glVertex3f(3,6,3); //tl
      glVertex3f(1,6,3); //tr
      //right
      glNormal3f(1,0,0);
      glColor3ub(82,9,9);
      glVertex3f(1,2,7); //br
      glVertex3f(5,2,7); //bl
      glVertex3f(3,6,7); //tl
      glVertex3f(1,6,7); //tr
      //top
      glNormal3f(0,1,0);
      glColor3ub(187,187,187);
      glVertex3f(3,6,3); //br
      glVertex3f(1,6,3); //bl
      glVertex3f(1,6,7); //tl
      glVertex3f(3,6,7); //tr
      //bottom
      glNormal3f(0,-1,0);
      glColor3ub(187,187,187);
      glVertex3f(5,2,3); //br
      glVertex3f(1,2,3); //bl
      glVertex3f(1,2,7); //tl
      glVertex3f(5,2,7); //tr

      glEnd();
    }
    static void drawtop(){
      glBegin(GL_QUADS);
      //front
      glNormal3f(0,0,1);
      glColor3ub(128,128,128);
      glVertex3f(4,6,4); //br
      glVertex3f(4,6,3); //bl
      glVertex3f(4,8,3); //tl
      glVertex3f(4,8,4); //tr
      //back
      glNormal3f(0,0,-1);
      glColor3ub(82,9,9);
      glVertex3f(0,6,4); //br
      glVertex3f(0,6,3); //bl
      glVertex3f(0,8,3); //tl
      glVertex3f(0,8,4); //tr
      //left
      glNormal3f(-1,0,0);
      glColor3ub(82,9,9);
      glVertex3f(4,6,3); //br
      glVertex3f(0,6,3); //bl
      glVertex3f(0,8,3); //tl
      glVertex3f(4,8,3); //tr
      //right
      glNormal3f(1,0,0);
      glColor3ub(82,9,9);
      glVertex3f(0,6,4); //br
      glVertex3f(4,6,4); //bl
      glVertex3f(4,8,4); //tl
      glVertex3f(0,8,4); //tr
      //top
      glNormal3f(0,1,0);
      glColor3ub(128,128,128);
      glVertex3f(4,8,4); //br
      glVertex3f(4,8,3); //bl
      glVertex3f(0,8,3); //tl
      glVertex3f(0,8,4); //tr
      //bottom
      glNormal3f(0,-1,0);
      glColor3ub(128,128,128);
      glVertex3f(4,6,4); //br
      glVertex3f(4,6,3); //bl
      glVertex3f(0,6,3); //tl
      glVertex3f(0,6,4); //tr

      glEnd();
    }
    void drawShip(double x,double y,double z,double r){
      glPushMatrix();
      glTranslated(x,y,z);
      glScaled(r,r,r);
      glRotated(180,0,1,0);
      glRotated(35,0,1,0);
      drawbaseship1();
      drawbaseship2();
      drawbaseship3();
      drawtower();
      drawtop();
      glTranslated(0,0,3);
      drawtop();
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
    texPitch = LoadTexBMP("textures/pitch.bmp");
    texPitchSide = LoadTexBMP("textures/pitchsides.bmp");
    texNight[0] = LoadTexBMP("textures/night0.bmp");
    texNight[1] = LoadTexBMP("textures/night1.bmp");
    texStand = LoadTexBMP("textures/stand.bmp");
    texTIEBody = LoadTexBMP("textures/tie_arm.bmp");
    texTIEArm = LoadTexBMP("textures/tie_arm.bmp");
    texTIEWing = LoadTexBMP("textures/tie_wing.bmp");
    texXWING = LoadTexBMP("textures/xwing.bmp");
    texTARDIS = LoadTexBMP("textures/TARDIS.bmp");
    texMars = LoadTexBMP("textures/mars.bmp");
    texNeptune = LoadTexBMP("textures/neptune.bmp");
    texSun = LoadTexBMP("textures/sun.bmp");
    texHUD = LoadTexBMP("textures/hud.bmp");
    texGoal = LoadTexBMP("textures/GOAL.bmp");
  }

//------------------------------------------------------------------
//---------------------------Display Scene--------------------------
//------------------------------------------------------------------
  // Display all the object in the viewer
  void display() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();

    if (fp) {
      Cx = +1*dim*Sin(rot); //Ajust the camera vector based on rot
      Cy = +1*dim*Sin(rotup); //Ajust the camera vector based on rotup
      Cz = -1*dim*Cos(rot);
      gluLookAt(Ex,Ey,Ez, Cx+Ex,Cy+Ey,Cz+Ez, 0,1,0); // y is the up-axis
    }
    //  Perspective - set eye position
    else {
      //  Perspective - set eye position
      double px = -0.1*dim*Sin(th)*Cos(ph);
      double py = +0.1*dim        *Sin(ph); // removed shifty
      double pz = +0.1*dim*Cos(th)*Cos(ph);
      gluLookAt(px,py,pz, 0,0,0, 0,Cos(ph),0); //removed shifty
    }

    glShadeModel(GL_SMOOTH);
    //  Translate intensity to color vectors. Change the coefficients for different colors.
      float coef1,coef2,coef3;
      if (planet == 2) {
        coef1 = 0.002;
        coef2 = 0.002;
        coef3 = 0.008; 
      }
      else if (planet == 1) {
        coef1 = 0.008;
        coef2 = 0.003;
        coef3 = 0.003;
      }
      else {
        coef1 = 0.01;
        coef2 = 0.01;
        coef3 = 0.01;
      }
      float Ambient[]   = {coef1*ambient ,coef2*ambient ,coef3*ambient ,1.0};
      float Diffuse[]   = {coef1*diffuse ,coef2*diffuse ,coef3*diffuse ,1.0};
      float Specular[]  = {coef1*specular,coef2*specular,coef3*specular,1.0};

    //  Light position
      float ballPos[]  = {-1e5,0,-1e5,1};
      //  Draw light position as ball (still no lighting here)
      ball(ballPos[0],ballPos[1],ballPos[2],1000);
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
      glLightfv(GL_LIGHT0,GL_POSITION,ballPos);

    // Everything was oriented weird wrong, so this was added in as a fix, sorry.
      glRotated(90,-1,0,0);

    // The fighters
      float TIEPos[]  = {distance*Cos(zhTIE)+1000,distance*Sin(qh)-500,distance*Sin(zhTIE),1};
      glPushMatrix();
      glRotated(90,1,0,0);
      moveTIE(TIEPos[0],TIEPos[1],TIEPos[2],10);
      glPopMatrix();
      float TIEPos2[]  = {2*distance*Cos(-zhTIE/2)+500,distance*Sin(qh)-1000,distance*Sin(-zhTIE),1};
      glPushMatrix();
      glRotated(90,1,0,0);
      moveTIE(TIEPos2[0],TIEPos2[1],TIEPos2[2],10);
      glPopMatrix();

      glPushMatrix();
      float X_WPos[]  = {2*distance*Cos(zhX_W)-1000,distance*Sin(qh)-500,distance*Sin(2*zhX_W),1};
      glRotated(90,1,0,0);
      moveX_W(X_WPos[0],X_WPos[1],X_WPos[2],10);
      glPopMatrix();

      glPushMatrix();
      float X_WPos2[]  = {2*distance*Cos(zhX_W)-1000,distance*Sin(qh)-500,distance*Sin(2*zhX_W),1};
      glRotated(90,0,1,0);
      moveX_W(X_WPos2[0],X_WPos2[1],X_WPos2[2],10);
      glPopMatrix();

    // Fleets in background
      glPushMatrix();
      float TIEf1[]  = {5*distance*Cos(zhTIEf1),distance*Sin(qh),7*distance*Sin(zhTIEf1),1};
      glRotated(90,0,-1,1);
      TIEfleet(TIEf1[0],TIEf1[1],TIEf1[2],10);
      glPopMatrix();
      
      glPushMatrix();
      float TIEf12[]  = {4*distance*Cos(zhTIEf1),distance*Sin(qh),3*distance*Sin(zhTIEf1),1};
      glRotated(90,1,1,0);
      TIEfleet(TIEf12[0],TIEf12[1],TIEf12[2],10);
      glPopMatrix();

      glPushMatrix();
      glTranslated(-50,50,0);
      float XWf1[]  = {5*distance*Cos(zhXWf1),distance*Sin(qh),3*distance*Sin(zhXWf1),1};
      glRotated(90,-1,-1,0);
      XWfleet(XWf1[0],XWf1[1],XWf1[2],10);
      glPopMatrix();

      glPushMatrix();
      glTranslated(-50,50,0);
      float XWf12[]  = {8*distance*Cos(zhXWf1),distance*Sin(qh),4*distance*Sin(zhXWf1),1};
      glRotated(90,1,0,1);
      XWfleet(XWf12[0],XWf12[1],XWf12[2],10);
      glPopMatrix();

    // Eric's field
      // Draw the major elements
      glPushMatrix();
      glScaled(100,100,100);
      glRotated(90,1,0,0);
      drawFieldLine(2,1,1);
      goalzone1(1,1,1);
      goalzone2(1,1,1);
      halffield(1,1,1);
      centercircle(2,2,2);
      glTranslated(0,-9,0);
      drawField(0,8,0,1,1,1,0);
      glTranslated(0,9,0);

      // Place each stand
      //1
      glTranslated(0,-7,9);
      drawStandGrf();
      //2
      glTranslated(-7.5,0,-1);
      drawStandHuf();
      //3
      glTranslated(-7.5,0,-2);
      drawStandRvn();
      //4
      glTranslated(-5,0,-6);
      drawStandHuf();
      //5
      glTranslated(5,0,-6);
      drawStandGrf();
      //6
      glTranslated(7.5,0,-2);
      drawStandSly();
      //7
      glTranslated(7.5,0,-1);
      drawStandRvn();
      //8
      glTranslated(7.5,0,1);
      drawStandHuf();
      //9
      glTranslated(7.5,0,2);
      drawStandGrf();
      //10
      glTranslated(5,0,6);
      drawStandSly();
      //11
      glTranslated(-5,0,6);
      drawStandRvn();
      //12
      glTranslated(-7.5,0,2);
      drawStandSly();

      // Place each goal post
      glTranslated(7.5,-2,-6);
      drawGoalPost(0);
      glTranslated(0,0,-2);
      drawGoalPost(0);
      glTranslated(0,0,-2);
      drawGoalPost(0);
      //Other side
      glTranslated(-30,0,0);
      drawGoalPost(1);
      glTranslated(0,0,2);
      drawGoalPost(1);
      glTranslated(0,0,2);
      drawGoalPost(1);

      //Place walls between each stand (each wall labeled by which stands it connects)
      glScaled(1.5,1.5,1.5);
      glTranslated(-14.5,1,-1.6);
      //1-2
        glPushMatrix();
        glRotated(5.5,0,1,0);
        glBindTexture(GL_TEXTURE_2D,texStand);
        drawWall(26,-2,8.5, 2.2,1,0.5, 0,'r'); 
        glPopMatrix();
      //2-3
        glPushMatrix();
        glRotated(-7,0,1,0);
        glBindTexture(GL_TEXTURE_2D,texStand);
        drawWall(22,-2,3.25, 2.5,1,0.5, 0,'y');  
        glPopMatrix();
      //3-4
        glPushMatrix();
        glRotated(-14,0,1,0);
        glBindTexture(GL_TEXTURE_2D,texStand);
        drawWall(17,-2,.75, 2.5,1,0.5, 0,'b'); 
        glPopMatrix();
      //4-5
        glPushMatrix();
        glRotated(-50,0,1,0);
        glBindTexture(GL_TEXTURE_2D,texStand);
        drawWall(9.4,-2,-8, 2.5,1,0.5, 0,'y'); 
        glPopMatrix();
      //5-6
        glPushMatrix();
        glRotated(50,0,1,0);
        glBindTexture(GL_TEXTURE_2D,texStand);
        drawWall(9.75,-2,8.25, 2.5,1,0.5, 0,'r');  
        glPopMatrix();
      //6-7
        glPushMatrix();
        glRotated(20,0,1,0);
        glBindTexture(GL_TEXTURE_2D,texStand);
        drawWall(17,-2,1.5, 2.5,1,0.5, 0,'g'); 
        glPopMatrix();
      //7-8
        glPushMatrix();
        glRotated(5,0,1,0);
        glBindTexture(GL_TEXTURE_2D,texStand);
        drawWall(22,-2,-3.5, 2.5,1,0.5, 0,'b');  
        glPopMatrix();  
      //8-9
        glPushMatrix();
        glRotated(-6,0,1,0);
        glBindTexture(GL_TEXTURE_2D,texStand);
        drawWall(26.1,-2,-8, 2.5,1,0.5, 0,'y');  
        glPopMatrix();
      //9-10
        glPushMatrix();
        glRotated(-14.9,0,1,0);
        glBindTexture(GL_TEXTURE_2D,texStand);
        drawWall(30,-2,-12.25, 2.5,1,0.5, 0,'r');  
        glPopMatrix();
      //10-11
        glPushMatrix();
        glRotated(-49.8,0,1,0);
        glBindTexture(GL_TEXTURE_2D,texStand);
        drawWall(22,-2,-28.5, 2.5,1,0.5, 0,'g'); 
        glPopMatrix();
      //11-12
        glPushMatrix();
        glRotated(51,0,1,0);
        glBindTexture(GL_TEXTURE_2D,texStand);
        drawWall(21,-2,29.5, 2.5,1,0.5, 0,'b');  
        glPopMatrix();
      //12-1
        glPushMatrix();
        glRotated(16,0,1,0);
        glBindTexture(GL_TEXTURE_2D,texStand);
        drawWall(29,-2,13.5, 2.5,1,0.5, 0,'g');  
        glPopMatrix();

        glPopMatrix();

    // Draw Sky
      glPushMatrix();
      glRotated(90,1,0,0);
      glScaled(2,2,2);
      Sky(0.8*dim);
      glPopMatrix();

    // Planets
      glPushMatrix();
      glScaled(1000,1000,1000);
      glTranslated(-100,100,0);
      glScaled(100,100,100);
      glRotated(90,1,0,0);
      if (planet == 0) 
        sphere(texSun);
      else if (planet == 1)
        sphere(texMars);
      else if (planet == 2)
        sphere(texNeptune);
      glPopMatrix();

    // Millenium Falcon
      glPushMatrix();
      float FalcPos[]  = {10000*Cos(zhFalc),10000*Sin(qh),10000*Sin(zhFalc),1};
      glRotated(90,1,1,0);
      moveFalc(FalcPos[0],FalcPos[1],FalcPos[2],20);
      glPopMatrix();

    // Golden Snitch
      glPushMatrix();
      float snitchPos[]  = {2*distance*Cos(zhSnitch),distance*Sin(qh)-1000,distance*Sin(2*zhSnitch),1};
      glRotated(90,1,0,0);
      moveSnitch(snitchPos[0],snitchPos[1],snitchPos[2],1);
      glPopMatrix();

    // Draw Star Destroyer
      glPushMatrix();
      float StarDesPos[]  = {100*distance*Cos(zhStarDes)+1000,0,100*distance*Sin(zhStarDes),1};
      glRotated(90,1,0,0);
      drawstardestroyer(StarDesPos[0],StarDesPos[1],StarDesPos[2],500);
      float shipPos[]  = {100*distance*Cos(zhStarDes)+1000,-10000,100*distance*Sin(zhStarDes),1};
      drawShip(shipPos[0],shipPos[1],shipPos[2],700);
      glPopMatrix();

    // Draw TARDIS
      glPushMatrix();
      glRotated(90,1,1,0);
      float TARpos[]  = {5*distance*Cos(zhTARDIS+20)+1000,distance*Sin(qh),3*distance*Sin(zhTARDIS+20),1};
      drawTardis(TARpos[0],TARpos[1],TARpos[2],100);
      glPopMatrix();

    // Draw quaffle
      if (goalFlag) {
        goalScored();

        glPushMatrix();
        glRotated(90,0,1,0);
        glScaled(1000,1000,1000);
        glTranslated(1,0,0);

        LASERSmove();

        glTranslated(0,0,-3);

        LASERSmove();

        glPopMatrix();
      }

      if (quaffle) {
        if ((fabs(saveEx+quafX) >= 1500) && 
                (fabs(saveEy+quafY) <= 800 && fabs(saveEy+quafY) >= 650) &&
                    ((fabs(saveEz+quafZ) > 150 && fabs(saveEz+quafZ) < 250) ||
                    (fabs(saveEz+quafZ) < 50))) {
            goalFlag = 1;
            stopQuaf = 1;
        }
        glRotated(90,1,0,0);
        drawQuaffle(saveEx+quafX,saveEy+quafY,saveEz+quafZ,10);
      }
      if (!quaffle) {
        saveEx = Ex;
        saveEy = Ey;
        saveEz = Ez;
        saveCx = Cx;
        saveCz = Cz;
        saveCy = Cy;
      }

    // Draw HUD
      if (fp) {
        glPushMatrix();
        projectHUD();
        reticle();
        glPopMatrix();
      }

    // Render the scene and make it visible
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
  }

//------------------------------------------------------------------
//--------------------------Idle Function---------------------------
//------------------------------------------------------------------
  // Called when the window is idling
  void idle() {
    // Elapsed time in seconds
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh          = fmod(90*t,360.0);
    zhTrain     = fmod(15*t,360.0);
    zhTIE       = fmod(90*t,360.0);
    zhX_W       = fmod(45*t + 180,360.0);
    zhFalc      = fmod(15*t,360.0);
    zhSnitch    = fmod(45*t,360.0);
    zhTIEf1     = fmod(30*t,360.0);
    zhXWf1      = fmod(30*t,360.0);
    zhTARDIS    = fmod(30*t,360.0);
    zhTARDIS    = fmod(30*t,360.0);

    if (goalFlag && (LASERSHOT < 10)) {
      double ddt = 1;
      LASERSHOT += ddt;
    }
    if (LASERSHOT >= 10) {
      LASERSHOT = 0;
    }

    if (quaffle && ((fabs(saveEx+quafX) >= 1600) || (fabs(saveEy+quafY) >= 2000) || (fabs(saveEz+quafZ) >= 1000)
          || (fabs(saveEx+quafX) < 0) || (fabs(saveEy+quafY) < 0) || (fabs(saveEz+quafZ) < 0))) { //rethink this
      stopQuaf = 1;
    }
    else if (quaffle && !stopQuaf) {
      double dt = 0.0005;
      quafX += saveCx*dt; //Update the eye vector based on the camera vector
      quafZ += saveCz*dt;
      quafY += saveCy*dt;
    }
    // Tell GLUT it is necessary to redisplay the scene
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
         ph -= 1;
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
    //  Toggle first person
    else if (ch == 'f' || ch == 'F')
      fp = 1-fp;
    else if (ch == 'm' || ch == 'M')
       planet = (planet+1)%3;

    if (fp) {
      double dt = 0.0005;
      if (ch == 'w' || ch == 'W'){
        Ex += Cx*dt; //Update the eye vector based on the camera vector
        Ez += Cz*dt;
        Ey += Cy*dt;
      }
      else if (ch == 'a' || ch == 'A'){
        rot -= 2;
      }
      else if (ch == 's' || ch == 'S'){
        Ex -= Cx*dt;
        Ez -= Cz*dt;
        Ey -= Cy*dt;
      }
      else if (ch == 'd' || ch == 'D'){
        rot += 2;
      }
      else if (ch == 'r' || ch == 'R'){
        if (rotup >= 0 || rotup ==0)
          Ey += Cy*dt;
        else
          Ey -= Cy*dt;
      }
      else if (ch == 'v' || ch == 'V'){
        if (rotup >= 0 || rotup == 0)
          Ey -= Cy*dt;
        else
          Ey += Cy*dt;
      }
      else if (ch == ' '){
        rotup += 2;
      }
      else if (ch == 'x' || ch == 'X'){
        rotup -= 2;
      }
      //  Keep angles to +/-360 degrees
      rot %= 360;
      rotup %= 360;

      if ((ch == '.') && goalFlag)
        goalFlag = 0;
        quafX = 0;
        quafY = 0;
        quafZ = 0;
        stopQuaf = 0;
        quaffle = 0;
    }
    else {
      //  Reset view angle
      if (ch == '0')
         th = ph = 0;
    }

    //  Reproject
    oldProject();
    //  Animate if requested
    glutIdleFunc(move?idle:NULL);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
  }

//------------------------------------------------------------------
//--------------------------Mouse Function--------------------------
//------------------------------------------------------------------
  void mouseClicks(int button, int state, int x, int y) {
      if(fp && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && quaffle == 0) {
          quaffle = (quaffle+1)%2;
      }
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
//---------------------------Main Function--------------------------
//------------------------------------------------------------------
  // Does the thing
  int main(int argc,char* argv[]) {
    //  Initialize GLUT and process user parameters
    glutInit(&argc,argv);
    glutInitWindowSize(aspectW,aspectH);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Liam Kolber & Eric Speaker: Final Project");
    //  Tell GLUT to call "display" when the scene should be drawn
    glutDisplayFunc(display);
    //  Tell GLUT to call "reshape" when the window is resized
    glutReshapeFunc(reshape);
    //  Tell GLUT to call "special" when an arrow key is pressed
    glutSpecialFunc(special);
    //  Tell GLUT to call "key" when a key is pressed
    glutKeyboardFunc(key);
    //  Tell GLUT to call "mouseClicks" when a button is pressed
    glutMouseFunc(mouseClicks);
    //  Pass control to GLUT so it can interact with the user
    ErrCheck("init");
    //  Tell GLUT what to do when idle
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