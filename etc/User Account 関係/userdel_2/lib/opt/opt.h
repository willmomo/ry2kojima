#if !defined(__OPT_H__)
#define __OPT_H__

#if !defined(_STRING_)
#error You must include header <string>.
#endif

#if !defined(_VECTOR_)
#error You must include header <vector>.
#endif

#if !defined(_IOSTREAM_)
#error You must include header <iostream>.
#endif

typedef struct {
	int option;
	std::string parm;
} OPTION;

class COpt
{
private:
	std::vector<OPTION> m_options;
	std::vector<std::string> m_nonoptions;

public:
	COpt(){};
	COpt(int argc, char** argv, char* optstring) {
		Parse(argc, argv, optstring);
	};
	virtual ~COpt(){};

	int Parse(int argc, char** argv, char* optstring);
	bool Get(int option);
	bool Get(int option, std::string& parm);
	bool GetNonOption(int index, std::string& parm);
};

#endif
