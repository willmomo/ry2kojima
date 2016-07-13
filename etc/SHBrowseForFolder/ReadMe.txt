========================================================================
       MICROSOFT FOUNDATION CLASS ライブラリ : SHBrowseForFolder
========================================================================


AppWizard が生成した SHBrowseForFolder には、Microsoft Foundation Class の基本的な
使い方が示されています。アプリケーション作成のひな型としてお使いください。

このファイルには SHBrowseForFolder を構成している各ファイルの概要説明が含まれています。


SHBrowseForFolder.h
    このファイルはアプリケーションの中心となるインクルードファイルです。この
    ファイルは他のプロジェクトの固有のインクルードファイル（Resource.h  も含
    みます） をインクルードし、また CSHBrowseForFolderApp アプリケーションクラスを宣
    言します。

SHBrowseForFolder.cpp
    このファイルは CSHBrowseForFolderApp アプリケーションクラスを含むアプリケーション
    の中心となるソースファイルです。

SHBrowseForFolder.rc
    このファイルはプログラムが使用する Microsoft Windows のリソースを列挙しま
	す。このファイルは RES サブディレクトリに保存されているアイコン、ビットマ
	ップ、カーソルを含みます。このファイルは Microsoft Developer Studio で直接
	編集可能です。

res\SHBrowseForFolder.ico
    このファイルは、アプリケーションのアイコンとして使用されるアイコンファイ
    ルです。このアイコンはリソース ファイル SHBrowseForFolder.rc によってインクルード
	されます。

res\SHBrowseForFolder.rc2
    このファイルは Microsoft Developer によって編集されないリソースを含んでいま
	す。このファイルにリソース エディタで編集されないすべてのリソースを記述
	してください。

SHBrowseForFolder.clw
    このファイルは ClassWizard  が既存のクラスの編集やクラスを新規作成すると
    きに使う情報を持っています。またこのファイルは、ClassWizard によるメッセ
    ージ マップやダイアログ データ マップの編集や生成、プロトタイプ メンバ関
    数の生成に必要な情報も持っています。


/////////////////////////////////////////////////////////////////////////////

AppWizard は 1 つのダイアログ クラスを作成します。

SHBrowseForFolderDlg.h, SHBrowseForFolderDlg.cpp - the dialog
    これらのファイルは CSHBrowseForFolderDlg クラスを含みます。このクラスは
	アプリケーションのメイン ダイアログの動作を定義します。ダイアログ
	テンプレートは Microsoft Developer Studio で編集可能な SHBrowseForFolder.rc に含
	まれます。


/////////////////////////////////////////////////////////////////////////////
その他の標準ファイル:

StdAfx.h, StdAfx.cpp
    これらのファイルはプリコンパイル済ヘッダー ファイル (PCH) SHBrowseForFolder.pch 
    やプリコンパイル済型ファイル StdAfx.obj を構築するために使われるファイルです。

Resource.h
    このファイルは新規リソース ID を定義する標準ヘッダー ファイルです。
    Microsoft Developer Studio はこのファイルを読み込み更新します。

/////////////////////////////////////////////////////////////////////////////
その他の注意:

"TODO:" で始まるコメントは、ソースコードの追加やカスタマイズの必要な場所を示
します。

アプリケーションで共有 DLL 内で MFC を使用する場合や、アプリケーション
がオペレーティング システムで現在使用している言語以外の言語を使用してい
る場合は、Microsoft Visual C++ CD-ROM の system または system32 ディレクト
リから、ローカライズされたリソースに対応する MFC40XX.DLL をコピーして、
ファイル名を MFCLOC.DLL にする必要があります。("XXX" は言語の省略形を現
します。例えば、 MFC40DEU.DLL はドイツ語に翻訳されたリソースを含みます。)
このようにしたくない場合は、アプリケーション内のいくつかの UI 要素はオペ
レーティング システムの言語のまま残ります。

/////////////////////////////////////////////////////////////////////////////

