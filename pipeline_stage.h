#ifndef PIPELINE_STAGE_H_
#define PIPELINE_STAGE_H_

#include <vector>
#include "trace.h"

class PipelineStage
{
public:
    std::vector<Trace> instructions;

    // insert, remove implementation
    virtual void process(const Trace &instruction) = 0;
    virtual bool isEmpty() const = 0;
    virtual ~PipelineStage() {}
};

// Instruction Fetch
class IFStage : public PipelineStage
{
private:
    std::vector<Trace> instructions;

public:
    void process(const Trace &instruction) override;
    bool isEmpty() const override;
};

// Instruction decode and read operands
class IDStage : public PipelineStage
{
private:
    std::vector<Trace> instructions;

public:
    void process(const Trace &instruction) override;
    bool isEmpty() const override;
};

// Issue and Execute
class EXStage : public PipelineStage
{
private:
    std::vector<Trace> instructions;

public:
    void process(const Trace &instruction) override;
    bool isEmpty() const override;
};

// Memory read/write for load/store instructions
class MEMStage : public PipelineStage
{
private:
    std::vector<Trace> instructions;

public:
    void process(const Trace &instruction) override;
    bool isEmpty() const override;
};

// Write back results to registers and Retire
class WBStage : public PipelineStage
{
private:
    std::vector<Trace> instructions;

public:
    void process(const Trace &instruction) override;
    bool isEmpty() const override;
};

#endif