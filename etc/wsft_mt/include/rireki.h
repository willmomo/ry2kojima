
#ifndef __rireki_h__
#define __rireki_h__


extern int WINAPI getTokuRireki(int handle, int no, void *buff, int day);
// extern int WINAPI getTokuRirekiKensu(long *kensu, long bufsize, int day);	r.kojima del 2008.02.12 [T2X004310] g‚í‚È‚­‚È‚Á‚½ getTokuRirekiKensu ‚ğg‚¦‚È‚¢‚æ‚¤‚É••ˆóB

extern BOOL WINAPI MakeF6102_FromF6105(LPCTSTR pszFname);

#endif							/* __rireki_h__ */
