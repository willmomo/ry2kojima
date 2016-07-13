Attribute VB_Name = "mdlDrtSub"
Option Explicit

Public Declare Function fno1000_init Lib "drtsub.dll" (ByVal fname As String, ByRef buffer As Any, ByVal bufsize As Long) As Long
Public Declare Function fno1000_get_daisu Lib "drtsub.dll" (ByRef buffer As Any) As Long
Public Declare Function fno1000_get_rs Lib "drtsub.dll" (ByRef buffer As Any) As Long
Public Declare Function fno1001_init Lib "drtsub.dll" (ByVal fname As String, ByRef buffer As Any, ByVal bufsize As Long) As Long
Public Declare Function fno1001_get_hdb Lib "drtsub.dll" (ByRef buffer As Any, ByVal rdb As Long) As Long
Public Declare Function fno1001_get_sbt Lib "drtsub.dll" (ByRef buffer As Any, ByVal rdb As Long) As Long
Public Declare Function fno1001_get_kinko Lib "drtsub.dll" (ByRef buffer As Any, ByVal rdb As Long) As Long
Public Declare Function fno1002_init Lib "drtsub.dll" (ByVal fname As String, ByRef buffer As Any, ByVal bufsize As Long) As Long
Public Declare Function fno1002_get_disp1 Lib "drtsub.dll" (ByRef buffer As Any, ByVal idx As Long) As Long
Public Declare Function fno1002_get_disp2 Lib "drtsub.dll" (ByRef buffer As Any, ByVal idx As Long) As Long
Public Declare Function fno1002_get_kind Lib "drtsub.dll" (ByRef buffer As Any, ByVal idx As Long) As Long
Public Declare Function fno1007_init Lib "drtsub.dll" (ByVal fname As String, ByRef buffer As Any, ByVal bufsize As Long) As Long
Public Declare Function fno1007_get_urikin Lib "drtsub.dll" (ByRef buffer As Any, ByVal sbt As Long) As Long
Public Declare Function fno1007_get_urikos Lib "drtsub.dll" (ByRef buffer As Any, ByVal sbt As Long) As Long
Public Declare Function fno1007_get_inpscl Lib "drtsub.dll" (ByRef buffer As Any, ByVal sbt As Long) As Long
Public Declare Function fno1008_init Lib "drtsub.dll" (ByVal fname As String, ByRef buffer As Any, ByVal bufsize As Long) As Long
Public Declare Function fno1008_get_sbtname Lib "drtsub.dll" (ByRef buffer As Any, ByVal sbt As Long, ByVal sbtname As String) As Long
Public Declare Function fno4102_init Lib "drtsub.dll" (ByVal fname As String, ByRef buffer As Any, ByVal bufsize As Long) As Long
Public Declare Function fno4102_get_out Lib "drtsub.dll" (ByRef buffer As Any, ByVal rdb As Long) As Long
Public Declare Function fno4102_get_safe Lib "drtsub.dll" (ByRef buffer As Any, ByVal rdb As Long) As Long
Public Declare Function fno4102_get_start Lib "drtsub.dll" (ByRef buffer As Any, ByVal rdb As Long) As Long
Public Declare Function fno4102_get_gen1uri Lib "drtsub.dll" (ByRef buffer As Any, ByVal rdb As Long) As Long
Public Declare Function fno4102_get_gen2uri Lib "drtsub.dll" (ByRef buffer As Any, ByVal rdb As Long) As Long
Public Declare Function fno4103_init Lib "drtsub.dll" (ByVal fname As String, ByRef buffer As Any, ByVal bufsize As Long) As Long
Public Declare Function fno4103_get_indata Lib "drtsub.dll" (ByRef buffer As Any, ByVal rdb As Long) As Long
Public Declare Function fno640X_build Lib "drtsub.dll" ( _
    ByRef buf6408 As Any, _
    ByVal size6408 As Long, _
    ByRef buf6409 As Any, _
    ByVal size6409 As Long, _
    ByRef buf1000 As Any, _
    ByRef buf1001 As Any, _
    ByRef buf1002 As Any, _
    ByRef buf4102 As Any, _
    ByRef buf4103 As Any, _
    ByRef relDai As Any, _
    ByRef relZen As Any) As Long
Public Declare Function lec_calccrc Lib "drtsub.dll" (ByRef buffer As Any, ByVal size As Long) As Integer
    
Public Type DAI_RELATIVE_DATA
    out As Long
    safe As Long
    start As Long
    gen1uri As Long
    gen2uri As Long
End Type

Public Type ZEN_RELATIVE_DATA
    urikin As Long
End Type

Public Const MAX_DAISU As Long = 1536
Public Const MAX_ZENTENSU As Long = 256
Public Const MAX_SHUBETSU As Long = 8

Private Const RECSIZE_FNO4102 As Long = 192

Private Const SIZE_OF_FNO1000 As Long = 128
Private Const SIZE_OF_FNO1001 As Long = 20 * MAX_DAISU
Private Const SIZE_OF_FNO1002 As Long = 16 * MAX_ZENTENSU
Private Const SIZE_OF_FNO1007 As Long = 16 * MAX_SHUBETSU
Private Const SIZE_OF_FNO1008 As Long = 192
Private Const SIZE_OF_FNO4102 As Long = RECSIZE_FNO4102 * MAX_DAISU
Private Const SIZE_OF_FNO4103 As Long = 8 * MAX_ZENTENSU
Private Const SIZE_OF_FNO6408 As Long = SIZE_OF_FNO4102
Private Const SIZE_OF_FNO6409 As Long = SIZE_OF_FNO4103

Private s_fno1000(SIZE_OF_FNO1000 - 1) As Byte
Private s_fno1001(SIZE_OF_FNO1001 - 1) As Byte
Private s_fno1002(SIZE_OF_FNO1002 - 1) As Byte
Private s_fno1007(SIZE_OF_FNO1007 - 1) As Byte
Private s_fno1008(SIZE_OF_FNO1008 - 1) As Byte
Private s_fno4102(SIZE_OF_FNO4102 - 1) As Byte
Private s_fno4103(SIZE_OF_FNO4103 - 1) As Byte
Private s_fno6408(SIZE_OF_FNO6408 - 1) As Byte
Private s_fno6409(SIZE_OF_FNO6409 - 1) As Byte

Private Const DEF_FILE_NAME As String = "reldata.csv"

Private g_fileName As String
Public g_daiRel(MAX_DAISU - 1) As DAI_RELATIVE_DATA
Public g_zenRel(MAX_ZENTENSU - 1) As ZEN_RELATIVE_DATA

Public Function f1000_get_daisu() As Long
    f1000_get_daisu = fno1000_get_daisu(s_fno1000(0))
End Function

Public Function f1000_get_rs() As Long
    f1000_get_rs = fno1000_get_rs(s_fno1000(0))
End Function

Public Function f1001_get_hdb(ByVal rdb As Long) As Long
    f1001_get_hdb = fno1001_get_hdb(s_fno1001(0), rdb)
End Function

Public Function f1001_get_sbt(ByVal rdb As Long) As Long
    f1001_get_sbt = fno1001_get_sbt(s_fno1001(0), rdb)
End Function

Public Function f1001_get_kinko(ByVal rdb As Long) As Long
    f1001_get_kinko = fno1001_get_kinko(s_fno1001(0), rdb)
End Function

Public Function f1002_get_disp1(ByVal idx As Long) As Long
    f1002_get_disp1 = fno1002_get_disp1(s_fno1002(0), idx)
End Function

Public Function f1002_get_disp2(ByVal idx As Long) As Long
    f1002_get_disp2 = fno1002_get_disp2(s_fno1002(0), idx)
End Function

Public Function f1002_get_kind(ByVal idx As Long) As Long
    f1002_get_kind = fno1002_get_kind(s_fno1002(0), idx)
End Function

Public Function f1002_get_kind_string(ByVal idx As Long) As String
    Dim kind As Long
    
    kind = fno1002_get_kind(s_fno1002(0), idx)
    If 1 <= kind And kind <= 10 Then
        f1002_get_kind_string = Array( _
            "金庫", "景品機", "現金販売機", "カード販売機", _
            "リプレイ玉", "プレミアム玉", "カード発行機", _
            "POS", "自販機", "その他")(kind - 1)
    End If
End Function

Public Function f1007_get_urikin(ByVal sbt As Long) As Long
    f1007_get_urikin = fno1007_get_urikin(s_fno1007(0), sbt)
End Function

Public Function f1007_get_urikos(ByVal sbt As Long) As Long
    f1007_get_urikos = fno1007_get_urikos(s_fno1007(0), sbt)
End Function

Public Function f1007_get_inpscl(ByVal sbt As Long) As Long
    f1007_get_inpscl = fno1007_get_inpscl(s_fno1007(0), sbt)
End Function

Public Function f1008_get_sbtname(ByVal sbt As Long) As String
    Dim sbtname As String
    
    sbtname = String(32, vbNullChar)
    Call fno1008_get_sbtname(s_fno1008(0), sbt, sbtname)
    f1008_get_sbtname = Left(sbtname, InStr(sbtname, vbNullChar) - 1)
End Function

Public Function f4102_get_out(ByVal rdb As Long) As Long
    f4102_get_out = fno4102_get_out(s_fno4102(0), rdb)
End Function

Public Function f4102_get_safe(ByVal rdb As Long) As Long
    f4102_get_safe = fno4102_get_safe(s_fno4102(0), rdb)
End Function

Public Function f4102_get_start(ByVal rdb As Long) As Long
    f4102_get_start = fno4102_get_start(s_fno4102(0), rdb)
End Function

Public Function f4102_get_gen1uri(ByVal rdb As Long) As Long
    f4102_get_gen1uri = fno4102_get_gen1uri(s_fno4102(0), rdb)
End Function

Public Function f4102_get_gen2uri(ByVal rdb As Long) As Long
    f4102_get_gen2uri = fno4102_get_gen2uri(s_fno4102(0), rdb)
End Function

Public Function f4103_get_indata(ByVal idx As Long) As Long
    f4103_get_indata = fno4103_get_indata(s_fno4103(0), idx)
End Function

Public Sub set_out_rel(ByVal rdb As Long, ByVal newValue As Long, ByVal dataLink As Boolean)
    g_daiRel(rdb - 1).out = newValue
    
    If dataLink Then
        g_daiRel(rdb - 1).safe = g_daiRel(rdb - 1).safe + newValue
    End If
End Sub

Public Sub set_safe_rel(ByVal rdb As Long, ByVal newValue As Long, ByVal dataLink As Boolean)
    g_daiRel(rdb - 1).safe = newValue
    
    If dataLink Then
        g_daiRel(rdb - 1).out = g_daiRel(rdb - 1).out + newValue
    End If
End Sub

Public Sub set_start_rel(ByVal rdb As Long, ByVal newValue As Long, ByVal dataLink As Boolean)
    g_daiRel(rdb - 1).start = newValue
    
    If dataLink Then
    End If
End Sub

Public Sub set_gen1uri_rel(ByVal rdb As Long, ByVal newValue As Long, ByVal dataLink As Boolean)
    Dim sbt As Long
    
    g_daiRel(rdb - 1).gen1uri = newValue
    
    If dataLink Then
        sbt = f1001_get_sbt(rdb)
        g_daiRel(rdb - 1).safe = g_daiRel(rdb - 1).safe + _
            (newValue / f1007_get_urikin(sbt) * f1007_get_urikos(sbt)) / f1007_get_inpscl(sbt)
        g_zenRel(f1001_get_kinko(rdb) - 1).urikin = g_zenRel(f1001_get_kinko(rdb) - 1).urikin + newValue
    End If
End Sub

Public Sub set_gen2uri_rel(ByVal rdb As Long, ByVal newValue As Long, ByVal dataLink As Boolean)
    Dim sbt As Long
    
    g_daiRel(rdb - 1).gen2uri = newValue
    
    If dataLink Then
        sbt = f1001_get_sbt(rdb)
        g_daiRel(rdb - 1).safe = g_daiRel(rdb - 1).safe + _
            (newValue / f1007_get_urikin(sbt) * f1007_get_urikos(sbt)) / f1007_get_inpscl(sbt)
        g_zenRel(f1001_get_kinko(rdb) - 1).urikin = g_zenRel(f1001_get_kinko(rdb) - 1).urikin + newValue
    End If
End Sub

Public Sub f640X_build()
    fno640X_build _
        s_fno6408(0), SIZE_OF_FNO6408, _
        s_fno6409(0), SIZE_OF_FNO6409, _
        s_fno1000(0), _
        s_fno1001(0), _
        s_fno1002(0), _
        s_fno4102(0), _
        s_fno4103(0), _
        g_daiRel(0), _
        g_zenRel(0)
End Sub

Private Sub MakeLocalFile()
    Dim fno As Integer
    Dim rs As Long
    Dim i As Long
    Dim crc As Integer
    Dim work(RECSIZE_FNO4102 * 512 - 1) As Byte
    
    For rs = 1 To 3
        fno = FreeFile
        Open "mf6408_" & CStr(rs) & ".bin" For Binary As fno
        
        For i = 0 To UBound(work)
            work(i) = s_fno6408(((rs - 1) * 512 * RECSIZE_FNO4102) + i)
        Next
        
        Put fno, , work
        Close fno
        
        crc = lec_calccrc(work(0), RECSIZE_FNO4102 * 512)
        fno = FreeFile
        Open "mf6408_" & CStr(rs) & ".crc" For Binary As fno
        Put fno, , crc
        Close fno
    Next
    
    fno = FreeFile
    Open "mf6409.bin" For Binary As fno
    Put fno, , s_fno6409
    Close fno

    crc = lec_calccrc(s_fno6409(0), SIZE_OF_FNO6409)
    fno = FreeFile
    Open "mf6409.crc" For Binary As fno
    Put fno, , crc
    Close fno
End Sub

Public Sub f640X_put()
    Dim rc As Long
    Dim rs As Long
    Dim ftp As BASP21Lib.ftp
    
    Set ftp = New BASP21Lib.ftp
    
    MakeLocalFile
    
    For rs = 1 To f1000_get_rs + 1
        rc = ftp.Connect("as" & CStr(rs), "super", "user")
        If rc = 0 Then
            rc = ftp.PutFile("mf6408_" & CStr(rs) & ".bin", "/r0", 1)
            rc = ftp.PutFile("mf6408_" & CStr(rs) & ".crc", "/r0", 1)
            rc = ftp.PutFile("mf6409.bin", "/r0", 1)
            rc = ftp.PutFile("mf6409.crc", "/r0", 1)
            ftp.Close
        End If
    Next
    
    Kill "mf6408_?.bin"
    Kill "mf6408_?.crc"
    Kill "mf6409.???"
    
    Set ftp = Nothing
End Sub

'データ復旧処理
'
Public Function DataRecover() As Long
    If CheckGuid Then
    
        f640X_build
        f640X_put
        
        SaveGuid
        
        MsgBox "データ復旧終了しました。"
    Else
        MsgBox "システム情報が一致しません。このPCでは、このツールを使うことが出来ません。"
    End If
End Function

'代表ドキュメントファイル名そ取得する
'
Public Function GetDocumentName() As String
    If g_fileName = "" Then
        GetDocumentName = DEF_FILE_NAME
    Else
        GetDocumentName = g_fileName
    End If
End Function

'新規ドキュメント作成
'
'補正情報をクリアして、操作から元データを取得する。
'
Public Function NewDocument() As Boolean
    Erase g_daiRel
    Erase g_zenRel
    
    NewDocument = LoadBaseFile
End Function

'指定したファイルを開く
'
'prefix_[d/z].binを開いて読み込む。
'
Public Function OpenDocument(typicalName As String) As Boolean
    Dim skipData As Long
    Dim i As Long
    Dim fno As Integer
    
    g_fileName = typicalName
    
    '台の差分データファイルを読み込む
    i = 0
    fno = FreeFile
    Open Replace(g_fileName, ".csv", "_d.csv") For Input As fno
    Do While Not EOF(fno)
        Input #fno, skipData, g_daiRel(i).out, g_daiRel(i).safe, g_daiRel(i).start, g_daiRel(i).gen1uri, g_daiRel(i).gen2uri
        
        i = i + 1
    Loop
    Close fno

    '全店の差分データファイルを読み込む
    i = 0
    fno = FreeFile
    Open Replace(g_fileName, ".csv", "_z.csv") For Input As fno
    Do While Not EOF(fno)
        Input #fno, skipData, g_zenRel(i).urikin
        
        i = i + 1
    Loop
    Close fno
    
    OpenDocument = True
End Function

'差分情報をファイルに保存する
'
Public Function SaveDocument(typicalName As String) As Boolean
    Dim i As Long
    Dim fno As Integer
    
    'ファイルopen/saveの都合で代表するファイルを作成する(日付を入れてあるだけ)
    fno = FreeFile
    Open typicalName For Output As fno
    Write #fno, Now
    Close fno
    
    '台の差分情報を保存
    i = 0
    fno = FreeFile
    Open Replace(typicalName, ".csv", "_d.csv") For Output As fno
    For i = 0 To UBound(g_daiRel)
        Write #fno, i + 1, g_daiRel(i).out, g_daiRel(i).safe, g_daiRel(i).start, g_daiRel(i).gen1uri, g_daiRel(i).gen2uri
    Next
    Close fno
    
    '全店の差分情報を保存
    i = 0
    fno = FreeFile
    Open Replace(typicalName, ".csv", "_z.csv") For Output As fno
    For i = 0 To UBound(g_zenRel)
        Write #fno, i + 1, g_zenRel(i).urikin
    Next
    Close fno
End Function

'元ファイル読み込み
'
'主操作のKPCフォルダから、元になるファイルを読み込む。
'
Public Function LoadBaseFile() As Boolean
    Dim wssName As String
    Dim fno As Integer
    
    wssName = GetSetting(App.ProductName, "Settings", "WssName", "sss")
    
    'mf1000.binの読み込み
    If fno1000_init("\\" & wssName & "\kpc\mf1000.bin", s_fno1000(0), SIZE_OF_FNO1000) <> 0 Then
        LoadBaseFile = False
        Exit Function
    End If
    
    'mf1001.binの読み込み
    If fno1001_init("\\" & wssName & "\kpc\mf1001.bin", s_fno1001(0), SIZE_OF_FNO1001) <> 0 Then
        LoadBaseFile = False
        Exit Function
    End If
    
    'mf1002.binの読み込み
    If fno1002_init("\\" & wssName & "\kpc\mf1002.bin", s_fno1002(0), SIZE_OF_FNO1002) <> 0 Then
        LoadBaseFile = False
        Exit Function
    End If
    
    'mf1007.binの読み込み
    If fno1007_init("\\" & wssName & "\kpc\mf1007.bin", s_fno1007(0), SIZE_OF_FNO1007) <> 0 Then
        LoadBaseFile = False
        Exit Function
    End If
    
    'mf1008.binの読み込み
    If fno1008_init("\\" & wssName & "\kpc\mf1008.bin", s_fno1008(0), SIZE_OF_FNO1008) <> 0 Then
        LoadBaseFile = False
        Exit Function
    End If
    
    'mf4102.binの読み込み
    If fno4102_init("\\" & wssName & "\kpc\mf4102.bin", s_fno4102(0), SIZE_OF_FNO4102) <> 0 Then
        LoadBaseFile = False
        Exit Function
    End If
    
    'mf4103.binの読み込み
    If fno4103_init("\\" & wssName & "\kpc\mf4103.bin", s_fno4103(0), SIZE_OF_FNO4103) <> 0 Then
        LoadBaseFile = False
        Exit Function
    End If
    
    LoadBaseFile = True
End Function
