Attribute VB_Name = "mdlSubClass"
Option Explicit

'�֐��錾
Public Declare Function SetWindowLong Lib "user32" _
    Alias "SetWindowLongA" (ByVal hwnd As Long, _
                            ByVal nIndex As Long, _
                            ByVal dwNewLong As Long) As Long
Public Declare Function CallWindowProc Lib "user32" _
    Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, _
                             ByVal hwnd As Long, _
                             ByVal Msg As Long, _
                             ByVal wParam As Long, _
                             ByVal lParam As Long) As Long

'�萔�錾
Private Const GWL_WNDPROC = (-4)

'�ϐ��錾
Private p_oldWndProc As Long

'�T�u�N���X���̊J�n
Public Sub StartSubClass(ByVal hwnd As Long)
    '���ɃT�u�N���X���ς݂̏ꍇ�́A�T�u�N���X���I��
    If p_oldWndProc <> 0 Then Call EndSubClass(hwnd)
    
    '�T�u�N���X���X�^�[�g
    p_oldWndProc = SetWindowLong(hwnd, GWL_WNDPROC, AddressOf WndProc)
End Sub

'�T�u�N���X���̏I��
Public Sub EndSubClass(ByVal hwnd As Long)
    If p_oldWndProc <> 0 Then _
        Call SetWindowLong(hwnd, GWL_WNDPROC, p_oldWndProc)
    p_oldWndProc = 0
End Sub

'�T�u�N���X�����̃E�B���h�E�v���V�[�W��
Public Function WndProc(ByVal hwnd As Long, _
                        ByVal uMsg As Long, _
                        ByVal wParam As Long, _
                        ByVal lParam As Long) As Long
    
    '���̃E�B���h�E�v���V�[�W�����Ăяo��
    WndProc = CallWindowProc(p_oldWndProc, hwnd, uMsg, wParam, lParam)
End Function

