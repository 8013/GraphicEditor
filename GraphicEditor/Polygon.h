#pragma once
#include "Shape.h"
class MyPolygon : public Shape{
private:
	void DrawPoints(CDC *pDC);
	bool isInnerShape;
	double rotate_angle;
	CPoint center;
	CPoint *p=NULL;
	vector<Edge> edges;
public:
	MyPolygon();
	MyPolygon(vector<CPoint> points, bool isFilled);
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

