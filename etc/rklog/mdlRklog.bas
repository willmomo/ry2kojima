Attribute VB_Name = "mdlRklog"
Option Explicit

Public Const RKL_TRACE  As Long = 0
Public Const RKL_WARNING As Long = 1
Public Const RKL_ERROR As Long = 2
Public Const RKL_FATAL As Long = 3
Public Const RKL_INFO As Long = 4

Public Declare Function RKLogVB Lib "rklog.dll" (ByVal lvl As Long, ByVal msg As String) As Long
Public Declare Function RKLGetLogLevel Lib "rklog.dll" () As Long
Public Declare Function RKLSetLogLevel Lib "rklog.dll" (ByVal newLogLevel As Long) As Long
Public Declare Function RKLGetLogPrefix Lib "rklog.dll" (ByVal pszLogPrefix As String, ByVal dwSize As Long) As Long
Public Declare Function RKLSetLogPrefix Lib "rklog.dll" (ByVal pszLogPrefix As String) As Long
Public Declare Function RKLGetLogPath Lib "rklog.dll" (ByVal pszLogPath As String, ByVal dwSize As Long) As Long
Public Declare Function RKLSetLogPath Lib "rklog.dll" (ByVal pszLogPath As String) As Long
Public Declare Function RKLGetLogModule Lib "rklog.dll" (ByVal pszLogModule As String, ByVal dwSize As Long) As Long
Public Declare Function RKLSetLogModule Lib "rklog.dll" (ByVal pszLogModule As String) As Long
Public Declare Function RKLClearLog Lib "rklog.dll" () As Long
Public Declare Function RKLGetLogPeriod Lib "rklog.dll" () As Long
Public Declare Function RKLSetLogPeriod Lib "rklog.dll" (ByVal newPeriod As Long) As Long

