#include "pipeline_stage.h"

void PipelineStage::insert(const Trace &instruction)
{
    instructions.push_back(instruction);
}

Trace PipelineStage::process()
{
    // returns and removes first item from list deque
    Trace res;
    if (!instructions.empty()) {
        res = instructions.front();
        instructions.erase(instructions.begin());
    }
    return res;
}

bool PipelineStage::isEmpty() const
{
    return instructions.empty();
}

void IFStage::insert(const Trace &instruction)
{
    PipelineStage::insert(instruction);
}

Trace IFStage::process()
{
    return PipelineStage::process();
}

bool IFStage::isEmpty() const
{
    return PipelineStage::isEmpty();
}

void IDStage::insert(const Trace &instruction)
{
    PipelineStage::insert(instruction);
}

Trace IDStage::process()
{
    return PipelineStage::process();
}

bool IDStage::isEmpty() const
{
    return PipelineStage::isEmpty();
}

int IDStage::size()
{
    return instructions.size();
}

void EXStage::insert(const Trace &instruction)
{
    PipelineStage::insert(instruction);
}

Trace EXStage::process()
{
    return PipelineStage::process();
}

bool EXStage::isEmpty() const
{
    return PipelineStage::isEmpty();
}

void MEMStage::insert(const Trace &instruction)
{
    PipelineStage::insert(instruction);
}

Trace MEMStage::process()
{
    return PipelineStage::process();
}

bool MEMStage::isEmpty() const
{
    return PipelineStage::isEmpty();
}

void WBStage::insert(const Trace &instruction)
{
    PipelineStage::insert(instruction);
}

Trace WBStage::process()
{
    return PipelineStage::process();
}

bool WBStage::isEmpty() const
{
    return PipelineStage::isEmpty();
}