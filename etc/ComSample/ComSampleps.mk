
ComSampleps.dll: dlldata.obj ComSample_p.obj ComSample_i.obj
	link /dll /out:ComSampleps.dll /def:ComSampleps.def /entry:DllMain dlldata.obj ComSample_p.obj ComSample_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ComSampleps.dll
	@del ComSampleps.lib
	@del ComSampleps.exp
	@del dlldata.obj
	@del ComSample_p.obj
	@del ComSample_i.obj
