#include <assert.h>
#include "kjmlib.h"


BOOL kjm::win32::makeDirectory(LPCTSTR pszFolder) {
	if (PathIsRoot(pszFolder))	return TRUE;
	if (PathFileExists(pszFolder))	return TRUE;

	assert(lstrlen(pszFolder) < MAX_PATH);

	TCHAR szParent[MAX_PATH];
	PathRemoveFileSpec(lstrcpy(szParent, pszFolder));

	if (kjm::win32::makeDirectory(szParent)) {
		if (CreateDirectory(pszFolder, FALSE)) {
			return TRUE;
		}
	}

	return FALSE;
}
