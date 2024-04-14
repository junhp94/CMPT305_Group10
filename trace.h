#ifndef TRACE_H_
#define TRACE_H_

#include <vector>

class Trace {
public:
    enum Type {
        INVALID,
        INT_INSTR,
        FP_INSTR,
        BRANCH,
        LOAD,
        STORE
    };

    unsigned long instructionAddr;
    Type type = Type::INVALID;
    std::vector<unsigned long> dependencyAddr;

    bool isValid();
};

#endif