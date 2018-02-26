
/*
*/

#include "CSCIx229.h"

//define some stuff
#define PI 3.1416

int axes = 0; //display axes
int mode = 0; //projection mode
int az = 35; //azimuth of view angle
int el = 0; //elevation of view angle
int fov = 55;
double asp = 1;  
double dim = 17.0;
double zoom = .4; //zoom of the world

int fp = 0; //first person thing
int rot = 0.0; 
//coordinates for first person
double fpx = 0;
double fpy = 0;
double fpz = 5;
//centering fp view -- these change the angle that you view at 
double cx = 0;
double cy = 0;
double cz = 0;

int movement = 1;

//numeric values used for lighting
int move      =   1;  //move light
int light     =   1; //lighting
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
float ylight  =   0;  // Elevation of light

unsigned int texture[10]; //texture names

/*
//void Print and #define LEN (max length of text string)
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN]; //set a buffer that is the max length of a text string
   char*   ch=buf;
   va_list args;
   // Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //Display characters at the current "raster" position (pixel and bitmap write operations)
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}*/


/*
 *  Draw vertex in polar coordinates with normal
 *  Adapted from ex13
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}



/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 *  Adapted from ex13
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y+10,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
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




//projection & mnipulation of projection matrix
static void myProject()
{ 
   glMatrixMode(GL_PROJECTION); //tell opengl that maniputlation of projection matrix will occur
   glLoadIdentity(); //undo previous transformations
   //if statement for perspective transformation and else is what was there before
   if(fp) {
	gluPerspective(fov,asp,dim/4,4*dim);
   }
   else{
	if (mode)
      		gluPerspective(fov,asp,dim/4,4*dim);
   //else statement for orthogonal transformations
   	else
      		glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   }   
   glMatrixMode(GL_MODELVIEW);    //'switch' to manipulation/manipulation the 'model' matrix -- we want to manipulate the model matrix not the projection matrix?
   glLoadIdentity();    //  Undo previous transformations
}

//---------------------------------------------------------------------------------
//------------------------------------GOAL POST------------------------------------
//---------------------------------------------------------------------------------
void drawCylinder(double x,double y,double z, double dx,double dy,double dz, double th, char lcol, char scol) {
	double radius = 0.3, height = 0.2;
	//Set specular color to white
	float white[] = {1,1,1,1};
	float black[] = {0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
	glPushMatrix();
	//transform cylinder
	glTranslated(x,y,z);
	glRotated(th,0,0,1);
	glRotated(180,0,1,0);
	glScaled(dx,dy,dz);
	//enable textures
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	//set colors
	if (scol == 'r'){
		glColor3f(1.0, 0.0, 0.0); //Set color to red
	} else if (scol == 'g') {
		glColor3f(0.0, 1.0, 0.0); //Set color to green
	} else if (scol == 'b') {
		glColor3f(0.0, 0.0, 1.0); //Set color to blue
	} else if (scol == 'w') {
		glColor3f(1.0, 1.0, 1.0); //Set color to white
	} else if (scol == ':') {
		glColor3ub(189,164,4); //Set color to gold
	} else if (lcol == '[') {
		glColor3ub(139,69,19); //Set color to brown
	} else {
		glColor3f(0.8, 0.8, 0.8); //Default color to gray
	}
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	double j;
	glBegin(GL_QUAD_STRIP);
	for (j = 0; j <= 360; j+=.125) {
		const float tc = (j / (float) 360);
		double x = radius * Cos(j);
		double y = height;
		double z = radius * Sin(j);
		glNormal3d(Cos(j), 0, Sin(j));
		glTexCoord2f(-tc, 0.0); glVertex3d(x, -y, z);
		glTexCoord2f(-tc, 1.0); glVertex3d(x, y, z);
	}
	glEnd();
	//Set colors
	if (lcol == 'r'){
		glColor3f(1.0, 0.0, 0.0); //Set color to red
	} else if (lcol == 'g') {
		glColor3f(0.0, 1.0, 0.0); //Set color to green
	} else if (lcol == 'b') {
		glColor3f(0.0, 0.0, 1.0); //Set color to blue
	} else if (lcol == 'w') {
		glColor3f(1.0, 1.0, 1.0); //Set color to white
	} else if (scol == ':') {
		glColor3ub(189,164,4); //Set color to gold
	} else if (lcol == '[') {
		glColor3ub(139,69,19); //Set color to yellow
	} else {
		glColor3f(0.8, 0.8, 0.8); //Default color to gray
	}
	double i; 
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glNormal3d(0,1,0);
	/* Top of Cylinder */
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5,0.5); 
	glVertex3d(0.0, height, 0.0);
	for(i = 0.0; i <= 360; i+=10) {
		glTexCoord2f(-0.5*Cos(i)+0.5, 0.5*Sin(i)+0.5);
		glVertex3d(radius * Cos(i), height, radius * Sin(i));
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glNormal3d(0,-1,0);

	/* Bottom of Cylinder */
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5,0.5); 
	glVertex3d(0.0, -height, 0.0);
	for(i = 0.0; i <= 360; i+=10) {
		glTexCoord2f(0.5*Cos(i)+0.5, 0.5*Sin(i)+0.5);
		glVertex3d(radius * Cos(i), -height, radius * Sin(i));
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D); 
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//------------------------------------STANDS---------------------------------------
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------


void drawCylinder1(double x,double y,double z, double dx,double dy,double dz, double th,char lcol, char scol) {
	double radius = 0.3, height = 0.2;
	//Set specular color to white
	float white[] = {1,1,1,1};
	float black[] = {0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
	glPushMatrix();
	//transform cylinder
	glTranslated(x,y,z);
	glRotated(th,0,0,1);
	glRotated(th,0,0,1);
	glScaled(dx,dy*2,dz);
	//enable textures
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	if (lcol == 'r'){
		glColor3f(1.0, 0.0, 0.0); //Set color to red
	} else if (lcol == 'g') {
		glColor3f(0.0, 1.0, 0.0); //Set color to green
	} else if (lcol == 'b') {
		glColor3f(0.0, 0.0, 1.0); //Set color to blue
	} else if (lcol == 'w') {
		glColor3f(1.0, 1.0, 1.0); //Set color to white
	} else if (lcol == 'y') {
		glColor3f(1.0, 1.0, 0); //Set color to yellow
	} else if (lcol == 'n') {
		glColor3ub(139,69,19); //Set color to brown
	} else {
		glColor3f(0.8, 0.8, 0.8); //Default color to gray
	}	

	double j;
	glBegin(GL_QUAD_STRIP);
	for (j = 0; j <= 360; j+=.125) {
		const float tc = (j / (float) 360);
		double x = radius * Cos(j);
		double y = height;
		double z = radius * Sin(j);
		glNormal3d(Cos(j), 0, Sin(j));
		glTexCoord2f(-tc, 0.0); glVertex3d(x, -y, z);
		glTexCoord2f(-tc, 1.0); glVertex3d(x, y, z);
	}
	glEnd();
	//Set colors
	if (lcol == 'r'){
		glColor3f(1.0, 0.0, 0.0); //Set color to red
	} else if (lcol == 'g') {
		glColor3f(0.0, 1.0, 0.0); //Set color to green
	} else if (lcol == 'b') {
		glColor3f(0.0, 0.0, 1.0); //Set color to blue
	} else if (lcol == 'w') {
		glColor3f(1.0, 1.0, 1.0); //Set color to white
	} else if (lcol == 'y') {
		glColor3f(1.0, 1.0, 0); //Set color to yellow
	} else if (lcol == 'n') {
		glColor3ub(139,69,19); //Set color to brown
	} else {
		glColor3f(0.8, 0.8, 0.8); //Default color to gray
	}
	double i; 
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glNormal3d(0,1,0);
	/* Top of Cylinder */
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5,0.5); 
	glVertex3d(0.0, height, 0.0);
	for(i = 0.0; i <= 360; i+=10) {
		glTexCoord2f(-0.5*Cos(i)+0.5, 0.5*Sin(i)+0.5);
		glVertex3d(radius * Cos(i), height, radius * Sin(i));
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glNormal3d(0,-1,0);

	/* Bottom of Cylinder */
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5,0.5); 
	glVertex3d(0.0, -height, 0.0);
	for(i = 0.0; i <= 360; i+=10) {
		glTexCoord2f(0.5*Cos(i)+0.5, 0.5*Sin(i)+0.5);
		glVertex3d(radius * Cos(i), -height, radius * Sin(i));
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D); 
}


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//-------------------------------Trees---------------------------------------------
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

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


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------




void drawStandGrf(){
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawCylinder1(0,0,0,4,10,4,0,'r','r');
}
void drawStandRvn(){
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawCylinder1(0,0,0,4,10,4,0,'b','b');
}
void drawStandSly(){
	glBindTexture(GL_TEXTURE_2D,texture[0]);	
	drawCylinder1(0,0,0,4,10,4,0,'g','g');
}
void drawStandHuf(){
	glBindTexture(GL_TEXTURE_2D,texture[0]);	
	drawCylinder1(0,0,0,4,10,4,0,'y','y');	
}
void drawGoalPost(){
	drawCylinder(0,0,0,2,12,2,0,':',':');
}

void drawTree(){
	drawCylinder1(0,0,0,3,8,3,0,'n','n');
	glTranslated(0,2,-0.5);
	glScaled(2.2,2.2,2.2);
	drawPyramid();
	glTranslated(0,.7,0);
	//glScaled(2.2,2.2,2.2);
	drawPyramid();
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------


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
	glScaled(dx*40, dy, dz*24);
  //start drawling of the the field
  	glBegin(GL_QUADS);
  // Front
  	glColor3ub(32,162,3);
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

//---------------------------------------------------------------------------------
//------------------------------------FIELD LINES----------------------------------
//---------------------------------------------------------------------------------
void drawFieldLine(double xSize, double ySize, double zSize){
	//with the rotation for this object and viewing from botton:
		//x -- left/right
		//y -- up/down
		//z -- in/out of frame (+ into frame / - out of frame)
	//since the field line is a circle this function is a replication of drawling
	//a circle using a loop where it draws points based on the position and that
	//position is changed based on the loop
	glPushMatrix();
	//this is the rotate function that rotates the 'matrix' of the object so the
	//designer can reproduce the object rotated around an axis
	//the 1st parameter is the angle of rotation and the 2nd,3rd,4th represent
	//the x,y,z axis and a '1' in any of those parameters is what axis the object
	//is being rotated around
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
	glColor3ub(144,144,144); //this is a different way to 'color' an object and uses the html color valules to do so
	glVertex3f(-1,-1,1);
  	glVertex3f(1,-1,1);
  	glVertex3f(1,1,1);
  	glVertex3f(-1,1,1);
	//back
	//glColor3ub(232,45,7);
	glVertex3f(+1,-1,-1);
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,+1,-1);
	glVertex3f(+1,+1,-1);
	// Right
	glVertex3f(+1,-1,1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,+1,-1);
	glVertex3f(+1,+1,1);
	  // Left
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,-1,+1);
	glVertex3f(-1,+1,+1);
	glVertex3f(-1,+1,-1);
	//top
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
	glColor3ub(144,144,144); //this is a different way to 'color' an object and uses the html color valules to do so
	glVertex3f(-1,-1,1);
  	glVertex3f(1,-1,1);
  	glVertex3f(1,1,1);
  	glVertex3f(-1,1,1);
	//back
	//glColor3ub(232,45,7);
	glVertex3f(+1,-1,-1);
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,+1,-1);
	glVertex3f(+1,+1,-1);
	// Right
	glVertex3f(+1,-1,1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,+1,-1);
	glVertex3f(+1,+1,1);
	  // Left
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,-1,+1);
	glVertex3f(-1,+1,+1);
	glVertex3f(-1,+1,-1);
	//top
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
	glColor3ub(144,144,144); //this is a different way to 'color' an object and uses the html color valules to do so
	glVertex3f(-1,-1,1);
  	glVertex3f(1,-1,1);
  	glVertex3f(1,1,1);
  	glVertex3f(-1,1,1);
	//back
	//glColor3ub(232,45,7);
	glVertex3f(+1,-1,-1);
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,+1,-1);
	glVertex3f(+1,+1,-1);
	// Right
	glVertex3f(+1,-1,1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,+1,-1);
	glVertex3f(+1,+1,1);
	  // Left
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,-1,+1);
	glVertex3f(-1,+1,+1);
	glVertex3f(-1,+1,-1);
	//top
	glVertex3f(-1,-1,+1);
	glVertex3f(+1,+1,+1);
	glVertex3f(+1,+1,-1);
	glVertex3f(-1,+1,-1);
	//bottom -- not needed
	glEnd();
	glPopMatrix();
}
void object5 (double xSize, double ySize, double zSize){
     //replication of drawpost1 function but with a change in some of the following: scale,position,color and angle
	glPushMatrix();
 	glTranslated(xSize-1, ySize+5, zSize-2); 

	glScaled(xSize, ySize, zSize);
	glBegin(GL_QUADS);
	//front
	glColor3ub(144,144,144); //this is a different way to 'color' an object and uses the html color valules to do so
	glVertex3f(-1,-1,1);
  	glVertex3f(1,-1,1);
  	glVertex3f(1,1,1);
  	glVertex3f(-1,1,1);
	//back
	//glColor3ub(232,45,7);
	glVertex3f(+1,-1,-1);
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,+1,-1);
	glVertex3f(+1,+1,-1);
	// Right
	glVertex3f(+1,-1,1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,+1,-1);
	glVertex3f(+1,+1,1);
	  // Left
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,-1,+1);
	glVertex3f(-1,+1,+1);
	glVertex3f(-1,+1,-1);
	//top
	glVertex3f(-1,-1,+1);
	glVertex3f(+1,+1,+1);
	glVertex3f(+1,+1,-1);
	glVertex3f(-1,+1,-1);
	//bottom -- not needed
	glEnd();
	glPopMatrix();
}


void display(){
	const double len = 2;//length of the axis of 'image window'

	//erase/remove previous drawling window and add depth butter
	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);
	//  Undo previous transformations
	glLoadIdentity();

	//if statement for first person pov display 
   	if(fp){
		cx = +2*dim*Sin(rot); //Ajust the camera vector based on rot
		cz = -2*dim*Cos(rot);
	//this changes the position from which you view fp
		gluLookAt(fpx+7,fpy+3,fpz-3, cx+fpx,cy+fpy,cz+fpz, 0,1,0); //  Use gluLookAt, y is the up-axis
	}
   	else{
   		if (mode)
		{	
	   		double fvx = (-2*dim*Sin(az)*Cos(el));
	   		double fvy = (+2*dim        *Sin(el));
	   		double fvz = (+2*dim*Cos(az)*Cos(el));
	   		gluLookAt(fvx,fvy,fvz,0,0,0,0,Cos(el),0);
		}
	//orthogonal - set world orientation
		else
		{
	   		glRotatef(az,1,0,0);
	   		glRotatef(el,0,1,0);
		}
   	}

   glShadeModel(GL_SMOOTH);
   //  Translate intensity to color vectors
   float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
   //  Light position
   float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
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


   glScaled(zoom,zoom,zoom);
//---------------------------------------------
   //drawshape
	drawField(0,0,0,1,1,1,0);
	//drawGPost1(0,0,0,1,1,1,0);
	//drawGPost2(0,0,0,1,1,1,0);
	//drawGPost3(0,0,0,1,1,1,0);
	//drawGPost4(0,0,0,1,1,1,0);
	//drawGPost5(0,0,0,1,1,1,0);
	//drawGPost6(0,0,0,1,1,1,0);
	//stand1(0,0,0,1,1,1,0);
	//stand2(0,0,0,1,1,1,0);
	//stand3(0,0,0,1,1,1,0);
	//stand4(0,0,0,1,1,1,0);
	//stand5(0,0,0,1,1,1,0);
	//stand6(0,0,0,1,1,1,0);
	//stand7(0,0,0,1,1,1,0);
	//stand8(0,0,0,1,1,1,0);
	//stand9(0,0,0,1,1,1,0);
	//stand10(0,0,0,1,1,1,0);
	//stand11(0,0,0,1,1,1,0);
	//stand12(0,0,0,1,1,1,0);
	drawFieldLine(2,1,1);
	goalzone1(1,1,1);
	goalzone2(1,1,1);
	halffield(1,1,1);
	centercircle(2,2,2);
	hut(0,0,0,1,1,1,0);
	//object1(1,1,1);
	//object2(1,1,1);
	//object3(1,1,1);
	//object4(1,1,1);
	//object5(1,1,1);

/*
	glTranslated(-5,0,0);
	drawStandGrf();

	glTranslated(10,0,0);
	drawStandSly();

	glTranslated(5,0,0);
	drawStandRvn();

	glTranslated(-10,0,0);
	drawStandHuf();
*/
//1
	glTranslated(0,-7,9);
	//glScaled(0,2,0);
	drawStandGrf();
//2
	glTranslated(0,5,0);
	glTranslated(-7.5,-5,-1);
	drawStandHuf();
//3

	glTranslated(0,5,0);
	glTranslated(-7.5,-5,-2);
	drawStandRvn();
//4
	glTranslated(0,5,0);
	glTranslated(-5,-5,-6);
	drawStandHuf();
//5
	glTranslated(0,5,0);
	glTranslated(5,-5,-6);
	drawStandGrf();
//6
	glTranslated(0,5,0);
	glTranslated(7.5,-5,-2);
	drawStandSly();
//7
	glTranslated(0,5,0);
	glTranslated(7.5,-5,-1);
	drawStandRvn();
//8
	glTranslated(0,5,0);
	glTranslated(7.5,-5,1);
	drawStandHuf();
//9
	glTranslated(0,5,0);
	glTranslated(7.5,-5,2);
	drawStandGrf();
//10
	glTranslated(0,5,0);
	glTranslated(5,-5,6);
	drawStandSly();
//11

	glTranslated(0,5,0);
	glTranslated(-5,-5,6);
	drawStandRvn();
//12
	glTranslated(0,5,0);
	glTranslated(-7.5,-5,2);
	drawStandSly();
//goal post
	glTranslated(7.5,-2,-6);
	drawGoalPost();

	glTranslated(0,5,0);
	glTranslated(0,-5,-2);
	drawGoalPost();

	glTranslated(0,5,0);
	glTranslated(0,-5,-2);
	drawGoalPost();

	glTranslated(0,5,0);
	glTranslated(-30,-5,0);
	drawGoalPost();

	glTranslated(0,5,0);
	glTranslated(0,-5,2);
	drawGoalPost();

	glTranslated(0,5,0);
	glTranslated(0,-5,2);
	drawGoalPost();

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




//1-2
	glPushMatrix();
	glRotated(5,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawWall(26,-2,8.5,2,1,.5,0,'r');	
	glPopMatrix();
//2-3
	glPushMatrix();
	glRotated(-8,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawWall(22,-2,3.25,2,1,.5,0,'y');	
	glPopMatrix();
//3-4
	glPushMatrix();
	glRotated(-15,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawWall(17,-2,.75,2,1,.5,0,'b');	
	glPopMatrix();
//4-5
	glPushMatrix();
	glRotated(-50,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawWall(9.5,-2,-8,2,1,.5,0,'y');	
	glPopMatrix();
//5-6
	glPushMatrix();
	glRotated(50,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawWall(9.75,-2,8.25,2.5,1,.5,0,'r');	
	glPopMatrix();
//6-7
	glPushMatrix();
	glRotated(20,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawWall(17,-2,1.5,2.5,1,.5,0,'g');	
	glPopMatrix();
//7-8
	glPushMatrix();
	glRotated(5,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawWall(21.5,-2,-3.5,2.5,1,.5,0,'b');	
	glPopMatrix();	
//8-9
	glPushMatrix();
	glRotated(-6,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawWall(26,-2,-8,2.5,1,.5,0,'y');	
	glPopMatrix();
//9-10
	glPushMatrix();
	glRotated(-15,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawWall(30,-2,-12.25,2.5,1,.5,0,'r');	
	glPopMatrix();
//10-11
	glPushMatrix();
	glRotated(-50,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawWall(22,-2,-28.5,2.5,1,.5,0,'g');	
	glPopMatrix();
//11-12
	glPushMatrix();
	glRotated(50,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawWall(21,-2,29.5,2.5,1,.5,0,'b');	
	glPopMatrix();
//12-1
	glPushMatrix();
	glRotated(15,0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	drawWall(29,-2,13.5,2.5,1,.5,0,'g');	
	glPopMatrix();
	


//--------------------------------------------	

//  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
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

	//display parameters
	glWindowPos2i(5,5);
   	Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",az,el,dim,fov,mode?"Perpective":"Orthogonal");
   	glPopMatrix();
	
	//render scene
	ErrCheck("display");
	glFlush();
	glutSwapBuffers();


}

/*
static void drawScene(){
	const double len = 2; //length of the axis of 'image window'

	//erase/remove previous drawling window and add depth butter
	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);
	//  Undo previous transformations
	glLoadIdentity();

	//if statement for first person pov display 
   	if(fp){
		cx = +2*dim*Sin(rot); //Ajust the camera vector based on rot
		cz = -2*dim*Cos(rot);
	//this changes the position from which you view fp
		gluLookAt(fpx+7,fpy+3,fpz-3, cx+fpx,cy+fpy,cz+fpz, 0,1,0); //  Use gluLookAt, y is the up-axis
	}
   	else{
   		if (mode)
		{	
	   		double fvx = (-2*dim*Sin(az)*Cos(el));
	   		double fvy = (+2*dim        *Sin(el));
	   		double fvz = (+2*dim*Cos(az)*Cos(el));
	   		gluLookAt(fvx,fvy,fvz,0,0,0,0,Cos(el),0);
		}
	//orthogonal - set world orientation
		else
		{
	   		glRotatef(az,1,0,0);
	   		glRotatef(el,0,1,0);
		}
   	}

	//lighting stuff
	glShadeModel(GL_SMOOTH);
   	//  Translate intensity to color vectors
   	float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
   	float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   	float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
   	//  Light position
   	float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
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

	//draw the shapes
	drawField(0,0,0,1,1,1,0);

	//display parameters
	glWindowPos2i(5,5);
   	Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",az,el,dim,fov,mode?"Perpective":"Orthogonal");
   	glPopMatrix();
	
	//render scene
	ErrCheck("display");
	glFlush();
	glutSwapBuffers();

}
*/

/*

 *  GLUT calls this routine when the window is resized

 *  Adopted from ex13

 */

void idle()

{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}


//glut stuff for key bindings and what happens when arrow key is pressed
//taken from ex6.c
//another key binding function 
//this funciton was taken from and build on ex6.cabs
//the variables being binded are the global variables created above
//these key bindings alos will affect the view point of the user
//arrow keys were chosen because they make the most sense for how a user would change thier viewpoint
//depending on which key pressed the graph/image may rotate and/or move a certain direction while maintaining the functions data
void special(int key,int x,int y)
{
   if(!fp){
	  //KEY BINDED:Right arrow key -> increase azimuth by 5 degrees
	   if (key == GLUT_KEY_RIGHT)
	      az += 5;
	  //KEY BINDED:Left arrow key -> decrease azimuth by 5 degrees
	   else if (key == GLUT_KEY_LEFT)
	      az -= 5;
	  //KEY BINDED:Up arrow key -> increase elevation by 5 degrees
	   else if (key == GLUT_KEY_UP)
	      el += 5;
	  //KEY BINDED:Down arrow key -> decrease elevation by 5 degrees
	   else if (key == GLUT_KEY_DOWN)
	      el -= 5;
	   //KEY BINDED:PageUp key -> increase dim
	   else if (key == GLUT_KEY_PAGE_UP)
	      dim += 0.1;
	   //KEY BINDED:PageDown key - decrease dim
	   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
	      dim -= 0.1;
	   //Keep angles to +/-360 degrees
	   az %= 360;
	   el %= 360;
   }
   //update projection matrix
   myProject();
   //  Tell GLUT it is necessary to redisplay the scene
  //This uses the same redisplay function explained above. This also works because
		//both functions are calling for window redisplays
   glutPostRedisplay();
}


//same general kev function from previous assignmet (hw2) with various/small changes
void key(unsigned char ch,int x,int y)
{
   //exit on ESC
   if (ch == 27)
      exit(0);
   //reset view angle
   else if (ch == '0')
   {
      az = 0;
      el = 5;
   }
   else if (ch == 'f' || ch == 'F') {
   	fp = 1-fp;
   }
   if(fp){
	double dt = 0.01; //this changes how much each step 'jumps' ...aka...step size
	if (ch == 'w' || ch == 'W') {
	fpx += cx*dt; //Update the eye vector based on the camera vector
	fpz += cz*dt;
        }
    else if (ch == 'a' || ch == 'A'){
	rot -= 3;
    }
    else if (ch == 's' || ch == 'S'){
	fpx -= cx*dt;
	fpz -= cz*dt;
    }
    else if (ch == 'd' || ch == 'D'){
	rot += 3;
    }
    //  Keep angles to +/-360 degrees
    rot %= 360;
   }
   else{
   //switch display mode
	if (ch == 'm' || ch == 'M')
		mode = 1-mode;
   //change field of view angle
	else if (ch == '[' && ch>1)
      		fov--;
	else if (ch == ']' && ch<179)
      		fov++;
   //changes the zoom
	else if ((ch == '-' || ch == '_') && ch>1)
      		zoom *=.9;
   	else if ((ch == '+' || ch == '=') && ch<179)
      		zoom /=.9;
}

   //  Reproject
   myProject();
   //  Tell GLUT it is necessary to redisplay the scene
   //This uses the same redisplay function explained above. This also works because
		//both functions are calling for window redisplays
   glutPostRedisplay();
	//this function marks the normal plane of current window as needing to be redisplayed
	//this means that through the next iteration through the glutMainLoop there will be a callback
		//that will end up redisplaying the window's normal plane
	//if this function is called multiple times before the glutmain is called then it will only produce 1 'redisplay'
}


//glut rutine that happens/is used when the user window gets resized
//void reshape function taken/adapted from ex6.c and then changed to fit needs
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   glViewport(0,0, width,height); //  Set the viewport to the entire window
   myProject(); //  Set projection
}

//funciton calls are built upon and taken from what was used in ex6.c
int main(int argc,char* argv[])
{
   glutInit(&argc,argv); //initialize GLUT
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE); //  Request double buffered, true color window with Z buffering at NxM dimensions
   glutInitWindowSize(1000,1000);
   glutCreateWindow(" hw5 - scene -- Eric Speaker");
   //set up the calbacks to other needed functions (display,reshape,etc)
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //this function passes control to GLUT so it can interact with the user
   texture[0]= LoadTexBMP("hp.bmp");
   ErrCheck("init");
   glutMainLoop();
   return 0;
}




