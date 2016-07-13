#include "twclib.h"

string WildCardToRegExp(const char* psz)
{
	string str;

	str += '^';
	for (int k = 0; psz[k] != '\0';) {
		if (psz[k] == '*') {
			str += '.';
			str += psz[k++];
		} else if (psz[k] == '?') {
			str += '?';
			k++;
		} else {
			str += psz[k++];
		}
	}
	str += '$';

	return str;
}
