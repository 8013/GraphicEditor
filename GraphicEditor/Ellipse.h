#pragma once
#include "Shape.h"
class MyEllipse : public Shape{
private:
	void DrawPoints(CDC *pDC);
public:
	MyEllipse() { ; }
	MyEllipse(CPoint p0, CPoint p1, bool isFilled);
	vector<Shape*> Trim(Shape *trim);

	void Zoom(int flag); 
	void Fill(CDC *pDC, bool fill);
	void Draw(CDC *pDC, COLORREF color);
	void Update(CPoint start, CPoint end, int transform_mode);
	bool Contains(CPoint p);
	int TransformMode(CPoint p);
};

