; CLW ファイルは MFC ClassWizard の情報を含んでいます。

[General Info]
Version=1
LastClass=CViewverDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "viewver.h"

ClassCount=3
Class1=CViewverApp
Class2=CViewverDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_VIEWVER_DIALOG

[CLS:CViewverApp]
Type=0
HeaderFile=viewver.h
ImplementationFile=viewver.cpp
Filter=N

[CLS:CViewverDlg]
Type=0
HeaderFile=viewverDlg.h
ImplementationFile=viewverDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_WSU_VERSION

[CLS:CAboutDlg]
Type=0
HeaderFile=viewverDlg.h
ImplementationFile=viewverDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_VIEWVER_DIALOG]
Type=1
Class=CViewverDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_RELOAD,button,1342242816
Control4=IDC_LIST1,SysListView32,1350631429

