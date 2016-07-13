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
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()

    Debug.Print VBGetPrivateProfileString(vbNullString, "", "", "d:\work\hoge.ini")
    Debug.Print VBGetPrivateProfileString("sec2", "", "", "d:\work\hoge.ini")
    Debug.Print VBGetPrivateProfileString("sec_hoge", vbNullString, "", "d:\work\hoge.ini")
    Debug.Print VBGetPrivateProfileString(vbNullString, vbNullString, "", "d:\work\hoge.ini")
    Debug.Print VBGetPrivateProfileString("sec_hoge", "key_hoge2", "", "d:\work\hoge.ini")
    
End Sub
