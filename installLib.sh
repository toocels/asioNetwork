echo "Installing custom library asioNetwork"

gcc -c src/server.cpp -Iinclude -o obj/server.o
gcc -c src/client.cpp -Iinclude -o obj/client.o
echo "Done compiling .cpp"

ar cr /usr/lib/cstm_libs/libserver.a obj/server.o
ar cr /usr/lib/cstm_libs/libclient.a obj/client.o
echo "Done creating object files."

cp include/Client.h /usr/include/cstm_libs/Client.h
cp include/Server.h /usr/include/cstm_libs/Server.h
echo "Done copying header files."
echo ""