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

class IFStage : public PipelineStage
{
private:
    std::vector<Trace> instructions;

public:
    void process(const Trace &instruction) override;
    bool isEmpty() const override;
};

class IDStage : public PipelineStage
{
private:
    std::vector<Trace> instructions;

public:
    void process(const Trace &instruction) override;
    bool isEmpty() const override;
};

class EXStage : public PipelineStage
{
private:
    std::vector<Trace> instructions;

public:
    void process(const Trace &instruction) override;
    bool isEmpty() const override;
};

class MEMStage : public PipelineStage
{
private:
    std::vector<Trace> instructions;

public:
    void process(const Trace &instruction) override;
    bool isEmpty() const override;
};

class WBStage : public PipelineStage
{
private:
    std::vector<Trace> instructions;

public:
    void process(const Trace &instruction) override;
    bool isEmpty() const override;
};

#endif