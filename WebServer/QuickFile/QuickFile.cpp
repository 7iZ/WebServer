#include "QuickFile.h"

std::string QuickFile::readFile(std::string filename) {
    std::string fileContent, fileLine;
    std::ifstream fileStream(filename);
    if (fileStream.is_open()) {
        while (std::getline(fileStream, fileLine, '\n'))
            fileContent += fileLine + "\n";
        fileStream.close();
    }
    return fileContent;
}

void QuickFile::writeFile(std::string filename, std::string content) {
    std::ofstream fileStream(filename);
    if (fileStream.is_open()) {
        fileStream << content;
        fileStream.close();
    }
}

