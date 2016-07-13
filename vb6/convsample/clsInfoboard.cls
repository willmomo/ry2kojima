VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsInfoboard"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False

' T_INFOBOARD��\������N���X
'
Option Explicit

' T_INFOBOARD �̃t�B�[���h��`
'
Private Type T_INFOBOARD
    InfoBoardNo As Integer
    DispMode As Integer
    InfoBoardTitle As String
    ButtonName As String
    infoBoardData As String
    BackPicture As Integer
    FontColor As Integer
    DateUpdate As String
End Type

' ���삷��f�[�^�x�[�X�I�u�W�F�N�g
Private m_db As clsHN2DB

' �f�[�^�x�[�X�T�[�o�[��
Private m_strServerName As String

' ���R�[�h���ۑ��o�b�t�@
Private m_infoBoardData() As T_INFOBOARD

'�w��C���f�b�N�X�̃t�B�[���h�f�[�^��Ԃ�
'
Public Property Get InfoBoardNo(Index As Long) As Integer
    InfoBoardNo = m_infoBoardData(Index).InfoBoardNo
End Property

Public Property Get DispMode(Index As Long) As Long
    DispMode = m_infoBoardData(Index).DispMode
End Property

Public Property Get InfoBoardTitle(Index As Long) As String
    InfoBoardTitle = m_infoBoardData(Index).InfoBoardTitle
End Property

Public Property Get ButtonName(Index As Long) As String
    ButtonName = m_infoBoardData(Index).ButtonName
End Property

Public Property Get infoBoardData(Index As Long) As String
    infoBoardData = m_infoBoardData(Index).infoBoardData
End Property

Public Property Get BackPicture(Index As Long) As Integer
    BackPicture = m_infoBoardData(Index).BackPicture
End Property

Public Property Get FontColor(Index As Long) As Integer
    FontColor = m_infoBoardData(Index).FontColor
End Property

Public Property Get DateUpdate(Index As Long) As String
    DateUpdate = m_infoBoardData(Index).DateUpdate
End Property

' T_INFOBOARD�̃f�[�^����Ԃ�
'
Public Function GetCount() As Long
    On Error GoTo ArrayErrHandler
    
    GetCount = UBound(m_infoBoardData) + 1
    
    Exit Function
    
ArrayErrHandler:
    GetCount = 0
    Resume Next
    
End Function

' �w�肳�ꂽ�T�[�o�[�̃f�[�^�x�[�X�ɐڑ�����
' T_INFOBOARD �̓��e����荞��
'
Public Function DBOpen(serverName As String) As Boolean
    
    Dim rs As ADODB.Recordset
    Dim i As Long
    
    Set rs = New ADODB.Recordset
    
    '�f�[�^�x�[�X�T�[�o�[����ݒ�
    m_strServerName = serverName
    m_db.serverName = m_strServerName
    
    '�f�[�^�x�[�X���J��
    If m_db.DBOpen = True Then
        
        Dim sql As String
        sql = "select * from T_INFOBOARD;"
        rs.Open sql, m_db.Connection, adOpenStatic, adLockOptimistic
    
        Do Until rs.EOF
        
            On Error GoTo ArrayErrHandler
            ReDim Preserve m_infoBoardData(UBound(m_infoBoardData) + 1)
            On Error GoTo 0
            
            With m_infoBoardData(UBound(m_infoBoardData))
                .InfoBoardNo = rs("INFOBOARDNO")
                .DispMode = rs("DISPMODE")
                .InfoBoardTitle = rs("INFOBOARDTITLE")
                .ButtonName = rs("BUTTONNAME")
                .infoBoardData = rs("INFOBOARDDATA")
                .BackPicture = rs("BACKPICTURE")
                .FontColor = rs("FONTCOLOR")
                .DateUpdate = rs("DATEUPDATE")
            End With
            
            rs.MoveNext
        Loop
    
        rs.Close
    
        m_db.DBClose
        DBOpen = True
    Else
        MsgBox "�f�[�^�x�[�X�ɐڑ��ł��܂���B" & vbNewLine & Err.Description
        DBOpen = False
    End If
    
    Set rs = Nothing

    Exit Function
    
ArrayErrHandler:
    ReDim m_infoBoardData(0)
    Resume Next
    
End Function

' �R���X�g���N�^
'
Private Sub Class_Initialize()
    Set m_db = New clsHN2DB
End Sub

' �f�X�g���N�^
'
Private Sub Class_Terminate()
    Set m_db = Nothing
End Sub
