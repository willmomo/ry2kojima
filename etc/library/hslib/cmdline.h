#if !defined(__CMDLINE_H)
#define __CMDLINE_H

#pragma warning(disable: 4786)

#include <string>
#include <list>
#include <map>

class cmdline_option
{
private:
	int m_state;			// オプションの状態
							// -3:エラー(未サポートオプション) -2:エラー(引数なし)
							// -1:未指定
							// 0:off 1:on(引数なし) 2:on(引数あり)
	std::string m_args;		// オプションの引数

public:
	// オプションの状態取得・設定
	int get_state() const { return m_state; };
	void set_state(int new_state) { m_state = new_state; };

	// オプション引数取得・設定
	std::string& get_args() { return m_args; };
	std::string get_args() const { return m_args; };
	void set_args(const std::string& new_args) { m_args = new_args; };

public:
	// デフォルトコンストラクタ
	cmdline_option() : m_state(-1), m_args() {};

	// コピーコンストラクタ
	cmdline_option(const cmdline_option& src)
		: m_state(src.get_state()), m_args(src.get_args()) {};

	// コンストラクタ(初期化付き)
	cmdline_option(int state, const std::string& args)
		: m_state(state), m_args(args) {};

	// デストラクタ
	virtual ~cmdline_option() {};

public:
	// 代入演算子(=)
	cmdline_option& operator=(const cmdline_option& src) {
		m_state = src.get_state();
		m_args = src.get_args();
		return *this;
	};
};

class cmdline
{
public:
	typedef std::list<std::string>			file_list_type;
	typedef std::map<int, cmdline_option>	options_type;
	typedef file_list_type::iterator		file_list_iterator;
	typedef file_list_type::const_iterator	file_list_const_iterator;

private:
	std::string m_exefile;						// 実行ファイルの名前(argv[0]相当)
	file_list_type m_file_list;					// オプション以外のコマンドライン
	options_type m_options;	// コマンドラインオプション

public:
	// 実行ファイル名の取得
	std::string& get_exefile() { return m_exefile; };
	std::string get_exefile() const { return m_exefile; };

	// オプション以外のコマンドライン取得
	file_list_type& get_file_list() { return m_file_list; };
	file_list_type get_file_list() const { return m_file_list; };
	file_list_iterator begin_file_list() { return m_file_list.begin(); };
	file_list_const_iterator begin_file_list() const { return m_file_list.begin(); };
	file_list_iterator end_file_list() { return m_file_list.end(); };
	file_list_const_iterator end_file_list() const { return m_file_list.end(); };

	// オプション情報の取得
	options_type& get_options() { return m_options; };
	options_type get_options() const { return m_options;};
	int get_option_state(int c) { return m_options[c].get_state(); };
	std::string& get_option_args(int c) { return m_options[c].get_args(); };

public:
	// デフォルトコンストラクタ
	cmdline()
		: m_exefile(), m_file_list(), m_options() {};

	// コピーコンストラクタ
	cmdline(const cmdline& src)
		: m_exefile(src.get_exefile()), m_file_list(src.get_file_list()), m_options(src.get_options()) {};

	// コンストラクタ(解析つき)
	cmdline(int argc, char** argv, const char* option_rule)
		: m_exefile(), m_file_list(), m_options() {

		chkarg(argc, argv, option_rule);
	};

	// デストラクタ
	virtual ~cmdline() {};

public:
	// コマンドライン解析
	int chkarg(int argc, char** argv, const char* option_rule);

public:
	// 代入演算子(=)
	cmdline& operator=(const cmdline& src) {
		m_exefile = src.get_exefile();
		m_file_list = src.get_file_list();
		m_options = src.get_options();
		return *this;
	};
};

#pragma warning(default: 4786)

#endif
