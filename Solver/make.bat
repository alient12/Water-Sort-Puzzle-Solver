call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
call cl -c -o Container.obj Container.cpp Container.h
call cl -c -o main.obj main.cpp
call cl -o main.exe main.obj Container.obj