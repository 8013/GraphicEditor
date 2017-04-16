#include "stdafx.h"
#include "CutLine.h"


CutLine::CutLine(){
	dash.CreatePen(PS_DASH, 1, selected_color);
}

CutLine::~CutLine(){
	dash.DeleteObject();
}

void CutLine::Draw(CDC * pDC, COLORREF color){
	pDC->SelectObject(&dash);
	pDC->MoveTo(points[0]);
	pDC->LineTo(points[1]);
}
