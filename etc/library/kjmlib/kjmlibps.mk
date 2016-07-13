
kjmlibps.dll: dlldata.obj kjmlib_p.obj kjmlib_i.obj
	link /dll /out:kjmlibps.dll /def:kjmlibps.def /entry:DllMain dlldata.obj kjmlib_p.obj kjmlib_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del kjmlibps.dll
	@del kjmlibps.lib
	@del kjmlibps.exp
	@del dlldata.obj
	@del kjmlib_p.obj
	@del kjmlib_i.obj
