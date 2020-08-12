#include "Car.h"
#include "OBB.h"
#include <windows.h>
#include "Image_Loading/nvImage.h"

//default
Car::Car() {

}

//constructor
Car::Car(float Xtri, float Ytri, OBB* obb, GLuint textureMap, bool CPU)
{
	this->Xtri = Xtri;
	this->Ytri = Ytri;
	Vtri = 0;
	this->CPU = CPU;
	heading = 0;
	damageCounter = 0;
	this->obb = obb;
	this->textureMap = textureMap;
}

//accessor methods
OBB* Car::getOBB() {
	return obb;
}

float Car::getXtri() {
	return Xtri;
}

float Car::getYtri() {
	return Ytri;
}

float Car::getVtri() {
	return Vtri;
}

float Car::getHeading() {
	return heading;
}

GLuint Car::getTexture() {
	return textureMap;
}

int Car::getDamage() {
	return damageCounter;
}

void Car::incrementHeading(float incr) {
	heading -= incr; 
}

void Car::decrementHeading(float dec) {
    heading += dec; 
}

void Car::incrementVtri(float inc) {  
	if (Vtri < 0.015f) { //max speed 0.015f
		Vtri += inc;
	}
	
}

void Car::decrementVtri(float dec) {
	Vtri -= dec;
}

//allows car to move
void Car::updatePosition(float PI) {
	Xtri += Vtri * cosf((90 + heading)*(PI / 180.0f)); //90 and 180
	Ytri += Vtri * sinf((90 + heading)*(PI / 180.0f));
}

//if collision, Have the car move in the opposite direction of where it contacted the collision
void Car::collisionResponseUp(float PI) {
	damageCounter++;
	Xtri -= Vtri * -sinf((90 - heading)*(PI / 180.0f)); //-sin
	Ytri -= Vtri * cosf((90 + heading)*(PI / 180.0f)); 
	
	Vtri -= 0.0004f;
}

void Car::collisionResponseReverse(float PI) {
	damageCounter++;
	Xtri -= Vtri * -sinf((90 - heading)*(PI / 180.0f)); //-sin
	Ytri -= Vtri * cosf((90 + heading)*(PI / 180.0f));

	Vtri += 0.0004f;
}

void Car::drawCar(float matrix[], GLuint texture) {
	
	glPushMatrix();
		glTranslatef(Xtri, Ytri, 0.0);
		glRotatef(heading, 0, 0, 1); 
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS); //Polygon b4
		    glTexCoord2f(0.0, 0.0); glVertex2f(-2.5, -2.5); //all 2.5
			glTexCoord2f(0.0, 1.0); glVertex2f(-2.5, 5);
			glTexCoord2f(1.0, 1.0); glVertex2f(2.5, 5);
			glTexCoord2f(1.0, 0.0); glVertex2f(2.5, -2.5);
		glEnd();
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();

}

//method to stop car immediately when moving
void Car::brake() {
	if (Vtri > 0) {
		Vtri -= 0.0009f; 
	}
	
	else if (Vtri < 0) {
		Vtri += 0.0009f;
	}

	else {
		Vtri = 0;
	}
}

//method to accelerate even further
void Car::turbo() {
	Vtri += 0.00003f;
	if (Vtri == 0.014f) {
		Vtri -= 0.0009f; //slow down
	}
}

//CPU car move to a coordinate on the map
void cpuMove(float inc) {
}