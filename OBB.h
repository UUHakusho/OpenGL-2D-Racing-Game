
#ifndef OBB_H
#define OBB_H

const int NUMVERTS = 4; //number of vertices for squares and rectangles

class Point
{
public:
	float x;
	float y;
};

class OBB
{
public:
	OBB();
	Point vert[NUMVERTS];
	Point vertOriginal[NUMVERTS];
	void drawOBB();
	void transformPoints(float matrix[16]);
	bool isColliding(OBB& p2);
	bool SATtest(float proj[4], float len);
};

#endif