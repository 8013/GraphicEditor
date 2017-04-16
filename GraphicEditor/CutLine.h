#pragma once
#include "StraightLine.h"

class CutLine : public StraightLine{
private:
	CPen dash;
public:
	CutLine();
	~CutLine();
	void Draw(CDC *pDC, COLORREF color);
};

