Attribute VB_Name = "mdlSub"
Option Explicit

Public Declare Function htons Lib "ws2_32.dll" (ByVal hostshort As Integer) As Integer

Private Type GUID
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(8 - 1) As Byte
End Type

Private Declare Function CreateGUID Lib "drtsub.dll" (ByRef buf As GUID) As Long

'
' �z��Ɏw�蕶���񂪊܂܂�邩�ǂ����A���ׂ�B
'
' �����񂪊܂܂��ꍇ�́ATrue�A�܂܂�Ȃ��ꍇ�́AFalse��Ԃ��B
'
Public Function InArray(ar As Variant, findText As String) As Boolean
    
    Dim i As Long
    
    '
    ' �����񂪁A�z��ar�̒��ɂ��邩�ǂ����T���B
    '
    For i = LBound(ar) To UBound(ar)
        If UCase(ar(i)) = UCase(findText) Then
            InArray = True
            Exit Function
        End If
    Next
    
    InArray = False
    
End Function

'
' GUID��{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXX}�^�̕�����ɂ���
'
Private Function GuidToString(g As GUID) As String
    Dim i As Long
    Dim temp As String
    
    temp = "{"
    temp = temp & Right("00000000" & Hex(g.Data1), 8)
    temp = temp & "-"
    temp = temp & Right("0000" & Hex(g.Data2), 4)
    temp = temp & "-"
    temp = temp & Right("0000" & Hex(g.Data3), 4)
    temp = temp & "-"
    temp = temp & Right("00" & Hex(g.Data4(0)), 2) & Right("00" & Hex(g.Data4(1)), 2)
    temp = temp & "-"
    For i = 2 To UBound(g.Data4)
        temp = temp & Right("00" & Hex(g.Data4(i)), 2)
    Next
    temp = temp & "}"
    
    GuidToString = temp
End Function

'
' GUID���f�B�X�N�ƃ��W�X�g���ɏ�������
'
' ���ł�GUID���������܂�Ă���Ƃ��́A�������Ȃ��B
'
Public Function SaveGuid() As Boolean
    Dim g As GUID
    Dim gstr As String
    Dim fno As Integer
    
    If GetSetting(App.ProductName, "Settings", "Key", "") = "" Then
        CreateGUID g
        
        gstr = GuidToString(g)
        
        fno = FreeFile
        Open "key.guid" For Output As fno
        Print #fno, gstr
        Close fno
        
        SetAttr "key.guid", GetAttr("key.guid") + vbHidden
        
        SaveSetting App.ProductName, "Settings", "Key", gstr
    End If
End Function

'
' �f�B�X�N���GUID��ǂݍ���
'
' �J�����g�f�B���N�g���ɂ���Akey.guid��ǂݍ���ŁA�Ԃ�
' �t�@�C���������ꍇ�́A�󕶎����Ԃ��B
'
Private Function GetGuidOnDisk() As String
    Dim buf As String
    
    On Error Resume Next
    
    Dim fno As Integer
    fno = FreeFile
    Open "key.guid" For Input As fno
    If Err.Number = 0 Then
        Line Input #fno, buf
        Close fno
    End If
    
    GetGuidOnDisk = buf
End Function

'
' GUID�̈�v�󋵂��`�F�b�N
'
' ���W�X�g�����GUID�ƃJ�����g�f�B���N�g�����GUID���A
' ��v���邩�ǂ����`�F�b�N����B
'
Public Function CheckGuid() As Boolean
    Dim GuidOnPC As String
    Dim GuidOnDisk As String
    
    GuidOnPC = GetSetting(App.ProductName, "Settings", "Key", "")
    GuidOnDisk = GetGuidOnDisk
    
    CheckGuid = IIf(GuidOnPC = GuidOnDisk, True, False)
End Function

Public Function ToTwipsX(ByVal X As Long) As Long
    ToTwipsX = X * Screen.TwipsPerPixelX
End Function

Public Function ToTwipsY(ByVal Y As Long) As Long
    ToTwipsY = Y * Screen.TwipsPerPixelY
End Function

Public Function AlignMiddle(ctlBase As Control, ctlTarget As Control) As Long
    AlignMiddle = ctlBase.Top + (ctlBase.Height - ctlTarget.Height) / 2
End Function

Public Function GetRight(ctrl As Control) As Long
    GetRight = ctrl.Left + ctrl.Width
End Function

Public Function GetBottom(ctrl As Control) As Long
    GetBottom = ctrl.Top + ctrl.Height
End Function
