VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsInformation"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False

' T_INFORMATION を表現するクラス
'
Option Explicit

Private Const MAX_LINE As Long = 14

' T_INFORMATION のフィールド定義
'
Private Type T_INFORMATION
    InfoBoardNo As Integer
    lineNo As Integer
    DispMode As Integer
    ButtonName As String
    InfoBoardTitle As String
    infoBoardData As String
    FontSize As Integer
    FontStyle As Integer
    FontColor As Integer
    BackPicture As Integer
    SelectFilePath As String
    SelectDougaPath As String
    DateUpdate As String
End Type

' 操作するデータベースオブジェクト
Private m_db As clsHN2DB

' データベースサーバー名
Private m_strServerName As String

Private m_information() As T_INFORMATION

' T_INFOBOARD(clsInfoboard)から、T_INFORMATIONを作成する
'
Public Sub UpdateInfoBoard(infoBoard As clsInfoboard)
    
    ReDim m_information(infoBoard.GetCount * MAX_LINE - 1)
    
    Dim InfoBoardNo As Long
    For InfoBoardNo = 0 To infoBoard.GetCount - 1
        
        Dim infoBoardData As String
        infoBoardData = infoBoard.infoBoardData(InfoBoardNo)
        
        Dim lines As Variant
        lines = Split(infoBoardData, vbNewLine)
        
        Dim lineNo As Long
        For lineNo = 0 To MAX_LINE - 1
        
            With m_information(InfoBoardNo * MAX_LINE + lineNo)
                .InfoBoardNo = infoBoard.InfoBoardNo(InfoBoardNo)
                .lineNo = lineNo + 1
                .DispMode = infoBoard.DispMode(InfoBoardNo)
                .ButtonName = infoBoard.ButtonName(InfoBoardNo)
                .InfoBoardTitle = infoBoard.InfoBoardTitle(InfoBoardNo)
                
                If lineNo <= UBound(lines) Then
                    .infoBoardData = lines(lineNo)
                Else
                    .infoBoardData = ""
                End If
                
                .FontSize = 18
                .FontStyle = 0
                .FontColor = infoBoard.FontColor(InfoBoardNo)
                .BackPicture = infoBoard.BackPicture(InfoBoardNo)
            End With
        Next
    Next
End Sub

' T_INFOMATIONのデータベースを更新する。
'
Public Function UpdateDatabase(serverName As String) As Boolean
    Dim rs As ADODB.Recordset
    Dim i As Long
    
    Set rs = New ADODB.Recordset
    
    'データベースサーバー名を設定
    m_strServerName = serverName
    m_db.serverName = m_strServerName
    
    'データベースを開く
    If m_db.DBOpen = True Then
        
        For i = 0 To UBound(m_information)
            Dim sql As String
            sql = "update T_INFORMATION set"
            sql = sql & " DISPMODE=" & m_information(i).DispMode
            sql = sql & ", BUTTONNAME='" & m_information(i).ButtonName & "'"
            sql = sql & ", INFOBOARDTITLE='" & m_information(i).InfoBoardTitle & "'"
            sql = sql & ", INFOBOARDDATA='" & m_information(i).infoBoardData & "'"
            ''sql = sql & ", FONTSIZE=" & m_information(i).FontSize
            ''sql = sql & ", FONTSTYLE=" & m_information(i).FontStyle
            sql = sql & ", FONTCOLOR=" & m_information(i).FontColor
            sql = sql & ", BACKPICTURE=" & m_information(i).BackPicture
            sql = sql & " where INFOBOARDNO=" & m_information(i).InfoBoardNo
            sql = sql & " and LINENO=" & m_information(i).lineNo
            sql = sql & ";"
            
            m_db.Execute sql
        Next
    
        m_db.DBClose
        UpdateDatabase = True
    Else
        MsgBox "データベースに接続できません。" & vbNewLine & Err.Description
        UpdateDatabase = False
    End If
    
    Set rs = Nothing

    Exit Function
End Function

' コンストラクタ
'
Private Sub Class_Initialize()
    Set m_db = New clsHN2DB
End Sub

' デストラクタ
'
Private Sub Class_Terminate()
    Set m_db = Nothing
End Sub


