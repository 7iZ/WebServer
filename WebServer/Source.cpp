#include "WinsSocket/WinsSocket.h"
#include "HttpParser/HttpParser.h"
#include "QuickFile/QuickFile.h"

using namespace std;
#include <iostream>

int main() {
    QuickFile qfile; 
    WinsSocket server("", "80", SocketMode::Server);
    
    while (true) {
        cout << "> Listening" << endl;
        server.serverListen();

        string requestContent = server.serverReceive();

        HttpParser httpRequest(HttpRequire::Request);
        if (!httpRequest.isPackage(requestContent)) cout << "invalid request!\n";
        else {
            // Parse request
            httpRequest.parseHttpPackage(requestContent, HttpRequire::Request);
            if (httpRequest.getURI().back() == '/')
                httpRequest.setURI(httpRequest.getURI() + "index.html");
            cout << "Receive request: " << httpRequest.getURI() << endl;
            cout << "From: " << httpRequest.getMessage("User-Agent") << endl;

            HttpParser httpResponse(HttpRequire::Response);
            string httpContent = qfile.readFile("http" + httpRequest.getURI());
            if (httpContent != "") httpResponse.setContent(httpContent);
            else {
                httpResponse.setStatus("404");
                httpResponse.setReason("Not found");
            }
            
            // Create response
            httpResponse.setMessage("Server", "7iZ Simplest Server");
            httpResponse.setMessage("Content-Length", to_string(httpContent.length()));
            httpResponse.setMessage("Content-Type", "text/html");
            httpResponse.setMessage("Connection", "Closed");

            // Re-send to client
            server.serverSend(httpResponse.getHttpPackage());
            cout << "Send result: " << httpResponse.getStatus() << " " << httpResponse.getReason() << endl;
        }

        cout << "Closed\n" << endl;
        server.serverDisconnect();
    }

    return 0; 
}
