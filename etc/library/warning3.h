
// STLで大量にはき出される不要な警告を除外する
//
#pragma warning(disable : 4786)	// 'identifier' : デバッグ情報で識別子が 'number' 文字に切り捨てられました。

// 警告レベル4の警告を3に引き上げる。
// STLが大量に警告をはき出すものは、除外する。
//
#pragma warning(3 : 4001)	// 標準外の拡張機能「単一行コメント」が使われています。
#pragma warning(3 : 4019)	// グローバル スコープに空行があります。
#pragma warning(3 : 4032)	// number 番目の引数の型は以前の宣言の型と一致しません。
#pragma warning(3 : 4057)	// 'operator' : 'identifier1' で使われているポインタ式は 'identifier2' とベース型が異なっています。
#pragma warning(3 : 4061)	// enum 'identifier' の switch ステートメントにある列挙子 'identifier' は、case ラベルによって明示的に処理されていません。
#pragma warning(3 : 4092)	// sizeof 演算子が 'unsigned long' の値を返しました。
//#pragma warning(3 : 4100)	// 'identifier' : 引数が関数の本体で 1 度も参照されません。
#pragma warning(3 : 4121)	// 'symbol' : メンバのアライメントは過剰にパッキングされています。
#pragma warning(3 : 4125)	// 8 進数のエスケープ シーケンスが、10 進数のところで中断されました。
#pragma warning(3 : 4127)	// 条件式が定数です。
#pragma warning(3 : 4128)	// 宣言の中の型の後に、記憶クラス指定子があります。
#pragma warning(3 : 4130)	// 'operator' : 文字列定数のアドレスで論理演算が行われました。
#pragma warning(3 : 4131)	// 'function' : 旧形式の宣言が使われています。
#pragma warning(3 : 4132)	// 'object' : const オブジェクトは初期化しなければなりません。
#pragma warning(3 : 4134)	// クラスのあるメンバへのポインタが、そのクラスの別のメンバへのポインタへ変換されました。
#pragma warning(3 : 4152)	// 関数へのポインタがデータへのポインタに変換されました。
#pragma warning(3 : 4189)	// 'identifier' : ローカル変数が初期化されましたが、参照されていません
#pragma warning(3 : 4201)	// 非標準の拡張機能が使用されています。無名の構造体または共用体です。
#pragma warning(3 : 4202)	// 非標準の拡張機能 '...' が使用されています。プロトタイプ引数が不正です。
#pragma warning(3 : 4206)	// 非標準の拡張機能が使用されています。翻訳単位が空です。
#pragma warning(3 : 4207)	// 非標準の拡張機能が使用されています。拡張された初期化子形式です。
#pragma warning(3 : 4208)	// 非標準の拡張機能が使用されています。delete [exp] - exp を評価しましたが無視します。
#pragma warning(3 : 4209)	// 非標準の拡張機能が使用されています。型が無意味に再定義されています。
#pragma warning(3 : 4210)	// 非標準の拡張機能が使用されています。関数にはファイル スコープが与えられています。
#pragma warning(3 : 4211)	// 非標準の拡張機能が使用されています。extern が static に再定義されました。
#pragma warning(3 : 4212)	// 非標準の拡張機能が使用されています。関数宣言で省略符号が使われています。
#pragma warning(3 : 4213)	// 非標準の拡張機能が使用されています。左辺値でキャストしました。
#pragma warning(3 : 4214)	// 非標準の拡張機能が使用されています。整数型以外のビットフィールドです。
#pragma warning(3 : 4220)	// varargs は残りの引数と一致させます。
#pragma warning(3 : 4221)	// 非標準の拡張機能が使用されています。'identifier' を自動変数のアドレスを使って初期化できません。
#pragma warning(3 : 4222)	// 非標準の拡張機能が使用されています。'identifier' : 'static' はメンバ関数の定義では使えません。
#pragma warning(3 : 4232)	// 非標準の拡張機能が使用されています。'identifier' : dllimport 'dllimport' のアドレスは静的ではありません。一貫性が保証されません。
#pragma warning(3 : 4233)	// 非標準の拡張機能が使用されています。 : 'keyword' キーワードがサポートされているのは C++ だけであり、C ではサポートされていません。
#pragma warning(3 : 4234)	// 非標準の拡張機能が使用されています。 : 'keyword' キーワードは将来使用するために予約されています。
#pragma warning(3 : 4235)	// 非標準の拡張機能が使用されています。: 'keyword' キーワードはこの製品ではサポートしていません。
#pragma warning(3 : 4236)	// 非標準の拡張機能が使用されています。 : 'keyword' は互換性のために残されているキーワードです。__declspec(dllexport) についてのマニュアルを参照してください。
#pragma warning(3 : 4238)	// 非標準の拡張機能が使用されています。 : クラス rvalue は lvalue として使用されます。
#pragma warning(3 : 4239)	// 非標準の拡張機能が使用されています : 'token' : 'type' から 'type' への変換です。
#pragma warning(3 : 4245)	// 'conversion' : 'type1' から 'type2' に変換しました。signed/unsigned が一致しません。
#pragma warning(3 : 4504)	// 'number' 個のトークンの解析後も型があいまいです。宣言を仮定します。
#pragma warning(3 : 4505)	// 'function' : 参照されていないローカル関数は削除されました。
#pragma warning(3 : 4507)	// デフォルトのリンケージの使用後に明示的なリンケージが指定されました。
#pragma warning(3 : 4514)	// 参照されていない inline または local 関数が削除されました。
#pragma warning(3 : 4515)	// 'namespace' : 名前空間が自分自身を使っています。
#pragma warning(3 : 4516)	// 'class::symbol' : アクセス宣言は将来使用されなくなります。 ; メンバの using 宣言が望ましい宣言です。
#pragma warning(3 : 4517)	// アクセス宣言は将来使用されなくなります。 ; メンバの using 宣言が望ましい宣言です。
#pragma warning(3 : 4611)	// '_setjmp' と C++ オブジェクト デストラクション間での相互作用は互換性がありません。
#pragma warning(3 : 4663)	// C++ 言語の変更: 明示的にクラス テンプレート 'identifier' を特殊化するためには以下の構文を使用してください。
#pragma warning(3 : 4665)	// C++ 言語の変更: 'declaration' は関数テンプレートの明示的な特殊化と仮定します。
#pragma warning(3 : 4670)	// 'identifier' : 基本クラスはアクセスできません。
#pragma warning(3 : 4671)	// 'identifier' : コピー コンストラクタはアクセスできません。
#pragma warning(3 : 4672)	// 関数 'identifier1' があいまいです。1 つ目を 'identifier2' とします。
#pragma warning(3 : 4673)	// 'identifier' のスロー。次の型はキャッチ側では評価されません。
#pragma warning(3 : 4674)	// 'identifier' : デストラクタはアクセスできません。
#pragma warning(3 : 4699)	// メ モ  : プリコンパイル済みヘッダーを使用
#pragma warning(3 : 4701)	// 値が割り当てられていないローカルな変数 'name' に対する参照が行われる可能性があります。
#pragma warning(3 : 4705)	// ステートメントは、プログラムの実行に何の効果も持ちません。
#pragma warning(3 : 4706)	// 条件式の比較値は、代入の結果になっています。
#pragma warning(3 : 4709)	// カンマ演算子で分割された表現式が、配列のインデックスとして使われています。
#pragma warning(3 : 4710)	// 'function' : 関数はインライン関数ではありません。
#pragma warning(3 : 4727)	// 条件式が定数です。
