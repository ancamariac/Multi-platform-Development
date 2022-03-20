CFLAGS   = /nologo /W3 /EHsc /MD



build: main.obj helper.obj hashmap.obj
	cl $(CFLAGS) /Feso-cpp.exe $**

main.obj : main.c 
	cl $(CFLAGS) /c /Fomain.obj $**

hashmap.obj : hashmap.c 
	cl $(CFLAGS) /c /Fohelper.obj $**	

helper.obj : helper.c 
	cl $(CFLAGS) /c /Fohelper.obj $**

clean : 
	del *.obj *.exe
