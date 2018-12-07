#pragma once
#include <vector>
using namespace std;
class Star
{
	int x, y, scale,vertices;
	POINT* p;

public:
	Star(int x, int y, int scale, int how_many_limbs);
	//~Star();
	void Draw(HDC hdc,COLORREF pencolor,COLORREF brushcolor);
	POINT* getExternalVertices();
	int getLimbs();
};