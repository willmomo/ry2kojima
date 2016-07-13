#if !defined(__CMDLINE_H)
#define __CMDLINE_H

#pragma warning(disable: 4786)

#include <string>
#include <list>
#include <map>

class cmdline_option
{
private:
	int m_state;			// �I�v�V�����̏��
							// -3:�G���[(���T�|�[�g�I�v�V����) -2:�G���[(�����Ȃ�)
							// -1:���w��
							// 0:off 1:on(�����Ȃ�) 2:on(��������)
	std::string m_args;		// �I�v�V�����̈���

public:
	// �I�v�V�����̏�Ԏ擾�E�ݒ�
	int get_state() const { return m_state; };
	void set_state(int new_state) { m_state = new_state; };

	// �I�v�V���������擾�E�ݒ�
	std::string& get_args() { return m_args; };
	std::string get_args() const { return m_args; };
	void set_args(const std::string& new_args) { m_args = new_args; };

public:
	// �f�t�H���g�R���X�g���N�^
	cmdline_option() : m_state(-1), m_args() {};

	// �R�s�[�R���X�g���N�^
	cmdline_option(const cmdline_option& src)
		: m_state(src.get_state()), m_args(src.get_args()) {};

	// �R���X�g���N�^(�������t��)
	cmdline_option(int state, const std::string& args)
		: m_state(state), m_args(args) {};

	// �f�X�g���N�^
	virtual ~cmdline_option() {};

public:
	// ������Z�q(=)
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
	std::string m_exefile;						// ���s�t�@�C���̖��O(argv[0]����)
	file_list_type m_file_list;					// �I�v�V�����ȊO�̃R�}���h���C��
	options_type m_options;	// �R�}���h���C���I�v�V����

public:
	// ���s�t�@�C�����̎擾
	std::string& get_exefile() { return m_exefile; };
	std::string get_exefile() const { return m_exefile; };

	// �I�v�V�����ȊO�̃R�}���h���C���擾
	file_list_type& get_file_list() { return m_file_list; };
	file_list_type get_file_list() const { return m_file_list; };
	file_list_iterator begin_file_list() { return m_file_list.begin(); };
	file_list_const_iterator begin_file_list() const { return m_file_list.begin(); };
	file_list_iterator end_file_list() { return m_file_list.end(); };
	file_list_const_iterator end_file_list() const { return m_file_list.end(); };

	// �I�v�V�������̎擾
	options_type& get_options() { return m_options; };
	options_type get_options() const { return m_options;};
	int get_option_state(int c) { return m_options[c].get_state(); };
	std::string& get_option_args(int c) { return m_options[c].get_args(); };

public:
	// �f�t�H���g�R���X�g���N�^
	cmdline()
		: m_exefile(), m_file_list(), m_options() {};

	// �R�s�[�R���X�g���N�^
	cmdline(const cmdline& src)
		: m_exefile(src.get_exefile()), m_file_list(src.get_file_list()), m_options(src.get_options()) {};

	// �R���X�g���N�^(��͂�)
	cmdline(int argc, char** argv, const char* option_rule)
		: m_exefile(), m_file_list(), m_options() {

		chkarg(argc, argv, option_rule);
	};

	// �f�X�g���N�^
	virtual ~cmdline() {};

public:
	// �R�}���h���C�����
	int chkarg(int argc, char** argv, const char* option_rule);

public:
	// ������Z�q(=)
	cmdline& operator=(const cmdline& src) {
		m_exefile = src.get_exefile();
		m_file_list = src.get_file_list();
		m_options = src.get_options();
		return *this;
	};
};

#pragma warning(default: 4786)

#endif
