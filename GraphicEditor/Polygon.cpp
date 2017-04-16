#include "stdafx.h"
#include "Polygon.h"
#include "StraightLine.h"

void MyPolygon::DrawPoints(CDC * pDC){
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);
	for (unsigned int i = 0; i < points.size(); i++) {
		pDC->Ellipse(points[i].x - size, points[i].y - size, points[i].x + size, points[i].y + size);
	}	
	if(!isInnerShape	)
		pDC->Ellipse(center.x - size, center.y - size, center.x + size, center.y + size);
}

MyPolygon::MyPolygon(){
	rotate_angle = 0;
	isInnerShape = false;
}

MyPolygon::MyPolygon(vector<CPoint> points, bool isFilled){
	rotate_angle = 0;
	isInnerShape = true;
	this->points = points;
	this->isFilled = isFilled;
	SetSpecialPoint();
}

vector<Shape*> MyPolygon::Cut(Shape *cut){
	CPoint trim_start = cut->points[0], trim_end = cut->points[1];
	vector<CPoint> temp;
	vector<int> index;
	bool *flag = new bool[points.size()];
	for (unsigned int i = 0; i < points.size(); i++) {
		flag[i] = 0;
	}

	for (unsigned int i = 0; i < points.size() - 1; i++) {
		CPoint start = points[i], end = points[i + 1];
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
				if (flag[i+1] == 0) {
					temp.push_back(intersection);
					index.push_back(temp.size() - 1);
					flag[i+1] = 1;
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
				if (flag[i+1] == 0) {
					temp.push_back(end);
					flag[i+1] = 1;
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
	for (unsigned int i = 0; i < index.size() / 2 * 2; i+=2) {
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

vector<Shape*> MyPolygon::Trim(Shape * trim){
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

	vector<CPoint> left = Left(points, min_x);
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

void MyPolygon::Zoom(int flag){
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

	for (unsigned int i = 0; i < points.size() - 1; i++) {
		p[i] = points[i];
	}
}

void MyPolygon::SetSpecialPoint(){
	int x=0, y=0;
	// 最后一个点与起始点相同，不能重复包含
	if (p == NULL) {
		p = new CPoint[points.size() - 1];
	}
	for (unsigned int i = 0; i < points.size() - 1; i++) {
		x += points[i].x;
		y += points[i].y;
		p[i] = points[i];
	}
	center.x = x / (points.size() - 1);
	center.y = y / (points.size() - 1);
}

void MyPolygon::Fill(CDC * pDC, bool fill){
	if (fill) {
		isFilled = true;
		pDC->SelectObject(&fill_pen);
	}
	else {
		pDC->SelectObject(&pen);
	}

	edges.clear();
	int n = (int)points.size() - 1;
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
		for (int i= 0; i < n; i++) {
			//扫描线经过一个顶点
			if (points[i].y == y && p[i]==true) {
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
			x.push_back((int)(it->start.x + (y - it->start.y)*it->dx));
		}
		sort(x.begin(), x.end());
		assert(x.size() % 2 == 0 && x.size()>=2);
		for (unsigned int i = 0; i < x.size(); i += 2) {
			pDC->MoveTo(x[i], y);
			pDC->LineTo(x[i + 1], y);
		}
	}
}

void MyPolygon::Draw(CDC * pDC, COLORREF color){
	this->DrawPoints(pDC);

	for (unsigned int i = 0; i < points.size() - 1 ; i++) {
		StraightLine::Paint(pDC, points[i], points[i + 1], color);
	}
}

void MyPolygon::Update(CPoint start, CPoint end, int transform_mode){
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
				if (i == 0) {
					points[points.size() - 1] = points[i];
				}
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
		for (unsigned int i = 0; i < points.size()-1; i++) {
			CPoint point = p[i];
			points[i].x = (int)((point.x - center.x)*cos(rotate_angle) - (point.y - center.y)*sin(rotate_angle) + center.x);
			points[i].y = (int)((point.x - center.x)*sin(rotate_angle) + (point.y - center.y)*cos(rotate_angle) + center.y);
		}
		points[points.size() - 1] = points[0];
	}
	else {
		;
	}
}

bool MyPolygon::Contains(CPoint p){
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

int MyPolygon::TransformMode(CPoint p){
	for (unsigned int i = 0; i < points.size()-1; i++) {
		if (Distance(points[i], p) < size)
			return ZOOM;
	}
	if (Distance(center,p) < size) {
		return ROTATE;
	}
	else if (Contains(p))
		return MOVE;
	else
		return NOTRANSFORM;
}
