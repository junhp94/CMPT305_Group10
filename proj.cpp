
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>

#include "file_input.h"

int main(int argc, char *argv[]) {
    FileInput file;

    file.ReadTraceFromFile(argv[1]);
    Trace tmp = file.GetNext();
    while (tmp.isValid()) {
        printf("%ld %d ", tmp.instructionAddr, tmp.type);
        for (size_t i = 0; i < tmp.dependencyAddr.size(); i++) {
            printf("%ld", tmp.dependencyAddr[i]);
        }
        printf("\n");
        tmp = file.GetNext();
    }
}