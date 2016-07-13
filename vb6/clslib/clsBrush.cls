VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsBrush"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'----------------------------------------------------------------------
'   [�T�v]
'   �u���V�Ǘ��N���X
'
'   [���]
'   �쐬��: Ryota Kojima
'   �쐬��: 2003/10/09
'   ���t�@�����X: clsBrush.html

Option Explicit

'----------------------------------------------------------------------
' �֐��錾

Private Declare Function apiCreateSolidBrush Lib "gdi32" Alias "CreateSolidBrush" (ByVal crColor As Long) As Long
Private Declare Function apiDeleteObject Lib "gdi32" Alias "DeleteObject" (ByVal hObject As Long) As Long

'----------------------------------------------------------------------
' �����o�[�ϐ�

Private m_hbrush As Long            '�Ǘ�����u���V�ւ̃n���h��
Private m_mustDelete As Boolean     'Create�����u���V��ێ�����Ƃ���True
                                    'DeleteObject��False
Private m_attached As Boolean       'Attach���ꂽ�Ƃ���True
                                    'Detach���ꂽ�Ƃ���False

'----------------------------------------------------------------------
' �v���p�e�B

'----------------------------------------------------------------------
'   [�T�v]
'   �u���V�ւ̃n���h�����擾
'
'   [����]
'   �Ȃ�
'
'   [�߂�l]
'   �ێ�����u���V�ւ̃n���h��
'
'   [���l]
'   �Ȃ�

Public Property Get Brush() As Long
    Brush = m_hbrush
End Property

'----------------------------------------------------------------------
' ���\�b�h

'----------------------------------------------------------------------
'   [�T�v]
'   �\���b�h�J���[�̃u���V���쐬
'
'   [����]
'   crColor �u���V�̐F
'
'   [�߂�l]
'   True=����I���BFalse=�G���[
'
'   [���l]
'   ��d�쐬�̂Ƃ��́AAssert���܂��B

Public Function CreateSolidBrush(ByVal crColor As Long) As Boolean
    Debug.Assert m_hbrush = 0
    
    '�C���X�^���X�����łɃu���V�������Ă���Ƃ��́A�G���[�I��
    If m_hbrush <> 0 Then
        CreateSolidBrush = False
        Exit Function
    End If
    
    '�\���b�h�J���[�̃u���V���쐬
    m_hbrush = apiCreateSolidBrush(crColor)
    If m_hbrush <> 0 Then
        m_mustDelete = True         '�폜�̕K�v������u���V
        m_attached = False
        CreateSolidBrush = True
    Else
        m_mustDelete = False
        m_attached = False
        CreateSolidBrush = False
    End If
End Function

'----------------------------------------------------------------------
'   [�T�v]
'   �u���V�̍폜
'
'   [����]
'   �Ȃ�
'
'   [�߂�l]
'   True=����I���BFalse=�G���[
'
'   [���l]
'   �폜�̕K�v�������u���V��ێ����Ă���Ƃ���Assert���܂��B

Public Function DeleteObject() As Boolean
    '�C���X�^���X���u���V��ێ����Ă��Ȃ��Ƃ��́A
    '���������ɏI���B
    If m_hbrush = 0 Then
        DeleteObject = True
        Exit Function
    End If
    
    Debug.Assert m_mustDelete = True
    
    '�폜�̕K�v������
    If m_mustDelete = False Then
        DeleteObject = True
        Exit Function
    End If
    
    '�u���V���폜
    If apiDeleteObject(m_hbrush) <> 0 Then
        m_hbrush = 0
        m_mustDelete = False
        m_attached = False
        DeleteObject = True
    Else
        '�폜���s�̂Ƃ��́A�n���h����t���O�ȂǏ�Ԃ�ێ�
        DeleteObject = False
    End If
End Function

'----------------------------------------------------------------------
'   [�T�v]
'   �u���V�����蓖�Ă�
'
'   [����]
'   hbrush  GDI�u���V
'
'   [�߂�l]
'   True=����I���BFalse=�G���[
'
'   [���l]
'   ���łɃu���V��ێ����Ă���Ƃ��́AAssert���܂��B

Public Function Attach(ByVal hbrush As Long) As Boolean
    Debug.Assert m_hbrush = 0
    
    '���łɃI�u�W�F�N�g��ۗL���Ă���?
    If m_hbrush <> 0 Then
        Attach = False
        Exit Function
    End If
    
    m_hbrush = hbrush
    m_mustDelete = False    '�폜�̕K�v�Ȃ�
    m_attached = True
    
    Attach = True
End Function

'----------------------------------------------------------------------
'   [�T�v]
'   ���蓖�Ă��u���V���J������
'
'   [����]
'   �Ȃ�
'
'   [�߂�l]
'   ���蓖�ĂĂ������u���V
'
'   [���l]
'   Attach���ꂽ�u���V���Ǘ����Ă��Ȃ��Ƃ��́AAssert���܂��B

Public Function Detach() As Long
    '�C���X�^���X���u���V��ێ����Ă��Ȃ��Ƃ��́A
    '���������ɏI���B
    If m_hbrush = 0 Then
        Detach = 0
        Exit Function
    End If
    
    Debug.Assert m_attached = True
    
    '���L����I�u�W�F�N�g�́AAttach���ꂽ���̂ł͂Ȃ��B
    If m_attached = False Then
        Detach = 0
        Exit Function
    End If
    
    Detach = m_hbrush
    
    m_hbrush = 0
    m_mustDelete = False
    m_attached = False
End Function

'----------------------------------------------------------------------
'   [�T�v]
'   �R���X�g���N�^
'
'   [����]
'   �Ȃ�
'
'   [�߂�l]
'   �Ȃ�
'
'   [���l]
'   �Ȃ�

Private Sub Class_Initialize()
    m_hbrush = 0
    m_mustDelete = False
    m_attached = False
End Sub

'----------------------------------------------------------------------
'   [�T�v]
'   �f�X�g���N�^
'
'   [����]
'   �Ȃ�
'
'   [�߂�l]
'   �Ȃ�
'
'   [���l]
'   Attach�����u���V��Detach�����ɏI�������ꍇ�AAssert���܂��B
'   DeleteObject�Ɏ��s�����ꍇ�AAssert���܂��B

Private Sub Class_Terminate()
    Debug.Assert m_attached = False
    
    Dim ret As Boolean
    
    '�폜�̕K�v������u���V���m�ۂ��ꂽ�܂�?
    If m_hbrush <> 0 And m_mustDelete = True Then
        ret = DeleteObject
        Debug.Assert ret = True
    End If
End Sub