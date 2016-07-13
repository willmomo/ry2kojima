
MyComps.dll: dlldata.obj MyCom_p.obj MyCom_i.obj
	link /dll /out:MyComps.dll /def:MyComps.def /entry:DllMain dlldata.obj MyCom_p.obj MyCom_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del MyComps.dll
	@del MyComps.lib
	@del MyComps.exp
	@del dlldata.obj
	@del MyCom_p.obj
	@del MyCom_i.obj
