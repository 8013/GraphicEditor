#pragma once
#include "Rectangle.h"

class TrimWindow : public MyRectangle {
private:
	CPen dash;
	vector<Shape*> inner;
public:
	TrimWindow(CPoint p);
	~TrimWindow();
	void AddShape(Shape *shape);
	
	void Fill(CDC *pDC, bool fill);
	void Draw(CDC *pDC, COLORREF color);
	void Update(CPoint start, CPoint end, int transform_mode);

	void Zoom(int flag) { ; }
	vector<Shape*> Cut(Shape *cut) { return vector<Shape*>(); }
	vector<Shape*> Trim(Shape *trim){ return vector<Shape*>(); }
};

