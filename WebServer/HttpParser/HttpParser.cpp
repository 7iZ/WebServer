#include "HttpParser.h"

std::vector<std::string> HttpParser::getParams(std::string content, std::string separate) {
    std::vector<std::string> params;
    size_t prevPos = 0, foundPos;
    while ((foundPos = content.find(separate, prevPos)) != std::string::npos) {
        params.push_back(content.substr(prevPos, foundPos - prevPos));
        prevPos = foundPos + separate.length();
    }
    params.push_back(content.substr(prevPos, content.length() - prevPos));
    return params;
}

HttpParser::HttpParser(int require) {
    m_nRequire = require;
    m_strMethod = "GET";
    m_strURI = "/";
    m_strVersion = "HTTP/1.1";
    m_strStatus = "200";
    m_strReason = "OK";
}

bool HttpParser::isPackage(std::string content) {
    std::vector<std::string> messages = getParams(content, "\r\n");
    if (messages.size() < 2)
        return false;

    std::vector<std::string> startParams = getParams(*messages.begin(), " ");
    if (int(startParams.size()) != 3)
        return false;

    for (int i = 1; i < int(messages.size()); ++i) {
        if (messages[i] == "")
            return true;
        if (messages[i].find(": ") == std::string::npos)
            return false;
    }

    return false;
}

void HttpParser::setMethod(std::string method) {
    m_strMethod = method;
}

std::string HttpParser::getMethod() {
    return m_strMethod;
}

void HttpParser::setURI(std::string uri) {
    m_strURI = uri;
}

std::string HttpParser::getURI() {
    return m_strURI;
}

void HttpParser::setVersion(std::string version) {
    m_strVersion = version;
}

std::string HttpParser::getVersion() {
    return m_strVersion;
}

void HttpParser::setStatus(std::string status) {
    m_strStatus = status;
}

std::string HttpParser::getStatus() {
    return m_strStatus;
}

void HttpParser::setReason(std::string reason) {
    m_strReason = reason;
}

std::string HttpParser::getReason() {
    return m_strReason;
}

std::string HttpParser::getMessage(std::string name) {
    return m_arrMessages[name];
}

void HttpParser::setMessage(std::string name, std::string value) {
    m_arrMessages[name] = value;
}

std::string HttpParser::getContent() {
    return m_strContent;
}

void HttpParser::setContent(std::string content) {
    m_strContent = content;
}

void HttpParser::parseMessage(std::vector<std::string> messages) {
    for (int i = 0; i < int(messages.size()); ++i) {
        if (messages[i] == "") {
            std::string content;
            while ((++i) < int(messages.size()))
                content += messages[i] + "\r\n";
            m_strContent = content;
            break;
        }

        std::vector<std::string> message = getParams(messages[i], ": ");
        m_arrMessages[message[0]] = message[1];
    }
}

bool HttpParser::parseHttpPackage(std::string requestContent, int require) {
    if (!isPackage(requestContent))
        return false;

    m_nRequire = require;
    std::vector<std::string> messages = getParams(requestContent, "\r\n");
    std::vector<std::string> startParams = getParams(*messages.begin(), " ");

    switch (m_nRequire)
    {
    case HttpRequire::Request:
        m_strMethod  = startParams[0];
        m_strURI     = startParams[1];
        m_strVersion = startParams[2];
        break;

    case HttpRequire::Response:
        m_strVersion = startParams[0];
        m_strStatus  = startParams[1];
        m_strReason  = startParams[2];
        break;
    }

    m_arrMessages.clear();
    messages.erase(messages.begin());
    parseMessage(messages);

    return true;
}

std::string HttpParser::getHttpPackage() {
    std::string httpPackage;

    // Header
    if (m_nRequire == HttpRequire::Request) {
        httpPackage += m_strMethod + " ";
        httpPackage += m_strURI + " ";
        httpPackage += m_strVersion + "\r\n";
    }
    if (m_nRequire == HttpRequire::Response) {
        httpPackage += m_strVersion + " ";
        httpPackage += m_strStatus + " ";
        httpPackage += m_strReason + "\r\n";
    }

    // Messages
    for (std::pair<std::string, std::string> message : m_arrMessages)
        httpPackage += message.first + ": " + message.second + "\r\n";

    // Content
    httpPackage += "\r\n";
    httpPackage += m_strContent;
    return httpPackage;
}
