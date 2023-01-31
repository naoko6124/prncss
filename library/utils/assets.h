#pragma once
#include "pch.h"
#include <fstream>

namespace prncss
{
    class assets
    {
    public:
        static std::string load_file(const char* path)
        {
            std::ifstream file;
            file.open(path);
            std::stringstream text_stream;
            text_stream << file.rdbuf();
            file.close();
            std::string text = text_stream.str();
            return text;
        }
    };
}