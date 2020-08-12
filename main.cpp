#include <windows.h>		// Header File For Windows
#include <mmsystem.h>
#include "glew.h"
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include <chrono>
#include <math.h>
#include "Car.h"
#include "OBB.h"
#include "Image_Loading/nvImage.h"

OBB  CarOBB, enemyOBB, spinningObstacle, obstacle1; //bounding boxes for collision detection for obstacles and car
OBB boundary1, boundary2, boundary3, boundary4, boundary5, boundary6, finishLine; //boundaries that make up the track and collide with car

GLuint carTextureMap = 0;
GLuint enemyTextureMap = 0;

GLuint boundary1Tex = 0;
GLuint boundary2Tex = 0;
GLuint boundary3Tex = 0;
GLuint boundary4Tex = 0;
GLuint boundary5Tex = 0;
GLuint boundary6Tex = 0;
GLuint finishLineTex = 0;

GLuint introTex = 0;

Car player = Car(9, -16, &CarOBB, carTextureMap, false); //initialised at starting position
Car enemy = Car(-9, -16, &enemyOBB, enemyTextureMap, true);

const float PI = 3.14159265358f;
bool isDestroyed = false;
bool reachTarget = false;
bool isDrivingUp;
bool isReversing;

int	mouse_x = 0, mouse_y = 0;
bool LeftPressed = false;
int screenWidth = 480, screenHeight = 480;  //480
bool keys[256];
float spin=0;

int displayFinishedMessage();
int displayDestroyedMessage();

GLuint loadPNG(char* file);

void drawBoundary(float matrix[], float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
					GLuint texture, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float tx4, float ty4);

//OPENGL FUNCTION PROTOTYPES
void display();		
void reshape(int width, int height);
void init();		 
void processKeys();			


/*************    START OF OPENGL FUNCTIONS   ****************/

void display()									
{
	 
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(-player.getXtri(), -player.getYtri(), 0); //how the camera follows the car
	
	//opening message to player
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, introTex);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(-15, -5);
		glTexCoord2f(0.0, 1.0); glVertex2f(-15, 5);
		glTexCoord2f(1.0, 1.0); glVertex2f(15, 5);
		glTexCoord2f(1.0, 0.0); glVertex2f(15, -5);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// --- OBSTACLES ---
	float obstacleMatrix[16];

	glPushMatrix();
		glRotatef(spin*2.0f, 0, 0, 1);
		glTranslatef(8, 0, 0);
		glRotatef(spin, 0, 0, 1);
		glTranslatef(-8, 0, 0);

		glGetFloatv(GL_MODELVIEW_MATRIX, obstacleMatrix);

		glColor3f(0.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
			glVertex2f(-4, 30); //4 -4
			glVertex2f(-4, 38); //4 4
			glVertex2f(4, 38); //12 4
			glVertex2f(4, 30); //12 -4
		glEnd();
	glPopMatrix();

	spinningObstacle.transformPoints(obstacleMatrix);
	//spinningObstacle.drawOBB();
	if (spinningObstacle.isColliding(*player.getOBB())) {
		glColor3f(1.0, 0.0, 0.0); //if collision turn OBB Red
		if (isDrivingUp) {
			player.collisionResponseUp(PI);
			}
		if (isReversing) {
			player.collisionResponseReverse(PI);
			}
	}
	else {
		glColor3f(0.0, 1.0, 1.0);
	}

	//--
	glPushMatrix();
		glTranslatef(8, 0, 0);
		glTranslatef(-8, 0, 0);

		glGetFloatv(GL_MODELVIEW_MATRIX, obstacleMatrix);

		glColor3f(0.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
			glVertex2f(-40, 45);
			glVertex2f(-40, 50);
			glVertex2f(-20, 50);
			glVertex2f(-20, 45);
		glEnd();
	glPopMatrix();

	obstacle1.transformPoints(obstacleMatrix);
	//obstacle1.drawOBB();
	if (obstacle1.isColliding(*player.getOBB())) {
		glColor3f(1.0, 0.0, 0.0);
		if (isDrivingUp) {
			player.collisionResponseUp(PI);
		}
		if (isReversing) {
			player.collisionResponseReverse(PI);
			}
	}
	else {
		glColor3f(0.0, 1.0, 1.0);
	}

	//--- TRACK BOUNDARIES ---

	float boundaryMatrix[16];

	drawBoundary(boundaryMatrix, -40, -20, -40, 40, -20, 40, -20, -20,
		boundary1Tex, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);

	boundary1.transformPoints(boundaryMatrix);
	//boundary1.drawOBB();
	if (boundary1.isColliding(*player.getOBB())) {
		glColor3f(1.0, 0.0, 0.0);
		if (isDrivingUp) {
			player.collisionResponseUp(PI);
		}
		if (isReversing) {
			player.collisionResponseReverse(PI);
		}
	}
	else {
		glColor3f(0.0, 1.0, 1.0);
	}


	//--
	drawBoundary(boundaryMatrix, -80, 20, -80, 40, -40, 40, -40, 20,
		boundary2Tex, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
	boundary2.transformPoints(boundaryMatrix);
	//boundary2.drawOBB();
	if (boundary2.isColliding(*player.getOBB())) {
		glColor3f(1.0, 0.0, 0.0);
		if (isDrivingUp) {
			player.collisionResponseUp(PI);
		}
		if (isReversing) {
			player.collisionResponseReverse(PI);
		}
	}
	else {
		glColor3f(0.0, 1.0, 1.0);
	}

	//--
	drawBoundary(boundaryMatrix, -100, 20, -100, 80, -80, 80, -80, 20,
		boundary3Tex, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
	boundary3.transformPoints(boundaryMatrix);
	//boundary3.drawOBB();
	if (boundary3.isColliding(*player.getOBB())) {
		glColor3f(1.0, 0.0, 0.0);
		if (isDrivingUp) {
			player.collisionResponseUp(PI);
		}
		if (isReversing) {
			player.collisionResponseReverse(PI);
		}
	}
	else {
		glColor3f(0.0, 1.0, 1.0);
	}

	//---
	drawBoundary(boundaryMatrix, -60, 60, -60, 80, 40, 80, 40, 60,
		boundary4Tex, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
	boundary4.transformPoints(boundaryMatrix);
	//boundary4.drawOBB();
	if (boundary4.isColliding(*player.getOBB())) {
		glColor3f(1.0, 0.0, 0.0);
		if (isDrivingUp) {
			player.collisionResponseUp(PI);
		}
		if (isReversing) {
			player.collisionResponseReverse(PI);
		}
	}
	else {
		glColor3f(0.0, 1.0, 1.0);
	}

	//---
	drawBoundary(boundaryMatrix, 20, -20, 20, 60, 40, 60, 40, -20,
		boundary5Tex, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
	boundary5.transformPoints(boundaryMatrix);
	//boundary5.drawOBB();
	if (boundary5.isColliding(*player.getOBB())) {
		glColor3f(1.0, 0.0, 0.0);
		if (isDrivingUp) {
			player.collisionResponseUp(PI);
		}
		if (isReversing) {
			player.collisionResponseReverse(PI);
		}
	}
	else {
		glColor3f(0.0, 1.0, 1.0);
	}

	//---
	drawBoundary(boundaryMatrix, -40, -40, -40, -20, 40, -20, 40, -40,
		boundary6Tex, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
	boundary6.transformPoints(boundaryMatrix);
	//boundary6.drawOBB();
	if (boundary6.isColliding(*player.getOBB())) {
		glColor3f(1.0, 0.0, 0.0);
		if (isDrivingUp) {
			player.collisionResponseUp(PI);
		}
		if (isReversing) {
			player.collisionResponseReverse(PI);
		}
	}
	else {
		glColor3f(0.0, 1.0, 1.0);
	}

	//---
	drawBoundary(boundaryMatrix, -100, 80, -100, 100, -40, 100, -40, 80,
		finishLineTex, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
	finishLine.transformPoints(boundaryMatrix);
	//finishLine.drawOBB();
	if (finishLine.isColliding(*player.getOBB())) {
		glColor3f(1.0, 0.0, 0.0);
		displayFinishedMessage();
	}
	else {
		glColor3f(0.0, 1.0, 1.0);
	}

	//--- Car OBB ---
	float CarOBBMatrix[16];
	player.drawCar(CarOBBMatrix, carTextureMap);

	player.getOBB()->transformPoints(CarOBBMatrix);

	//player.getOBB()->drawOBB();

	//--
	float EnemyOBBMatrix[16];
	enemy.drawCar(EnemyOBBMatrix, enemyTextureMap);

	enemy.getOBB()->transformPoints(EnemyOBBMatrix);

	//enemy.getOBB()->drawOBB();

	if ((*enemy.getOBB()).isColliding(*player.getOBB())) {
		glColor3f(1.0, 0.0, 0.0);
		if (isDrivingUp) {
			player.collisionResponseUp(PI);
		}
		if (isReversing) {
			player.collisionResponseReverse(PI);
		}
	}
	else {
		glColor3f(0.0, 1.0, 1.0);
	}


	glFlush();

	//for spinning obstacle
	spin += 0.01f;
	if (spin > 360)
		spin = 0;

	player.updatePosition(PI);

	enemy.updatePosition(PI);
}

GLuint loadPNG(char* file) {
	nv::Image img;

	GLuint textureID;

	// Return true on success
	if (img.loadImageFromFile(file))
	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}

	else {
		MessageBox(NULL, "Failed to load texture", "", MB_OK | MB_ICONINFORMATION);
	}
	return textureID;
}

//method that passes in all coordinates and draws the boundary polygons which will have an OBB around them. 
void drawBoundary(float matrix[], float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
		GLuint texture, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float tx4, float ty4)
{
	glPushMatrix();
		glTranslatef(8, 0, 0);
		glTranslatef(-8, 0, 0);

		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
			glTexCoord2f(tx1, ty1); glVertex2f(x1, y1);
			glTexCoord2f(tx2, ty2); glVertex2f(x2, y2);
			glTexCoord2f(tx3, ty3); glVertex2f(x3, y3);
			glTexCoord2f(tx4, ty4); glVertex2f(x4, y4);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth=width; screenHeight = height;           //ensures mouse coordinates
					
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									

	gluOrtho2D(-20, 20,-20, 20);            // set the coordinate system for the window
	glMatrixMode(GL_MODELVIEW);			
	glLoadIdentity();					
}
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0); //background colour
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	carTextureMap = loadPNG("car.png");
	enemyTextureMap = loadPNG("enemy.png");

	boundary1Tex = loadPNG("boundary1Img.png");
	boundary2Tex = loadPNG("boundary2Img.png");
	boundary3Tex = loadPNG("boundary3Img.png");
	boundary4Tex = loadPNG("boundary4Img.png");
	boundary5Tex = loadPNG("boundary5Img.png");
	boundary6Tex = loadPNG("boundary6Img.png");
	finishLineTex = loadPNG("finishLineImg.png");

	introTex = loadPNG("intro.png");
	
	PlaySound("Hyper_Drive.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP); //background music for the game

	player.getOBB()->vertOriginal[0].x = -2.5; 
	player.getOBB()->vertOriginal[0].y = -2.5;

	player.getOBB()->vertOriginal[1].x = -2.5;
	player.getOBB()->vertOriginal[1].y = 5;

	player.getOBB()->vertOriginal[2].x = 2.5;
	player.getOBB()->vertOriginal[2].y = 5;

	player.getOBB()->vertOriginal[3].x = 2.5;
	player.getOBB()->vertOriginal[3].y = -2.5;

	//--opponent car
	enemy.getOBB()->vertOriginal[0].x = -2.5;
	enemy.getOBB()->vertOriginal[0].y = -2.5;

	enemy.getOBB()->vertOriginal[1].x = -2.5;
	enemy.getOBB()->vertOriginal[1].y = 5;

	enemy.getOBB()->vertOriginal[2].x = 2.5;
	enemy.getOBB()->vertOriginal[2].y = 5;

	enemy.getOBB()->vertOriginal[3].x = 2.5;
	enemy.getOBB()->vertOriginal[3].y = -2.5;

	//---
	obstacle1.vertOriginal[0].x = -40;
	obstacle1.vertOriginal[0].y = 45;

	obstacle1.vertOriginal[1].x = -40;
	obstacle1.vertOriginal[1].y = 50;

	obstacle1.vertOriginal[2].x = -20;
	obstacle1.vertOriginal[2].y = 50;

	obstacle1.vertOriginal[3].x = -20;
	obstacle1.vertOriginal[3].y = 45;

	spinningObstacle.vertOriginal[0].x = -4;
	spinningObstacle.vertOriginal[0].y = 30;
	
	spinningObstacle.vertOriginal[1].x = -4;
	spinningObstacle.vertOriginal[1].y = 38;
	
	spinningObstacle.vertOriginal[2].x = 4;
	spinningObstacle.vertOriginal[2].y = 38;
	
	spinningObstacle.vertOriginal[3].x = 4;
	spinningObstacle.vertOriginal[3].y = 30;

	//--- Boundaries for the track ---
	boundary1.vertOriginal[0].x = -40;
	boundary1.vertOriginal[0].y = -20;

	boundary1.vertOriginal[1].x = -40;
	boundary1.vertOriginal[1].y = 40;

	boundary1.vertOriginal[2].x = -20;
	boundary1.vertOriginal[2].y = 40;

	boundary1.vertOriginal[3].x = -20;
	boundary1.vertOriginal[3].y = -20;

	//---
	boundary2.vertOriginal[0].x = -80;
	boundary2.vertOriginal[0].y = 20;

	boundary2.vertOriginal[1].x = -80;
	boundary2.vertOriginal[1].y = 40;

	boundary2.vertOriginal[2].x = -40;
	boundary2.vertOriginal[2].y = 40;

	boundary2.vertOriginal[3].x = -40;
	boundary2.vertOriginal[3].y = 20;

	//---
	boundary3.vertOriginal[0].x = -100;
	boundary3.vertOriginal[0].y = 20;

	boundary3.vertOriginal[1].x = -100;
	boundary3.vertOriginal[1].y = 80;

	boundary3.vertOriginal[2].x = -80;
	boundary3.vertOriginal[2].y = 80;

	boundary3.vertOriginal[3].x = -80;
	boundary3.vertOriginal[3].y = 20;

	//---
	boundary4.vertOriginal[0].x = -60;
	boundary4.vertOriginal[0].y = 60;

	boundary4.vertOriginal[1].x = -60;
	boundary4.vertOriginal[1].y = 80;

	boundary4.vertOriginal[2].x = 40;
	boundary4.vertOriginal[2].y = 80;

	boundary4.vertOriginal[3].x = 40;
	boundary4.vertOriginal[3].y = 60;
	
	//---
	boundary5.vertOriginal[0].x = 20;
	boundary5.vertOriginal[0].y = -20;

	boundary5.vertOriginal[1].x = 20;
	boundary5.vertOriginal[1].y = 60;

	boundary5.vertOriginal[2].x = 40;
	boundary5.vertOriginal[2].y = 60;

	boundary5.vertOriginal[3].x = 40;
	boundary5.vertOriginal[3].y = -20;

	//---
	boundary6.vertOriginal[0].x = -40;
	boundary6.vertOriginal[0].y = -40;

	boundary6.vertOriginal[1].x = -40;
	boundary6.vertOriginal[1].y = -20;

	boundary6.vertOriginal[2].x = 40;
	boundary6.vertOriginal[2].y = -20;

	boundary6.vertOriginal[3].x = 40;
	boundary6.vertOriginal[3].y = -40;

	//---
	finishLine.vertOriginal[0].x = -100;
	finishLine.vertOriginal[0].y = 80;

	finishLine.vertOriginal[1].x = -100;
	finishLine.vertOriginal[1].y = 100;

	finishLine.vertOriginal[2].x = -40;
	finishLine.vertOriginal[2].y = 100;

	finishLine.vertOriginal[3].x = -40;
	finishLine.vertOriginal[3].y = 80;
	
}

void processKeys()
{
	if(keys[VK_LEFT])
	{
		player.decrementHeading(0.04f); 
	}
	if(keys[VK_RIGHT])
	{
		
		player.incrementHeading(0.04f);
	}
	if(keys[VK_UP])
	{
		isDrivingUp = true;
		isReversing = false;
		player.incrementVtri(0.00001f); 
	}
	if(keys[VK_DOWN])
	{
		isReversing = true;
		isDrivingUp = false;
		player.decrementVtri(0.00001f);
	}

	if (keys[VK_SPACE])
	{
		player.brake();
	}

	if (keys[VK_SHIFT])
	{
		player.turbo();
	}
}

/**************** END OPENGL FUNCTIONS *************************/


//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop

	// Create Our OpenGL Window
	if (!CreateGLWindow("Uyi's Racing Game", screenWidth, screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(keys[VK_ESCAPE])
				done = true;

			processKeys();

			display();					// Draw The Scene
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

int displayFinishedMessage()
{
	MSG m;
	int msgboxID = MessageBox(
		NULL, "You have finished the race! Well done! you did not crash and burn!",
		"Finished",
		MB_ICONINFORMATION | MB_OK | MB_DEFBUTTON2
	);

	switch (msgboxID)
	{
	case IDOK:
		KillGLWindow();
		return (int)(m.wParam);
		break;
	}

	return msgboxID;
}

int displayDestroyedMessage()
{
	MSG m = { 0 };
	int msgboxID = MessageBox(
		NULL, "OH NO, you damage reached above 5 points - your car is DESTROYED!",
		"Unlucky",
		MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
	);

	switch (msgboxID)
	{
	case IDOK:
		KillGLWindow();
		return (int)(m.wParam);
		break;
	}

	return msgboxID;
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_LBUTTONDOWN:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight - HIWORD(lParam);
				LeftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
	            LeftPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight  - HIWORD(lParam);
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();
	
	return true;									// Success
}
