'------------------------------------------------------------------------------
' PAPIMO�ɃA�b�v���[�h����ZIP�t�@�C���̃p�X���[�h�����߂�X�N���v�g
'
' 2011/06/27 r.kojima
'
' zip�̃p�X���[�h�ɂ��Ă�
' [YYYYMMDD][�����X�R�[�h]
' ��YYYYMMDD�͈ȉ��̈Í������s�����Ƃ���B
' 
' Y �� 40�����Z���e�L�X�g��
' Y �� 50�����Z���e�L�X�g��
' Y �� 60�����Z���e�L�X�g��
' Y �� 70�����Z���e�L�X�g��
' M �� 80�����Z���e�L�X�g��
' M �� 90�����Z���e�L�X�g��
' D �� 100�����Z���e�L�X�g��
' D �� 110�����Z���e�L�X�g��
'
' ��) Y��2�̎��A2+40 = 42 -> 0x2a �u*�v�ƂȂ�B
'
' �I���W�i���̃h�L�������g�́A
' \\Ts00\share\@�J��\document\_Projects\A1000296-00_�p�s���V�K�C�h�@��f�[�^�\���Ή� (���Y)\0001_�v����`\�O���Ƃ̂��Ƃ�\IFPC.�ۑ�ꗗ20110311.xls
' �ɂȂ�܂��B
'------------------------------------------------------------------------------

Set fso = WScript.CreateObject("Scripting.FileSystemObject")

For i = 0 To WScript.Arguments.Count - 1
	yyyymmdd = Mid(fso.GetFileName(WScript.Arguments(0)), 9, 8)

	zipPassword = ""

	For j = 0 To 7
		zipPassword = zipPassword & Chr(Int(Mid(yyyymmdd, j + 1, 1)) + (40 + 10 * j))
	Next

	WScript.Echo fso.GetFileName(WScript.Arguments(i)) & " �̃p�X���[�h [" & zipPassword & Left(fso.GetFileName(WScript.Arguments(0)), 8) & "]"
Next
