#include "file_input.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <limits>

void FileInput::ReadTraceFromFile(const std::string& file_path) {
    file_.open(file_path);

    if (!file_.is_open()) {
        std::cerr << "Error: Unable to open file " << file_path << std::endl;
        exit(1);
    }
}

void FileInput::SkipToInstruction(unsigned long start_inst) {
    for (unsigned long line_number = 0; line_number < start_inst; ++line_number ) {
        if (!file_.ignore(std::numeric_limits<std::streamsize>::max(), '\n')){
            return;
        }
    }

    return;
}

Trace FileInput::GetNext() {
    std::string text;
    Trace res;
    if (!getline(file_, text)) {
        return res;
    }

    std::vector<std::string> vect;

    std::stringstream ss(text);

    while (ss.good()) {
        std::string substr;
        getline(ss, substr, ',');
        vect.push_back(substr);
    }

    if (vect.size() < 2) {
        return res;
    }
    else {
        res.instructionAddr = std::stoul(vect[0], nullptr, 16);
        res.type = static_cast<Trace::Type>(std::stoi(vect[1]));
        if (vect.size() > 2) {
            for (size_t i = 2; i < vect.size(); i++) {
                res.dependencyAddr.push_back(std::stoul(vect[i], nullptr, 16));
            }
        }
    }
    return res;
}

FileInput::~FileInput() {
    if (file_.is_open()) {
        file_.close();
    }
}