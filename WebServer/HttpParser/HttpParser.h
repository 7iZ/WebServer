#pragma once

#include <string>
#include <vector>
#include <map>

enum HttpRequire { Request, Response };

class HttpParser {
private:
    std::vector<std::string> getParams(std::string content, std::string separate);

public:
    HttpParser(int require);
    bool isPackage(std::string content);

    void setMethod(std::string method);
    std::string getMethod();

    void setURI(std::string direct);
    std::string getURI();

    void setVersion(std::string version);
    std::string getVersion();

    void setStatus(std::string status);
    std::string getStatus();

    void setReason(std::string reason);
    std::string getReason();

    std::string getMessage(std::string name);
    void setMessage(std::string name, std::string value);

    std::string getContent();
    void setContent(std::string content);

    void parseMessage(std::vector<std::string> messages);
    bool parseHttpPackage(std::string requestContent, int require);
    std::string getHttpPackage();

private:
    int m_nRequire;
    std::string m_strMethod;
    std::string m_strURI;
    std::string m_strVersion;
    std::string m_strStatus;
    std::string m_strReason;
    std::map<std::string, std::string> m_arrMessages;
    std::string m_strContent;
};
