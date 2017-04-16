#include "stdafx.h"
#include "Test3D.h"

Test3D::Test3D(CPoint p){
	dash.CreatePen(PS_DASH, 1, RGB(0,0,0));
	line.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	for (int i = 0; i < 8; i++) {
		points.push_back(p);
	}
}

Test3D::~Test3D(){
	dash.DeleteObject();
	line.DeleteObject();
}

void Test3D::SetSpecialPoint(){
	int dx = points[6].x - points[2].x, dy = points[6].y - points[2].y;
	CPoint start = points.at(0), end = points.at(2);
	CPoint v(start.x, end.y);
	CPoint h(end.x, start.y);
	if (start.x < end.x&&start.y < end.y) {
		points[3] = h;
		points[1] = v;
	}
	else if (start.x > end.x&&start.y > end.y) {
		points[0] = end;
		points[1] = h;
		points[2] = start;
		points[3] = v;
	}
	else if (start.x<end.x&&start.y>end.y) {
		points[0] = v;
		points[1] = start;
		points[2] = h;
		points[3] = end;
	}
	else {
		points[0] = h;
		points[1] = end;
		points[2] = v;
		points[3] = start;
	}

	
	for (int i = 0; i < 4; i++) {
		points[i + 4].x = points[i].x + dx;
		points[i + 4].y = points[i].y + dy;
	}
	if (dx > 0 && dy < 0)
		m = 5;
	else if (dx > 0 && dy > 0)
		m = 2;
	else if (dx < 0 && dy < 0)
		m = 6;
	else
		m = 1;
}

void Test3D::Draw(CDC * pDC, COLORREF color){
	pDC->SelectObject(&line);
	if (m == 5) {
		pDC->MoveTo(points[0]);
		pDC->LineTo(points[1]);

		pDC->MoveTo(points[0]);
		pDC->LineTo(points[3]);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[1]);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[3]);

		pDC->MoveTo(points[0]);
		pDC->LineTo(points[4]);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[6]);

		pDC->MoveTo(points[3]);
		pDC->LineTo(points[7]);

		pDC->MoveTo(points[4]);
		pDC->LineTo(points[7]);

		pDC->MoveTo(points[6]);
		pDC->LineTo(points[7]);

		pDC->SelectObject(&dash);

		pDC->MoveTo(points[1]);
		pDC->LineTo(points[5]);

		pDC->MoveTo(points[4]);
		pDC->LineTo(points[5]);

		pDC->MoveTo(points[6]);
		pDC->LineTo(points[5]);
	}
	else if (m == 2) {
		pDC->MoveTo(points[0]);
		pDC->LineTo(points[1]);

		pDC->MoveTo(points[0]);
		pDC->LineTo(points[3]);

		pDC->MoveTo(points[0]);
		pDC->LineTo(points[4]);

		pDC->MoveTo(points[1]);
		pDC->LineTo(points[5]);

		pDC->MoveTo(points[3]);
		pDC->LineTo(points[7]);

		pDC->MoveTo(points[4]);
		pDC->LineTo(points[5]);

		pDC->MoveTo(points[4]);
		pDC->LineTo(points[7]);

		pDC->MoveTo(points[6]);
		pDC->LineTo(points[5]);

		pDC->MoveTo(points[6]);
		pDC->LineTo(points[7]);

		pDC->SelectObject(&dash);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[1]);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[3]);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[6]);
	}
	else if (m == 6) {
		pDC->MoveTo(points[0]);
		pDC->LineTo(points[1]);

		pDC->MoveTo(points[0]);
		pDC->LineTo(points[3]);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[1]);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[3]);

		pDC->MoveTo(points[0]);
		pDC->LineTo(points[4]);

		pDC->MoveTo(points[1]);
		pDC->LineTo(points[5]);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[6]);

		pDC->MoveTo(points[3]);
		pDC->LineTo(points[7]);

		pDC->MoveTo(points[4]);
		pDC->LineTo(points[5]);

		pDC->MoveTo(points[4]);
		pDC->LineTo(points[7]);

		pDC->MoveTo(points[6]);
		pDC->LineTo(points[5]);

		pDC->MoveTo(points[6]);
		pDC->LineTo(points[7]);

		pDC->SelectObject(&dash);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[6]);

		pDC->MoveTo(points[6]);
		pDC->LineTo(points[5]);

		pDC->MoveTo(points[6]);
		pDC->LineTo(points[7]);
	}
	else {

		pDC->MoveTo(points[0]);
		pDC->LineTo(points[3]);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[3]);

		pDC->MoveTo(points[0]);
		pDC->LineTo(points[4]);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[6]);

		pDC->MoveTo(points[3]);
		pDC->LineTo(points[7]);

		pDC->MoveTo(points[4]);
		pDC->LineTo(points[5]);

		pDC->MoveTo(points[4]);
		pDC->LineTo(points[7]);

		pDC->MoveTo(points[6]);
		pDC->LineTo(points[5]);

		pDC->MoveTo(points[6]);
		pDC->LineTo(points[7]);

		pDC->SelectObject(&dash);

		pDC->MoveTo(points[1]);
		pDC->LineTo(points[5]);

		pDC->MoveTo(points[0]);
		pDC->LineTo(points[1]);

		pDC->MoveTo(points[2]);
		pDC->LineTo(points[1]);
	}
}

void Test3D::Update(CPoint start, CPoint end, int transform_mode){
	if (transform_mode == MOVE) {
		int dx = end.x - start.x, dy = end.y - start.y;
		for (unsigned int i = 0; i < points.size(); i++) {
			points[i].x += dx;
			points[i].y += dy;
		}
	}
	else {
		;
	}
}

bool Test3D::Contains(CPoint p){
	int max_y = points.at(0).y, max_x = points.at(0).x, min_y = points.at(0).y, min_x = points.at(0).x;
	for (unsigned int i = 0; i < points.size(); i++) {
		if (points.at(i).x > max_x)
			max_x = points.at(i).x;
		if (points.at(i).x < min_x)
			min_x = points.at(i).x;
		if (points.at(i).y > max_y)
			max_y = points.at(i).y;
		if (points.at(i).y < min_y)
			min_y = points.at(i).y;
	}
	if ((p.x - min_x)*(p.x - max_x) <= 0 && (p.y - min_y)*(p.y - max_y) <= 0)
		return true;
	else
		return false;
}

int Test3D::TransformMode(CPoint p){
	if (Contains(p))
		return MOVE;
	else
		return NOTRANSFORM;
}
