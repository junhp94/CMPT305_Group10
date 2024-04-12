#ifndef PIPELINE_SIMULATOR_H_
#define PIPELINE_SIMULATOR_H_

#include <string>

#include "pipeline_stage.h"
#include "trace.h"

class PipelineSimulator
{
private:
    // stage array
    unsigned long currentCycle;
    unsigned long int_instr_count;
    unsigned long float_instr_count;
    unsigned long branch_instr_count;
    unsigned long load_instr_count;
    unsigned long store_instr_count;
    unsigned long finished_instr_count;

public:
    PipelineSimulator();
    ~PipelineSimulator();

    void runSimulation(const std::string &tfile_name, unsigned long start_inst, unsigned long inst_count, int pipeline_width);

    void printStats();

    void ExperimentalDesign(const std::string &file_name);
};

#endif