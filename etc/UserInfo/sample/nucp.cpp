// make sure that we are compiling for Unicode
#define UNICODE 1
#define _UNICODE 1

#include <windows.h>
#include <lm.h>
#include <stdio.h>
#include <time.h>
#include <wchar.h>
#pragma hdrstop
#pragma comment( lib, "netapi32.lib" )


#define lenof(a) (sizeof(a)/sizeof((a)[0]))



int wmain( int argc, TCHAR *argv[] )
{
	wchar_t *machine = 0, *user = 0, *oldpw = 0, *newpw = 0;
	int i;
	DWORD rc;

	if ( argc < 3 || argc > 5 )
	{
usage:
		wprintf(
			L"usage: nucp [\\\\machine] user [oldpassword] newpassword\n"
			L"If \\\\machine is omitted, the local computer is assumed.\n"
			L"If oldpassword is omitted, a different method to change the password\n"
			L"is used; this requires administrative privilege.\n"
		);
		return 1;
	}

	i = 1;
	if ( argv[i][0] == L'\\' && argv[i][1] == L'\\' )
	{
		machine = argv[i];
		++ i;
	}

	user = argv[i];
	++ i;

	if ( i == argc ) // need at least newpassword
		goto usage;

	newpw = argv[i];
	++ i;

	if ( i < argc ) // not yet at end? there must have been an oldpassword
	{
		oldpw = newpw;
		newpw = argv[i];
	}

	if ( oldpw )
	{
		// try NetUserChangePassword()
		wprintf( L"%s %s %s %s\n", machine, user, oldpw, newpw );
		rc = NetUserChangePassword( machine, user, oldpw, newpw );
		if ( rc )
			wprintf( L"NUCP() returned %lu\n", rc );
	}
	else
	{
		// try NetUserSetInfo() at 1003
		USER_INFO_1003 ui1003;
		ui1003.usri1003_password = newpw;
		DWORD dummy;
		rc = NetUserSetInfo( machine, user, 1003, (byte *) &ui1003, &dummy );
		if ( rc )
			wprintf( L"NUSI() returned %lu\n", rc );
	}

	return rc;
}
