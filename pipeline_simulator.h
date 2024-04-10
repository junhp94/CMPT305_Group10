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

    void runSimulation(const std::string &tfile_name, unsigned long start_inst, unsigned long inst_count, int pipeline_width);
};

#endif