VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsPanelExt"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'----------------------------------------------------------------------
' [�T�v]
' XML�ɏ����ꂽ���e���w�肳�ꂽDC�ɕ`�悷��B
'----------------------------------------------------------------------

Option Explicit

Private Declare Function SetRect Lib "user32" ( _
    lpRect As RECT, _
    ByVal X1 As Long, _
    ByVal Y1 As Long, _
    ByVal X2 As Long, _
    ByVal Y2 As Long _
) As Long

Private Declare Function GetPrivateProfileString Lib "kernel32" _
    Alias "GetPrivateProfileStringA" ( _
        ByVal lpApplicationName As String, _
        ByVal lpKeyName As Any, _
        ByVal lpDefault As String, _
        ByVal lpReturnedString As String, _
        ByVal nSize As Long, _
        ByVal lpFileName As String _
) As Long

Private Declare Function GetPrivateProfileSection Lib "kernel32" _
    Alias "GetPrivateProfileSectionA" ( _
        ByVal lpAppName As String, _
        ByVal lpReturnedString As String, _
        ByVal nSize As Long, _
        ByVal lpFileName As String _
) As Long

Private Declare Function FrameRect Lib "user32" (ByVal hdc As Long, lpRect As RECT, ByVal hBrush As Long) As Long

Private Declare Function FillRect Lib "user32" (ByVal hdc As Long, lpRect As RECT, ByVal hBrush As Long) As Long

Private Declare Function CreateSolidBrush Lib "gdi32" (ByVal crColor As Long) As Long

Private Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long

Private Declare Function MoveToEx Lib "gdi32" ( _
    ByVal hdc As Long, _
    ByVal x As Long, _
    ByVal y As Long, _
    lpPoint As POINTAPI _
) As Long

Private Declare Function LineTo Lib "gdi32" (ByVal hdc As Long, ByVal x As Long, ByVal y As Long) As Long

Private Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) As Long

Private Declare Function CreatePen Lib "gdi32" ( _
    ByVal nPenStyle As Long, _
    ByVal nWidth As Long, _
    ByVal crColor As Long _
) As Long

Private Type POINTAPI
    x As Long
    y As Long
End Type

Private Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Private Const PS_SOLID = 0

'----------------------------------------------------------------------
' �����o�[�ϐ�

Private m_xmlDoc As DOMDocument
Private m_cmdNodes As IXMLDOMNodeList
Private m_fname As String

'----------------------------------------------------------------------
' [�T�v]
' XML�t�@�C���̓ǂݍ���
'
' [����]
' fname     �ǂݍ��ރt�@�C����
' errMsg    �G���[�������̃G���[���e
'
' [���l]
' �w�肳�ꂽ�t�@�C����ǂݍ��݁B�����Ŏw�肵���t�@�C���́A
' ReLoadFile���\�b�h�ōēǍ��݉\�B
'----------------------------------------------------------------------

Public Function LoadFile(ByRef fname As String, errMsg As String) As Boolean
    On Error GoTo Err_LoadFile
    
    '�t�@�C������ۑ�
    m_fname = fname
    
    'XML�t�@�C���Ƃ��ēǂݍ���
    LoadFile = m_xmlDoc.Load(fname)
    If LoadFile = False Then
        errMsg = m_xmlDoc.parseError.reason
        errMsg = errMsg & "���\�[�X '" & m_xmlDoc.parseError.url & "' �̎��s�G���[�ł��B���C�� " & m_xmlDoc.parseError.Line & "�A�ʒu " & m_xmlDoc.parseError.linepos
        If m_xmlDoc.parseError.linepos > 0 Then
            errMsg = errMsg & vbCrLf & vbCrLf & Replace(m_xmlDoc.parseError.srcText, Chr(9), " ") & vbCrLf & String(m_xmlDoc.parseError.linepos - 1, "-") & "^"
        End If
    Else
        errMsg = ""
    End If
    
    'command�m�[�h���X�g���擾���Ă���
    Set m_cmdNodes = m_xmlDoc.getElementsByTagName("panelex/command")
    
Exit_LoadFile:
    Exit Function
    
Err_LoadFile:
    LoadFile = False
    Resume Exit_LoadFile
    
End Function

'----------------------------------------------------------------------
' [�T�v]
' XML�t�@�C���̍ēǂݍ���
'
' [����]
' errMsg    �G���[�������̃G���[���e
'
' [���l]
' LoadFile�����s������Ɍďo�\�BLoadFile�Ŏw�肵���t�@�C����
' �ēǍ��݂���B
'----------------------------------------------------------------------

Public Function ReLoadFile(errMsg As String) As Boolean
    On Error GoTo Err_ReLoadFile
    
    'XML�t�@�C���̓ǂݍ���
    ReLoadFile = m_xmlDoc.Load(m_fname)
    If ReLoadFile = False Then
        errMsg = m_xmlDoc.parseError.reason
        errMsg = errMsg & "���\�[�X '" & m_xmlDoc.parseError.url & "' �̎��s�G���[�ł��B���C�� " & m_xmlDoc.parseError.Line & "�A�ʒu " & m_xmlDoc.parseError.linepos
        If m_xmlDoc.parseError.linepos > 0 Then
            errMsg = errMsg & vbCrLf & vbCrLf & Replace(m_xmlDoc.parseError.srcText, Chr(9), " ") & vbCrLf & String(m_xmlDoc.parseError.linepos - 1, "-") & "^"
        End If
    Else
        errMsg = ""
    End If
    
    Set m_cmdNodes = m_xmlDoc.getElementsByTagName("panelex/command")
    
Exit_ReLoadFile:
    Exit Function
    
Err_ReLoadFile:
    ReLoadFile = False
    Resume Exit_ReLoadFile
    
End Function

'----------------------------------------------------------------------
' [�T�v]
' DC�ɕ`��
'
' [����]
' hdc   �`�悳���DC�ւ̃n���h��
'
' [���l]
' command�m�[�h���X�g�ɏ����ꂽ���e��DC�ɕ`�悷��
'----------------------------------------------------------------------

Public Function Draw(ByVal hdc As Long) As Boolean
    On Error GoTo Err_Draw
    
    Dim i As Long
    Dim e As IXMLDOMElement
    Dim rc As RECT
    Dim hbr As Long
    Dim pt As POINTAPI
    Dim hpen As Long
    Dim hold As Long
    
    For i = 0 To m_cmdNodes.length - 1
        Set e = m_cmdNodes.Item(i)
        
        Select Case UCase(e.getElementsByTagName("name").Item(0).Text)
        Case "LINE"
            'LINE�R�}���h
            SetRect rc, e.getElementsByTagName("rect/left").Item(0).Text, e.getElementsByTagName("rect/top").Item(0).Text, _
                e.getElementsByTagName("rect/right").Item(0).Text, e.getElementsByTagName("rect/bottom").Item(0).Text
            
            MoveToEx hdc, rc.Left, rc.Top, pt
            
            hpen = CreatePen(PS_SOLID, 0, RGB(e.getElementsByTagName("color/R").Item(0).Text, _
                e.getElementsByTagName("color/G").Item(0).Text, e.getElementsByTagName("color/B").Item(0).Text))
            
            hold = SelectObject(hdc, hpen)
            LineTo hdc, rc.Right, rc.Bottom
            SelectObject hdc, hold
            
            DeleteObject hbr
        
        Case "BOX"
            'BOX�R�}���h
            SetRect rc, e.getElementsByTagName("rect/left").Item(0).Text, e.getElementsByTagName("rect/top").Item(0).Text, _
                e.getElementsByTagName("rect/right").Item(0).Text, e.getElementsByTagName("rect/bottom").Item(0).Text
            
            hbr = CreateSolidBrush(RGB(e.getElementsByTagName("color/R").Item(0).Text, _
                e.getElementsByTagName("color/G").Item(0).Text, e.getElementsByTagName("color/B").Item(0).Text))
                
            FrameRect hdc, rc, hbr
            DeleteObject hbr
            
        Case "FILL"
            'FILL�R�}���h
            SetRect rc, e.getElementsByTagName("rect/left").Item(0).Text, e.getElementsByTagName("rect/top").Item(0).Text, _
                e.getElementsByTagName("rect/right").Item(0).Text, e.getElementsByTagName("rect/bottom").Item(0).Text
            
            hbr = CreateSolidBrush(RGB(e.getElementsByTagName("color/R").Item(0).Text, _
                e.getElementsByTagName("color/G").Item(0).Text, e.getElementsByTagName("color/B").Item(0).Text))
                
            FillRect hdc, rc, hbr
            DeleteObject hbr
        
        End Select
        
        Set e = Nothing
    Next
    
Exit_Draw:
    Exit Function
    
Err_Draw:
    Resume Exit_Draw
    
End Function

'----------------------------------------------------------------------
' [�T�v]
' �R���X�g���N�^
'
' [���l]
' m_xmlDoc�̃C���X�^���X���쐬
'----------------------------------------------------------------------

Private Sub Class_Initialize()
    Set m_xmlDoc = New DOMDocument
End Sub

'----------------------------------------------------------------------
' [�T�v]
' �f�X�g���N�^
'
' [���l]
' m_xmlDoc�̃C���X�^���X��j��
'----------------------------------------------------------------------

Private Sub Class_Terminate()
    Set m_xmlDoc = Nothing
End Sub