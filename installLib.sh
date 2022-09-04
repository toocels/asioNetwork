gcc -c src/server.cpp -Iinclude -o obj/server.o
gcc -c src/client.cpp -Iinclude -o obj/client.o

ar cr /usr/lib/cstm_libs/libserver.a obj/server.o
ar cr /usr/lib/cstm_libs/libclient.a obj/client.o

cp include/Client.h /usr/include/cstm_libs/Client.h
cp include/Server.h /usr/include/cstm_libs/Server.h
