Attribute VB_Name = "mdlSysUtils"
Option Explicit

'----------------------------------------------------------------------
' Win32�֌W�̊e��錾
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
' [�T�v]
' �w�肵���E�B���h�E���A���������T�|�[�g���邩�`�F�b�N����
'
' [����]
' hwnd          �E�B���h�E�ւ̃n���h��
'
' [�߂�l]
' �T�|�[�g����      True
' �T�|�[�g���Ȃ�    False
'
' [��O]
' �G���[�����������ꍇ�AWin32�̃G���[�R�[�h�Ń����^�C���G���[�𔭐�������B
' �G���[�R�[�h�́AWINERROR.H�𒲂ׂĂ��������B
'
' [����]
' �������@�\���T�|�[�g���Ă��邩�ǂ����`�F�b�N���܂��B
'----------------------------------------------------------------------

Public Function IsLayered(ByVal hWnd As Long) As Boolean
    Dim exStyle As Long
    
    '�E�B���h�E�X�^�C�����擾����
    exStyle = GetWindowLong(hWnd, GWL_EXSTYLE)
    If exStyle = 0 Then
        Err.Raise Err.LastDllError
    End If
    
    '�������T�|�[�g���́ATrue, ���T�|�[�g����False��Ԃ�
    IsLayered = IIf((exStyle And WS_EX_LAYERED) = WS_EX_LAYERED, True, False)
End Function

'----------------------------------------------------------------------
' [�T�v]
' �w�肵���E�B���h�E�́A�������T�|�[�g��ON/OFF���܂��B
'
' [����]
' hwnd      �E�B���h�E�ւ̃n���h��
' bLayered  �������T�|�[�g(True:ON False:OFF)
'
' [�߂�l]
' �ݒ�ł���        0
' �ݒ�ł��Ȃ�����  -1
' ���̑��̃G���[    Win32�̃G���[�R�[�h
'
' [����]
' �������@�\��ON/OFF���s���B
'----------------------------------------------------------------------

Public Function SetLayered(ByVal hWnd As Long, ByVal bLayered As Boolean) As Long
    Dim exStyle As Long
    
    '�E�B���h�E�X�^�C�����擾����
    exStyle = GetWindowLong(hWnd, GWL_EXSTYLE)
    If exStyle = 0 Then
        SetLayered = Err.LastDllError
        Exit Function
    End If
    
    '�����ɉ����Ĕ������T�|�[�g�̃t���O��ω�������
    If bLayered Then
        exStyle = exStyle Or WS_EX_LAYERED
    Else
        exStyle = exStyle And (Not WS_EX_LAYERED)
    End If
    
    '�E�B���h�E�X�^�C����ݒ肷��B
    SetLastError 0
    exStyle = SetWindowLong(hWnd, GWL_EXSTYLE, exStyle)
    If exStyle = 0 And Err.LastDllError <> 0 Then
        SetLayered = Err.LastDllError
        Exit Function
    End If
    
    On Error GoTo Err_SetLayered
    
    '�������̃T�|�[�g��Ԃ��ݒ�ł������ǂ�����߂�
    SetLayered = IIf(IsLayered(hWnd) = bLayered, 0, -1)
    
    Exit Function

Err_SetLayered:
    SetLayered = Err
    
End Function

'----------------------------------------------------------------------
' [�T�v]
' �E�B���h�E��s����������
'
' [����]
' hwnd      �E�B���h�E�ւ̃n���h��
' dOpacity  �s�����̊���(0.0�`1.0)
'
' [�߂�l]
' ����I��          0
' �G���[�I��        -1
' ���̑��̃G���[    Win32�G���[�R�[�h
'
' [����]
' �E�B���h�E��s���������܂��B
'----------------------------------------------------------------------

Public Function SetOpacity(ByVal hWnd As Long, ByVal dOpacity As Double) As Long
    On Error GoTo Err_SetOpacity
    
    Dim ret As Long
    
    '���������T�|�[�g�E�B���h�E�𔼓����ɂ��悤�Ƃ����Ƃ��́A�t���O���Z�b�g����
    '�t���O���Z�b�g�ł��Ȃ��Ƃ��́A�G���[���^�[��
    If IsLayered(hWnd) = False Then
        ret = SetLayered(hWnd, True)
        If ret <> 0 Then
            SetOpacity = ret
            Exit Function
        End If
    End If
    
    '��������(�s������)����
    ret = SetLayeredWindowAttributes(hWnd, 0, 255 * dOpacity, LWA_ALPHA)
    If ret = 0 Then
        SetOpacity = Err.LastDllError
    End If
    
    Exit Function
    
Err_SetOpacity:
    SetOpacity = Err
    
End Function

'----------------------------------------------------------------------
' [�T�v]
' �w��t�H���_�ȉ��ɂ���t�@�C�������ׂč폜����B
'
' [����]
' strFolder     ���̃t�H���_��艺�ɂ���t�@�C�����폜����
' strExcept     ���̖��O�Ɉ�v����t�@�C��/�t�H���_�͍폜���Ȃ��B
'
' [�߂�l]
' ����I��  0
' �G���[    0�ȊO�BError�֐��ŃG���[��������擾�\�B
'
' [����]
' strFolder�ȉ��ɂ���t�@�C�����AstrExcept�������Ă��ׂč폜����B
' strExcept�́A�P���ȕ�����B�t�H���_���ƈ�v����΁A�t�H���_��
' �ȉ����ׂĂ��폜�ΏۊO�ɂ��܂��B
'
' [���l]
' �Ԉ���āAc:\�Ƃ��w�肵����A������ρB
'----------------------------------------------------------------------

Public Function RemoveSubFolders(strFolder As String, strExcept As String) As Long
    On Error GoTo Err_RemoveSubFolders
    
    Dim objFS As FileSystemObject
    Dim objFolder As Folder
    Dim objFile As File
    Dim objSubFolder As Folder
    
    Set objFS = New FileSystemObject
    
    '�w�肳�ꂽ�t�H���_�I�u�W�F�N�g���擾����
    Set objFolder = objFS.GetFolder(strFolder)
    
    '�w�肳�ꂽ�t�H���_�����̃t�@�C�����폜����
    For Each objFile In objFolder.Files
        If UCase(objFile.Name) <> UCase(strExcept) Then
            objFile.Delete True
        End If
    Next
    
    '�w�肳�ꂽ�t�H���_�����̃t�H���_���폜����
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
