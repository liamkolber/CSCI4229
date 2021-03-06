// Liam Kolber
// CSCI 4229
// CU Boulder Fall 2017
// Assignment 6
//------------------------------------------------------------------
//---------------------------Initializations------------------------
//------------------------------------------------------------------
#include "CSCIx229.h"
//------------------------------------------------------------------
//-------------------------Global Variables-------------------------
//------------------------------------------------------------------
int th        =  60;  // Azimuth of view angle
int ph        =  20;  // Elevation of view angle
int axes      =   0;  // Display axes
int mode      =   0;  // What to display
double asp    =   1;  // Aspect ratio
double dim    = 5.0;  // Size of world
int fov       =  55;  // Field of view (for perspective)
// First-person specific
int fp        =   0;  // Toggle
int rot       =90.0;  // Rotational angle
double Ex     =  -4;  // Eye coords
double Ey     =   0;
double Ez     =   0;
double Cx     =   0;  // Centering coords
double Cz     =   0; 

int move      =   1;

// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
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
   glColor3d(255,255,255);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
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
// Draw cylinder
void drawCylinder(double x,double y,double z, double dx,double dy,double dz, double th, char lcol, char scol, unsigned int texture) {
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
   glDisable(GL_TEXTURE_2D);
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
        gluLookAt(px,py,pz, 1,0,0, 0,Cos(ph),0);
      }
      //  Orthogonal - set world orientation
      else {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
      }
  }

   glShadeModel(GL_SMOOTH);
   //  Translate intensity to color vectors
   float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
   //  Light position
   float Position[]  = {distance*Cos(zh),distance*Sin(qh),distance*Sin(zh),1};
   //  Draw light position as ball (still no lighting here)
   glColor3f(1,1,1);
   ball(Position[0],Position[1],Position[2] , 0.1);
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

   // Train body
   drawCylinder(0,0,0, 2,7,2, 90,'w','q',texEngine);
   drawCylinder(2,0.2,0, 3,4,3, 0,'g','w',texBody);
   drawCylinder(-1,0.7,0, 1,2,1, 0,'g','w',texSmoke);
   drawCylinder(2,1,0, 2,3,2, 0,'g','w',texSmoke);

   // Wheels and axels
   glRotated(90,0,1,0);
   drawCylinder(0,-0.6,-1, 0.2,5.5,0.2, 90,'r','r',texPlain);
   drawCylinder(0,-0.6,2, 0.2,5.5,0.2, 90,'r','r',texPlain);
   drawCylinder(0,-0.6,0.5, 0.2,5.5,0.2, 90,'r','r',texPlain);

   drawCylinder(1,-0.6,-1, 2,0.3,2, 90,'r','r',texWheel);
   drawCylinder(-1,-0.6,-1, 2,0.3,2, 90,'r','r',texWheel);
   drawCylinder(1,-0.6,2, 2,0.3,2, 90,'r','r',texWheel);
   drawCylinder(-1,-0.6,2, 2,0.3,2, 90,'r','r',texWheel);
   drawCylinder(1,-0.6,0.5, 2,0.3,2, 90,'r','r',texWheel);
   drawCylinder(-1,-0.6,0.5, 2,0.3,2, 90,'r','r',texWheel);

   // Base
   drawCylinder(0,-1.3,0, 10,0.5,10, 0,'w','w',texTracks);

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
      Print("Angle = %d,%d   Dim = %.1f   FOV = %d   Projection = %s   First Person = Off",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
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
         dim -= 0.1;
      //  PageDown key - decrease dim
      else if (key == GLUT_KEY_PAGE_DOWN)
         dim += 0.1;
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
   else if (ch == ' ') {
      move = 1-move;
   }
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
   {
      fp = 1-fp;
   }

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
         mode = 1-mode;
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
   glutCreateWindow("Liam Kolber: Assignment 6");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   // Load textures
   texEngine = LoadTexBMP("engine.bmp");
   texSmoke = LoadTexBMP("smokestack.bmp");
   texBody = LoadTexBMP("body.bmp");
   texWheel = LoadTexBMP("wheel.bmp");
   texPlain = LoadTexBMP("plain.bmp");
   texFront = LoadTexBMP("front.bmp");
   texTracks = LoadTexBMP("tracks.bmp");
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
//------------------------------------------------------------------
//--------------------------------End-------------------------------
//------------------------------------------------------------------