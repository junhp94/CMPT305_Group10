
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>

#include "file_input.h"
#include "pipeline_simulator.h"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Not enough arguments\n");
        return 0;
    }
    std::string file_name = argv[1];
    long start_inst = std::stol(argv[2]);
    long inst_count = std::stol(argv[3]);
    int width = std::stoi(argv[4]);

    PipelineSimulator simulation;
    simulation.runSimulation(file_name, start_inst, inst_count, width);
}