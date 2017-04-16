#include "stdafx.h"
#include "Circle.h"

void Circle::DrawPoints(CDC * pDC){
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);
	for (unsigned int i = 0; i < points.size(); i++) {
		pDC->Ellipse(points[i].x - size, points[i].y - size, points[i].x + size, points[i].y + size);
	}
}

Circle::Circle(CPoint p0, CPoint p1, bool isFilled){
	points.push_back(p0);
	points.push_back(p1);
	this->isFilled = isFilled;
}

vector<Shape*> Circle::Trim(Shape *trim){
	// 裁剪窗口完全包含圆时才会被裁剪出来
	int r = (int)Distance(points[0], points[1]);
	CPoint center = points[0];
	int min_x, min_y, max_x, max_y;
	if (trim->points[0].x > trim->points[2].x) {
		min_x = trim->points[2].x;
		max_x = trim->points[0].x;
	}
	else {
		min_x = trim->points[0].x;
		max_x = trim->points[2].x;
	}
	if (trim->points[0].y > trim->points[2].y) {
		min_y = trim->points[2].y;
		max_y = trim->points[0].y;
	}
	else {
		min_y = trim->points[0].y;
		max_y = trim->points[2].y;
	}

	vector<Shape*> result;
	if (center.x - min_x > r && max_x - center.x > r && center.y - min_y > r && max_y - center.y > r) {
		Shape *rst = new Circle(points[0], points[1], isFilled);
		result.push_back(rst);
	}

	return result;
}

void Circle::Zoom(int flag){
	// 放大比率为1.2，缩小比率为1/1.2。
	double rate = 1.2;
	rate = (flag == ZOOMIN) ? rate : 1.0 / rate;

	if ((abs(points[1].x - points[0].x) < 30 && abs(points[1].y - points[0].y) < 30) && flag == ZOOMOUT)
		return;

	points[1].x = points[0].x + (int)round((points[1].x - points[0].x) * rate);
	points[1].y = points[0].y + (int)round((points[1].y - points[0].y) * rate);
}

void Circle::Fill(CDC * pDC, bool fill){
	if (fill) {
		isFilled = true;
		pDC->SelectObject(&fill_pen);
	}
	else
		pDC->SelectObject(&pen);
	CPoint center = points[0];
	int x0 = center.x, y0 = center.y;
	int r = (int)Distance(center, points[1]);
	double p = 1.25 - r;

	int x = 0, y = r;
	while (x <= y) {
		pDC->MoveTo(x+x0, y+y0);
		pDC->LineTo(x+x0, -y+y0);

		pDC->MoveTo(-x + x0, y + y0);
		pDC->LineTo(-x + x0, -y + y0);

		pDC->MoveTo(y+x0, x+y0);
		pDC->LineTo(y+x0, -x+y0);

		pDC->MoveTo(-y + x0, x + y0);
		pDC->LineTo(-y + x0, -x + y0);

		if (p >= 0) {
			p += (x - y) * 2 + 5;
			x++;
			y--;
		}
		else {
			p += x * 2 + 3;
			x++;
		}
	}
}

void Circle::Draw(CDC * pDC, COLORREF color){
	DrawPoints(pDC);
	CPoint center = points[0];
	int x0 = center.x, y0 = center.y;
	int r = (int)Distance(center, points[1]);
	double p = 1.25 - r;

	int x = 0, y = r;
	while (x <= y) {
		pDC->SetPixel(x + x0, y + y0, color);
		pDC->SetPixel(-x + x0, y + y0, color);
		pDC->SetPixel(x + x0, -y + y0, color);
		pDC->SetPixel(-x + x0, -y + y0, color);
		pDC->SetPixel(y + x0, x + y0, color);
		pDC->SetPixel(-y + x0, x + y0, color);
		pDC->SetPixel(y + x0, -x + y0, color);
		pDC->SetPixel(-y + x0, -x + y0, color);

		if (p >= 0) {
			p += (x - y) * 2 + 5;
			x++;
			y--;
		}
		else {
			p += x * 2 + 3;
			x++;
		}
	}
}

void Circle::Update(CPoint start, CPoint end, int transform_mode){
	if (transform_mode == MOVE) {
		int dx = end.x - start.x, dy = end.y - start.y;
		points[0].x += dx;
		points[0].y += dy;
		points[1].x += dx;
		points[1].y += dy;
	}
	else if (transform_mode == ZOOM) {
		int dx = end.x - start.x, dy = end.y - start.y;
		points[1].x += dx;
		points[1].y += dy;
	}
	else {
		;
	}
}

bool Circle::Contains(CPoint p){
	int r = (int)(Distance(points[0], points[1]));
	if (Distance(p, points[0]) <= r)
		return true;
	else
		return false;
}

int Circle::TransformMode(CPoint p){
	if (Distance(points[1], p) <= size)
		return ZOOM;
	else if (Contains(p))
		return MOVE;
	else
		return NOTRANSFORM;
}
