#include "stdafx.h"
#include "TrimWindow.h"

TrimWindow::TrimWindow(CPoint p){
	dash.CreatePen(PS_DASH, 1, selected_color);

	for (int i = 0; i < 4; i++) {
		points.push_back(p);
	}
}

TrimWindow::~TrimWindow(){
	dash.DeleteObject();
	for (unsigned int i = 0; i < inner.size(); i++) {
		delete inner[i];
	}
}

void TrimWindow::AddShape(Shape *shape){
	inner.push_back(shape);
}

void TrimWindow::Draw(CDC * pDC, COLORREF color){
	pDC->SelectObject(&dash);

	pDC->MoveTo(points[0]);
	pDC->LineTo(points[1]);

	pDC->MoveTo(points[0]);
	pDC->LineTo(points[3]);

	pDC->MoveTo(points[2]);
	pDC->LineTo(points[1]);

	pDC->MoveTo(points[2]);
	pDC->LineTo(points[3]);

	for (unsigned int i = 0; i < inner.size(); i++) {
		inner[i]->Display(pDC);
	}
}

void TrimWindow::Fill(CDC * pDC, bool fill){
	for (unsigned int i = 0; i < inner.size(); i++) {
		inner[i]->Fill(pDC, fill);
	}
}

void TrimWindow::Update(CPoint start, CPoint end, int transform_mode){
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
		for (unsigned int i = 0; i < inner.size(); i++) {
			inner[i]->Update(start, end, transform_mode);
		}
	}
	else {
		;
	}

}
