VERSION 5.00
Object = "{48E59290-9880-11CF-9754-00AA00C00908}#1.0#0"; "MSINET.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   8355
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   9465
   LinkTopic       =   "Form1"
   ScaleHeight     =   8355
   ScaleWidth      =   9465
   StartUpPosition =   3  'Windows の既定値
   Begin VB.TextBox Text2 
      Height          =   2175
      Left            =   240
      MultiLine       =   -1  'True
      TabIndex        =   3
      Text            =   "Form1.frx":0000
      Top             =   2520
      Width           =   9015
   End
   Begin VB.TextBox Text1 
      Height          =   495
      Left            =   120
      TabIndex        =   1
      Text            =   $"Form1.frx":0006
      Top             =   840
      Width           =   9255
   End
   Begin VB.CommandButton Command1 
      Caption         =   "3"
      Height          =   495
      Left            =   1080
      TabIndex        =   2
      Top             =   1560
      Width           =   1215
   End
   Begin InetCtlsObjects.Inet Inet1 
      Left            =   360
      Top             =   1440
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      URL             =   "http://root:axis@"
      UserName        =   "root"
      Password        =   "axis"
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   9135
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub DoResponseReceived()
    'Inet1.GetChunk
End Sub

Private Sub DoResponseCompleted()
    Dim vtData() As Byte
    Dim strData As String
    Dim bDone As Boolean: bDone = False
    Dim sz As Long
    
    Text2.Text = Inet1.GetHeader("Content-Type")
    
    If UCase(Text2.Text) = UCase("image/jpeg") Then
        Text2.Text = Inet1.GetHeader()
        Open App.Path & "\a.jpg" For Binary As #1
        
        'vtData = Inet1.GetChunk(4, icByteArray)
        
        
        vtData = Inet1.GetChunk(8192, icByteArray)
        'Debug.Print Now, LenB(vtData)
        Put #1, , vtData
        
        DoEvents
        Do While Not bDone
            DoEvents
            'strData = strData & vtData
            DoEvents
            vtData = Inet1.GetChunk(8192, icByteArray)
            Debug.Print Now, UBound(vtData)
            If UBound(vtData) = -1 Then
                bDone = True
            Else
                Put #1, , vtData
            End If
        Loop
        
        Close #1
    Else
        vtData = Inet1.GetChunk(1024, icByteArray)
        DoEvents
        Do While Not bDone
            DoEvents
            'strData = strData & vtData
            DoEvents
            vtData = Inet1.GetChunk(1024, icByteArray)
            'If Len(vtData) = 0 Then
            If True Then
                bDone = True
            End If
        Loop
    End If
    
    'Text2.Text = strData
End Sub

Private Sub Command1_Click()
    Inet1.UserName = "root"
    Inet1.Password = "axis"
    Inet1.URL = Text1.Text
    Inet1.Execute , "GET"
End Sub

Private Sub Inet1_StateChanged(ByVal State As Integer)
    Dim strMsg As String
    
    Select Case State
    Case icNone
        strMsg = "状態についての報告はありません。"
    Case icResolvingHost
        strMsg = "コントロールは、指定されたホスト コンピュータの IP アドレスを調べています。"
    Case icHostResolved
        strMsg = "コントロールは、指定されたホスト コンピュータの IP アドレスを見つけました。"
    Case icConnecting
        strMsg = "コントロールは、ホスト コンピュータとの接続処理を行っています。"
    Case icConnecting
        strMsg = "コントロールは、ホスト コンピュータと正常に接続しました。"
    Case icRequesting
        strMsg = "コントロールは、ホスト コンピュータに要求を送信しています。"
    Case icRequestSent
        strMsg = "コントロールは、要求を正常に送信しました。"
    Case icReceivingResponse
        strMsg = "コントロールは、ホスト コンピュータからの応答を受信しています。"
    Case icResponseReceived
        strMsg = "コントロールは、ホスト コンピュータからの応答を正常に受信しました。"
        
        DoResponseReceived
        
    Case icDisconnecting
        strMsg = "コントロールは、ホスト コンピュータとの接続を解除しています。"
    Case icDisconnected
        strMsg = "コントロールは、ホスト コンピュータとの接続を正常に解除しました。"
    Case icError
        strMsg = "ホスト コンピュータとの通信中にエラーが発生しました。"
    Case icResponseCompleted
        strMsg = "要求処理が完了し、すべてのデータが受信されました。"
        
        DoResponseCompleted
        
    End Select

    Label1.Caption = Now & " - " & State & " - " & strMsg
End Sub
