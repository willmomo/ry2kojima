Option Explicit

Const vbHide = 0				'�E�B���h�E���\���ɂ��܂�
Const vbNormalFocus = 1			'�ʏ�̃E�B���h�E�A���őO�ʂ̃E�B���h�E�ɂ��܂�
Const vbMinimizedFocus = 2		'�ŏ����A���őO�ʂ̃E�B���h�E�ɂ��܂�
Const vbMaximizedFocus = 3		'�ő剻�A���őO�ʂ̃E�B���h�E�ɂ��܂�
Const vbNormalNoFocus = 4		'�ʏ�̃E�B���h�E�ł��B�������A�őO�ʂɂ͂Ȃ�܂���
Const vbMinimizedNoFocus = 6	'�ŏ������܂��B�������A�őO�ʂɂ͂Ȃ�܂���

Function WshShell_Run(strCommand, intWindowStyle, bWaitOnReturn)
	Dim oShell
	Set oShell = WScript.CreateObject("WScript.Shell")
	WshShell_Run = oShell.Run(strCommand, intWindowStyle, bWaitOnReturn)
	Set oShell = Nothing
End Function


