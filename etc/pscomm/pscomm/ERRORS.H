#ifndef ERRORS_INCLUDED
#define ERRORS_INCLUDED

#include <windows.h>

/* ERRORS.H */

enum {
      ERR_MIN = 0,                     // �G���[�ԍ��͂��ׂĂ��̒l�ȏ�
      ERR_NOT_DIB = 0,                 // DIB�ł͂Ȃ��t�@�C�������[�h���悤
                                       // �Ƃ���!
      ERR_MEMORY,                      // �������s��!
      ERR_READ,                        // �t�@�C���ǂݎ��G���[!
      ERR_LOCK,                        // GlobalLock()�̃G���[!
      ERR_OPEN,                        // �t�@�C���I�[�v���G���[!
      ERR_CREATEPAL,                   // �p���b�g�쐬�G���[
      ERR_GETDC,                       // DC���擾�ł��Ȃ�
      ERR_CREATECHILD,                 // MDI�q�E�B���h�E�쐬�G���[
      ERR_CREATEDDB,                   // DDB�쐬�G���[
      ERR_STRETCHBLT,                  // StretchBlt()�����s��Ԃ���
      ERR_STRETCHDIBITS,               // StretchDIBits()�����s��Ԃ���
      ERR_NODIBORDDB,                  // �`��G���[ - DDB��DIB�̗������K�v
      ERR_SETDIBITSTODEVICE,           // SetDIBitsToDevice()���Ȃ�
      ERR_STARTDOC,                    // StartDoc()�Ăяo���G���[
      ERR_NOGDIMODULE,                 // ����������GDI���Ȃ�
      ERR_SETABORTPROC,                // SetAbortProc()�Ăяo���G���[
      ERR_STARTPAGE,                   // StartPage()�Ăяo���G���[
      ERR_NEWFRAME,                    // NEWFRAME�G�X�P�[�v�Ăяo���G���[
      ERR_ENDPAGE,                     // EndPage()�Ăяo���G���[
      ERR_ENDDOC,                      // EndDoc()�Ăяo���G���[
      ERR_ANIMATE,                     // �����ɃA�j���[�g�ł���DIB��1����
      ERR_NOTIMERS,                    // �A�j���[�V�����p�̃^�C�}�[���Ȃ�
      ERR_NOCLIPWINDOW,                // �N���b�v�{�[�h�̃E�B���h�E���Ȃ�
      ERR_CLIPBUSY,                    // �N���b�v�{�[�h���g�p��
      ERR_NOCLIPFORMATS,               // �\��t������DIB��DDB���Ȃ�
      ERR_SETDIBITS,                   // SetDIBits()�Ăяo���G���[
      ERR_FILENOTFOUND,                // GetDib()�̃t�@�C���I�[�v���G���[
      ERR_WRITEDIB,                    // DIB�t�@�C���������݃G���[
      ERR_MAX                          // ���ׂẴG���[�͂��̒l����������
     };

#ifdef __cplusplus
extern "C" {
#endif
void DIBError (int ErrNo);
#ifdef __cplusplus
}
#endif

#endif //ERRORS_INCLUDED