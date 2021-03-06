/*************************************************************************

  ファイル: ERRORS.C

  目的:     エラーメッセージボックスの処理ルーチンです。

  関数:     DIBError()

  コメント:

      エラーメッセージは文字列テーブルからテキストをロードするようにして
      ください。直接テキストを埋め込むコードはDSを消費し、また国際市場向け
      製品の場合ローカライズが困難になります。

*************************************************************************/


#include "errors.h"

static char *szErrors[] = {"DIBファイルではありません。",
                           "メモリを割り当てられません。",
                           "ファイルを読み取れません。",
                           "メモリ ロック エラーです。",
                           "ファイルを開くことができません。",
                           "パレットを作成できません。",
                           "DC取得エラーです。",
                           "MDI子ウィンドウを作成できません。",
                           "デバイスに依存する依存ビットマップを作成できません。",
                           "StretchBlt()に失敗しました。",
                           "StretchDIBits()に失敗しました。",
                           "描画にはDDBとDIBの両方が必要です。",
                           "SetDIBitsToDevice()がありません。",
                           "プリンタ: StartDocがありません。",
                           "印刷: GetModuleHandle()でGDIを見つけられません。",
                           "プリンタ: SetAbortProcがありません。",
                           "プリンタ: StartPageがありません。",
                           "プリンタ: NEWFRAMEがありません。",
                           "プリンタ: EndPageがありません。",
                           "プリンタ: EndDocがありません。",
                           "DIBのアニメートは一度に1つしかできません。",
                           "アニメーション用のタイマーを使えません。",
                           "クリップボードにコピーできません。DIBがありません。",
                           "クリップボードがいっぱいです。操作は中止されました。",
                           "貼り付けできません。クリップボードにDIBまたはDDBがありません。",
                           "SetDIBits()がありません。",
                           "ファイルが見つかりません。",
                           "DIBファイル書き込みエラーです。"
                          };

void DIBError (int ErrNo)
{
   if ((ErrNo < ERR_MIN) || (ErrNo >= ERR_MAX))
   {
      MessageBox (GetFocus (), "未定義エラーです。", NULL, MB_OK | MB_ICONHAND);
   }
   else
   {
      MessageBox (GetFocus (), szErrors[ErrNo], NULL, MB_OK | MB_ICONHAND);
   }
}
