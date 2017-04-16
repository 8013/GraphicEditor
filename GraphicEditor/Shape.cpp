#include "stdafx.h"
#include "Shape.h"
COLORREF Shape::line_color = RGB(0, 0, 0);
COLORREF Shape::fill_color = RGB(135, 206, 235);

Shape::Shape(){
	selected_color = RGB(255, 0, 0); 
	background_color = RGB(255, 255, 255);
	// 画刷用于画点
	brush.CreateSolidBrush(selected_color);
	// 与背景色相同，画点的时候没有黑色边框
	pen.CreatePen(PS_SOLID, 0, background_color);
	fill_pen.CreatePen(PS_SOLID, 0, fill_color);
	isFilled = false;
	isSelected = false;
}

Shape::~Shape(){
	brush.DeleteObject();
	pen.DeleteObject();
	fill_pen.DeleteObject();
}

void Shape::Display(CDC * pDC){
	if (isFilled) {
		fill_pen.DeleteObject();
		fill_pen.CreatePen(PS_SOLID, 0, fill_color);
		Fill(pDC, true);
	}

	brush.DeleteObject();
	brush.CreateSolidBrush(selected_color);
	if(isSelected)
		this->Draw(pDC, selected_color);
	else
		this->Draw(pDC, line_color);
}

double Shape::Distance(CPoint p1, CPoint p2){
	return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

double Shape::Distance(double x1, double y1, double x2, double y2){
	return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

double Shape::Determinant(double v1, double v2, double v3, double v4){
	return (v1*v4 - v2*v3);
}

bool Shape::HasIntersect(CPoint aa, CPoint bb, CPoint cc, CPoint dd){
	// aa,bb为一条线段端点，cc,dd为另一条线段端点
	double delta = Determinant(bb.x - aa.x, cc.x - dd.x, bb.y - aa.y, cc.y - dd.y);
	// delta=0，表示两线段重合或平行  
	if (delta <= (1e-6) && delta >= -(1e-6))  {
		return false;
	}
	double namenda = Determinant(cc.x - aa.x, cc.x - dd.x, cc.y - aa.y, cc.y - dd.y) / delta;
	if (namenda>1 || namenda<0){
		return false;
	}
	double miu = Determinant(bb.x - aa.x, cc.x - aa.x, bb.y - aa.y, cc.y - aa.y) / delta;
	if (miu>1 || miu<0){
		return false;
	}
	return true;
}

vector<CPoint> Shape::Left(vector<CPoint> p, int left) {
	vector<CPoint> result;
	if (p.empty())
		return result;
	for (unsigned int i = 0; i < p.size() - 1; i++) {
		CPoint p1 = p[i], p2 = p[i + 1];
		double k = (double)(p1.y - p2.y) / (double)(p1.x - p2.x);
		if (p1.x <= left && p2.x > left) {
			// intersection, p2
			CPoint intersection;
			intersection.x = left;
			intersection.y = int(k*(intersection.x - p2.x) + p2.y);
			result.push_back(intersection);
			result.push_back(p2);
		}
		else if (p1.x > left && p2.x > left) {
			// p2
			result.push_back(p2);
		}
		else if (p1.x > left && p2.x <= left) {
			// intersection
			CPoint intersection;
			intersection.x = left;
			intersection.y = int(k*(intersection.x - p2.x) + p2.y);
			result.push_back(intersection);
		}
		else {
			// none
		}
	}
	if (!result.empty()) {
		result.push_back(result[0]);
	}
	return result;
}

vector<CPoint> Shape::Right(vector<CPoint> p, int right) {
	vector<CPoint> result;
	if (p.empty())
		return result;
	for (unsigned int i = 0; i < p.size() - 1; i++) {
		CPoint p1 = p[i], p2 = p[i + 1];
		double k = (double)(p1.y - p2.y) / (double)(p1.x - p2.x);
		if (p1.x >= right && p2.x <right) {
			// intersection, p2
			CPoint intersection;
			intersection.x = right;
			intersection.y = int(k*(intersection.x - p2.x) + p2.y);
			result.push_back(intersection);
			result.push_back(p2);
		}
		else if (p1.x < right && p2.x <right) {
			// p2
			result.push_back(p2);
		}
		else if (p1.x <right && p2.x >= right) {
			// intersection
			CPoint intersection;
			intersection.x = right;
			intersection.y = int(k*(intersection.x - p2.x) + p2.y);
			result.push_back(intersection);
		}
		else {
			// none
		}
	}
	if (!result.empty()) {
		result.push_back(result[0]);
	}
	return result;
}

vector<CPoint> Shape::Top(vector<CPoint> p, int top) {
	vector<CPoint> result;
	if (p.empty())
		return result;
	for (unsigned int i = 0; i < p.size() - 1; i++) {
		CPoint p1 = p[i], p2 = p[i + 1];
		double k = (double)(p1.y - p2.y) / (double)(p1.x - p2.x);
		if (p1.y <= top && p2.y > top) {
			// intersection, p2
			CPoint intersection;
			intersection.y = top;
			intersection.x = int((intersection.y - p2.y) / k + p2.x);
			result.push_back(intersection);
			result.push_back(p2);
		}
		else if (p1.y > top && p2.y > top) {
			// p2
			result.push_back(p2);
		}
		else if (p1.y > top && p2.y <= top) {
			// intersection
			CPoint intersection;
			intersection.y = top;
			intersection.x = int((intersection.y - p2.y) / k + p2.x);
			result.push_back(intersection);
		}
		else {
			// none
		}
	}
	if (!result.empty()) {
		result.push_back(result[0]);
	}
	return result;
}

vector<CPoint> Shape::Buttom(vector<CPoint> p, int buttom) {
	vector<CPoint> result;
	if (p.empty())
		return result;
	for (unsigned int i = 0; i < p.size() - 1; i++) {
		CPoint p1 = p[i], p2 = p[i + 1];
		double k = (double)(p1.y - p2.y) / (double)(p1.x - p2.x);
		if (p1.y >= buttom && p2.y < buttom) {
			// intersection, p2
			CPoint intersection;
			intersection.y = buttom;
			intersection.x = int((intersection.y - p2.y) / k + p2.x);
			result.push_back(intersection);
			result.push_back(p2);
		}
		else if (p1.y < buttom && p2.y < buttom) {
			// p2
			result.push_back(p2);
		}
		else if (p1.y < buttom && p2.y >= buttom) {
			// intersection
			CPoint intersection;
			intersection.y = buttom;
			intersection.x = int((intersection.y - p2.y) / k + p2.x);
			result.push_back(intersection);
		}
		else {
			// none
		}
	}
	if (!result.empty()) {
		result.push_back(result[0]);
	}
	return result;
}
