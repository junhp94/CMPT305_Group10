#include <iostream>
#include <set>

#include "pipeline_simulator.h"
#include "file_input.h"
#include "functional_unit.h"


PipelineSimulator::PipelineSimulator() : currentCycle(0)
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
    unsigned long finished_instr_count = 0;

    file.ReadTraceFromFile(file_name);
    std::set<unsigned long> in_progress_instructions;

    
    file.SkipToInstruction(start_inst);

    // TODO: probably need a set for when something is done ex and for mem
    // "For dependence on integer or FP instructions, dependences are satisfied when they complete EX"
    //"For dependence on load or store instructions, dependences are satisfied when they complete MEM"
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

    // Temp BRANCH dependency
    int Branch_dep = 0;

    while (finished_instr_count < inst_count) {
        std::vector<Trace> to_remove;
        // All instructions in WB retire and leave the pipeline 
        while(!WB_stage.isEmpty()){
            WB_stage.process();
            finished_instr_count++;
            std::cout << finished_instr_count << std::endl;
        }
        // All instructions in MEM move to WB
        while(!MEM_stage.isEmpty()){
            Trace instr = MEM_stage.process();
            WB_stage.insert(instr);

            //"For dependence on load or store instructions, dependences are satisfied when they complete MEM"
            if(instr.type == Trace::Type::LOAD || instr.type == Trace::Type::STORE){
                to_remove.push_back(instr);
            }
        }
        // All instructions in EX move to MEM (in order) except more than one load or store
        while(!EX_stage.isEmpty()){
            Trace instr = EX_stage.process();
            if(instr.type == Trace::Type::LOAD){
                // We can’t have two load instructions go to MEM in the same cycle
                if(Load_unit.isAvailable()){
                    MEM_stage.insert(instr);
                    Load_unit.setBusy();
                }
            }else if(instr.type == Trace::Type::STORE){
                // We can’t have two store instructions go to MEM in the same cycle
                if(Store_unit.isAvailable()){
                    MEM_stage.insert(instr);
                    Store_unit.setBusy();
                }
            }else{
                // Move INT/FP/Branch instructions to MEM
                MEM_stage.insert(instr);
                // Finish Branch dependencies
                if(instr.type == Trace::Type::BRANCH){
                    Branch_dep = 0;
                // "For dependence on integer or FP instructions, dependences are satisfied when they complete EX"
                }else if(instr.type == Trace::Type::INT_INSTR || instr.type == Trace::Type::FP_INSTR) {
                    to_remove.push_back(instr);
                }
            
            }
        }
        // All instructions in ID move to EX (in order) if (1) all dependences are satisfied; (2) no structural hazards
        // while(!ID_stage.isEmpty()){
        for (int i = 0; i < ID_stage.size(); i++) {
            Trace instr = ID_stage.process();
            bool dependencies_satisfied = true;
            for (std::vector<unsigned long>::size_type j = 0; j < instr.dependencyAddr.size(); j++) {
                if (in_progress_instructions.find(instr.dependencyAddr[i]) != in_progress_instructions.end()) {
                    dependencies_satisfied = false;
                    ID_stage.insert(instr);
                    break;
                }
            }
            if (dependencies_satisfied) {
                if(instr.type == Trace::Type::INT_INSTR){
                    // We can’t have two integer ALU instructions go to EX in the same cycle
                    if (ALU_unit.isAvailable()){
                        EX_stage.insert(instr);
                        ALU_unit.setBusy();
                    }
                }else if(instr.type == Trace::Type::FP_INSTR){
                    // We can’t have two FP instructions go to EX in the same cycle
                    if(FP_unit.isAvailable()){
                        EX_stage.insert(instr);
                        FP_unit.setBusy();
                    }
                }else if(instr.type == Trace::Type::BRANCH){
                    // We can’t have two branch instructions go to EX in the same cycle
                    if(Branch_unit.isAvailable()){
                        EX_stage.insert(instr);
                        Branch_unit.setBusy();
                        Branch_dep = 1;
                    }
                }else{
                    // Move Load/Store instructions to EX
                    EX_stage.insert(instr);
                }
            }
        }
        // All instructions in IF move to ID if pipeline slots are available (no instructions stalled in ID)
        while(!IF_stage.isEmpty()){
            Trace instr = IF_stage.process();
            ID_stage.insert(instr);
        }
        // Fetch W new instructions from the trace to IF unless a previous branch hasn’t passed EX
        if(Branch_dep == 0){
            for(int i = 0; i < pipeline_width; ++i) {
                Trace instr = file.GetNext();
                IF_stage.insert(instr);
                if (instr.type != Trace::Type::BRANCH) {
                    in_progress_instructions.insert(instr.instructionAddr);
                }
            }
        }

        for (std::vector<Trace>::size_type i = 0; i < to_remove.size(); i++) {
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
