// Liam Kolber and Eric Speaker
// CSCI 4229
// CU Boulder Fall 2017
// Final Project
//------------------------------------------------------------------
//---------------------------Initializations------------------------
//------------------------------------------------------------------
  #include "CSCIx229.h"
  // All other necessary files are included in the zip

//------------------------------------------------------------------
//--------------------------Global Variables------------------------
//------------------------------------------------------------------
  // General views
  int th        =   0;  // Azimuth of view angle
  int ph        =  20;  // Elevation of view angle
  int axes      =   0;  // Display axes
  int mode      =   2;  // What to display
  double asp    =   1;  // Aspect ratio
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
  int shiftx    =-7e3;  // Shift used to adjust eye and ball position in x
  int shifty    =1000;  // Shift used to adjust eye and ball position in y
  int shiftz    =4000;  // Shift used to adjust eye and ball position in z
  int trainX    =-7e3;  // Shift used to adjust train position in x
  int trainY    =1000;  // Shift used to adjust train position in y
  int trainZ    =4000;  // Shift used to adjust train position in z
  int zhTrain   =   0;  // Train azimuth
  int zhTIE     =   0;
  int zhX_W     =   0;
  double elipA  = 2.8;
  double elipB  = 1.5;
  int distTrain = 9e2;

  // First-person specific
  int fp        =   0;  // Toggle
  int rot       = 0.0;  // Rotational angle
  double Ex     =-8e3;  // Eye coords
  double Ey     =-1e3;
  double Ez     =6000;
  double Cx     =   0;  // Centering coords
  double Cy     =   0;
  double Cz     =   0; 
  int move      =   1;  // Movement

  // Light values
  int one       =   1;  // Unit value
  int distance  = 9e2;  // Light distance 9e4
  int inc       =  10;  // Ball increment
  int smooth    =   1;  // Smooth/Flat shading
  int local     =   0;  // Local Viewer Model
  int emission  =   0;  // Emission intensity (%)
  int ambient   =  30;  // Ambient intensity (%)
  int diffuse   = 100;  // Diffuse intensity (%)
  int specular  =   0;  // Specular intensity (%)
  int shininess =   0;  // Shininess (power of two)
  float shinyvec[1];    // Shininess (value)
  int zh        = 120;  // Light azimuth
  int qh        =  20;
  float ylight  =1000;  // Elevation of light
  int timeOday  =   0;  // Day/Night toggle

  unsigned int texEngine;
  unsigned int texSmoke;
  unsigned int texBody;
  unsigned int texWheel;
  unsigned int texPlain;
  unsigned int texFront;
  unsigned int texTracks;
  unsigned int texGround;
  unsigned int texPitch;
  unsigned int texSky[2];
  unsigned int texNight[2];
  unsigned int texStand;

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
    glPushMatrix();
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
    glPopMatrix();
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
  // Draw sky texture borrowed from ex25.c
  static void Sky(double D) {
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);

    //  Sides
    if (timeOday)
      glBindTexture(GL_TEXTURE_2D,texNight[0]);
    else
      glBindTexture(GL_TEXTURE_2D,texSky[0]);
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
    if (timeOday)
      glBindTexture(GL_TEXTURE_2D,texNight[1]);
    else
      glBindTexture(GL_TEXTURE_2D,texSky[1]);
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
  // Move train
  static void drawTrainMove(double x,double y,double z,double r) {
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(r,r,r);
    float trigX = elipA*Cos(zhTrain)/2;//-elipA;
    float trigY = elipB*Sin(zhTrain)/2;//-elipB;
    float theta = TanInv(trigX,trigY);

    glRotated(90,0,1,0);
    glRotated(theta,0,-1,0);

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
    //  Undo transofrmations
    glPopMatrix();
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

    //glRotated(zhX_W,0,-1,0);
    glRotated(90,-1,0,0);
    glRotated(zhX_W,0,0,-1);
    glRotated(180,0,0,1);
    drawXWing();
    //  Undo transofrmations
    glPopMatrix();
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
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

      glBindTexture(GL_TEXTURE_2D,texPitch);
      //start drawling of the the field
      glColor3f(1.0f,1.0f,1.0f);
      glBegin(GL_QUADS);        // start drawing the cube.
     
      // Front Face
      glNormal3f( 0, 0, 1); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad

      // Top Face
      glNormal3f(0, 1, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
     
      // Right face
      glNormal3f(1, 0, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
      glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad

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
  //--------------------------------Other-----------------------------
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
    // Pyramid used for trees and hut roof
    void drawPyramid(){
      glBegin( GL_TRIANGLES );
      glNormal3f(0,2,0);
      glColor3ub(34,139,34); glVertex3f( 0.0f, 1.f, 0.0f );
      glColor3ub(34,139,34); glVertex3f( -1.0f, -1.0f, 1.0f );
      glColor3ub(34,139,34); glVertex3f( 1.0f, -1.0f, 1.0f);

      glNormal3f(-2,-1,0);
      glColor3ub(34,139,34); glVertex3f( 0.0f, 1.0f, 0.0f);
      glColor3ub(34,139,34); glVertex3f( -1.0f, -1.0f, 1.0f);
      glColor3ub(34,139,34); glVertex3f( 0.0f, -1.0f, -1.0f);

      glNormal3f(4,-1,2);
      glColor3ub(34,139,34); glVertex3f( 0.0f, 1.0f, 0.0f);
      glColor3ub(34,139,34); glVertex3f( 0.0f, -1.0f, -1.0f);
      glColor3ub(34,139,34); glVertex3f( 1.0f, -1.0f, 1.0f);

      //base
      glColor3ub(34,139,34); glVertex3f( -1.0f, -1.0f, 1.0f);
      glColor3ub(34,139,34); glVertex3f( 0.0f, -1.0f, -1.0f);
      glColor3ub(34,139,34); glVertex3f( 1.0f, -1.0f, 1.0f);

      glEnd();
    }
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
    static void torus(int numc, int numt) {
      glPushMatrix();
      int i, j, k;
      double s, t, x, y, z, twopi;
      glBindTexture(GL_TEXTURE_2D,texPlain);
      glColor3ub(255, 215, 0);
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
    void drawGoalPost(){
      glPushMatrix();
      glRotated(90,0,1,0);

      torus(8,25);
      drawTower(0,-1,0, 0.2,12,0.2, 0,'y','y');
      glPopMatrix();
    }
    void drawTree(){
      glPushMatrix();
      glBindTexture(GL_TEXTURE_2D,texPlain);
      drawTower(0,0,0, 3,8,3, 0,'n','n');
      glTranslated(0,2,-0.5);
      glScaled(2.2,2.2,2.2);
      drawPyramid();
      glTranslated(0,.7,0);
      //glScaled(2.2,2.2,2.2);
      drawPyramid();
      glPopMatrix();
    }
    // This generates a bunch of trees throughout
    void forest() {
      glPushMatrix();
      int i,j;
      double z0 = (zmin+zmax)/2;
      //  Show DEM wire frame
      //glColor3f(1,1,0);
      for (i=0;i<64;i+=16)
         for (j=0;j<64;j+=8) {
            float x=16*i-512;
            float y=16*j-512;
            // trying to place them at various points on the ground
            glTranslated(y,zmag*(z[i+0][j+0]-z0),x);//zmag*(z[i+0][j+0]-z0));
            //glScaled(5,5,5);
            drawTree();
            //glScaled(0.2,0.2,0.2);
         }
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

    //glBindTexture(GL_TEXTURE_2D,texPitch);
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
    texPitch = LoadTexBMP("textures/pitch.bmp");
    texSky[0] = LoadTexBMP("textures/sky0.bmp");
    texSky[1] = LoadTexBMP("textures/sky1.bmp");
    texNight[0] = LoadTexBMP("textures/night0.bmp");
    texNight[1] = LoadTexBMP("textures/night1.bmp");
    texStand = LoadTexBMP("textures/stand.bmp");
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
      Cx = +1*dim*Sin(rot); //Ajust the camera vector based on rot
      Cy = +1*dim; //Ajust the camera vector based on rot
      Cz = -1*dim*Cos(rot);
      gluLookAt(Ex,Ey,Ez, Cx+Ex,Ey,Cz+Ez, 0,1,0); // y is the up-axis
    }
    //  Perspective - set eye position
    else {
      //  Perspective - set eye position
      double px = -0.1*dim*Sin(th)*Cos(ph)+shiftx;
      double py = +0.1*dim        *Sin(ph)+0; // removed shifty
      double pz = +0.1*dim*Cos(th)*Cos(ph)+shiftz;
      gluLookAt(px,py,pz, shiftx,0,shiftz, 0,Cos(ph),0); //removed shifty
    }

    glShadeModel(GL_SMOOTH);
    //  Translate intensity to color vectors. Change the coefficients for different colors.
    double coef1,coef2,coef3;
    if (timeOday) {
      coef1 = 0.002;
      coef2 = 0.002;
      coef3 = 0.008; 
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
      //float ballPos[]  = {distance*Cos(zh)+shiftx,distance*Sin(qh)+shifty,distance*Sin(zh)+shiftz,1};
      float ballPos[]  = {-7e4,5e4,1e5,1};
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

    // Everything was oriented weird
    glRotated(90,-1,0,0);

    // The fighters
      float TIEPos[]  = {distance*Cos(zhTIE)-7e3,distance*Sin(qh)-500,distance*Sin(zhTIE)+4000,1};
      glPushMatrix();
      glRotated(90,1,0,0);
      moveTIE(TIEPos[0],TIEPos[1],TIEPos[2],10);
      glPopMatrix();

      glPushMatrix();
      float X_WPos[]  = {distance*Cos(zhX_W)-8e3,distance*Sin(qh)-500,distance*Sin(zhX_W)+4000,1};
      glRotated(90,1,0,0);
      moveX_W(X_WPos[0],X_WPos[1],X_WPos[2],10);
      glPopMatrix();

    // The ground
      glPushMatrix();
      glScaled(50,50,50);
      DEM();
      glRotated(90,1,0,0);
      // Place forest
      forest();
      glPopMatrix();

    // Eric's field
      // Draw the major elements
      glPushMatrix();
      glScaled(100,100,100);
      glRotated(90,1,0,0);
      glTranslated(-75,0,40);
      drawFieldLine(2,1,1);
      goalzone1(1,1,1);
      goalzone2(1,1,1);
      halffield(1,1,1);
      centercircle(2,2,2);
      hut(0,0,0,1,1,1,0);
      glTranslated(0,-9,0);
      drawField(0,0,0,1,1,1,0);
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
      drawGoalPost();
      glTranslated(0,0,-2);
      drawGoalPost();
      glTranslated(0,0,-2);
      drawGoalPost();
      glTranslated(-30,0,0);
      drawGoalPost();
      glTranslated(0,0,2);
      drawGoalPost();
      glTranslated(0,0,2);
      drawGoalPost();

      // A few sample trees
      glTranslated(-12,0,10);
      drawTree();

      glTranslated(-2,-1,-10);
      glScaled(.5,.5,.5);
      drawTree();

      glTranslated(-1,-2.5,3);
      glScaled(.25,.25,.25);
      drawTree();

      glTranslated(-1,-1,3);
      glScaled(.5,.5,.5);
      drawTree();

      //Place walls between each stand
      glScaled(24,24,24);
      glTranslated(-4.5,3,-3);
          //1-2
      glPushMatrix();
      glRotated(5,0,1,0);
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawWall(26,-2,8.5,2,1,.5,0,'r'); 
      glPopMatrix();
    //2-3
      glPushMatrix();
      glRotated(-8,0,1,0);
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawWall(22,-2,3.25,2,1,.5,0,'y');  
      glPopMatrix();
    //3-4
      glPushMatrix();
      glRotated(-15,0,1,0);
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawWall(17,-2,.75,2,1,.5,0,'b'); 
      glPopMatrix();
    //4-5
      glPushMatrix();
      glRotated(-50,0,1,0);
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawWall(9.5,-2,-8,2,1,.5,0,'y'); 
      glPopMatrix();
    //5-6
      glPushMatrix();
      glRotated(50,0,1,0);
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawWall(9.75,-2,8.25,2.5,1,.5,0,'r');  
      glPopMatrix();
    //6-7
      glPushMatrix();
      glRotated(20,0,1,0);
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawWall(17,-2,1.5,2.5,1,.5,0,'g'); 
      glPopMatrix();
    //7-8
      glPushMatrix();
      glRotated(5,0,1,0);
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawWall(21.5,-2,-3.5,2.5,1,.5,0,'b');  
      glPopMatrix();  
    //8-9
      glPushMatrix();
      glRotated(-6,0,1,0);
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawWall(26,-2,-8,2.5,1,.5,0,'y');  
      glPopMatrix();
    //9-10
      glPushMatrix();
      glRotated(-15,0,1,0);
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawWall(30,-2,-12.25,2.5,1,.5,0,'r');  
      glPopMatrix();
    //10-11
      glPushMatrix();
      glRotated(-50,0,1,0);
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawWall(22,-2,-28.5,2.5,1,.5,0,'g'); 
      glPopMatrix();
    //11-12
      glPushMatrix();
      glRotated(50,0,1,0);
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawWall(21,-2,29.5,2.5,1,.5,0,'b');  
      glPopMatrix();
    //12-1
      glPushMatrix();
      glRotated(15,0,1,0);
      glBindTexture(GL_TEXTURE_2D,texStand);
      drawWall(29,-2,13.5,2.5,1,.5,0,'g');  
      glPopMatrix();

      glPopMatrix();

    // Draw Sky
      glPushMatrix();
      glRotated(90,1,0,0);
      glScaled(2,2,2);
      Sky(0.8*dim);
      glPopMatrix();

    // Hogwarts Express
      glPushMatrix();
      glRotated(90,1,0,0);
      //glRotated(90,0,1,0);
      // Train position
      float posTrain[]  = {elipA*distTrain*Cos(zhTrain)+trainX,distTrain*Sin(qh)+trainY,elipB*distTrain*Sin(zhTrain)+trainZ};
      // Draw train
      //glRotated(zhTrain,1,0,0);
      glTranslated(0,-70,0);
      drawTrainMove(posTrain[0]-500,posTrain[1]-2280,posTrain[2],50);
      glPopMatrix();

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
        glWindowPos2i(5,80);
        Print("W = Forwards       R = Up");
        glWindowPos2i(5,55);
        Print("S  = Backwards    V = Down");
        glWindowPos2i(5,30);
        Print("A  = Look left        Q = Strafe left");
        glWindowPos2i(5,5);
        Print("D  = Look right      E = Strafe right");
      }
      else {
        // Five pixels from the lower left corner of the window
        glWindowPos2i(5,5);
        Print("Angle = %d,%d   Dim = %.1f   FOV = %d   First Person = Off",th,ph,dim,fov);
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
    zh = fmod(90*t,360.0);
    zhTrain = fmod(15*t,360.0);
    zhTIE   = fmod(90*t,360.0);
    zhX_W   = fmod(90*t + 180,360.0);
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
      //  PageUp key - increase dim
      else if (key == GLUT_KEY_PAGE_UP && dim>1)
         dim -= 5000;
      //  PageDown key - decrease dim
      else if (key == GLUT_KEY_PAGE_DOWN)
         dim += 5000;
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
    else if (ch == 't' || ch == 'T'){
       timeOday = 1-timeOday;
    }

    if (fp) {
      double dt = 0.001;
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
      else if (ch == 'r' || ch == 'R'){
         Ey += Cy*dt;
      }
      else if (ch == 'v' || ch == 'V'){
         Ey -= Cy*dt;
      }
      else if (ch == 'q' || ch == 'Q'){
         Ex += Cz*dt;
         Ez += Cx*dt;
      }
      else if (ch == 'e' || ch == 'E'){
         Ex -= Cz*dt; //Update the eye vector based on the camera vector
         Ez -= Cx*dt;
      }
      //  Keep angles to +/-360 degrees
      rot %= 360;
    }
    else {
      //  Reset view angle
      if (ch == '0')
         th = ph = 0;
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
    glutCreateWindow("Liam Kolber & Eric Speaker: Final Project");
    //  Tell GLUT to call "display" when the scene should be drawn
    glutDisplayFunc(display);
    //  Tell GLUT to call "reshape" when the window is resized
    glutReshapeFunc(reshape);
    //  Tell GLUT to call "special" when an arrow key is pressed
    glutSpecialFunc(special);
    //  Tell GLUT to call "key" when a key is pressed
    glutKeyboardFunc(key);
    //  Load DEM  <-- This process is borrowed from ex17.c
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