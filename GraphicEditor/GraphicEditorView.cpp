
// GraphicEditorView.cpp : CGraphicEditorView 类的实现

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "GraphicEditor.h"
#endif

#include "GraphicEditorDoc.h"
#include "GraphicEditorView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGraphicEditorView

IMPLEMENT_DYNCREATE(CGraphicEditorView, CView)

BEGIN_MESSAGE_MAP(CGraphicEditorView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_STRAIGHT_LINE, &CGraphicEditorView::OnStraightLine)
	ON_COMMAND(ID_CURVE, &CGraphicEditorView::OnCurve)
	ON_COMMAND(ID_RECTANGLE, &CGraphicEditorView::OnRectangle)
	ON_COMMAND(ID_CIRCLE, &CGraphicEditorView::OnCircle)
	ON_COMMAND(ID_ELLIPSE, &CGraphicEditorView::OnEllipse)
	ON_COMMAND(ID_POLYGON, &CGraphicEditorView::OnPolygon)
	ON_UPDATE_COMMAND_UI(ID_STRAIGHT_LINE, &CGraphicEditorView::OnUpdateStraightLine)
	ON_UPDATE_COMMAND_UI(ID_CURVE, &CGraphicEditorView::OnUpdateCurve)
	ON_UPDATE_COMMAND_UI(ID_RECTANGLE, &CGraphicEditorView::OnUpdateRectangle)
	ON_UPDATE_COMMAND_UI(ID_CIRCLE, &CGraphicEditorView::OnUpdateCircle)
	ON_UPDATE_COMMAND_UI(ID_ELLIPSE, &CGraphicEditorView::OnUpdateEllipse)
	ON_UPDATE_COMMAND_UI(ID_POLYGON, &CGraphicEditorView::OnUpdatePolygon)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()

	ON_COMMAND(ID_PEN_COLOR, &CGraphicEditorView::OnPenColor)
	ON_COMMAND(ID_FILL_COLOR, &CGraphicEditorView::OnFillColor)
	ON_COMMAND(ID_SELECT, &CGraphicEditorView::OnSelect)
	ON_UPDATE_COMMAND_UI(ID_SELECT, &CGraphicEditorView::OnUpdateSelect)
	ON_COMMAND(ID_ZOOM_IN, &CGraphicEditorView::OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, &CGraphicEditorView::OnZoomOut)

	ON_COMMAND(ID_FILE_NEW, &CGraphicEditorView::OnFileNew)
	ON_COMMAND(ID_FILE_SAVE, &CGraphicEditorView::OnFileSave)
	ON_COMMAND(ID_FILL, &CGraphicEditorView::OnFill)
	ON_UPDATE_COMMAND_UI(ID_FILL, &CGraphicEditorView::OnUpdateFill)
	ON_COMMAND(ID_TRANSFORM, &CGraphicEditorView::OnTransform)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM, &CGraphicEditorView::OnUpdateTransform)
ON_COMMAND(ID_CUT, &CGraphicEditorView::OnCut)
ON_UPDATE_COMMAND_UI(ID_CUT, &CGraphicEditorView::OnUpdateCut)
ON_COMMAND(ID_TRIM, &CGraphicEditorView::OnTrim)
ON_UPDATE_COMMAND_UI(ID_TRIM, &CGraphicEditorView::OnUpdateTrim)
ON_COMMAND(ID_3D, &CGraphicEditorView::On3d)
ON_UPDATE_COMMAND_UI(ID_3D, &CGraphicEditorView::OnUpdate3d)
END_MESSAGE_MAP()

// CGraphicEditorView 构造/析构

CGraphicEditorView::CGraphicEditorView(){
	mode = -1;
	transform_mode = NOTRANSFORM;
	button_down = false;
	has_start = false;
	over = false;
	shape = NULL;
	cut = NULL;
}

CGraphicEditorView::~CGraphicEditorView(){

}

BOOL CGraphicEditorView::PreCreateWindow(CREATESTRUCT& cs){
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGraphicEditorView 打印

BOOL CGraphicEditorView::OnPreparePrinting(CPrintInfo* pInfo){

	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGraphicEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/){

	// TODO: 添加额外的打印前进行的初始化过程
}

void CGraphicEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/){

	// TODO: 添加打印后进行的清理过程
}


// CGraphicEditorView 诊断

#ifdef _DEBUG
void CGraphicEditorView::AssertValid() const{

	CView::AssertValid();
}

void CGraphicEditorView::Dump(CDumpContext& dc) const{

	CView::Dump(dc);
}

// 非调试版本是内联的
CGraphicEditorDoc* CGraphicEditorView::GetDocument() const {

	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicEditorDoc)));
	return (CGraphicEditorDoc*)m_pDocument;
}
#endif //_DEBUG

// 绘图
void CGraphicEditorView::OnDraw(CDC* pDC) {
	CGraphicEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// 设置双缓冲
	GetClientRect(&rect);
	h_BufDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	SelectObject(h_BufDC, memBitmap);
	h_BufDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255, 255, 255));

	// 显示所有图形
	for (unsigned int i = 0; i < shapes.size(); i++)
		shapes[i]->Display(&h_BufDC);

	// 显示切割线
	if (cut != NULL) 
		cut->Display(&h_BufDC);

	// 显示后删除缓冲
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &h_BufDC, 0, 0, SRCCOPY);//将暂存内容复制到显示器设备上下文中，从而显示  
	memBitmap.DeleteObject();       //删除位图对象  
	h_BufDC.DeleteDC();             //删除缓冲dc  

}

// 鼠标左键按下
void CGraphicEditorView::OnLButtonDown(UINT nFlags, CPoint point){
	current = point;
	button_down = true;

	// 根据不同模式创建不同的图形
	switch (mode) {
	case STRAIGHT_LINE:
		shape = new StraightLine(point,point);
		break;
	case RECTANGLE:
		shape = new MyRectangle(point, point, point, point);
		break;
	case CIRCLE:
		shape = new Circle(point, point, false);
		break;
	case ELLIPSE:
		shape = new MyEllipse(point, point, false);
		break;
	// 构造多边形，当前点的坐标在起始点5个像素的范围内判断为构造结束
	case POLYGON:
		if (!has_start) {
			shape = new MyPolygon();
			shape->points.push_back(point);
			shape->points.push_back(point);
			has_start = true;
		}
		else {
			CPoint start = shape->points[0];
			if ((pow((point.x - start.x), 2) + pow((point.y - start.y), 2)) < 25) {
				shape->points.at(shape->points.size() - 1) = start;
				shapes.push_back(shape);
				shape->SetSpecialPoint();
				has_start = false;
			}
			else {
				shape->points.at(shape->points.size() - 1) = point;
				shape->points.push_back(point);
			}
		}
		break;
	case CURVE:
		if (!has_start) {
			shape = new Curve();
			shape->points.push_back(point);
			has_start = true;
		}
		else {
			shape->points.push_back(point);
			if (shape->points.size() == 6) {
				shapes.push_back(shape);
				shape->SetSpecialPoint();
				has_start = false;
				mode = TRANSFORM;
			}
		}
		break;

	case THREED:
		if (a == 0) {
			shape = new Test3D(point);
			a++;
		}
		else if (a == 1) {
			shape->points[2] = point;
			a++;
		}
		else {
			shape->points[6] = point;
			shape->SetSpecialPoint();
			a = 0;
			shapes.push_back(shape);
		}			
		break;
	// 鼠标点击后进入不同的图形变换模式，进行操作。
	case TRANSFORM:
		start = point;
		if (transform_mode != NOTRANSFORM && shape!=NULL) 
			mode = transform_mode;
		break;

	// 生成切割线
	case CUT:
		if (cut != NULL) 
			delete cut;

		cut = new CutLine();
		cut->points.push_back(point);
		cut->points.push_back(point);
		over = false;
		break;
	// 生成裁剪窗口
	case TRIM:
		trim = new TrimWindow(point);
		shape = trim;
		break;
	default:
		break;
	}
	CView::OnLButtonDown(nFlags, point);
}

// 鼠标左键弹起
void CGraphicEditorView::OnLButtonUp(UINT nFlags, CPoint point) {
	current = point;
	button_down = false;

	switch (mode) {
	// 鼠标左键抬起后，更新直线，矩形，圆和椭圆的终点
	case STRAIGHT_LINE:
	case CIRCLE:
	case ELLIPSE:
		shape->points[1] = point;
		shape->SetSpecialPoint();
		shapes.push_back(shape);
		Invalidate();
		break;
	case RECTANGLE:
		shape->points[2] = point;
		shape->SetSpecialPoint();
		shapes.push_back(shape);
		Invalidate();
		break;
	case CURVE:
		Invalidate();
		break;
	case THREED:
		Invalidate();
		break;
	// 图形变换更新点的位置，结束后返回TRANSFORM（图形变换）模式
	case MOVE:
	case ZOOM:
	case ROTATE:
		mode = TRANSFORM;
		break;
	case FILL:
		for (unsigned int i = 0; i < shapes.size(); i++) {
			if (shapes[i]->Contains(current)) {
				shapes[i]->isFilled = true;
				mode = TRANSFORM;
			}
		}
		Invalidate();
		break;
	case SELECT:
		for (unsigned int i = 0; i < shapes.size(); i++) {
			if (shapes[i]->Contains(current)) {
				shape = shapes[i];
				shapes[i]->isSelected = true;
			}
			else {
				shapes[i]->isSelected = false;
			}
		}
		Invalidate();
		break;
	case CUT:
		cut->points[1] = point;
		cut->SetSpecialPoint();
		over = true;
		Invalidate();
		break;
	case TRIM:
		shape->points[2] = point;
		shape->SetSpecialPoint();
		Trim();
		shapes.push_back(shape);
		Invalidate();
		break;
	default:
		break;
	}
	
	CView::OnLButtonUp(nFlags, point);
}

// 鼠标移动
void CGraphicEditorView::OnMouseMove(UINT nFlags, CPoint point) {
	current = point;

	// 设置双缓冲
	CDC *pDC = GetDC();
	GetClientRect(&rect);
	h_BufDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	SelectObject(h_BufDC, memBitmap);
	h_BufDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255, 255, 255));

	// 显示所有图形
	for (unsigned int i = 0; i < shapes.size(); i++)
		shapes[i]->Display(&h_BufDC);

	if (cut != NULL)
		cut->Display(&h_BufDC);

	switch (mode) {
	// 直线，圆和椭圆的动态画图
	case STRAIGHT_LINE:
	case CIRCLE:
	case ELLIPSE:
		if (button_down) {
			shape->points[1] = point;
			shape->SetSpecialPoint();
			shape->Display(&h_BufDC);
		}
		break;
	// 矩形和裁剪窗口的动态画图
	case TRIM:
	case RECTANGLE:
		if (button_down) {
			shape->points[2] = point;
			shape->SetSpecialPoint();
			shape->Display(&h_BufDC);
		}
		break;
	// 多边形动态画图
	case POLYGON:
		if (has_start) {
			shape->points.at(shape->points.size() - 1) = point;
			shape->Display(&h_BufDC);
		}
		break;
	
	// 保证画曲线时每次点击都会显示图形
	case CURVE:
		if (shape != NULL)
			shape->Display(&h_BufDC);
		break;

	// 图形变换，根据鼠标样式决定图形变换的种类：移动，旋转还是缩放
	case TRANSFORM:
		if(shape!=NULL)
			transform_mode = shape->TransformMode(point);
		if (transform_mode == ZOOM)
			SetCursor(zoom_nwse);
		else if (transform_mode == ROTATE)
			SetCursor(rotate);
		else if (transform_mode == MOVE)
			SetCursor(move);
		else
			;
		break;

	// 移动，缩放和旋转的图形变换，更新图形中 点的位置
	case MOVE:
	case ZOOM:
	case ROTATE:
		shape->Update(start, point, transform_mode);
		start = point;
		break;

	// 切割线的动态画图
	case CUT:
		if (button_down) {
			cut->points[1] = point;
			cut->SetSpecialPoint();
		}
		break;
	default:
		break;
	}

	// 显示后删除缓冲
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &h_BufDC, 0, 0, SRCCOPY);//将暂存内容复制到显示器设备上下文中，从而显示  
	memBitmap.DeleteObject();       //删除位图对象  
	h_BufDC.DeleteDC();             //删除缓冲dc  
	ReleaseDC(pDC);
	CView::OnMouseMove(nFlags, point);
}

// 键盘响应
void CGraphicEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){

	switch (nChar) {
	case 'I':
		OnZoomIn();	break;
	case 'O':
		OnZoomOut();		break;
	case 'S':
		OnSelect();	break;
	case 'T':
		OnTransform(); break;
	case 'F':
		OnFill(); break;
	case 'C':
		OnCut(); break;
	case 'X':
		OnTrim(); break;
	case 'D':
		Delete(); break;
	case VK_SPACE:
		// 绘制好裁剪窗口后，按空格键进行裁剪，裁剪结束后裁剪窗口被删除掉，更新整个窗口
		if (mode == CUT && over) 
			Cut();
		// 曲线的控制点添加结束
		if (mode == CURVE) {
			shapes.push_back(shape);
			shape->SetSpecialPoint();
			has_start = false;
		}
		break;
	default:	
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

// 切割
void CGraphicEditorView::Cut(){
	vector<Shape*> result;
	for (vector<Shape*>::iterator it = shapes.begin(); it != shapes.end();) {
		vector<Shape*> temp = (*it)->Cut(cut);
		if (!temp.empty()) {
			for (unsigned int j = 0; j < temp.size(); j++) {
				result.push_back(temp[j]);
			}
			it = shapes.erase(it);
		}
		else {
			it++;
		}
	}
	for (unsigned int i = 0; i < result.size(); i++) {
		shapes.push_back(result[i]);
	}
	delete cut;
	cut = NULL;
	shape = NULL;
	over = false;
	Invalidate();
}

// 裁剪
void CGraphicEditorView::Trim(){
	for (unsigned int i = 0; i < shapes.size(); i++) {
		vector<Shape*> temp = shapes[i]->Trim(shape);
		if (!temp.empty()) {
			for (unsigned int j = 0; j < temp.size(); j++) {
				trim->AddShape(temp[j]);
			}
		}
	}
	Invalidate();
}

// 删除当前选中的图形
void CGraphicEditorView::Delete(){
	if (shape == NULL)
		return;

	for (vector<Shape*>::iterator it = shapes.begin(); it != shapes.end(); it++) {
		if ((*it) == shape) {
			shapes.erase(it);
			shape = NULL;
			mode = SELECT;
			Invalidate();
			break;
		}
	}
}

// 调整画笔颜色
void CGraphicEditorView::OnPenColor(){
	CColorDialog m_setClrDlg;
	m_setClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;   // CC_RGBINIT可以让上次选择的颜色作为初始颜色显示出来
	m_setClrDlg.m_cc.rgbResult = Shape::line_color;
	if (IDOK == m_setClrDlg.DoModal()){
		Shape::line_color = m_setClrDlg.m_cc.rgbResult;            // 保存用户选择的颜色
	}
}

// 调整填充颜色
void CGraphicEditorView::OnFillColor(){
	CColorDialog m_setClrDlg;
	m_setClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;   // CC_RGBINIT可以让上次选择的颜色作为初始颜色显示出来
	m_setClrDlg.m_cc.rgbResult = Shape::fill_color;
	if (IDOK == m_setClrDlg.DoModal()) {
		Shape::fill_color = m_setClrDlg.m_cc.rgbResult;            // 保存用户选择的颜色
	}
}

// 工具栏图形按钮对应的响应函数 改变当前输入图形的模式
void CGraphicEditorView::OnStraightLine() {
	mode = STRAIGHT_LINE;
}

void CGraphicEditorView::OnCurve() {
	mode = CURVE;
}

void CGraphicEditorView::OnRectangle() {
	mode = RECTANGLE;
}

void CGraphicEditorView::OnCircle() {
	mode = CIRCLE;
}

void CGraphicEditorView::OnEllipse() {
	mode = ELLIPSE;
}

void CGraphicEditorView::OnPolygon() {
	mode = POLYGON;
}

void CGraphicEditorView::OnUpdateStraightLine(CCmdUI *pCmdUI) {
	if (mode == STRAIGHT_LINE)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CGraphicEditorView::OnUpdateCurve(CCmdUI *pCmdUI) {
	if (mode == CURVE)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CGraphicEditorView::OnUpdateRectangle(CCmdUI *pCmdUI) {
	if (mode == RECTANGLE)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CGraphicEditorView::OnUpdateCircle(CCmdUI *pCmdUI) {
	if (mode == CIRCLE)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CGraphicEditorView::OnUpdateEllipse(CCmdUI *pCmdUI) {
	if (mode == ELLIPSE)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CGraphicEditorView::OnUpdatePolygon(CCmdUI *pCmdUI) {
	if (mode == POLYGON)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CGraphicEditorView::On3d(){
	mode = THREED;
}

void CGraphicEditorView::OnUpdate3d(CCmdUI *pCmdUI){
	if (mode == THREED)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

// 选择按钮，进入选择模式
void CGraphicEditorView::OnSelect() {
	mode = SELECT;
}

void CGraphicEditorView::OnUpdateSelect(CCmdUI *pCmdUI) {
	if (mode == SELECT)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

// 填充按钮，进入填充模式
void CGraphicEditorView::OnFill(){
	mode = FILL;
}

void CGraphicEditorView::OnUpdateFill(CCmdUI *pCmdUI){
	if (mode == FILL)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

// 图形变换按钮，进入变换模式
void CGraphicEditorView::OnTransform(){
	mode = TRANSFORM;
}

void CGraphicEditorView::OnUpdateTransform(CCmdUI *pCmdUI){
	if (mode == TRANSFORM)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

// 切割按钮，进入切割模式
void CGraphicEditorView::OnCut(){
	mode = CUT;
}

void CGraphicEditorView::OnUpdateCut(CCmdUI *pCmdUI){
	if (mode == CUT)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

// 裁剪按钮，进入裁剪模式
void CGraphicEditorView::OnTrim(){
	mode = TRIM;
}

void CGraphicEditorView::OnUpdateTrim(CCmdUI *pCmdUI){
	if (mode == TRIM)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

// 放大
void CGraphicEditorView::OnZoomIn() {
	if (shape == NULL)
		return;
	shape->Zoom(ZOOMIN);
	Invalidate();
}

// 缩小 
void CGraphicEditorView::OnZoomOut() {
	if (shape == NULL)
		return;
	shape->Zoom(ZOOMOUT);
	Invalidate();
}

// 新建文件
void CGraphicEditorView::OnFileNew(){
	mode = -1;
	button_down = false;
	has_start = false;
	over = false;
	shape = NULL;
	cut = NULL;
	shapes.clear();

	RedrawWindow();
}

// 保存文件
void CGraphicEditorView::OnFileSave(){
		//保存位图:可以保存工作区上的图片，不仅可以保存原有的位图文件，还可以保存在位图上绘制的图形
		CClientDC  dc(this);
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap bm;
		CRect rect;
		GetClientRect(&rect);
		int   Width = rect.Width();
		int   Height = rect.Height();
		bm.CreateCompatibleBitmap(&dc, Width, Height);
		CBitmap *pOld = memDC.SelectObject(&bm);
		memDC.BitBlt(0, 0, Width, Height, &dc, 0, 0, SRCCOPY);
		memDC.SelectObject(pOld);
		BITMAP     btm;
		bm.GetBitmap(&btm);
		DWORD     size = btm.bmWidthBytes   *   btm.bmHeight;
		LPSTR   lpData = (LPSTR)GlobalAlloc(GPTR, size);
		BITMAPFILEHEADER       bfh;
		BITMAPINFOHEADER     bih;
		bih.biBitCount = btm.bmBitsPixel;
		bih.biClrImportant = 0;
		bih.biClrUsed = 0;
		bih.biCompression = 0;
		bih.biHeight = btm.bmHeight;
		bih.biPlanes = 1;
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biSizeImage = size;
		bih.biWidth = btm.bmWidth;
		bih.biXPelsPerMeter = 0;
		bih.biYPelsPerMeter = 0;
		GetDIBits(dc, bm, 0, bih.biHeight, lpData, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
		bfh.bfReserved1 = bfh.bfReserved2 = 0;
		bfh.bfType = ((WORD)('M' << 8) | 'B');   //bfh.bfType=0x4D42;//bmp类型
		bfh.bfSize = 54 + size;
		bfh.bfOffBits = 54;
		CFileDialog   dlg(false, _T("BMP"), _T("*.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("*.bmp|*.bmp|*.*|*.*|"));
		if (dlg.DoModal() != IDOK)
			return;

		CFile bf;
		CString   ss = dlg.GetPathName();
		if (bf.Open(ss, CFile::modeCreate | CFile::modeWrite)){
			bf.Write(&bfh, sizeof(BITMAPFILEHEADER));
			bf.Write(&bih, sizeof(BITMAPINFOHEADER));
			bf.Write(lpData, size);
			bf.Close();
		}
		GlobalFree(lpData);
}

