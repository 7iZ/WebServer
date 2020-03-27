#include "WinsSocket.h"

WinsSocket::WinsSocket(std::string host, std::string port, int mode) {
    m_nMode = mode;
    m_strHost = host;
    m_strPort = port;
    m_socketListen = INVALID_SOCKET;
    m_socketClient = INVALID_SOCKET;
    m_paiResult = NULL;

    // Initialize Winsock
    m_nResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
    if (m_nResult != 0) {
        printf("WSAStartup failed with error: %d\n", m_nResult);
        exit(EXIT_FAILURE);
    }

    ZeroMemory(&m_aiHints, sizeof(m_aiHints));
    m_aiHints.ai_family   = AF_INET;
    m_aiHints.ai_socktype = SOCK_STREAM;
    m_aiHints.ai_protocol = IPPROTO_TCP;
    m_aiHints.ai_flags    = AI_PASSIVE;

    // Resolve the server address and port
    m_nResult = getaddrinfo(m_nMode == SocketMode::Server ? NULL : m_strHost.c_str(), m_strPort.c_str(), &m_aiHints, &m_paiResult);
    if (m_nResult != 0) {
        printf("getaddrinfo failed with error: %d\n", m_nResult);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Create a SOCKET for connecting to server
    m_socketListen = socket(m_paiResult->ai_family, m_paiResult->ai_socktype, m_paiResult->ai_protocol);
    if (m_socketListen == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(m_paiResult);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Setup the TCP listening socket
    m_nResult = bind(m_socketListen, m_paiResult->ai_addr, (int)m_paiResult->ai_addrlen);
    if (m_nResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(m_paiResult);
        closesocket(m_socketListen);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(m_paiResult);
}

void WinsSocket::serverListen() {
    m_nResult = listen(m_socketListen, SOMAXCONN);
    if (m_nResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(m_socketListen);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Accept a client socket
    m_socketClient = accept(m_socketListen, NULL, NULL);
    if (m_socketClient == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(m_socketListen);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

std::string WinsSocket::serverReceive() {
    m_nResult = recv(m_socketClient, m_cBuffer, sizeof(m_cBuffer), 0);
    return std::string(m_cBuffer);
}

void WinsSocket::serverSend(std::string content) {
    m_nResult = send(m_socketClient, content.c_str(), content.length(), 0);
    if (m_nResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(m_socketClient);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void WinsSocket::serverDisconnect() {
    // Shutdown the connection since we're done
    m_nResult = shutdown(m_socketClient, SD_SEND);
    if (m_nResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(m_socketClient);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    closesocket(m_socketClient);
}

void WinsSocket::serverClose() {
    // No longer need server socket
    closesocket(m_socketListen);
}
