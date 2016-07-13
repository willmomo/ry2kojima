#include "./kjmlib-20120626/kjmlib.h"


int _tmain(int argc, TCHAR** argv) {

	std::vector<char> data;
	data.push_back('a');
	data.push_back('A');

	std::string enc64 = kjm::base64::encode(data);
	printf("%s\n", enc64.c_str());

	std::vector<char> dec64 = kjm::base64::decode(enc64);

	enc64 = kjm::crypt::encrypt(enc64);
	std::string s2 = kjm::crypt::decrypt(enc64);
	return 0;
}
