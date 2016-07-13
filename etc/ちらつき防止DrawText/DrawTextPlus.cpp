//---------------------------------------------------------------------
// Memory DC ���g���āA�������}����DrawText
//
// [param]
// hDC	     DrawText�ɓ���
// lpString  DrawText�ɓ���(null terminate)
// lpRect    DrawText�ɓ���
// uFormat   DrawText�ɓ���
// crText    SetTextColor�Ɏg���F
// crBack    CreateSolidBrush�Ɏg���F
// hfont     �`��Ɏg���t�H���g
// edge      DrawEdge�ɓ���(EDGE_RAISED, EDGE_SUNKEN, etc.)
//
// [return]
// DrawText�ɓ���(0�ŃG���[)
//---------------------------------------------------------------------
int DrawTextPlus(HDC hDC, LPCTSTR lpString, LPRECT lpRect, UINT uFormat, COLORREF crText, COLORREF crBack, HFONT hfont, UINT edge = 0) {
	int ret = 0;
	int cx = lpRect->right - lpRect->left;
	int cy = lpRect->bottom - lpRect->top;
	RECT rc = {0, 0, cx, cy};

	HDC hdcMem = CreateCompatibleDC(hDC);
	if (hdcMem) {
		HBITMAP hBitmap = CreateCompatibleBitmap(hDC, cx, cy);
		if (hBitmap) {
			HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

			if (HBRUSH hbrBack = CreateSolidBrush(crBack)) {
				FillRect(hdcMem, &rc, hbrBack);
				DeleteObject(hbrBack);
			}

			SetBkMode(hdcMem, TRANSPARENT);
			SetTextColor(hdcMem, crText);
			HFONT hfontOld = (HFONT)SelectObject(hdcMem, hfont);
			ret = DrawText(hdcMem, lpString, -1, &rc, uFormat);
			SelectObject(hdcMem, hfontOld);

			if (edge != 0) {
				DrawEdge(hdcMem, &rc, edge, BF_RECT | BF_SOFT);
			}

			BitBlt(hDC, lpRect->left, lpRect->top, cx, cy, hdcMem, 0, 0, SRCCOPY);

			SelectObject(hdcMem, hBitmapOld);
			DeleteObject(hBitmap);
		}
		DeleteDC(hdcMem);
	}

	return ret;
}
