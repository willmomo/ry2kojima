Attribute VB_Name = "mdlMain"
Option Explicit

Public g_log As New clsWssLog

Public Sub Main()
    g_log.Ident = "TR60"
    g_log.WriteInfo "�������� " & App.EXEName & " �N�� ��������"
    
    frmMain.Show vbModal

    g_log.WriteInfo "�������� " & App.EXEName & " �I�� ��������"
End Sub
