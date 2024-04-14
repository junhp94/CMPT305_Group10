#include <iostream>
#include <set>
#include <iomanip>

#include "pipeline_simulator.h"
#include "file_input.h"
#include "functional_unit.h"

PipelineSimulator::PipelineSimulator() : currentCycle(0), int_instr_count(0), float_instr_count(0), branch_instr_count(0), load_instr_count(0), store_instr_count(0), finished_instr_count(0)
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
    std::set<unsigned long> in_progress_instructions;
    
    file.SkipToInstruction(start_inst);

    FunctionalUnit ALU_unit;
    FunctionalUnit FP_unit;
    FunctionalUnit Branch_unit;
    FunctionalUnit Load_unit;
    FunctionalUnit Store_unit;

    IFStage IF_stage;
    IDStage ID_stage;
    EXStage EX_stage;
    MEMStage MEM_stage;
    WBStage WB_stage;

    int Branch_dep = 0;

    while (finished_instr_count < inst_count)
    {
        std::vector<Trace> to_remove;
        // All instructions in WB retire and leave the pipeline
        while (!WB_stage.isEmpty())
        {
            WB_stage.process();
            this->finished_instr_count++;
            std::cout << finished_instr_count << std::endl;
        }
        // All instructions in MEM move to WB
        while (!MEM_stage.isEmpty())
        {
            Trace instr = MEM_stage.process();
            WB_stage.insert(instr);

            //"For dependence on load or store instructions, dependences are satisfied when they complete MEM"
            if (instr.type == Trace::Type::LOAD || instr.type == Trace::Type::STORE)
            {
                to_remove.push_back(instr);
            }
        }
        bool EX_blocked = false;
        int EX_completions = 0;
        // All instructions in EX move to MEM (in order) except more than one load or store
        while (!EX_stage.isEmpty() && !EX_blocked && EX_completions < pipeline_width)
        {
            Trace instr = EX_stage.process();
            if (instr.type == Trace::Type::LOAD)
            {
                // We can’t have two load instructions go to MEM in the same cycle
                if (Load_unit.isAvailable())
                {
                    MEM_stage.insert(instr);
                    EX_completions++;
                    Load_unit.setBusy();
                }
                else
                {
                    EX_blocked = true;
                    EX_stage.push_front(instr);
                }
            }
            else if (instr.type == Trace::Type::STORE)
            {
                // We can’t have two store instructions go to MEM in the same cycle
                if (Store_unit.isAvailable())
                {
                    MEM_stage.insert(instr);
                    EX_completions++;
                    Store_unit.setBusy();
                }
                else
                {
                    EX_blocked = true;
                    EX_stage.push_front(instr);
                }
            }
            else
            {
                // Move INT/FP/Branch instructions to MEM
                MEM_stage.insert(instr);
                EX_completions++;
                // Finish Branch dependencies
                if (instr.type == Trace::Type::BRANCH)
                {
                    Branch_dep = 0;
                }
                // "For dependence on integer or FP instructions, dependences are satisfied when they complete EX"
                else if (instr.type == Trace::Type::INT_INSTR || instr.type == Trace::Type::FP_INSTR)
                {
                    to_remove.push_back(instr);
                }
            }
        }
        // All instructions in ID move to EX (in order) if (1) all dependences are satisfied; (2) no structural hazards
        int ID_completions = 0;
        bool ID_blocked = false;
        while (!ID_stage.isEmpty() && ID_completions < pipeline_width && !ID_blocked)
        {
            Trace instr = ID_stage.process();
            for (std::vector<unsigned long>::size_type j = 0; j < instr.dependencyAddr.size(); j++)
            {
                if (in_progress_instructions.find(instr.dependencyAddr[j]) != in_progress_instructions.end())
                {
                    ID_blocked = true;
                    ID_stage.push_front(instr);
                    break;
                }
            }
            if (!ID_blocked)
            {
                if (instr.type == Trace::Type::INT_INSTR)
                {
                    // We can’t have two integer ALU instructions go to EX in the same cycle
                    if (ALU_unit.isAvailable())
                    {
                        EX_stage.insert(instr);
                        ID_completions++;
                        ALU_unit.setBusy();
                    }
                    else
                    {
                        ID_blocked = true;
                        ID_stage.push_front(instr);
                    }
                }
                else if (instr.type == Trace::Type::FP_INSTR)
                {
                    // We can’t have two FP instructions go to EX in the same cycle
                    if (FP_unit.isAvailable())
                    {
                        EX_stage.insert(instr);
                        ID_completions++;
                        FP_unit.setBusy();
                    }
                    else
                    {
                        ID_blocked = true;
                        ID_stage.push_front(instr);
                    }
                }
                else if (instr.type == Trace::Type::BRANCH)
                {
                    // We can’t have two branch instructions go to EX in the same cycle
                    if (Branch_unit.isAvailable())
                    {
                        EX_stage.insert(instr);
                        ID_completions++;
                        Branch_unit.setBusy();
                        Branch_dep = 1;
                    }
                    else
                    {
                        ID_blocked = true;
                        ID_stage.push_front(instr);
                    }
                }
                else
                {
                    // Move Load/Store instructions to EX
                    EX_stage.insert(instr);
                    ID_completions++;
                }
            }
        }
        // All instructions in IF move to ID if pipeline slots are available (no instructions stalled in ID)
        while (!IF_stage.isEmpty())
        {
            Trace instr = IF_stage.process();
            ID_stage.insert(instr);

            switch (instr.type)
            {
            case Trace::INT_INSTR:
                this->int_instr_count++;
                break;
            case Trace::BRANCH:
                this->branch_instr_count++;
                break;
            case Trace::FP_INSTR:
                this->float_instr_count++;
                break;
            case Trace::LOAD:
                this->load_instr_count++;
                break;
            case Trace::STORE:
                this->store_instr_count++;
                break;
            case Trace::INVALID:
                break;
            }
        }
        // Fetch W new instructions from the trace to IF unless a previous branch hasn’t passed EX
        if (Branch_dep == 0)
        {
            for (int i = 0; i < pipeline_width; ++i)
            {
                Trace instr = file.GetNext();
                IF_stage.insert(instr);
                if (instr.type != Trace::Type::BRANCH)
                {
                    in_progress_instructions.insert(instr.instructionAddr);
                }
            }
        }

        for (std::vector<Trace>::size_type i = 0; i < to_remove.size(); i++)
        {
            in_progress_instructions.erase(to_remove[i].instructionAddr);
        }

        // End of cycle
        this->currentCycle++;
        ALU_unit.setFree();
        FP_unit.setFree();
        Branch_unit.setFree();
        Load_unit.setFree();
        Store_unit.setFree();
    }
}

void PipelineSimulator::printStats()
{
    unsigned long total_instr_count = int_instr_count + float_instr_count + branch_instr_count + load_instr_count + store_instr_count;

    double int_percentage = static_cast<double>(int_instr_count) / total_instr_count * 100.0;
    double float_percentage = static_cast<double>(float_instr_count) / total_instr_count * 100.0;
    double branch_percentage = static_cast<double>(branch_instr_count) / total_instr_count * 100.0;
    double load_percentage = static_cast<double>(load_instr_count) / total_instr_count * 100.0;
    double store_percentage = static_cast<double>(store_instr_count) / total_instr_count * 100.0;

    // print stats
    std::cout << "Instruction Type Histogram" << std::endl;
    std::cout << "Total Execution Time : " << currentCycle << " cycles" << std::endl;
    std::cout << "%int : " << std::fixed << std::setprecision(2) << int_percentage << "%" << std::endl;
    std::cout << "%FP : " << std::fixed << std::setprecision(2) << float_percentage << "%" << std::endl;
    std::cout << "%branch : " << std::fixed << std::setprecision(2) << branch_percentage << "%" << std::endl;
    std::cout << "%load : " << std::fixed << std::setprecision(2) << load_percentage << "%" << std::endl;
    std::cout << "%store : " << std::fixed << std::setprecision(2) << store_percentage << "%" << std::endl;
}

void PipelineSimulator::ExperimentalDesign(const std::string &file_name){
    // Replication 1
    for (int W = 1; W <= 4; ++W){
        runSimulation(file_name, 1, 1000000, W);
    }
    // Replication 2
    for (int W = 1; W <= 4; ++W){
        runSimulation(file_name, 5000000, 1000000, W);
    }
    // Replication 3
    for (int W = 1; W <= 4; ++W){
        runSimulation(file_name, 10000000, 1000000, W);
    }
    // Replication 4
    for (int W = 1; W <= 4; ++W){
        runSimulation(file_name, 15000000, 1000000, W);
    }
    // Replication 5
    for (int W = 1; W <= 4; ++W){
        runSimulation(file_name, 20000000, 1000000, W);
    }
    // Replication 6
    for (int W = 1; W <= 4; ++W){
        runSimulation(file_name, 25000000, 1000000, W);
    }
}
