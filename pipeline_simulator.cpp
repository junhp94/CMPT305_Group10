#include <iostream>
#include <set>

#include "pipeline_simulator.h"
#include "file_input.h"


PipelineSimulator::PipelineSimulator() : currentCycle(0)
{
    // initialize stages
}

PipelineSimulator::~PipelineSimulator()
{
    // cleanup
}

void PipelineSimulator::runSimulation(const std::string &file_name, long start_inst, long inst_count, int pipelineWidth)
{
    FileInput file;

    file.ReadTraceFromFile(file_name);
    std::set<unsigned long> completed_instructions;
    long executed = 0;
    while (executed < start_inst) {
        Trace tmp = file.GetNext();
        completed_instructions.insert(tmp.instructionAddr);
        executed++;
    }
    executed = 0;
    printf("%ld\n", completed_instructions.size());
    // while (executed < inst_count) {
    //     Trace next_instr = file.GetNext();
    //     if (!next_instr.isValid()) {
    //         return;
    //     }
    //     completed_instructions.erase(next_instr.instructionAddr);
    //     // do something with the instruction
    //     executed++;
    // }
}
