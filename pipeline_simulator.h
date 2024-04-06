#ifndef PIPELINE_SIMULATOR_H_
#define PIPELINE_SIMULATOR_H_

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

    void runSimulation(const std::string &traceFile, int pipelineWidth);
};

#endif