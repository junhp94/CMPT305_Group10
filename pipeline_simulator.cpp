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

void PipelineSimulator::runSimulation(const std::string &file_name, unsigned long start_inst, unsigned long inst_count, int pipeline_width)
{
    FileInput file;

    file.ReadTraceFromFile(file_name);
    std::set<unsigned long> completed_instructions;

    unsigned long executed = 0;
    while (executed < start_inst) {
        Trace tmp = file.GetNext();
        completed_instructions.insert(tmp.instructionAddr);

        executed++;
    }

    // TODO: probably need a set for when something is done ex and for mem
    // "For dependence on integer or FP instructions, dependences are satisfied when they complete EX"
    //"For dependence on load or store instructions, dependences are satisfied when they complete MEM"

    IFStage IF_stage;
    IDStage ID_stage;
    EXStage EX_stage;
    MEMStage MEM_stage;
    WBStage WB_stage;
    while (this->currentCycle < inst_count) {
        Trace* finished_IF = new Trace[pipeline_width];
        // process IF
        for (int i = 0; i < pipeline_width; i++) {
            Trace next_instr = file.GetNext();
            if (!next_instr.isValid()) {
                return;
            }
            completed_instructions.erase(next_instr.instructionAddr);
            IF_stage.insert(next_instr);
            finished_IF[i] = IF_stage.process();
        }
        // process ID
        Trace* finished_ID = new Trace[pipeline_width];
        for (int i = 0; i < pipeline_width; i++) {
            finished_ID[i] = ID_stage.process();
        }
        // add finished IFs to ID
        for (int i = 0; i < pipeline_width; i++) {
            if (finished_IF[i].isValid()) {
                ID_stage.insert(finished_IF[i]);
            }
        }
        
        delete[] finished_IF;
        delete[] finished_ID;
        this->currentCycle++;
    }
}
