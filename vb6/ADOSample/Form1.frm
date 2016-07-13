VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5355
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10170
   LinkTopic       =   "Form1"
   ScaleHeight     =   5355
   ScaleWidth      =   10170
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   960
      TabIndex        =   0
      Top             =   720
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub DoMain()

    Dim strConn
    
    strConn = "driver={Oracle in OraHome92}; UID=m83_test; PWD=mpcadmin; DBQ=mpc"
    
    Dim objConn As New ADODB.Connection
    
    objConn.Open strConn
    
    Dim rs As New ADODB.Recordset
    
    rs.Open "select * from teiji_info_file;", objConn
    
    Do Until rs.EOF
    
        Debug.Print rs("ti_date")
        
        rs.MoveNext
    Loop
End Sub


Private Sub Command1_Click()
    Command1.Enabled = False
    
    DoMain
    
    Command1.Enabled = True
End Sub
