
// GraphicEditorView.cpp : CGraphicEditorView ���ʵ��

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
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

// CGraphicEditorView ����/����

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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CGraphicEditorView ��ӡ

BOOL CGraphicEditorView::OnPreparePrinting(CPrintInfo* pInfo){

	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CGraphicEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/){

	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CGraphicEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/){

	// TODO: ��Ӵ�ӡ����е��������
}


// CGraphicEditorView ���

#ifdef _DEBUG
void CGraphicEditorView::AssertValid() const{

	CView::AssertValid();
}

void CGraphicEditorView::Dump(CDumpContext& dc) const{

	CView::Dump(dc);
}

// �ǵ��԰汾��������
CGraphicEditorDoc* CGraphicEditorView::GetDocument() const {

	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicEditorDoc)));
	return (CGraphicEditorDoc*)m_pDocument;
}
#endif //_DEBUG

// ��ͼ
void CGraphicEditorView::OnDraw(CDC* pDC) {
	CGraphicEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// ����˫����
	GetClientRect(&rect);
	h_BufDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	SelectObject(h_BufDC, memBitmap);
	h_BufDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255, 255, 255));

	// ��ʾ����ͼ��
	for (unsigned int i = 0; i < shapes.size(); i++)
		shapes[i]->Display(&h_BufDC);

	// ��ʾ�и���
	if (cut != NULL) 
		cut->Display(&h_BufDC);

	// ��ʾ��ɾ������
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &h_BufDC, 0, 0, SRCCOPY);//���ݴ����ݸ��Ƶ���ʾ���豸�������У��Ӷ���ʾ  
	memBitmap.DeleteObject();       //ɾ��λͼ����  
	h_BufDC.DeleteDC();             //ɾ������dc  

}

// ����������
void CGraphicEditorView::OnLButtonDown(UINT nFlags, CPoint point){
	current = point;
	button_down = true;

	// ���ݲ�ͬģʽ������ͬ��ͼ��
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
	// �������Σ���ǰ�����������ʼ��5�����صķ�Χ���ж�Ϊ�������
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
	// ���������벻ͬ��ͼ�α任ģʽ�����в�����
	case TRANSFORM:
		start = point;
		if (transform_mode != NOTRANSFORM && shape!=NULL) 
			mode = transform_mode;
		break;

	// �����и���
	case CUT:
		if (cut != NULL) 
			delete cut;

		cut = new CutLine();
		cut->points.push_back(point);
		cut->points.push_back(point);
		over = false;
		break;
	// ���ɲü�����
	case TRIM:
		trim = new TrimWindow(point);
		shape = trim;
		break;
	default:
		break;
	}
	CView::OnLButtonDown(nFlags, point);
}

// ����������
void CGraphicEditorView::OnLButtonUp(UINT nFlags, CPoint point) {
	current = point;
	button_down = false;

	switch (mode) {
	// ������̧��󣬸���ֱ�ߣ����Σ�Բ����Բ���յ�
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
	// ͼ�α任���µ��λ�ã������󷵻�TRANSFORM��ͼ�α任��ģʽ
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

// ����ƶ�
void CGraphicEditorView::OnMouseMove(UINT nFlags, CPoint point) {
	current = point;

	// ����˫����
	CDC *pDC = GetDC();
	GetClientRect(&rect);
	h_BufDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	SelectObject(h_BufDC, memBitmap);
	h_BufDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255, 255, 255));

	// ��ʾ����ͼ��
	for (unsigned int i = 0; i < shapes.size(); i++)
		shapes[i]->Display(&h_BufDC);

	if (cut != NULL)
		cut->Display(&h_BufDC);

	switch (mode) {
	// ֱ�ߣ�Բ����Բ�Ķ�̬��ͼ
	case STRAIGHT_LINE:
	case CIRCLE:
	case ELLIPSE:
		if (button_down) {
			shape->points[1] = point;
			shape->SetSpecialPoint();
			shape->Display(&h_BufDC);
		}
		break;
	// ���κͲü����ڵĶ�̬��ͼ
	case TRIM:
	case RECTANGLE:
		if (button_down) {
			shape->points[2] = point;
			shape->SetSpecialPoint();
			shape->Display(&h_BufDC);
		}
		break;
	// ����ζ�̬��ͼ
	case POLYGON:
		if (has_start) {
			shape->points.at(shape->points.size() - 1) = point;
			shape->Display(&h_BufDC);
		}
		break;
	
	// ��֤������ʱÿ�ε��������ʾͼ��
	case CURVE:
		if (shape != NULL)
			shape->Display(&h_BufDC);
		break;

	// ͼ�α任�����������ʽ����ͼ�α任�����ࣺ�ƶ�����ת��������
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

	// �ƶ������ź���ת��ͼ�α任������ͼ���� ���λ��
	case MOVE:
	case ZOOM:
	case ROTATE:
		shape->Update(start, point, transform_mode);
		start = point;
		break;

	// �и��ߵĶ�̬��ͼ
	case CUT:
		if (button_down) {
			cut->points[1] = point;
			cut->SetSpecialPoint();
		}
		break;
	default:
		break;
	}

	// ��ʾ��ɾ������
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &h_BufDC, 0, 0, SRCCOPY);//���ݴ����ݸ��Ƶ���ʾ���豸�������У��Ӷ���ʾ  
	memBitmap.DeleteObject();       //ɾ��λͼ����  
	h_BufDC.DeleteDC();             //ɾ������dc  
	ReleaseDC(pDC);
	CView::OnMouseMove(nFlags, point);
}

// ������Ӧ
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
		// ���ƺòü����ں󣬰��ո�����вü����ü�������ü����ڱ�ɾ������������������
		if (mode == CUT && over) 
			Cut();
		// ���ߵĿ��Ƶ���ӽ���
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

// �и�
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

// �ü�
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

// ɾ����ǰѡ�е�ͼ��
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

// ����������ɫ
void CGraphicEditorView::OnPenColor(){
	CColorDialog m_setClrDlg;
	m_setClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;   // CC_RGBINIT�������ϴ�ѡ�����ɫ��Ϊ��ʼ��ɫ��ʾ����
	m_setClrDlg.m_cc.rgbResult = Shape::line_color;
	if (IDOK == m_setClrDlg.DoModal()){
		Shape::line_color = m_setClrDlg.m_cc.rgbResult;            // �����û�ѡ�����ɫ
	}
}

// ���������ɫ
void CGraphicEditorView::OnFillColor(){
	CColorDialog m_setClrDlg;
	m_setClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;   // CC_RGBINIT�������ϴ�ѡ�����ɫ��Ϊ��ʼ��ɫ��ʾ����
	m_setClrDlg.m_cc.rgbResult = Shape::fill_color;
	if (IDOK == m_setClrDlg.DoModal()) {
		Shape::fill_color = m_setClrDlg.m_cc.rgbResult;            // �����û�ѡ�����ɫ
	}
}

// ������ͼ�ΰ�ť��Ӧ����Ӧ���� �ı䵱ǰ����ͼ�ε�ģʽ
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

// ѡ��ť������ѡ��ģʽ
void CGraphicEditorView::OnSelect() {
	mode = SELECT;
}

void CGraphicEditorView::OnUpdateSelect(CCmdUI *pCmdUI) {
	if (mode == SELECT)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

// ��䰴ť���������ģʽ
void CGraphicEditorView::OnFill(){
	mode = FILL;
}

void CGraphicEditorView::OnUpdateFill(CCmdUI *pCmdUI){
	if (mode == FILL)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

// ͼ�α任��ť������任ģʽ
void CGraphicEditorView::OnTransform(){
	mode = TRANSFORM;
}

void CGraphicEditorView::OnUpdateTransform(CCmdUI *pCmdUI){
	if (mode == TRANSFORM)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

// �иť�������и�ģʽ
void CGraphicEditorView::OnCut(){
	mode = CUT;
}

void CGraphicEditorView::OnUpdateCut(CCmdUI *pCmdUI){
	if (mode == CUT)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

// �ü���ť������ü�ģʽ
void CGraphicEditorView::OnTrim(){
	mode = TRIM;
}

void CGraphicEditorView::OnUpdateTrim(CCmdUI *pCmdUI){
	if (mode == TRIM)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

// �Ŵ�
void CGraphicEditorView::OnZoomIn() {
	if (shape == NULL)
		return;
	shape->Zoom(ZOOMIN);
	Invalidate();
}

// ��С 
void CGraphicEditorView::OnZoomOut() {
	if (shape == NULL)
		return;
	shape->Zoom(ZOOMOUT);
	Invalidate();
}

// �½��ļ�
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

// �����ļ�
void CGraphicEditorView::OnFileSave(){
		//����λͼ:���Ա��湤�����ϵ�ͼƬ���������Ա���ԭ�е�λͼ�ļ��������Ա�����λͼ�ϻ��Ƶ�ͼ��
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
		bfh.bfType = ((WORD)('M' << 8) | 'B');   //bfh.bfType=0x4D42;//bmp����
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

