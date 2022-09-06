echo "Installing custom library asioNetwork"
echo "Installing location is in /usr/lib/cstm_libs or /usr/include/cstm_libs."
echo "Create cstm_libs directory in those location if not already."

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
