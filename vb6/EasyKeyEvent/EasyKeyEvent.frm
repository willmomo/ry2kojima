VERSION 5.00
Begin VB.Form frmEasyKeyEvent 
   Caption         =   "Easy Key Event"
   ClientHeight    =   9135
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   12960
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   ScaleHeight     =   9135
   ScaleWidth      =   12960
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.ListBox List1 
      BeginProperty Font 
         Name            =   "‚l‚r ƒSƒVƒbƒN"
         Size            =   9.75
         Charset         =   128
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   7620
      IntegralHeight  =   0   'False
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   11535
   End
End
Attribute VB_Name = "frmEasyKeyEvent"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_dic As New Dictionary

Private Sub InitDic()
    With m_dic
        .Add KeyCodeConstants.vbKey0, "vbKey0"
        .Add KeyCodeConstants.vbKey1, "vbKey1"
        .Add KeyCodeConstants.vbKey2, "vbKey2"
        .Add KeyCodeConstants.vbKey3, "vbKey3"
        .Add KeyCodeConstants.vbKey4, "vbKey4"
        .Add KeyCodeConstants.vbKey5, "vbKey5"
        .Add KeyCodeConstants.vbKey6, "vbKey6"
        .Add KeyCodeConstants.vbKey7, "vbKey7"
        .Add KeyCodeConstants.vbKey8, "vbKey8"
        .Add KeyCodeConstants.vbKey9, "vbKey9"
        .Add KeyCodeConstants.vbKeyA, "vbKeyA"
        .Add KeyCodeConstants.vbKeyAdd, "vbKeyAdd"
        .Add KeyCodeConstants.vbKeyB, "vbKeyB"
        .Add KeyCodeConstants.vbKeyBack, "vbKeyBack"
        .Add KeyCodeConstants.vbKeyC, "vbKeyC"
        .Add KeyCodeConstants.vbKeyCancel, "vbKeyCancel"
        .Add KeyCodeConstants.vbKeyCapital, "vbKeyCapital"
        .Add KeyCodeConstants.vbKeyClear, "vbKeyClear"
        .Add KeyCodeConstants.vbKeyControl, "vbKeyControl"
        .Add KeyCodeConstants.vbKeyD, "vbKeyD"
        .Add KeyCodeConstants.vbKeyDecimal, "vbKeyDecimal"
        .Add KeyCodeConstants.vbKeyDelete, "vbKeyDelete"
        .Add KeyCodeConstants.vbKeyDivide, "vbKeyDivide"
        .Add KeyCodeConstants.vbKeyDown, "vbKeyDown"
        .Add KeyCodeConstants.vbKeyE, "vbKeyE"
        .Add KeyCodeConstants.vbKeyEnd, "vbKeyEnd"
        .Add KeyCodeConstants.vbKeyEscape, "vbKeyEscape"
        .Add KeyCodeConstants.vbKeyExecute, "vbKeyExecute"
        .Add KeyCodeConstants.vbKeyF, "vbKeyF"
        .Add KeyCodeConstants.vbKeyF1, "vbKeyF1"
        .Add KeyCodeConstants.vbKeyF10, "vbKeyF10"
        .Add KeyCodeConstants.vbKeyF11, "vbKeyF11"
        .Add KeyCodeConstants.vbKeyF12, "vbKeyF12"
        .Add KeyCodeConstants.vbKeyF13, "vbKeyF13"
        .Add KeyCodeConstants.vbKeyF14, "vbKeyF14"
        .Add KeyCodeConstants.vbKeyF15, "vbKeyF15"
        .Add KeyCodeConstants.vbKeyF16, "vbkeyF16"
        .Add KeyCodeConstants.vbKeyF2, "vbKeyF2"
        .Add KeyCodeConstants.vbKeyF3, "vbKeyF3"
        .Add KeyCodeConstants.vbKeyF4, "vbKeyF4"
        .Add KeyCodeConstants.vbKeyF5, "vbKeyF5"
        .Add KeyCodeConstants.vbKeyF6, "vbKeyF6"
        .Add KeyCodeConstants.vbKeyF7, "vbKeyF7"
        .Add KeyCodeConstants.vbKeyF8, "vbKeyF8"
        .Add KeyCodeConstants.vbKeyF9, "vbKeyF9"
        .Add KeyCodeConstants.vbKeyG, "vbKeyG"
        .Add KeyCodeConstants.vbKeyH, "vbKeyH"
        .Add KeyCodeConstants.vbKeyHelp, "vbKeyHelp"
        .Add KeyCodeConstants.vbKeyHome, "vbKeyHome"
        .Add KeyCodeConstants.vbKeyI, "vbKeyI"
        .Add KeyCodeConstants.vbKeyInsert, "vbKeyInsert"
        .Add KeyCodeConstants.vbKeyJ, "vbKeyJ"
        .Add KeyCodeConstants.vbKeyK, "vbKeyK"
        .Add KeyCodeConstants.vbKeyL, "vbKeyL"
        .Add KeyCodeConstants.vbKeyLButton, "vbKeyLButton"
        .Add KeyCodeConstants.vbKeyLeft, "vbKeyLeft"
        .Add KeyCodeConstants.vbKeyM, "vbKeyM"
        .Add KeyCodeConstants.vbKeyMButton, "vbKeyMButton"
        .Add KeyCodeConstants.vbKeyMenu, "vbKeyMenu"
        .Add KeyCodeConstants.vbKeyMultiply, "vbKeyMultiply"
        .Add KeyCodeConstants.vbKeyN, "vbKeyN"
        .Add KeyCodeConstants.vbKeyNumlock, "vbKeyNumlock"
        .Add KeyCodeConstants.vbKeyNumpad0, "vbKeyNumpad0"
        .Add KeyCodeConstants.vbKeyNumpad1, "vbKeyNumpad1"
        .Add KeyCodeConstants.vbKeyNumpad2, "vbKeyNumpad2"
        .Add KeyCodeConstants.vbKeyNumpad3, "vbKeyNumpad3"
        .Add KeyCodeConstants.vbKeyNumpad4, "vbKeyNumpad4"
        .Add KeyCodeConstants.vbKeyNumpad5, "vbKeyNumpad5"
        .Add KeyCodeConstants.vbKeyNumpad6, "vbKeyNumpad6"
        .Add KeyCodeConstants.vbKeyNumpad7, "vbKeyNumpad7"
        .Add KeyCodeConstants.vbKeyNumpad8, "vbKeyNumpad8"
        .Add KeyCodeConstants.vbKeyNumpad9, "vbKeyNumpad9"
        .Add KeyCodeConstants.vbKeyO, "vbKeyO"
        .Add KeyCodeConstants.vbKeyP, "vbKeyP"
        .Add KeyCodeConstants.vbKeyPageDown, "vbKeyPageDown"
        .Add KeyCodeConstants.vbKeyPageUp, "vbKeyPageUp"
        .Add KeyCodeConstants.vbKeyPause, "vbKeyPause"
        .Add KeyCodeConstants.vbKeyPrint, "vbKeyPrint"
        .Add KeyCodeConstants.vbKeyQ, "vbKeyQ"
        .Add KeyCodeConstants.vbKeyR, "vbKeyR"
        .Add KeyCodeConstants.vbKeyRButton, "vbKeyRButton"
        .Add KeyCodeConstants.vbKeyReturn, "vbKeyReturn"
        .Add KeyCodeConstants.vbKeyRight, "vbKeyRight"
        .Add KeyCodeConstants.vbKeyS, "vbKeyS"
        .Add KeyCodeConstants.vbKeyScrollLock, "vbKeyScrollLock"
        .Add KeyCodeConstants.vbKeySelect, "vbKeySelect"
        .Add KeyCodeConstants.vbKeySeparator, "vbKeySeparator"
        .Add KeyCodeConstants.vbKeyShift, "vbKeyShift"
        .Add KeyCodeConstants.vbKeySnapshot, "vbKeySnapshot"
        .Add KeyCodeConstants.vbKeySpace, "vbKeySpace"
        .Add KeyCodeConstants.vbKeySubtract, "vbKeySubtract"
        .Add KeyCodeConstants.vbKeyT, "vbKeyT"
        .Add KeyCodeConstants.vbKeyTab, "vbKeyTab"
        .Add KeyCodeConstants.vbKeyU, "vbKeyU"
        .Add KeyCodeConstants.vbKeyUp, "vbKeyUp"
        .Add KeyCodeConstants.vbKeyV, "vbKeyV"
        .Add KeyCodeConstants.vbKeyW, "vbKeyW"
        .Add KeyCodeConstants.vbKeyX, "vbKeyX"
        .Add KeyCodeConstants.vbKeyY, "vbKeyY"
        .Add KeyCodeConstants.vbKeyZ, "vbKeyZ"
    End With
End Sub

Private Function KeyCodeToStr(KeyCode As Integer) As String
    If m_dic.Exists(KeyCode) Then
        KeyCodeToStr = m_dic(KeyCode) & "(" & CStr(KeyCode) & ")"
    Else
        KeyCodeToStr = "vb????(" & CStr(KeyCode) & ")"
    End If
End Function

Private Function ShiftState(Shift As Integer) As String
    ShiftState = IIf((Shift And vbShiftMask) > 0, "SHIFT", "-----")
End Function

Private Function CtrlState(Shift As Integer) As String
    CtrlState = IIf((Shift And vbCtrlMask) > 0, "CTRL", "----")
End Function

Private Function AltState(Shift As Integer) As String
    AltState = IIf((Shift And vbAltMask) > 0, "ALT", "---")
End Function

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    List1.AddItem Now & vbTab & "KeyDown" & vbTab & KeyCodeToStr(KeyCode) & vbTab & ShiftState(Shift) & vbTab & CtrlState(Shift) & vbTab & AltState(Shift)
    List1.ListIndex = List1.ListCount - 1
    
    KeyCode = 0
End Sub

Private Sub Form_KeyPress(KeyAscii As Integer)
    List1.AddItem Now & vbTab & "KeyPress" & vbTab & KeyAscii
    List1.ListIndex = List1.ListCount - 1
    
    KeyAscii = 0
End Sub

Private Sub Form_KeyUp(KeyCode As Integer, Shift As Integer)
    List1.AddItem Now & vbTab & "KeyUp" & vbTab & KeyCodeToStr(KeyCode) & vbTab & ShiftState(Shift) & vbTab & CtrlState(Shift) & vbTab & AltState(Shift)
    List1.ListIndex = List1.ListCount - 1
    
    KeyCode = 0
End Sub

Private Sub Form_Load()
    InitDic
End Sub

Private Sub Form_Resize()
    List1.Move 0, 0, Me.ScaleWidth, Me.ScaleHeight
End Sub
