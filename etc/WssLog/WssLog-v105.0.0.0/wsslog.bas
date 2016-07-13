Attribute VB_Name = "mdlWssLog"
'----------------------------------------------------------------------
'   WssLog���ʃw�b�_(C++�p��WssLog.h)
'
'   [�T�v]
'   WssLog.dll���g�p����l�́A������v���W�F�N�g�ɒǉ�

Option Explicit

' ���O�̃��x����`

Public Const LOGLV_DEBUG As Long = 0        ' �f�o�b�O�p
Public Const LOGLV_TRACE As Long = 1        ' �g���[�X
Public Const LOGLV_WARNING As Long = 2      ' ���[�j���O(�ʏ�͂��̃��x���ȏ���o��)
Public Const LOGLV_ERR As Long = 3          ' �G���[
Public Const LOGLV_FATALERR As Long = 4     ' �v���I�G���[
Public Const LOGLV_INFO As Long = 5         ' ���
Public Const LOGLV_ALWAYS As Long = 9       ' �K�����O���o��
                                            ' (��).�p�ɂɋN������̂Ɋւ��Ă͎g�p�֎~
' ���O���ʃR�[�h(6�����ȓ��Œ�`)

Public Const LOGC_APPCTRL As String = "APPCTL"  ' WssAppCtrl.exe
Public Const LOGC_COM As String = "COM"         ' WssCom.exe
Public Const LOGC_SYUUKEI As String = "SYUKEI"  ' Syukei.exe
Public Const LOGC_TNIO As String = "TNIO"       ' Tnio.exe
'Public Const LOGC_UPS As String = "UPS"         ' Ups.exe
Public Const LOGC_UNSET As String = "UNYOU"     ' Unyou.exe
Public Const LOGC_GYOSET As String = "GYOSET"   ' GyoSet.exe
Public Const LOGC_SYSSET As String = "SYSSET"   ' SysSet.exe
Public Const LOGC_MAILPROC As String = "MAILPROC" 'MailProc.exe
Public Const LOGC_TOUINJI As String = "TOUKA"   ' Toukainji.exe
Public Const LOGC_WINKT As String = "WINKT"     ' WinKT.exe
Public Const LOGC_STARTUP As String = "TACHI"   ' Tachiagari.exe
Public Const LOGC_CALDLL As String = "CALDLL"   ' WinSousaCalc.dll
Public Const LOGC_WSFT As String = "WSFT"       ' Wsft.dll
Public Const LOGC_FILDLL As String = "FILDLL"   ' Wsft.dll
Public Const LOGC_UNDLL As String = "OPEDLL"    ' Opesend.dll
Public Const LOGC_GFTP As String = "GFTP"       ' GFTP.exe
Public Const LOGC_DLG As String = "WSSDLG"      ' WssDialog.exe
Public Const LOGC_TNCOMD As String = "TNCOMD"   ' TnCommander.exe
Public Const LOGC_SPLASH As String = "SPLASH"   ' wss_splash
Public Const LOGC_HALLMP As String = "HALLMP"   ' HallMap.exe
Public Const LOGC_OFF As String = "SWOFF"       ' KagiSWOFF

' export ���ꂽ�֐��̒�`

Public Declare Sub WinssaVB_Log Lib "wsslog.dll" (ByVal szIdent As String, ByVal nLevel As Long, ByVal szMsg As String)
Public Declare Sub WinssaVB_Dump Lib "wsslog.dll" (ByVal szIdent As String, ByVal nLevel As Long, ByVal szBuf As String, ByVal nSize As Long)
Public Declare Sub WLSetLogLevel Lib "wsslog.dll" (ByVal nNewLogLevel As Long)
Public Declare Function WLGetLogLevel Lib "wsslog.dll" () As Long

Public Declare Function WLGetLogPath Lib "wsslog.dll" (ByVal pszBuffer As String, ByVal nSize As Long) As Long
Public Declare Sub WLSetLogPath Lib "wsslog.dll" (ByVal pszPath As String)

Public Sub WinssaVB_Log1(ByVal szIdent As String, ByVal nLevel As Long, ByVal szMsg As String)

#If VB_DEBUG = 0 Then
    WinssaVB_Log szIdent, nLevel, szMsg
#Else
    Debug.Print szIdent
#End If

End Sub


