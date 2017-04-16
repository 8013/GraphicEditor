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
	// ����βü��㷨���ĸ��߽紦�������ɾ��κͶ�����������
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

	// �����麯��������̳�ʵ�֣�δ�̳еı�������ͼ��û��ʵ����Ӧ���ܡ�

	virtual vector<Shape*> Cut(Shape *cut) { return vector<Shape*>(); }				// �и�
	virtual vector<Shape*> Trim(Shape *trim){ return vector<Shape*>(); }			// �ü�
	virtual void Zoom(int flag) { ; }																			// ����
	virtual void SetSpecialPoint() { ; }																		// ���������
	virtual void Draw(CDC *pDC, COLORREF color) { ; }											// ����
	virtual void Fill(CDC *pDC, bool fill) { ; }																// ���
	virtual void Update(CPoint start, CPoint end, int transform_mode) { ; }				// ͼ�α任 ���¡���ı༭
	virtual bool Contains(CPoint p) { return false; }													// �ж��Ƿ������P
	virtual int TransformMode(CPoint p) { return -1; }												// ����ƶ����ض�λ�øı���ʽ��������Ӧ�ı任ģʽ
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