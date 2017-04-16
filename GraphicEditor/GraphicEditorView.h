
// GraphicEditorView.h : CGraphicEditorView ��Ľӿ�

#pragma once
#include "Common.h"
class CGraphicEditorView : public CView{

protected: // �������л�����
	CGraphicEditorView();
	DECLARE_DYNCREATE(CGraphicEditorView)

// ����
public:
	CGraphicEditorDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CGraphicEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

	// ��Ա�����ͳ�Ա����
private:
	int a = 0;
	int mode,transform_mode;
	bool button_down, has_start, over;
	CPoint start,current;
	Shape *shape;
	vector<Shape*> shapes;

	CutLine *cut;
	TrimWindow *trim;

	HCURSOR move=LoadCursor(NULL, IDC_SIZEALL);
	HCURSOR rotate = LoadCursor(NULL, IDC_HAND);
	HCURSOR zoom_nwse = LoadCursor(NULL, IDC_SIZENWSE);
	HCURSOR zoom_nesw = LoadCursor(NULL, IDC_SIZENESW);

	CRect rect;
	CDC	h_BufDC;							// ˫�����  
	CBitmap memBitmap;            //����һ��λͼ����  

	void Cut();
	void Trim();
	void Delete();
public:
	afx_msg void OnStraightLine();
	afx_msg void OnCurve();
	afx_msg void OnRectangle();
	afx_msg void OnCircle();
	afx_msg void OnEllipse();
	afx_msg void OnPolygon();
	afx_msg void OnUpdateStraightLine(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCurve(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRectangle(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCircle(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEllipse(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePolygon(CCmdUI *pCmdUI);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnPenColor();
	afx_msg void OnSelect();
	afx_msg void OnUpdateSelect(CCmdUI *pCmdUI);
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnFileSave();
	afx_msg void OnFileNew();
	afx_msg void OnFill();
	afx_msg void OnUpdateFill(CCmdUI *pCmdUI);
	afx_msg void OnFillColor();
	afx_msg void OnTransform();
	afx_msg void OnUpdateTransform(CCmdUI *pCmdUI);
	afx_msg void OnCut();
	afx_msg void OnUpdateCut(CCmdUI *pCmdUI);
	afx_msg void OnTrim();
	afx_msg void OnUpdateTrim(CCmdUI *pCmdUI);
	afx_msg void On3d();
	afx_msg void OnUpdate3d(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // GraphicEditorView.cpp �еĵ��԰汾
inline CGraphicEditorDoc* CGraphicEditorView::GetDocument() const
   { return reinterpret_cast<CGraphicEditorDoc*>(m_pDocument); }
#endif

