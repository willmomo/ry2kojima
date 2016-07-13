//---------------------------------------------------------------------
// Memory DC を使って、ちらつきを抑えたDrawText
//
// [param]
// hDC	     DrawTextに同じ
// lpString  DrawTextに同じ(null terminate)
// lpRect    DrawTextに同じ
// uFormat   DrawTextに同じ
// crText    SetTextColorに使う色
// crBack    CreateSolidBrushに使う色
// hfont     描画に使うフォント
// edge      DrawEdgeに同じ(EDGE_RAISED, EDGE_SUNKEN, etc.)
//
// [return]
// DrawTextに同じ(0でエラー)
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
