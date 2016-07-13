Attribute VB_Name = "mdlMain"
Option Explicit

Public g_log As New clsWssLog

Public Sub Main()
    g_log.Ident = "TR60"
    g_log.WriteInfo "Å•Å•Å•Å• " & App.EXEName & " ãNìÆ Å•Å•Å•Å•"
    
    frmMain.Show vbModal

    g_log.WriteInfo "Å£Å£Å£Å£ " & App.EXEName & " èIóπ Å£Å£Å£Å£"
End Sub
