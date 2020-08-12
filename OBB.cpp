
// Polygon objects implementation

#include <math.h>
#include "OBB.h"
#include "Car.h"

#include <iostream>
using namespace std;

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLu.h>

OBB::OBB()
{
	for (int i = 0; i < NUMVERTS; i++)
	{
		vert[i].x = 0.0; vert[i].y = 0.0;
		vertOriginal[i].x = 0.0; vertOriginal[i].y = 0.0;
	}
}

//Draws OBB around objects for visual representation
void OBB::drawOBB()
{
	glLineWidth(4.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < NUMVERTS; i++)
		glVertex2f(vert[i].x, vert[i].y);
	glEnd();
	glLineWidth(1.0);
}


//Method to take in matrix and The transform the points by multiplying the original vertex positions by the matrix
void OBB::transformPoints(float matrix[16])
{
	for (int i = 0; i < NUMVERTS; i++)
	{
		vert[i].x = matrix[0] * vertOriginal[i].x + matrix[4] * vertOriginal[i].y + matrix[12]; //make all - i think
		vert[i].y = matrix[1] * vertOriginal[i].x + matrix[5] * vertOriginal[i].y + matrix[13];

	}
}

//using Separting Axis Theorem test each edge of the polygon (4 tests for 4 edges) to see if they overlap (separate axis) with another OBB
bool OBB::isColliding(OBB& p2)
{
	float axis[2], len, proj[4];

	//// Axis 1 Poly 1
	axis[0] = vert[1].x - vert[0].x;
	axis[1] = vert[1].y - vert[0].y;
	len = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
	axis[0] /= len; axis[1] /= len;  // unit vector

	//project second poly points on axis
	for (int i = 0; i < NUMVERTS; i++)
		proj[i] = (p2.vert[i].x - this->vert[0].x)*axis[0] + (p2.vert[i].y - this->vert[0].y)*axis[1];   // dot product
	if (SATtest(proj, len)) return false;

	// Axis 2 Poly 1
	axis[0] = this->vert[2].x - this->vert[1].x;
	axis[1] = this->vert[2].y - this->vert[1].y;
	len = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
	axis[0] /= len; axis[1] /= len;  // unit vector

	// project second poly points on axis
	for (int i = 0; i < NUMVERTS; i++)
		proj[i] = (p2.vert[i].x - this->vert[1].x)*axis[0] + (p2.vert[i].y - this->vert[1].y)*axis[1]; 
	if (SATtest(proj, len)) return false;

	// Axis 3 Poly 2
	axis[0] = p2.vert[1].x - p2.vert[0].x;
	axis[1] = p2.vert[1].y - p2.vert[0].y;
	len = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
	axis[0] /= len; axis[1] /= len;  // unit vector
	// project first poly points on axis
	for (int i = 0; i < NUMVERTS; i++)
		proj[i] = (this->vert[i].x - p2.vert[0].x)*axis[0] + (this->vert[i].y - p2.vert[0].y)*axis[1];   // dot product
	if (SATtest(proj, len)) return false;

	// Axis 4 Poly 2
	axis[0] = p2.vert[2].x - p2.vert[1].x;
	axis[1] = p2.vert[2].y - p2.vert[1].y;
	len = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
	axis[0] /= len; axis[1] /= len;  // unit vector
	// project first poly points on axis
	for (int i = 0; i < NUMVERTS; i++)
		proj[i] = (this->vert[i].x - p2.vert[1].x)*axis[0] + (this->vert[i].y - p2.vert[1].y)*axis[1];   // dot product
	if (SATtest(proj, len)) return false;

	//no separating axis - must be in collision
	return true;
}

//Helper method to test the theroem 
bool OBB::SATtest(float proj[4], float len)
{
	float min = proj[0], max = proj[0];
	for (int i = 1; i <= 3; ++i) {  // Find minimum and maximum projections - the other two will lie in between and are of no consequence
		if (proj[i] < min)min = proj[i];
		else if (proj[i] > max)max = proj[i];
	}
	//cout << "min: "<< min << " max: " << max << endl;

	// comparison noting that the first point of first poly is at distance 0 and the second point at distance len
	// along the edge of the first poly acting as the axis
	if (min > len || max < 0.0) return true; // found separating axis
	else return false;
}