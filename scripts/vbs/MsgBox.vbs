Option Explicit

Dim result

'result = MsgBox("[OK] �{�^���݂̂�\�����܂��B", vbOKOnly)
'result = MsgBox("[OK] �{�^���� [�L�����Z��] �{�^����\�����܂��B", vbOKCancel)
'result = MsgBox("[���~]�A[�Ď��s]�A����� [����] �� 3 �̃{�^����\�����܂��B", vbAbortRetryIgnore)
'result = MsgBox("[�͂�]�A[������]�A����� [�L�����Z��] �� 3 �̃{�^����\�����܂��B", vbYesNoCancel)
'result = MsgBox("[�͂�] �{�^���� [������] �{�^����\�����܂��B", vbYesNo)
'result = MsgBox("[�Ď��s] �{�^���� [�L�����Z��] �{�^����\�����܂��B", vbRetryCancel)

'result = MsgBox("�x�����b�Z�[�W �A�C�R����\�����܂��B", vbCritical Or vbOKOnly)
'result = MsgBox("�₢���킹���b�Z�[�W �A�C�R����\�����܂��B", vbQuestion Or vbOKOnly)
'result = MsgBox("���Ӄ��b�Z�[�W �A�C�R����\�����܂��B", vbExclamation Or vbOKOnly)
'result = MsgBox("��񃁃b�Z�[�W �A�C�R����\�����܂��B", vbInformation Or vbOKOnly)

'result = MsgBox("�� 1 �{�^����W���{�^���ɂ��܂��B", vbDefaultButton1 Or vbYesNoCancel)
'result = MsgBox("�� 2 �{�^����W���{�^���ɂ��܂��B", vbDefaultButton2 Or vbYesNoCancel)
'result = MsgBox("�� 3 �{�^����W���{�^���ɂ��܂��B", vbDefaultButton3 Or vbYesNoCancel)
'result = MsgBox("�� 4 �{�^����W���{�^���ɂ��܂��B", vbDefaultButton4 Or vbYesNoCancel)

'result = MsgBox("�A�v���P�[�V���� ���[�_���ɐݒ肵�܂��B���b�Z�[�W �{�b�N�X�ɉ�������܂ŁA���ݑI�𒆂̃A�v���P�[�V�����̎��s���p���ł��܂���B", vbApplicationModal Or vbOKOnly)
'result = MsgBox("�V�X�e�� ���[�_���ɐݒ肵�܂��B���b�Z�[�W �{�b�N�X�ɉ�������܂ŁA���ׂẴA�v���P�[�V���������f����܂��B", vbSystemModal Or vbOKOnly)


result = MsgBox("���\�W���I�ȁA���b�Z�[�W�{�b�N�X�̏����B", vbQuestion Or vbYesNoCancel, WScript.ScriptName)
Select Case result
Case vbOK
	MsgBox "�I�����ꂽ�{�^���F[OK]", vbInformation Or vbOKOnly, WScript.ScriptName
Case vbCancel
	MsgBox "�I�����ꂽ�{�^���F[�L�����Z��]", vbInformation Or vbOKOnly, WScript.ScriptName
Case vbAbort
	MsgBox "�I�����ꂽ�{�^���F[���~]", vbInformation Or vbOKOnly, WScript.ScriptName
Case vbRetry
	MsgBox "�I�����ꂽ�{�^���F[�Ď��s]", vbInformation Or vbOKOnly, WScript.ScriptName
Case vbIgnore
	MsgBox "�I�����ꂽ�{�^���F[����]", vbInformation Or vbOKOnly, WScript.ScriptName
Case vbYes
	MsgBox "�I�����ꂽ�{�^���F[�͂�]", vbInformation Or vbOKOnly, WScript.ScriptName
Case vbNo
	MsgBox "�I�����ꂽ�{�^���F[������]", vbInformation Or vbOKOnly, WScript.ScriptName
Case Else
	MsgBox "�I�����ꂽ�{�^���F�s��(" & result & ")", vbInformation Or vbOKOnly, WScript.ScriptName
End Select
