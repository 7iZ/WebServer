#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#pragma comment (lib, "Ws2_32.lib")

#include <cstdio>
#include <string>

#define BUFFER_LEN 2048

enum SocketMode { Server, Client };

class WinsSocket {
    public:
        WinsSocket(std::string host, std::string port, int mode);

        void serverListen();
        std::string serverReceive();
        void serverSend(std::string content);
        void serverDisconnect();
        void serverClose();

    private:
        int m_nMode;
        std::string m_strHost;
        std::string m_strPort;
        char m_cBuffer[BUFFER_LEN];

        int m_nResult;
        WSADATA m_wsaData;
        struct addrinfo m_aiHints;
        struct addrinfo* m_paiResult;
        SOCKET m_socketListen;
        SOCKET m_socketClient;
};

