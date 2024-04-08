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

public:
    PipelineSimulator();
    ~PipelineSimulator();

    void runSimulation(const std::string &tfile_name, long start_inst, long inst_count, int pipelineWidth);
};

#endif