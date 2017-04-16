#pragma once
#include "Shape.h"
class Curve :	public Shape{
private:
	void DrawPoints(CDC *pDC);
	double C(int nn, int k);
	CPoint center;
	CPoint *p = NULL;
	double rotate_angle;
public:
	Curve() { rotate_angle = 0; }

	void Zoom(int flag);
	void SetSpecialPoint();
	void Draw(CDC *pDC, COLORREF color);
	void Update(CPoint start, CPoint end, int transform_mode);
	bool Contains(CPoint p);
	int TransformMode(CPoint p);
};

