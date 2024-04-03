#ifndef FILE_INPUT_H_
#define FILE_INPUT_H_

#include <fstream>
#include <string>

#include "trace.h"

class FileInput {
public:
    FileInput() = default;
    ~FileInput();

    void ReadTraceFromFile(const std::string& file_path);
    Trace GetNext();

private:
    std::ifstream file_;
};

#endif