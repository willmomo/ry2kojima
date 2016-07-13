// Meter.cpp: rkMeter �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "kjmutil.h"
#include "Meter.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

rkMeter::rkMeter()
{
	int i;

	m_hbr[0][0] = CreateSolidBrush(RGB(255, 0, 0));		// �ԓ_��
	m_hbr[0][1] = CreateSolidBrush(RGB(96, 0, 0));		// �ԏ���
	m_hbr[1][0] = CreateSolidBrush(RGB(255, 255, 0));	// ���_��
	m_hbr[1][1] = CreateSolidBrush(RGB(96, 96, 0));		// ������
	m_hbr[2][0] = CreateSolidBrush(RGB(0, 255, 128));	// �Γ_��
	m_hbr[2][1] = CreateSolidBrush(RGB(0, 96, 32));		// �Ώ���

	SetRect(&m_rc[0], 0, 0, 5, 13);
	for (i = 0; i < 20-1; i++) {
		CopyRect(&m_rc[i+1], &m_rc[i]);
		OffsetRect(&m_rc[i+1], 5, 0);
	}
}

rkMeter::~rkMeter()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 2; j++)
			DeleteObject(m_hbr[i][j]);
}

//////////////////////////////////////////////////////////////////////
// ���[�^�[��C�ӂ̈ʒu�Ɉړ�

void rkMeter::Offset(int x, int y)
{
	for (int i = 0; i < 20; i++) {
		OffsetRect(&m_rc[i], x, y);
	}
}

//////////////////////////////////////////////////////////////////////
// n�Ԗڂ̎l�p�`��`��

BOOL rkMeter::DrawRect(HDC hdc, int n)
{
	return Rectangle(hdc, m_rc[n].left, m_rc[n].top, m_rc[n].right, m_rc[n].bottom);
}

//////////////////////////////////////////////////////////////////////
// �p�[�Z���g�Ɉ�v�������[�^�[��`��

BOOL rkMeter::DrawMeter(HDC hdc, int per)
{
	static int colIndex[] = {0,0,0,0,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2};
	int i;
	int light;

	for (i = 0; i < 20; i++) {
		light = (per > (i * 5)) ? 0 : 1;

		SelectObject(hdc, m_hbr[colIndex[i]][light]);
		DrawRect(hdc, i);
	}

	return TRUE;
}
