; CLW ファイルは MFC ClassWizard の情報を含んでいます。

[General Info]
Version=1
LastClass=CSHBrowseForFolderDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SHBrowseForFolder.h"

ClassCount=3
Class1=CSHBrowseForFolderApp
Class2=CSHBrowseForFolderDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_SHBROWSEFORFOLDER_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX

[CLS:CSHBrowseForFolderApp]
Type=0
HeaderFile=SHBrowseForFolder.h
ImplementationFile=SHBrowseForFolder.cpp
Filter=N

[CLS:CSHBrowseForFolderDlg]
Type=0
HeaderFile=SHBrowseForFolderDlg.h
ImplementationFile=SHBrowseForFolderDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_SHBROWSEFORFOLDER

[CLS:CAboutDlg]
Type=0
HeaderFile=SHBrowseForFolderDlg.h
ImplementationFile=SHBrowseForFolderDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SHBROWSEFORFOLDER_DIALOG]
Type=1
Class=CSHBrowseForFolderDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SHBROWSEFORFOLDER,button,1342242816

