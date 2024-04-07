#include "pipeline_stage.h"

void PipelineStage::process(const Trace &instruction)
{
    instructions.push_back(instruction);
}

bool PipelineStage::isEmpty() const
{
    return instructions.empty();
}

void IFStage::process(const Trace &instruction)
{
    PipelineStage::process(instruction);
}

bool IFStage::isEmpty() const
{
    return PipelineStage::isEmpty();
}

void IDStage::process(const Trace &instruction)
{
    PipelineStage::process(instruction);
}

bool IDStage::isEmpty() const
{
    return PipelineStage::isEmpty();
}

void EXStage::process(const Trace &instruction)
{
    PipelineStage::process(instruction);
}

bool EXStage::isEmpty() const
{
    return PipelineStage::isEmpty();
}

void MEMStage::process(const Trace &instruction)
{
    PipelineStage::process(instruction);
}

bool MEMStage::isEmpty() const
{
    return PipelineStage::isEmpty();
}

void WBStage::process(const Trace &instruction)
{
    PipelineStage::process(instruction);
}

bool WBStage::isEmpty() const
{
    return PipelineStage::isEmpty();
}