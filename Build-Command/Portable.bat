g++ tes.cpp -o tes.exe -static-libstdc++ -static-libgcc -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic
g++ tes.cpp -o tes2.exe -static-libstdc++ -static-libgcc -static
