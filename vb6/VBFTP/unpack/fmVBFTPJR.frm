VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.2#0"; "comctl32.ocx"
Begin VB.Form fmVBFTPJR 
   Caption         =   "vbftpjr"
   ClientHeight    =   5640
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   8715
   LinkTopic       =   "Form1"
   ScaleHeight     =   5640
   ScaleWidth      =   8715
   StartUpPosition =   3  'Windows Default
   Begin VB.OptionButton optAscii 
      Caption         =   "Ascii"
      Height          =   195
      Left            =   3600
      TabIndex        =   20
      Top             =   3120
      Width           =   1095
   End
   Begin VB.OptionButton optBin 
      Caption         =   "Binary"
      Height          =   375
      Left            =   3600
      TabIndex        =   19
      Top             =   2640
      Width           =   855
   End
   Begin VB.CommandButton cmdPut 
      Caption         =   "<---Put<---"
      Height          =   495
      Left            =   3480
      TabIndex        =   18
      Top             =   3840
      Width           =   975
   End
   Begin VB.CommandButton cmdGet 
      Caption         =   "--->Get--->"
      Height          =   495
      Left            =   3480
      TabIndex        =   17
      Top             =   3360
      Width           =   975
   End
   Begin ComctlLib.TreeView TreeView1 
      Height          =   2655
      Left            =   360
      TabIndex        =   16
      Top             =   2640
      Width           =   3015
      _ExtentX        =   5318
      _ExtentY        =   4683
      _Version        =   327682
      Style           =   7
      Appearance      =   1
   End
   Begin VB.FileListBox File1 
      Height          =   2040
      Left            =   6960
      TabIndex        =   15
      Top             =   3120
      Width           =   1575
   End
   Begin VB.DirListBox Dir1 
      Height          =   2115
      Left            =   5040
      TabIndex        =   14
      Top             =   3120
      Width           =   1695
   End
   Begin VB.DriveListBox Drive1 
      Height          =   315
      Left            =   5040
      TabIndex        =   13
      Top             =   2640
      Width           =   3495
   End
   Begin VB.CheckBox chkPassive 
      Caption         =   "Passive FTP syntax"
      Height          =   255
      Left            =   3120
      TabIndex        =   12
      Top             =   1800
      Width           =   2175
   End
   Begin VB.CommandButton cmdDisconnect 
      Caption         =   "Disconnect"
      Height          =   495
      Left            =   5760
      TabIndex        =   11
      Top             =   1680
      Width           =   1815
   End
   Begin VB.CommandButton cmdConnect 
      Caption         =   "Connect"
      Height          =   495
      Left            =   360
      TabIndex        =   10
      Top             =   1680
      Width           =   2175
   End
   Begin VB.TextBox txtPassword 
      Height          =   375
      IMEMode         =   3  'DISABLE
      Left            =   7200
      PasswordChar    =   "*"
      TabIndex        =   9
      Top             =   1080
      Width           =   1095
   End
   Begin VB.TextBox txtUser 
      Height          =   375
      Left            =   4680
      TabIndex        =   7
      Top             =   1080
      Width           =   1095
   End
   Begin VB.TextBox txtServer 
      Height          =   375
      Left            =   1680
      TabIndex        =   4
      Top             =   1080
      Width           =   1455
   End
   Begin VB.CommandButton cmdClosehOpen 
      Caption         =   "Close Internet Session"
      Height          =   495
      Left            =   2280
      TabIndex        =   3
      Top             =   240
      Width           =   1935
   End
   Begin VB.TextBox txtProxy 
      Height          =   435
      Left            =   7200
      TabIndex        =   1
      Top             =   240
      Width           =   1095
   End
   Begin VB.CommandButton cmdInternetOpen 
      Caption         =   "Start Internet Session"
      Height          =   495
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1935
   End
   Begin ComctlLib.ImageList ImageList1 
      Left            =   2280
      Top             =   5160
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      MaskColor       =   12632256
      _Version        =   327682
   End
   Begin VB.Label label4 
      Caption         =   "Password:"
      Height          =   255
      Left            =   6000
      TabIndex        =   8
      Top             =   1080
      Width           =   855
   End
   Begin VB.Label Label3 
      Caption         =   "User Name:"
      Height          =   255
      Left            =   3360
      TabIndex        =   6
      Top             =   1080
      Width           =   975
   End
   Begin VB.Label Label2 
      Caption         =   "FTP Server Name:"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   1080
      Width           =   1335
   End
   Begin VB.Line Line1 
      X1              =   240
      X2              =   8520
      Y1              =   840
      Y2              =   840
   End
   Begin VB.Label Label1 
      Caption         =   "TIS Compatible Proxy (No CERN):"
      Height          =   255
      Left            =   4440
      TabIndex        =   2
      Top             =   360
      Width           =   2535
   End
End
Attribute VB_Name = "fmVBFTPJR"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim bActiveSession As Boolean
Dim hOpen As Long, hConnection As Long
Dim dwType As Long

Dim EnumItemNameBag As New Collection
Dim EnumItemAttributeBag As New Collection
    

Private Sub Form_Load()
    bActiveSession = False
    hOpen = 0
    hConnection = 0
    chkPassive.Value = 1
    optBin.Value = 1
    dwType = FTP_TRANSFER_TYPE_BINARY
    Dim imgI As ListImage
    Set imgI = ImageList1.ListImages.Add(, "open", LoadPicture("open.bmp"))
    Set imgI = ImageList1.ListImages.Add(, "closed", LoadPicture("closed.bmp"))
    Set imgI = ImageList1.ListImages.Add(, "leaf", LoadPicture("leaf.bmp"))
    Set imgI = ImageList1.ListImages.Add(, "root", LoadPicture("root.bmp"))
    TreeView1.ImageList = ImageList1
    TreeView1.Style = tvwTreelinesPictureText
    EnableUI (False)
End Sub

Private Sub Form_Unload(Cancel As Integer)
    cmdClosehOpen_Click
End Sub

Private Sub cmdInternetOpen_Click()
    If Len(txtProxy.Text) <> 0 Then
        hOpen = InternetOpen(scUserAgent, INTERNET_OPEN_TYPE_PROXY, txtProxy.Text, vbNullString, 0)
    Else
        hOpen = InternetOpen(scUserAgent, INTERNET_OPEN_TYPE_DIRECT, vbNullString, vbNullString, 0)
    End If
    If hOpen = 0 Then ErrorOut Err.LastDllError, "InternetOpen"
    EnableUI (True)
End Sub

Private Sub cmdClosehOpen_Click()
    If hConnection <> 0 Then InternetCloseHandle (hConnection)
    If hOpen <> 0 Then InternetCloseHandle (hOpen)
    hConnection = 0
    hOpen = 0
    If bActiveSession Then TreeView1.Nodes.Remove txtServer.Text
    bActiveSession = False
    ClearTextBoxAndBag
    EnableUI (False)
End Sub

Private Sub cmdConnect_Click()
    If Not bActiveSession And hOpen <> 0 Then
        If txtServer.Text = "" Then
            MsgBox "Please enter a server name!"
            Exit Sub
        End If
        Dim nFlag As Long
        If chkPassive.Value Then
            nFlag = INTERNET_FLAG_PASSIVE
        Else
            nFlag = 0
        End If
        hConnection = InternetConnect(hOpen, txtServer.Text, INTERNET_INVALID_PORT_NUMBER, _
        txtUser, txtPassword, INTERNET_SERVICE_FTP, nFlag, 0)
        If hConnection = 0 Then
            bActiveSession = False
            ErrorOut Err.LastDllError, "InternetConnect"
        Else
            bActiveSession = True
            EnableUI (CBool(hOpen))
            FillTreeViewControl (txtServer.Text)
            FtpEnumDirectory ("")
            If EnumItemNameBag.Count = 0 Then Exit Sub
            FillTreeViewControl (txtServer.Text)
       End If
    End If
End Sub

Private Sub cmdDisconnect_Click()
    bDirEmpty = True
    If hConnection <> 0 Then InternetCloseHandle hConnection
    hConnection = 0
    ClearBag
    TreeView1.Nodes.Remove txtServer.Text
    bActiveSession = False
    EnableUI (True)
End Sub

Private Sub ClearTextBoxAndBag()
    txtServer.Text = ""
    txtUser.Text = ""
    txtPassword.Text = ""
    txtProxy.Text = ""
    ClearBag
End Sub

Private Sub ClearBag()
    Dim Num As Integer
    For Num = 1 To EnumItemNameBag.Count
        EnumItemNameBag.Remove 1
    Next Num
    For Num = 1 To EnumItemAttributeBag.Count
        EnumItemAttributeBag.Remove 1
    Next Num
End Sub

Private Sub FillTreeViewControl(strParentKey As String)
    Dim nodX As Node
    Dim strImg As String
    Dim nCount As Integer, i As Integer
    Dim nAttr As Integer
    Dim strItem As String
    
    If EnumItemNameBag.Count = 0 And strParentKey = txtServer.Text Then
        Set nodX = TreeView1.Nodes.Add(, tvwFirst, txtServer.Text, txtServer.Text, "root")
        Exit Sub
    End If
    nCount = EnumItemAttributeBag.Count
    If nCount = 0 Then Exit Sub
    For i = 1 To nCount
        nAttr = EnumItemAttributeBag.Item(i)
        strItem = EnumItemNameBag(i)
        If nAttr = FILE_ATTRIBUTE_DIRECTORY Then
            strImg = "closed"
        Else
            strImg = "leaf"
        End If
        Set nodX = TreeView1.Nodes.Add(strParentKey, tvwChild, strParentKey & "/" & strItem, _
            strParentKey & "/" & strItem, strImg)
    Next
    nodX.EnsureVisible
End Sub

Private Sub cmdGet_Click()
    Dim bRet As Boolean
    Dim szFileRemote As String, szDirRemote As String, szFileLocal As String
    Dim szTempString As String
    Dim nPos As Long, nTemp As Long
    Dim nodX As Node
    Set nodX = TreeView1.SelectedItem
    If bActiveSession Then
        If nodX Is Nothing Then
            MsgBox "Please select the item to GET!"
            Exit Sub
        End If
        szTempString = TreeView1.SelectedItem.Text
        szFileRemote = szTempString
        nPos = 0
        nTemp = 0
        Do
            nTemp = InStr(1, szTempString, "/", vbBinaryCompare)
            If nTemp = 0 Then Exit Do
            szTempString = Right(szTempString, Len(szTempString) - nTemp)
            nPos = nTemp + nPos
        Loop
        szDirRemote = Left(szFileRemote, nPos)
        szFileRemote = Right(szFileRemote, Len(szFileRemote) - nPos)
        szFileLocal = File1.Path
        rcd szDirRemote
        bRet = FtpGetFile(hConnection, szFileRemote, szFileLocal & "/" & szFileRemote, False, _
        INTERNET_FLAG_RELOAD, dwType, 0)
        File1.Refresh
        If bRet = False Then ErrorOut Err.LastDllError, "FtpGetFile"
    Else
        MsgBox "Not in session"
    End If
End Sub

Private Sub cmdPut_Click()
    Dim bRet As Boolean
    Dim szFileRemote As String, szDirRemote As String, szFileLocal As String
    Dim szTempString As String
    Dim nPos As Long, nTemp As Long
    Dim nodX As Node
    Set nodX = TreeView1.SelectedItem
  
    If bActiveSession Then
        If nodX Is Nothing Then
            MsgBox "Please select a remote directory to PUT to!"
            Exit Sub
        End If
        If nodX.Image = "leaf" Then
            MsgBox "Please select a remote directory to PUT to!"
            Exit Sub
        End If
        If File1.filename = "" Then
            MsgBox "Please select a local file to put"
            Exit Sub
        End If
        szTempString = nodX.Text
        szDirRemote = Right(szTempString, Len(szTempString) - Len(txtServer.Text))
        szFileRemote = File1.filename
        szFileLocal = File1.Path & "\" & File1.filename
        If (szDirRemote = "") Then szDirRemote = "\"
        rcd szDirRemote
        
        bRet = FtpPutFile(hConnection, szFileLocal, szFileRemote, _
         dwType, 0)
        If bRet = False Then
            ErrorOut Err.LastDllError, "FtpPutFile"
            Exit Sub
        End If
        
        Dim nodChild As Node, nodNextChild As Node
        Set nodChild = nodX.Child
        Do
          If nodChild Is Nothing Then Exit Do
          Set nodNextChild = nodChild.Next
            TreeView1.Nodes.Remove nodChild.Index
            If nodNextChild Is Nothing Then Exit Do
            Set nodChild = nodNextChild
        Loop
        If nodX.Image = "closed" Then
            nodX.Image = "open"
        End If
        FtpEnumDirectory (nodX.Text)
        FillTreeViewControl (nodX.Text)
   End If
End Sub

Private Sub Dir1_Change()
    File1.Path = Dir1.Path
End Sub

Private Sub Drive1_Change()
    On Error GoTo ErrProc
    Dir1.Path = Drive1.Drive
    Exit Sub
ErrProc:
    Drive1.Drive = "c:"
    Dir1.Path = Drive1.Drive
End Sub

Private Sub rcd(pszDir As String)
    If pszDir = "" Then
        MsgBox "Please enter the directory to CD"
        Exit Sub
    Else
        Dim sPathFromRoot As String
        Dim bRet As Boolean
        If InStr(1, pszDir, txtServer.Text) Then
        sPathFromRoot = Mid(pszDir, Len(txtServer.Text) + 1, Len(pszDir) - Len(txtServer.Text))
        Else
        sPathFromRoot = pszDir
        End If
        If sPathFromRoot = "" Then sPathFromRoot = "/"
        bRet = FtpSetCurrentDirectory(hConnection, sPathFromRoot)
        If bRet = False Then ErrorOut Err.LastDllError, "rcd"
    End If
End Sub

Function ErrorOut(dError As Long, szCallFunction As String)
    Dim dwIntError As Long, dwLength As Long
    Dim strBuffer As String
    If dError = ERROR_INTERNET_EXTENDED_ERROR Then
        InternetGetLastResponseInfo dwIntError, vbNullString, dwLength
        strBuffer = String(dwLength + 1, 0)
        InternetGetLastResponseInfo dwIntError, strBuffer, dwLength
        
        MsgBox szCallFunction & " Extd Err: " & dwIntError & " " & strBuffer
       
        
    End If
    If MsgBox(szCallFunction & " Err: " & dError & _
        vbCrLf & "Close Connection and Session?", vbYesNo) = vbYes Then
        If hConnection Then InternetCloseHandle hConnection
        If hOpen Then InternetCloseHandle hOpen
        hConnection = 0
        hOpen = 0
        If bActiveSession Then TreeView1.Nodes.Remove txtServer.Text
        bActiveSession = False
        ClearTextBoxAndBag
        EnableUI (False)
    End If
End Function

Private Sub EnableUI(bEnabled As Boolean)
    txtServer.Enabled = bEnabled
    txtUser.Enabled = bEnabled
    txtPassword.Enabled = bEnabled
    cmdConnect.Enabled = bEnabled And Not bActiveSession
    cmdDisconnect.Enabled = bEnabled And bActiveSession
    chkPassive.Enabled = bEnabled
    cmdClosehOpen.Enabled = bEnabled
    cmdInternetOpen.Enabled = Not bEnabled
    txtProxy.Enabled = Not bEnabled
    optBin.Enabled = bEnabled
    optAscii.Enabled = bEnabled
    cmdGet.Enabled = bEnabled And bActiveSession
    cmdPut.Enabled = bEnabled And bActiveSession
End Sub

Private Sub FtpEnumDirectory(strDirectory As String)
    
    ClearBag
    Dim hFind As Long
    Dim nLastError As Long
    Dim dError As Long
    Dim ptr As Long
    Dim pData As WIN32_FIND_DATA
    
    If Len(strDirectory) > 0 Then rcd (strDirectory)
    pData.cFileName = String(MAX_PATH, 0)
    hFind = FtpFindFirstFile(hConnection, "*.*", pData, 0, 0)
    nLastError = Err.LastDllError
    
    If hFind = 0 Then
        If (nLastError = ERROR_NO_MORE_FILES) Then
            MsgBox "This directory is empty!"
        Else
            ErrorOut nLastError, "FtpFindFirstFile"
        End If
        Exit Sub
    End If
    
    dError = NO_ERROR
    Dim bRet As Boolean
    Dim strItemName As String
    
    EnumItemAttributeBag.Add pData.dwFileAttributes
    strItemName = Left(pData.cFileName, InStr(1, pData.cFileName, String(1, 0), vbBinaryCompare) - 1)
    EnumItemNameBag.Add strItemName
    Do
        pData.cFileName = String(MAX_PATH, 0)
        bRet = InternetFindNextFile(hFind, pData)
        If Not bRet Then
            dError = Err.LastDllError
            If dError = ERROR_NO_MORE_FILES Then
                Exit Do
            Else
                ErrorOut dError, "InternetFindNextFile"
                InternetCloseHandle (hFind)
               Exit Sub
            End If
        Else
            EnumItemAttributeBag.Add pData.dwFileAttributes
            strItemName = Left(pData.cFileName, InStr(1, pData.cFileName, String(1, 0), vbBinaryCompare) - 1)
            EnumItemNameBag.Add strItemName
       End If
    Loop
    
    InternetCloseHandle (hFind)
End Sub


Private Sub optAscii_Click()
    dwType = FTP_TRANSFER_TYPE_ASCII
End Sub

Private Sub optBin_Click()
    dwType = FTP_TRANSFER_TYPE_BINARY
End Sub

Private Sub TreeView1_DblClick()
    Dim nodX As Node
    Set nodX = TreeView1.SelectedItem
    If Not bActiveSession Then
        MsgBox "No in session!"
        Exit Sub
    End If
    If nodX Is Nothing Then
        MsgBox "no Selection to enumerate"
    End If
    If nodX.Image = "closed" Then
        nodX.Image = "open"
        FtpEnumDirectory (nodX.Text)
        FillTreeViewControl (nodX.Text)
    Else
        If nodX.Image = "open" Then
            nodX.Image = "closed"
            Dim nodChild As Node, nodNextChild As Node
            Set nodChild = nodX.Child
            Do
            Set nodNextChild = nodChild.Next
                TreeView1.Nodes.Remove nodChild.Index
                If nodNextChild Is Nothing Then Exit Do
                Set nodChild = nodNextChild
            Loop
        End If
    End If
End Sub

