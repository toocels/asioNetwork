# asioNetwork

A thin C++ wrapper around [standalone Asio](https://think-async.com/Asio/) that
turns the usual TCP boilerplate into two classes you can drop into a project and
get on with.

- **`Client`** — connect to a host/port, send a message, fetch a message.
- **`Server`** — accept connections, keep a registry of the clients that are
  connected, send a message to one client, or listen for incoming ones (either
  one at a time or through a callback), with optional logging to a file.

## Requirements

- a C++ compiler (developed with `g++`)
- standalone Asio headers
- `ar`, for the static libraries

## Build and install

`installLib.sh` compiles the two sources, packs them into static libraries and
copies the headers into place:

```bash
mkdir -p obj
sudo mkdir -p /usr/lib/cstm_libs /usr/include/cstm_libs
sudo ./installLib.sh
```

You end up with `libserver.a` and `libclient.a` in `/usr/lib/cstm_libs`, and
`Server.h` and `Client.h` in `/usr/include/cstm_libs`.

The script writes to system directories, so those paths have to exist first and
the script needs `sudo`.

## Usage

Server, with a callback for whatever arrives:

```cpp
#include "Server.h"

void recvMsg(std::string msg, tcp::socket* client) {
    std::cout << "Msg from client: " << msg << std::endl;
}

int main() {
    Server server(69, "127.0.0.1");
    server.listen_connections();
    server.listen_messages(recvMsg);
}
```

Client:

```cpp
#include "Client.h"

int main() {
    Client client;
    if (!client.connect(69, "127.0.0.1"))
        return 1;

    client.sendMsg("hello");
    std::cout << client.fetch_msg() << std::endl;
}
```

`test/mainS.cpp` and `test/mainC.cpp` are interactive versions of those two: a
small REPL where you type `send`, `recv`, `cons`, `msgs`, `rm` or `exit`.

## Layout

| Path | |
|---|---|
| `include/Client.h`, `include/Server.h` | the public API |
| `src/client.cpp`, `src/server.cpp` | the implementation |
| `test/mainC.cpp`, `test/mainS.cpp` | hand-driven client and server |
| `installLib.sh` | builds and installs the libraries and headers |

## Notes

This is a 2022 learning project, so the API is shaped by what was useful at the
time rather than by what a library should look like — treat it as a starting
point, not a stable interface.
