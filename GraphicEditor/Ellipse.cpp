#include "stdafx.h"
#include "Ellipse.h"

void MyEllipse::DrawPoints(CDC * pDC){
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);
	CPoint start = points.at(0), end = points.at(1);
	int dx = end.x - start.x;
	pDC->Ellipse(start.x - size, start.y - size, start.x + size, start.y + size);

	CPoint p1(start.x+dx, start.y);
	pDC->Ellipse(p1.x - size, p1.y - size, p1.x + size, p1.y + size);
}

MyEllipse::MyEllipse(CPoint p0, CPoint p1, bool isFilled){
	points.push_back(p0);
	points.push_back(p1);
	this->isFilled = isFilled;
}

vector<Shape*> MyEllipse::Trim(Shape * trim){
	// 裁剪窗口完全包含椭圆时才会被裁剪出来
	int a = abs(points[1].x - points[0].x), b = abs(points[1].y - points[0].y);
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
	if (center.x - min_x > a && max_x - center.x > a && center.y - min_y > b && max_y - center.y > b) {
		Shape *rst = new MyEllipse(points[0], points[1], isFilled);
		result.push_back(rst);
	}

	return result;
}

void MyEllipse::Zoom(int flag){
	// 放大比率为1.2，缩小比率为1/1.2。
	double rate = 1.2;
	rate = (flag == ZOOMIN) ? rate : 1.0 / rate;

	if ((abs(points[1].x - points[0].x) < 30 && abs(points[1].y - points[0].y) < 30) && flag == ZOOMOUT)
		return;

	points[1].x = points[0].x + (int)round((points[1].x - points[0].x) * rate);
	points[1].y = points[0].y + (int)round((points[1].y - points[0].y) * rate);
}

void MyEllipse::Fill(CDC * pDC, bool fill){
	if (fill) {
		isFilled = true;
		pDC->SelectObject(&fill_pen);
	}
	else
		pDC->SelectObject(&pen);
	int a = abs(points[1].x - points[0].x), b = abs(points[1].y - points[0].y);
	int xc = points[0].x, yc = points[0].y;
	double sqa = a * a;
	double sqb = b * b;
	double d = sqb + sqa * (-b + 0.25);
	int x = 0;
	int y = b;		
	pDC->MoveTo(xc + x, yc + y);
	pDC->LineTo(xc + x, yc - y);
	pDC->MoveTo(xc - x, yc + y);
	pDC->LineTo(xc - x, yc - y);
	while (sqb * (x + 1) < sqa * (y - 0.5)) {
		if (d < 0) {
			d += sqb * (2 * x + 3);
		}
		else {
			d += (sqb * (2 * x + 3) + sqa * (-2 * y + 2));
			y--;
		}
		x++;
		pDC->MoveTo(xc + x, yc + y);
		pDC->LineTo(xc + x, yc - y);
		pDC->MoveTo(xc - x, yc + y);
		pDC->LineTo(xc - x, yc - y);
	}
	d = (b * (x + 0.5)) * 2 + (a * (y - 1)) * 2 - (a * b) * 2;
	while (y > 0) {
		if (d < 0) {
			d += sqb * (2 * x + 2) + sqa * (-2 * y + 3);
			x++;
		}
		else {
			d += sqa * (-2 * y + 3);
		}
		y--;
		pDC->MoveTo(xc + x, yc + y);
		pDC->LineTo(xc + x, yc - y);
		pDC->MoveTo(xc - x, yc + y);
		pDC->LineTo(xc - x, yc - y);
	}
}

void MyEllipse::Draw(CDC * pDC, COLORREF color){
	DrawPoints(pDC);
	int a = abs(points[1].x - points[0].x), b = abs(points[1].y - points[0].y);
	int xc = points[0].x, yc = points[0].y;
	double sqa = a * a;
	double sqb = b * b;
	double d = sqb + sqa * (-b + 0.25);
	int x = 0;
	int y = b;
	pDC->SetPixel(xc + x, yc + y, color);
	pDC->SetPixel(xc - x, yc + y, color);
	pDC->SetPixel(xc + x, yc - y, color);
	pDC->SetPixel(xc - x, yc - y, color);

	while (sqb * (x + 1) < sqa * (y - 0.5)) {
		if (d < 0) {
			d += sqb * (2 * x + 3);
		}
		else {
			d += (sqb * (2 * x + 3) + sqa * (-2 * y + 2));
			y--;
		}
		x++;
		pDC->SetPixel(xc + x, yc + y, color);
		pDC->SetPixel(xc - x, yc + y, color);
		pDC->SetPixel(xc + x, yc - y, color);
		pDC->SetPixel(xc - x, yc - y, color);
	}
	d = (b * (x + 0.5)) * 2 + (a * (y - 1)) * 2 - (a * b) * 2;
	while (y > 0) {
		if (d < 0) {
			d += sqb * (2 * x + 2) + sqa * (-2 * y + 3);
			x++;
		}
		else {
			d += sqa * (-2 * y + 3);
		}
		y--;
		pDC->SetPixel(xc + x, yc + y, color);
		pDC->SetPixel(xc - x, yc + y, color);
		pDC->SetPixel(xc + x, yc - y, color);
		pDC->SetPixel(xc - x, yc - y, color);
	}
/*	DrawPoints(pDC);
	CPoint start = points.at(0), end = points.at(1);
	int x0 = start.x, y0 = start.y;
	int rx = abs(end.x - start.x), ry = abs(end.y - start.y);
	int rx2 = rx*rx, ry2 = ry*ry;
	int tworx2 = rx2 * 2, twory2 = ry2 * 2;
	int x = 0, y = ry;
	int px = 0, py = tworx2*y;

	int p = (int)round(ry2 - (rx2 * ry) + (0.25 * rx2));
	while (px < py) {
		pDC->SetPixel(x0 + x, y0 + y, color);
		pDC->SetPixel(x0 - x, y0 + y, color);
		pDC->SetPixel(x0 + x, y0 - y, color);
		pDC->SetPixel(x0 - x, y0 - y, color);

		x++;
		px += twory2;
		if (p < 0)
			p += ry2 + px;
		else {
			y--;
			py -= tworx2;
			p += ry2 + px - py;
		}
	}
	p = (int)round(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
	while (y > 0) {
		pDC->SetPixel(x0 + x, y0 + y, color);
		pDC->SetPixel(x0 - x, y0 + y, color);
		pDC->SetPixel(x0 + x, y0 - y, color);
		pDC->SetPixel(x0 - x, y0 - y, color);

		y--;
		py -= tworx2;
		if (p > 0)
			p += rx2 - py;
		else {
			x++;
			px += twory2;
			p += rx2 - py + px;
		}
	}
*/
}

void MyEllipse::Update(CPoint start, CPoint end, int transform_mode){
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

bool MyEllipse::Contains(CPoint p){
	CPoint start = points.at(0), end = points.at(1);
	int dx = abs(end.x - start.x), dy = abs(end.y - start.y);
	if (Distance(p,points[0])<=(dx>dy?dx:dy))
		return true;
	else
		return false;
}

int MyEllipse::TransformMode(CPoint p){
	CPoint start = points.at(0), end = points.at(1);
	int dx = end.x - start.x, dy = end.y - start.y;
	CPoint p1(start.x + dx, start.y),  p2(start.x - dx, start.y);

	if (Distance(p1, p) < size )
		return ZOOM;
	else if (Contains(p))
		return MOVE;
	else
		return NOTRANSFORM;
}
