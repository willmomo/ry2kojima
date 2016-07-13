#ifndef ERRORS_INCLUDED
#define ERRORS_INCLUDED

#include <windows.h>

/* ERRORS.H */

enum {
      ERR_MIN = 0,                     // エラー番号はすべてこの値以上
      ERR_NOT_DIB = 0,                 // DIBではないファイルをロードしよう
                                       // とした!
      ERR_MEMORY,                      // メモリ不足!
      ERR_READ,                        // ファイル読み取りエラー!
      ERR_LOCK,                        // GlobalLock()のエラー!
      ERR_OPEN,                        // ファイルオープンエラー!
      ERR_CREATEPAL,                   // パレット作成エラー
      ERR_GETDC,                       // DCを取得できない
      ERR_CREATECHILD,                 // MDI子ウィンドウ作成エラー
      ERR_CREATEDDB,                   // DDB作成エラー
      ERR_STRETCHBLT,                  // StretchBlt()が失敗を返した
      ERR_STRETCHDIBITS,               // StretchDIBits()が失敗を返した
      ERR_NODIBORDDB,                  // 描画エラー - DDBとDIBの両方が必要
      ERR_SETDIBITSTODEVICE,           // SetDIBitsToDevice()がない
      ERR_STARTDOC,                    // StartDoc()呼び出しエラー
      ERR_NOGDIMODULE,                 // メモリ中にGDIがない
      ERR_SETABORTPROC,                // SetAbortProc()呼び出しエラー
      ERR_STARTPAGE,                   // StartPage()呼び出しエラー
      ERR_NEWFRAME,                    // NEWFRAMEエスケープ呼び出しエラー
      ERR_ENDPAGE,                     // EndPage()呼び出しエラー
      ERR_ENDDOC,                      // EndDoc()呼び出しエラー
      ERR_ANIMATE,                     // 同時にアニメートできるDIBは1つだけ
      ERR_NOTIMERS,                    // アニメーション用のタイマーがない
      ERR_NOCLIPWINDOW,                // クリップボードのウィンドウがない
      ERR_CLIPBUSY,                    // クリップボードが使用中
      ERR_NOCLIPFORMATS,               // 貼り付け時にDIBかDDBがない
      ERR_SETDIBITS,                   // SetDIBits()呼び出しエラー
      ERR_FILENOTFOUND,                // GetDib()のファイルオープンエラー
      ERR_WRITEDIB,                    // DIBファイル書き込みエラー
      ERR_MAX                          // すべてのエラーはこの値よりも小さい
     };

#ifdef __cplusplus
extern "C" {
#endif
void DIBError (int ErrNo);
#ifdef __cplusplus
}
#endif

#endif //ERRORS_INCLUDED
