#include "stdafx.h"
#include "Curve.h"
#include "StraightLine.h"

void Curve::DrawPoints(CDC * pDC){
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);
	for (unsigned int i = 0; i < points.size(); i++) {
		pDC->Ellipse(points[i].x - size, points[i].y - size, points[i].x + size, points[i].y + size);
	}
	pDC->Ellipse(center.x - size, center.y - size, center.x + size, center.y + size);
}

// 计算多项式系数
double Curve::C(int nn, int k){
	int i;
	double sum = 1;
	for (i = 1; i <= nn; i++)
		sum *= i;
	for (i = 1; i <= k; i++)
		sum /= i;
	for (i = 1; i <= nn - k; i++)
		sum /= i;
	return sum;
}

void Curve::Zoom(int flag){
	// 放大比率为1.2，缩小比率为1/1.2。
	double rate = 1.2;
	rate = (flag == ZOOMIN) ? rate : 1.0 / rate;

	for (unsigned int i = 0; i < points.size(); i++) {
		if ((abs(center.x - points[i].x) < 30 && abs(center.y - points[i].y) < 30) && flag == ZOOMOUT)
			return;
	}

	for (unsigned int i = 0; i < points.size(); i++) {
		points[i].x = center.x + (int)round((points[i].x - center.x) * rate);
		points[i].y = center.y + (int)round((points[i].y - center.y) * rate);
	}

	for (unsigned int i = 0; i < points.size(); i++) {
		p[i] = points[i];
	}
}

void Curve::SetSpecialPoint(){
	int x = 0, y = 0;
	if (p == NULL) {
		p = new CPoint[points.size()];
	}
	for (unsigned int i = 0; i < points.size(); i++) {
		x += points[i].x;
		y += points[i].y;
		p[i] = points[i];
	}
	center.x = x / (points.size());
	center.y = y / (points.size());
}

void Curve::Draw(CDC * pDC, COLORREF color){
	int n = points.size();
	DrawPoints(pDC);
	double m = 0;
	for (int i = 0; i < n - 1; i++) {
		m += Distance(points[i], points[i + 1]);
	}
	for (int i = 0; i < (int)m; i++) {
		double t = i / m;
		double x = 0, y = 0, Ber;
		for (int k = 0; k < n; k++){
			Ber = C(n - 1, k) * pow(t, k) * pow(1 - t, n - 1 - k);
			x += points[k].x * Ber;
			y += points[k].y * Ber;
		}
		pDC->SetPixel((int)x, (int)y, color);
	}
}

void Curve::Update(CPoint start, CPoint end, int transform_mode){
	if (transform_mode == MOVE) {
		int dx = end.x - start.x, dy = end.y - start.y;
		for (unsigned int i = 0; i < points.size(); i++) {
			points[i].x += dx;
			points[i].y += dy;
		}
		SetSpecialPoint();
	}
	else if (transform_mode == ZOOM) {
		int dx = end.x - start.x, dy = end.y - start.y;
		for (unsigned int i = 0; i < points.size(); i++) {
			if (Distance(points[i], start) < size) {
				points[i].x += dx;
				points[i].y += dy;
				break;
			}
		}
		SetSpecialPoint();
	}
	else if (transform_mode == ROTATE) {
		double a = Distance(start.x, start.y, center.x, center.y), b = Distance(end.x, end.y, center.x, center.y), c = Distance(start, end);
		if (a == 0 || b == 0) return;
		double angle = acos((a*a + b*b - c*c) / (2 * a * b));
		rotate_angle += angle;
		for (unsigned int i = 0; i < points.size(); i++) {
			CPoint point = p[i];
			points[i].x = (int)((point.x - center.x)*cos(rotate_angle) - (point.y - center.y)*sin(rotate_angle) + center.x);
			points[i].y = (int)((point.x - center.x)*sin(rotate_angle) + (point.y - center.y)*cos(rotate_angle) + center.y);
		}
	}
	else {
		;
	}
}

bool Curve::Contains(CPoint p){
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

int Curve::TransformMode(CPoint p){
	for (unsigned int i = 0; i < points.size(); i++) {
		if (Distance(points[i], p) < size)
			return ZOOM;
	}
	if (Distance(center, p) < size) {
		return ROTATE;
	}
	else if (Contains(p))
		return MOVE;
	else
		return NOTRANSFORM;
}
