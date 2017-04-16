#pragma once
#include "Shape.h"
class Test3D : public Shape{
private:
	CPen dash;
	CPen line;
	CPen *old;
	int m;
public:
	Test3D(CPoint p);
	~Test3D();

	void SetSpecialPoint();
	void Draw(CDC *pDC, COLORREF color);
	void Update(CPoint start, CPoint end, int transform_mode);
	bool Contains(CPoint p);
	int TransformMode(CPoint p);
};

	