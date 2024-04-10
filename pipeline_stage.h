#ifndef PIPELINE_STAGE_H_
#define PIPELINE_STAGE_H_

#include <deque>
#include "trace.h"

class PipelineStage
{
public:
    std::deque<Trace> instructions;

    // insert, remove implementation
    virtual void insert(const Trace &instruction) = 0;
    virtual Trace process() = 0;
    virtual bool isEmpty() const = 0;
    virtual int size() const = 0;
    virtual ~PipelineStage() {}
};

// Instruction Fetch
class IFStage : public PipelineStage
{
private:
    std::deque<Trace> instructions;

public:
    void insert(const Trace &instruction) override;
    Trace process() override;
    bool isEmpty() const override;
    int size() const override;
};

// Instruction decode and read operands
class IDStage : public PipelineStage
{
private:
    std::deque<Trace> instructions;

public:
    void insert(const Trace &instruction) override;
    Trace process() override;
    bool isEmpty() const override;
    int size() const override;
};

// Issue and Execute
class EXStage : public PipelineStage
{
private:
    std::deque<Trace> instructions;

public:
    void insert(const Trace &instruction) override;
    Trace process() override;
    bool isEmpty() const override;
    int size() const override;
};

// Memory read/write for load/store instructions
class MEMStage : public PipelineStage
{
private:
    std::deque<Trace> instructions;

public:
    void insert(const Trace &instruction) override;
    Trace process() override;
    bool isEmpty() const override;
    int size() const override;
};

// Write back results to registers and Retire
class WBStage : public PipelineStage
{
private:
    std::deque<Trace> instructions;

public:
    void insert(const Trace &instruction) override;
    Trace process() override;
    bool isEmpty() const override;
    int size() const override;
};

#endif