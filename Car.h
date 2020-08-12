#pragma once
#include "OBB.h"
#include "Image_Loading/nvImage.h"

class Car
{
private:
	//variables for moving object
	float Xtri; //position
	float Ytri;
	float Vtri; //velocity
	float heading; //heading of moving obj
	int damageCounter;
	bool CPU; //flag for player controlled or not
	OBB* obb; //object detection, can use it to draw the ring around car to see detections
	GLuint textureMap; //car's texture


public:
	Car();

	Car(float x, float y, OBB* obb, GLuint textureMap, bool CPU); //default 

	OBB* getOBB();

	float getXtri();

	float getYtri();

	float getVtri();

	float getHeading();

	GLuint getTexture();

	int getDamage();

	void incrementHeading(float inc);

	void decrementHeading(float dec);

	void incrementVtri(float inc);

	void decrementVtri(float dec);

	void updatePosition(float PI);

	void collisionResponseUp(float PI);

	void collisionResponseReverse(float PI);

	void drawCar(float matrix[], GLuint texture);

	void brake();

	void turbo();

	void cpuMove(float inc);
};