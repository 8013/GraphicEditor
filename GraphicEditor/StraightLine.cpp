#include "stdafx.h"
#include "StraightLine.h"

void StraightLine::DrawPoints(CDC *pDC){
	pDC->SelectObject(&pen);
	pDC->SelectObject(&brush);
	for (unsigned int i = 0; i < points.size(); i++) {
		pDC->Ellipse(points[i].x - size, points[i].y - size, points[i].x + size, points[i].y + size);
	}
}

int StraightLine::Encode(int min_x, int max_x, int min_y, int max_y, CPoint p){
	int c=0;
	if (p.x < min_x)
		c |= 0x1;

	if (p.x > max_x)
		c |= 0x2;

	if (p.y < min_y)
		c |= 0x4;

	if (p.y > max_y)
		c |= 0x8;
	return c;
}

StraightLine::StraightLine(){
	rotate_angle = 0; 
}

StraightLine::StraightLine(CPoint start, CPoint end){
	rotate_angle = 0;

	points.push_back(start);
	points.push_back(end);
	SetSpecialPoint();
}

vector<Shape*> StraightLine::Cut(Shape * cut){
	CPoint start = points[0], end = points[1];
	CPoint trim_start = cut->points[0], trim_end = cut->points[1];
	// 有交点
	if (HasIntersect(start, end, trim_start, trim_end)) {
		CPoint intersection;
		// 计算交点位置
		if (start.x == end.x) {
			intersection.x = start.x;
			intersection.y =(int)(trim_end.y - (double(trim_end.y - trim_start.y) / double(trim_end.x - trim_start.x))*(trim_end.x - intersection.x));
		}
		else {
			double k = double(start.y - end.y) / double(start.x - end.x);
			if (trim_start.x == trim_end.x)
				trim_end.x++;
			double trim_k = double(trim_start.y - trim_end.y) / double(trim_start.x - trim_end.x);
			intersection.x = (int)((k*start.x - trim_k*trim_start.x + trim_start.y - start.y) / (k - trim_k));
			intersection.y = (int)(start.y + (intersection.x - start.x)*k);
		}
		vector<Shape*> result;
		// 如果不和原直线两端点重合（5个像素误差），裁剪为两部分，返回结果集
		if (Distance(intersection, start) > size && Distance(intersection, end) > size) {
			Shape *rst1 = new StraightLine(start, intersection);
			Shape *rst2 = new StraightLine(intersection, end);
			result.push_back(rst1);
			result.push_back(rst2);
		}
		return result;
	}
	else {
		return vector<Shape*>();
	}
}

vector<Shape*> StraightLine::Trim(Shape * trim){
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

	CPoint p1 = points[0], p2 = points[1];

	vector<Shape*> result;

	while (true) {
		int c1 = Encode(min_x, max_x, min_y, max_y, p1);
		int c2 = Encode(min_x, max_x, min_y, max_y, p2);
		if ((c1&c2) != 0) {
			// 完全在窗口外 放弃
			break;
		}
		else if ((c1 | c2) == 0) {
			// 完全在窗口内 保留
			Shape *rst = new StraightLine(p1, p2);
			result.push_back(rst);
			break;
		}
		else {
			if (c1 == 0) {
				CPoint temp = p1;
				p1 = p2;
				p2 = temp;
				int c = c1;
				c1 = c2;
				c2 = c;
			}
			double k = (double)(p1.y - p2.y) / (double)(p1.x - p2.x);
			CPoint intersection;
			if ((c1 & 0x1) != 0) {
				intersection.x = min_x;
				intersection.y = int(k*(intersection.x - p2.x) + p2.y);
			}
			else if ((c1 & 0x2) != 0) {
				intersection.x = max_x;
				intersection.y = int(k*(intersection.x - p2.x) + p2.y);
			}
			else if ((c1 & 0x4) != 0) {
				intersection.y = min_y;
				intersection.x = int((intersection.y - p2.y) / k + p2.x);
			}
			else {
				intersection.y = max_y;
				intersection.x = int((intersection.y - p2.y) / k + p2.x);
			}
			p1 = intersection;
		}
	}
	
	return result;
}

void StraightLine::Zoom(int flag){
	// 放大比率为1.2，缩小比率为1/1.2。
	double rate = 1.2;
	rate = (flag == ZOOMIN) ? rate : 1.0 / rate;

	if ((abs(center.x - points[0].x) < 30 && abs(center.y - points[0].y) < 30) && flag ==ZOOMOUT)
		return;
	for (unsigned int i = 0; i < points.size(); i++) {
		points[i].x = center.x + (int)round((points[i].x - center.x) * rate);
		points[i].y = center.y + (int)round((points[i].y - center.y) * rate);
	}
	m_start = points[0];
	m_end = points[1];
}

void StraightLine::SetSpecialPoint(){
	m_start = points[0];
	m_end = points[1];
	center.x = (m_start.x + m_end.x) / 2;
	center.y = (m_start.y + m_end.y) / 2;
}

void StraightLine::Draw(CDC * pDC, COLORREF color){
	DrawPoints(pDC);
	CPoint start = points.at(0), end = points.at(1);

	int dx = end.x - start.x;
	int dy = end.y - start.y;
	int ux = ((dx > 0) << 1) - 1;			//x的增量方向，取或-1
	int uy = ((dy > 0) << 1) - 1;			//y的增量方向，取或-1
	dx = abs(dx); dy = abs(dy);
	if (dx > dy) {
		int p = dy * 2 - dx, p1 = 2 * (dy - dx), p2 = 2 * dy;
		int x = start.x, y = start.y;
		for (; x != end.x + ux; x += ux) {
			pDC->SetPixel(x, y, color);
			if (p >= 0) {
				y += uy;
				p += p1;
			}
			else
				p += p2;
		}
	}
	else {
		int p = dx * 2 - dy, p1 = 2 * (dx - dy), p2 = 2 * dx;
		int x = start.x, y = start.y;
		for (; y != end.y + uy; y += uy) {
			pDC->SetPixel(x, y, color);
			if (p >= 0) {
				x += ux;
				p += p1;
			}
			else
				p += p2;
		}
	}
}

void StraightLine::Update(CPoint start, CPoint end, int transform_mode) {
	if (transform_mode == MOVE) {
		int dx = end.x - start.x, dy = end.y - start.y;
		points[0].x += dx;
		points[0].y += dy;
		points[1].x += dx;
		points[1].y += dy;
		SetSpecialPoint();
	}
	// 缩放变换写成了编辑点的位置了
	else if (transform_mode == ZOOM) {
		int dx = end.x - start.x, dy = end.y - start.y;
		CPoint p0 = points.at(0), p1 = points.at(1);
		if (Distance(p0, start) < Distance(p1, start)) {
			points[0].x += dx;
			points[0].y += dy;
		}
		else {
			points[1].x += dx;
			points[1].y += dy;
		}
		SetSpecialPoint();
	}
	else if (transform_mode == ROTATE) {
		double a = Distance(start.x, start.y, center.x, center.y), b = Distance(end.x, end.y, center.x, center.y), c = Distance(start, end);
		if (a == 0 || b == 0) return;
		double angle = acos((a*a + b*b - c*c) / (2 * a * b));
		rotate_angle += angle;
		points[0].x = (int)((m_start.x - center.x)*cos(rotate_angle) - (m_start.y - center.y)*sin(rotate_angle) + center.x);
		points[0].y = (int)((m_start.x - center.x)*sin(rotate_angle) + (m_start.y - center.y)*cos(rotate_angle) + center.y);
		points[1].x = (int)((m_end.x - center.x)*cos(rotate_angle) - (m_end.y - center.y)*sin(rotate_angle) + center.x);
		points[1].y = (int)((m_end.x - center.x)*sin(rotate_angle) + (m_end.y - center.y)*cos(rotate_angle) + center.y);
	}
	else {
		;
	}
}

bool StraightLine::Contains(CPoint p) {
	CPoint start = points.at(0), end = points.at(1);
	if ((p.x - start.x)*(p.x - end.x) <= 0 && (p.y - start.y)*(p.y - end.y) <= 0)
		return true;
	else
		return false;
}

int StraightLine::TransformMode(CPoint p){
	CPoint start = points.at(0), end = points.at(1);
	if (Distance(start, p) < size || Distance(end,p) < size) 
		return ZOOM;			
	else if (Distance(start, p) + Distance(end, p) - Distance(start, end) < 1) 
		return ROTATE;			
	else if (Contains(p))
		return MOVE;		
	else 
		return NOTRANSFORM;
}

void StraightLine::Paint(CDC * pDC, CPoint start, CPoint end, COLORREF color){
	int dx = end.x - start.x;
	int dy = end.y - start.y;
	int ux = ((dx > 0) << 1) - 1;			//x的增量方向，取或-1
	int uy = ((dy > 0) << 1) - 1;			//y的增量方向，取或-1
	dx = abs(dx); dy = abs(dy);
	if (dx > dy) {
		int p = dy * 2 - dx, p1 = 2 * (dy - dx), p2 = 2 * dy;
		int x = start.x, y = start.y;
		for (; x != end.x + ux; x += ux) {
			pDC->SetPixel(x, y, color);
			if (p >= 0) {
				y += uy;
				p += p1;
			}
			else
				p += p2;
		}
	}
	else {
		int p = dx * 2 - dy, p1 = 2 * (dx - dy), p2 = 2 * dx;
		int x = start.x, y = start.y;
		for (; y != end.y + uy; y += uy) {
			pDC->SetPixel(x, y, color);
			if (p >= 0) {
				x += ux;
				p += p1;
			}
			else
				p += p2;
		}
	}
}
