#pragma once

#include <fstream>
#include <string>

class QuickFile {
    public:
        std::string readFile(std::string filename);
        void writeFile(std::string filename, std::string content);
};

