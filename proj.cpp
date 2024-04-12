#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>

#include "file_input.h"
#include "pipeline_simulator.h"

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("Not enough arguments\n");
        return 0;
    }

    // std::string file_name = argv[1];
    // unsigned long start_inst = std::stoul(argv[2]);
    // unsigned long inst_count = std::stoul(argv[3]);
    // int width = std::stoi(argv[4]);

    PipelineSimulator simulation;
    // simulation.runSimulation(file_name, start_inst, inst_count, width);

    // simulation.printStats();

    // Experimental Design
    simulation.ExperimentalDesign();
}