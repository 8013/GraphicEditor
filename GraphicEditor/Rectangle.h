#pragma once
#include "Shape.h"

class MyRectangle :public Shape{
protected:
	void DrawPoints(CDC *pDC);
	bool rotated;	
	CPoint center;
	CPoint p[4];
	double rotate_angle;
	vector<Edge> edges;
public:
	MyRectangle(){ rotated = false; rotate_angle = 0;}
	MyRectangle(CPoint p1, CPoint p2, CPoint p3, CPoint p4);
	vector<Shape*> Cut(Shape *cut);
	vector<Shape*> Trim(Shape *trim);

	void Zoom(int flag);
	void SetSpecialPoint();
	void Fill(CDC *pDC, bool fill);
	void Draw(CDC *pDC, COLORREF color);
	void Update(CPoint start, CPoint end, int transform_mode);
	bool Contains(CPoint p);
	int TransformMode(CPoint p);
};

