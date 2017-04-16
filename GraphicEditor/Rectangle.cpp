#include "stdafx.h"
#include "Rectangle.h"
#include "StraightLine.h"
#include "Polygon.h"

void MyRectangle::DrawPoints(CDC * pDC){
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);
	for (unsigned int i = 0; i < points.size(); i++) {
		pDC->Ellipse(points[i].x - size, points[i].y - size, points[i].x + size, points[i].y + size);
	}
	pDC->Ellipse(center.x - size, center.y - size, center.x + size, center.y + size);
}

MyRectangle::MyRectangle(CPoint p1, CPoint p2, CPoint p3, CPoint p4){
	rotated = false; 
	rotate_angle = 0;

	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	points.push_back(p4);
}

vector<Shape*> MyRectangle::Cut(Shape * cut){
	CPoint trim_start = cut->points[0], trim_end = cut->points[1];
	// 复用多边形切割算法，保持和多边形点的存储类型一样
	vector<CPoint> pointss = points;
	pointss.push_back(points[0]);

	vector<CPoint> temp;
	vector<int> index;
	bool *flag = new bool[pointss.size()];
	for (unsigned int i = 0; i < pointss.size(); i++) {
		flag[i] = 0;
	}

	for (unsigned int i = 0; i < pointss.size() - 1; i++) {
		CPoint start = pointss[i], end = pointss[i + 1];
		// 有交点
		if (HasIntersect(start, end, trim_start, trim_end)) {
			CPoint intersection;
			if (start.x == end.x) {
				intersection.x = start.x;
				intersection.y = (int)(trim_end.y - (double(trim_end.y - trim_start.y) / double(trim_end.x - trim_start.x))*(trim_end.x - intersection.x));
			}
			else {
				double k = double(start.y - end.y) / double(start.x - end.x);
				double trim_k = double(trim_start.y - trim_end.y) / double(trim_start.x - trim_end.x);
				if (k == trim_k) continue;
				intersection.x = (int)((k*start.x - trim_k*trim_start.x + trim_start.y - start.y) / (k - trim_k));
				intersection.y = (int)(start.y + (intersection.x - start.x)*k);
			}
			// 和起点重合
			if (Distance(intersection, start) == 0) {
				if (flag[i] == 0) {
					temp.push_back(intersection);
					index.push_back(temp.size() - 1);
					flag[i] = 1;
				}
			}
			// 和终点重合
			else if (Distance(intersection, end) == 0) {
				if (flag[i + 1] == 0) {
					temp.push_back(intersection);
					index.push_back(temp.size() - 1);
					flag[i + 1] = 1;
				}
			}
			// 不和端点重合
			else {
				if (flag[i] == 0) {
					temp.push_back(start);
					flag[i] = 1;
				}
				temp.push_back(intersection);
				index.push_back(temp.size() - 1);
				if (flag[i + 1] == 0) {
					temp.push_back(end);
					flag[i + 1] = 1;
				}
			}
		}
		// 没有交点
		else {
			if (flag[i] == 0) {
				temp.push_back(start);
				flag[i] = 1;
			}
			if (flag[i + 1] == 0) {
				temp.push_back(end);
				flag[i + 1] = 1;
			}
		}
	}

	vector<Shape*> result;
	for (unsigned int i = 0; i < index.size() / 2 * 2; i += 2) {
		int index0 = index[i], index1 = index[i + 1];
		vector<CPoint> ps;
		for (int i = index0; i <= index1; i++) {
			ps.push_back(temp[i]);
		}
		ps.push_back(temp[index0]);
		temp.erase(temp.begin() + index0 + 1, temp.begin() + index1);
		Shape *rst = new MyPolygon(ps, isFilled);
		result.push_back(rst);
	}
	Shape *rst = new MyPolygon(temp, isFilled);
	result.push_back(rst);

	return result;
}

vector<Shape*> MyRectangle::Trim(Shape * trim){
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

	vector<CPoint> ps = points;
	ps.push_back(points[0]);
	vector<CPoint> left = Left(ps, min_x);
	vector<CPoint> right = Right(left, max_x);
	vector<CPoint> top = Top(right, min_y);
	vector<CPoint> buttom = Buttom(top, max_y);

	vector<Shape*> result;
	if (!buttom.empty()) {
		Shape *rst = new MyPolygon(buttom, isFilled);
		result.push_back(rst);
	}
	return result;
}

void MyRectangle::Zoom(int flag){
	// 放大比率为1.2，缩小比率为1/1.2。
	double rate = 1.2;
	rate = (flag == ZOOMIN) ? rate : 1.0 / rate;

	if ((abs(center.x - points[0].x) < 30 || abs(center.y - points[0].y) < 30) && flag == ZOOMOUT)
		return;
	for (unsigned int i = 0; i < points.size(); i++) {
		points[i].x = center.x + (int)round((points[i].x - center.x) * rate);
		points[i].y = center.y + (int)round((points[i].y - center.y) * rate);
	}
	for (int i = 0; i < 4; i++) {
		p[i] = points[i];
	}
}

void MyRectangle::SetSpecialPoint(){
	CPoint start = points.at(0), end = points.at(2);
	CPoint v(start.x, end.y);
	CPoint h(end.x, start.y);
	points[3] = h;
	points[1] = v;
	center.x = (points[0].x + points[1].x + points[2].x + points[3].x) / 4;
	center.y = (points[0].y + points[1].y + points[2].y + points[3].y) / 4;
	for (int i = 0; i < 4; i++) {
		p[i] = points[i];
	}
}

void MyRectangle::Fill(CDC * pDC, bool fill){
	if (fill) {
		isFilled = true;
		pDC->SelectObject(&fill_pen);
	}
	else {
		pDC->SelectObject(&pen);
	}

	edges.clear();
	int n = (int)points.size();
	// 获取所有点纵坐标的最大最小值
	int max = points.at(0).y, min = points.at(0).y;
	for (int i = 0; i < n; i++) {
		if (points.at(i).y > max)
			max = points.at(i).y;
		if (points.at(i).y < min)
			min = points.at(i).y;
	}
	bool *p = new bool[n];
	for (int i = 0; i < n; i++) {
		p[i] = true;
	}
	for (int y = min; y < max; y++) {
		for (int i = 0; i < n; i++) {
			//扫描线经过一个顶点
			if (points[i].y == y && p[i] == true) {
				p[i] = false;
				int a = i - 1 >= 0 ? i - 1 : n + (i - 1);
				int b = i + 1 < n ? i + 1 : (i + 1) - n;
				bool f1 = true, f2 = true;
				Edge e1(a, points[i], points[a]);
				Edge e2(i, points[i], points[b]);
				for (vector<Edge>::iterator it = edges.begin(); it != edges.end(); ) {
					if (it->no == e1.no) {
						it = edges.erase(it);
						f1 = false;
					}
					else if (it->no == e2.no) {
						it = edges.erase(it);
						f2 = false;
					}
					else {
						it++;
					}
				}
				if (f1) edges.push_back(e1);
				if (f2) edges.push_back(e2);
			}
		}

		vector<int> x;
		for (vector<Edge>::iterator it = edges.begin(); it != edges.end(); it++) {
			double dx = it->dx;
			int temp_x = (int)(it->start.x + (y - it->start.y)*dx);
			x.push_back(temp_x);
		}
		sort(x.begin(), x.end());
		assert(x.size() % 2 == 0 && x.size() >= 2);
		for (unsigned int i = 0; i < x.size(); i += 2) {
			pDC->MoveTo(x[i], y);
			pDC->LineTo(x[i + 1], y);
		}
	}
}

void MyRectangle::Draw(CDC * pDC, COLORREF color){
	DrawPoints(pDC);
	StraightLine::Paint(pDC, points[0], points[1], color);
	StraightLine::Paint(pDC, points[0], points[3], color);
	StraightLine::Paint(pDC, points[2], points[1], color);
	StraightLine::Paint(pDC, points[2], points[3], color);
}

void MyRectangle::Update(CPoint start, CPoint end, int transform_mode){
	if (transform_mode == MOVE) {
		int dx = end.x - start.x, dy = end.y - start.y;
		for (unsigned int i = 0; i < points.size(); i++) {
			points[i].x += dx;
			points[i].y += dy;
		}
		center.x = (points[0].x + points[1].x + points[2].x + points[3].x) / 4;
		center.y = (points[0].y + points[1].y + points[2].y + points[3].y) / 4;
		for (int i = 0; i < 4; i++) {
			p[i] = points[i];
		}
	}
	else if (transform_mode == ZOOM) {
		int dx = end.x - start.x, dy = end.y - start.y;
		CPoint p0 = points.at(0), p2 = points.at(2), p1(p0.x, p2.y), p3(p2.x, p0.y);
		if (Distance(p0, start) < size) {
			points[0].x += dx;
			points[0].y += dy;
		}
		else if(Distance(p2, start) < size){
			points[2].x += dx;
			points[2].y += dy;
		}
		else if (Distance(p1, start) < size) {
			points[0].x += dx;
			points[2].y += dy;
		}
		else {
			points[2].x += dx;
			points[0].y += dy;
		}
		SetSpecialPoint();
	}
	else if (transform_mode == ROTATE) {
		rotated = true;
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

bool MyRectangle::Contains(CPoint p){
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

int MyRectangle::TransformMode(CPoint p){
	if (!rotated && (Distance(points.at(0), p) < size || Distance(points.at(1), p) < size || Distance(points.at(2), p) < size || Distance(points.at(3), p) < size))
		return ZOOM;
	else if (Distance(center, p) < size)
		return ROTATE;
	else if (Contains(p))
		return MOVE;
	else
		return NOTRANSFORM;
}
