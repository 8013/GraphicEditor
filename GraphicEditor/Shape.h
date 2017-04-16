#pragma once
#include <vector>
#include <algorithm>
using namespace std;

#define PI 3.1415926535897932
enum { STRAIGHT_LINE, CURVE, RECTANGLE, CIRCLE, ELLIPSE, POLYGON, SELECT, TRANSFORM, MOVE, ROTATE, ZOOM, NOTRANSFORM, EDIT, FILL, ZOOMIN, ZOOMOUT, TRIM, CUT, THREED};

class Shape{
protected:
	int size=5; 
	CBrush brush;
	CPen pen,fill_pen;
	COLORREF selected_color, background_color;
	double Distance(CPoint p1, CPoint p2);
	double Distance(double x1, double y1, double x2, double y2);
	double Determinant(double v1, double v2, double v3, double v4);
	bool HasIntersect(CPoint aa, CPoint bb, CPoint cc, CPoint dd);
	// 多边形裁剪算法的四个边界处理函数，由矩形和多边形子类调用
	vector<CPoint> Left(vector<CPoint> p, int left);
	vector<CPoint> Right(vector<CPoint> p, int right);
	vector<CPoint> Top(vector<CPoint> p, int top);
	vector<CPoint> Buttom(vector<CPoint> p, int buttom);
public:
	static COLORREF line_color, fill_color;
	bool isSelected, isFilled;
	vector<CPoint> points;
public:
	Shape();
	~Shape();
	void Display(CDC *pDC);

	// 多种虚函数共子类继承实现，未继承的表名该类图形没有实现响应功能。

	virtual vector<Shape*> Cut(Shape *cut) { return vector<Shape*>(); }				// 切割
	virtual vector<Shape*> Trim(Shape *trim){ return vector<Shape*>(); }			// 裁剪
	virtual void Zoom(int flag) { ; }																			// 缩放
	virtual void SetSpecialPoint() { ; }																		// 设置特殊点
	virtual void Draw(CDC *pDC, COLORREF color) { ; }											// 绘制
	virtual void Fill(CDC *pDC, bool fill) { ; }																// 填充
	virtual void Update(CPoint start, CPoint end, int transform_mode) { ; }				// 图形变换 更新。点的编辑
	virtual bool Contains(CPoint p) { return false; }													// 判断是否包含点P
	virtual int TransformMode(CPoint p) { return -1; }												// 鼠标移动到特定位置改变样式，代表相应的变换模式
};

class Edge {
public:
	int no;
	CPoint start, end;
	double dx;
	Edge(int no, CPoint start, CPoint end) {
		this->no = no;
		this->start = start;
		this->end = end;
		this->dx = (double(end.x - start.x) / double(end.y - start.y));
	}
};