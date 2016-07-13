unix用のeuc版は、clf.euc

○ VC++6, VC++4.2でのコンパイル方法

プロジェクトを作成後「必要なファイル」と「必要なdefine」を定義して作成

必要なファイル
clf.c
_getopt.h
getopt.c (glibc 2.1.2より)
getopt1.c (glibc 2.1.2より)
getopt.h (glibc 2.1.2より)

必要なdefine
HAVE_STRING_H

○ Cygwinでのコンパイル方法

make -f cygwin.mak で作成できます。
Cygwinβ20.1のgetopt_longの動きが変なので、glibc 2.1.2のgetopt.c, getopt1.cをリンクします。
ヘッダは、Cygwin標準のgetopt.hを使用します。

必要なファイル
unpack.cpp
_getopt.h
getopt.c (glibc 2.1.2より)
getopt1.c (glibc 2.1.2より)

