VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   1800
      TabIndex        =   0
      Top             =   1320
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    Dim nLoop As Long
    Dim nCnt As Long
    Dim strDirList() As String
    
    ReDim Preserve strDirList(nCnt)
    strDirList(nCnt) = "GL1.2.2.2"
    nCnt = nCnt + 1
    
    ReDim Preserve strDirList(nCnt)
    strDirList(nCnt) = "GL1.2.2.10"
    nCnt = nCnt + 1
    
    ReDim Preserve strDirList(nCnt)
    strDirList(nCnt) = "GL2.0.0.0"
    nCnt = nCnt + 1
    
    ReDim Preserve strDirList(nCnt)
    strDirList(nCnt) = "GL1.2.2.1"
    nCnt = nCnt + 1
    
    ReDim Preserve strDirList(nCnt)
    strDirList(nCnt) = "GL1.2.3.0"
    nCnt = nCnt + 1
    
    ReDim Preserve strDirList(nCnt)
    strDirList(nCnt) = "backup"
    nCnt = nCnt + 1
    
    Debug.Print "---[ ソート前 ]---"
    For nLoop = LBound(strDirList) To UBound(strDirList)
        Debug.Print nLoop, strDirList(nLoop)
    Next
    
    Call BubbleSort(strDirList)

    Debug.Print "---[ ソート後 ]---"
    For nLoop = LBound(strDirList) To UBound(strDirList)
        Debug.Print nLoop, strDirList(nLoop)
    Next
End Sub
