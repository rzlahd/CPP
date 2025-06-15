g++ tes.cpp -o tes.exe -static-libstdc++ -static-libgcc -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic

Down Here to Fix DLL Error libwinpthread-1.dll 
For Portable Program
g++ tes.cpp -o tes2.exe -static-libstdc++ -static-libgcc -static
