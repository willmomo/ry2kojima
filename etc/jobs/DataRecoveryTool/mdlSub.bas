Attribute VB_Name = "mdlSub"
Option Explicit

Private Type GUID
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(8 - 1) As Byte
End Type

Private Declare Function CreateGUID Lib "drtsub.dll" (ByRef buf As GUID) As Long

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

Public Function CheckGuid() As Boolean
    Dim GuidOnPC As String
    Dim GuidOnDisk As String
    
    GuidOnPC = GetSetting(App.ProductName, "Settings", "Key", "")
    GuidOnDisk = GetGuidOnDisk
    
    If GuidOnPC = GuidOnDisk Then
        CheckGuid = True
    Else
        CheckGuid = False
    End If
End Function

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
