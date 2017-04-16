#pragma once
#include "Shape.h"

class StraightLine :	public Shape{
protected:
	void DrawPoints(CDC *pDC);
	// 裁剪算法的端点编码函数
	int Encode(int min_x, int max_x, int min_y, int max_y, CPoint p);
	CPoint center, m_start, m_end;
	double rotate_angle;
public:
	StraightLine();
	StraightLine(CPoint start, CPoint end);
	vector<Shape*> Cut(Shape *cut);
	vector<Shape*> Trim(Shape *trim);

	void Zoom(int flag);
	void SetSpecialPoint();
	void Draw(CDC *pDC, COLORREF color);
	void Update(CPoint start, CPoint end, int transform_mode);
	bool Contains(CPoint p);
	int TransformMode(CPoint p);

	// 供其他图形画直线所用
	static void Paint(CDC * pDC, CPoint start, CPoint end, COLORREF color);
};

