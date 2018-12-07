#include <iostream>
#include <windows.h>
#include <cmath>
#include <time.h>
#include <vector>
#include "Star.h"

Star::Star(int center_x_cord, int center_y_cord, int scale, int how_many_limbs)
{
	this->x = center_x_cord;
	this->y = center_y_cord;
	this->scale = scale;
	this->vertices = how_many_limbs * 2;

	p = new POINT[vertices];

	float internal_vertices_angle = 0;
	float external_vertices_angle = -360/vertices;
	for (int i = 0; i < how_many_limbs * 2; i++)
	{
		if (i % 2)
		{
			p[i].x = x + scale*(10 * sin(internal_vertices_angle*3.14 / 180));
			p[i].y = y + scale*(-10 * cos(internal_vertices_angle*3.14 / 180));
			internal_vertices_angle += 360 / how_many_limbs;
		}
		else
		{
			p[i].x = x + scale*(20 * sin(external_vertices_angle*3.14 / 180));
			p[i].y = y + scale*(-20 * cos(external_vertices_angle*3.14 / 180));
			external_vertices_angle += 360 / how_many_limbs;
		}
	}

	
}

//Star::~Star()
//{
//	delete p;
//}

void Star::Draw(HDC hdc, COLORREF pencolor, COLORREF brushcolor)
{
	HPEN mypen;
	HBRUSH mybrush;
	mypen = CreatePen(PS_SOLID, 1, pencolor);
	mybrush = CreateSolidBrush(brushcolor);
	SelectObject(hdc, mypen);
	SelectObject(hdc, mybrush);

	Polygon(hdc, p, vertices);
}

POINT * Star::getExternalVertices()
{
	POINT* tmp;
	tmp = new POINT[vertices / 2];
	for (int i = 0; i < vertices / 2; i++)
	{
		tmp[i] = p[2 * i];
	}
	return tmp;
}

int Star::getLimbs()
{
	return vertices/2;
}
