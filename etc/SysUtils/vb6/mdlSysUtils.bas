Attribute VB_Name = "mdlSysUtils"
Option Explicit

'----------------------------------------------------------------------
' Win32関係の各種宣言
'----------------------------------------------------------------------

Private Const WS_EX_LAYERED As Long = &H80000

Private Const GWL_EXSTYLE As Long = (-20)

Private Const LWA_ALPHA As Long = 2

Private Declare Function SetLayeredWindowAttributes Lib "user32.dll" ( _
    ByVal hWnd As Long, _
    ByVal crKey As Long, _
    ByVal bAlpha As Byte, _
    ByVal dwFlags As Long _
) As Long

Private Declare Function GetWindowLong Lib "user32" Alias "GetWindowLongA" ( _
    ByVal hWnd As Long, _
    ByVal nIndex As Long _
) As Long

Private Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" ( _
    ByVal hWnd As Long, _
    ByVal nIndex As Long, _
    ByVal dwNewLong As Long _
) As Long

Private Declare Sub SetLastError Lib "kernel32" (ByVal dwErrCode As Long)

'----------------------------------------------------------------------
' [概要]
' 指定したウィンドウが、半透明をサポートするかチェックする
'
' [引数]
' hwnd          ウィンドウへのハンドル
'
' [戻り値]
' サポートする      True
' サポートしない    False
'
' [例外]
' エラーが発生した場合、Win32のエラーコードでランタイムエラーを発生させる。
' エラーコードは、WINERROR.Hを調べてください。
'
' [説明]
' 半透明機能をサポートしているかどうかチェックします。
'----------------------------------------------------------------------

Public Function IsLayered(ByVal hWnd As Long) As Boolean
    Dim exStyle As Long
    
    'ウィンドウスタイルを取得する
    exStyle = GetWindowLong(hWnd, GWL_EXSTYLE)
    If exStyle = 0 Then
        Err.Raise Err.LastDllError
    End If
    
    '半透明サポート時は、True, 未サポート時はFalseを返す
    IsLayered = IIf((exStyle And WS_EX_LAYERED) = WS_EX_LAYERED, True, False)
End Function

'----------------------------------------------------------------------
' [概要]
' 指定したウィンドウの、半透明サポートをON/OFFします。
'
' [引数]
' hwnd      ウィンドウへのハンドル
' bLayered  半透明サポート(True:ON False:OFF)
'
' [戻り値]
' 設定できた        0
' 設定できなかった  -1
' その他のエラー    Win32のエラーコード
'
' [説明]
' 半透明機能のON/OFFを行う。
'----------------------------------------------------------------------

Public Function SetLayered(ByVal hWnd As Long, ByVal bLayered As Boolean) As Long
    Dim exStyle As Long
    
    'ウィンドウスタイルを取得する
    exStyle = GetWindowLong(hWnd, GWL_EXSTYLE)
    If exStyle = 0 Then
        SetLayered = Err.LastDllError
        Exit Function
    End If
    
    '引数に応じて半透明サポートのフラグを変化させる
    If bLayered Then
        exStyle = exStyle Or WS_EX_LAYERED
    Else
        exStyle = exStyle And (Not WS_EX_LAYERED)
    End If
    
    'ウィンドウスタイルを設定する。
    SetLastError 0
    exStyle = SetWindowLong(hWnd, GWL_EXSTYLE, exStyle)
    If exStyle = 0 And Err.LastDllError <> 0 Then
        SetLayered = Err.LastDllError
        Exit Function
    End If
    
    On Error GoTo Err_SetLayered
    
    '半透明のサポート状態が設定できたかどうかを戻す
    SetLayered = IIf(IsLayered(hWnd) = bLayered, 0, -1)
    
    Exit Function

Err_SetLayered:
    SetLayered = Err
    
End Function

'----------------------------------------------------------------------
' [概要]
' ウィンドウを不透明化する
'
' [引数]
' hwnd      ウィンドウへのハンドル
' dOpacity  不透明の割合(0.0～1.0)
'
' [戻り値]
' 正常終了          0
' エラー終了        -1
' その他のエラー    Win32エラーコード
'
' [説明]
' ウィンドウを不透明化します。
'----------------------------------------------------------------------

Public Function SetOpacity(ByVal hWnd As Long, ByVal dOpacity As Double) As Long
    On Error GoTo Err_SetOpacity
    
    Dim ret As Long
    
    '半透明未サポートウィンドウを半透明にしようとしたときは、フラグをセットする
    'フラグがセットできないときは、エラーリターン
    If IsLayered(hWnd) = False Then
        ret = SetLayered(hWnd, True)
        If ret <> 0 Then
            SetOpacity = ret
            Exit Function
        End If
    End If
    
    '半透明化(不透明化)する
    ret = SetLayeredWindowAttributes(hWnd, 0, 255 * dOpacity, LWA_ALPHA)
    If ret = 0 Then
        SetOpacity = Err.LastDllError
    End If
    
    Exit Function
    
Err_SetOpacity:
    SetOpacity = Err
    
End Function

'----------------------------------------------------------------------
' [概要]
' 指定フォルダ以下にあるファイルをすべて削除する。
'
' [引数]
' strFolder     このフォルダより下にあるファイルを削除する
' strExcept     この名前に一致するファイル/フォルダは削除しない。
'
' [戻り値]
' 正常終了  0
' エラー    0以外。Error関数でエラー文字列を取得可能。
'
' [説明]
' strFolder以下にあるファイルを、strExceptを除いてすべて削除する。
' strExceptは、単純な文字列。フォルダ名と一致すれば、フォルダを
' 以下すべてを削除対象外にします。
'
' [備考]
' 間違って、c:\とか指定したら、もう大変。
'----------------------------------------------------------------------

Public Function RemoveSubFolders(strFolder As String, strExcept As String) As Long
    On Error GoTo Err_RemoveSubFolders
    
    Dim objFS As FileSystemObject
    Dim objFolder As Folder
    Dim objFile As File
    Dim objSubFolder As Folder
    
    Set objFS = New FileSystemObject
    
    '指定されたフォルダオブジェクトを取得する
    Set objFolder = objFS.GetFolder(strFolder)
    
    '指定されたフォルダ直下のファイルを削除する
    For Each objFile In objFolder.Files
        If UCase(objFile.Name) <> UCase(strExcept) Then
            objFile.Delete True
        End If
    Next
    
    '指定されたフォルダ直下のフォルダを削除する
    For Each objSubFolder In objFolder.SubFolders
        If UCase(objSubFolder.Name) <> UCase(strExcept) Then
            objSubFolder.Delete True
        End If
    Next

Exit_RemoveSubFolders:
    Set objFolder = Nothing
    Set objFS = Nothing
    
    Exit Function
    
Err_RemoveSubFolders:
    RemoveSubFolders = Err.Number
    Resume Exit_RemoveSubFolders
    
End Function
