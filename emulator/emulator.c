#include "emulator.h"
#include "assembler/assembler.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void init_cpu(CPU* cpu) {
    memset(cpu, 0, sizeof(cpu));
    cpu->running = 1;
    cpu->pc = 0;
}

int load_hex(const char* hex_path, CPU* cpu) {
    if (!hex_path || !cpu) {
        return 1;
    }

    FILE* in = fopen(hex_path, "R");
    if (!in) {
        perror("Couldn't open hex file");
        return 1;
    }

    uint16_t addr = 0;
    char line[64];

    while (fgets(line, sizeof(line), in)) {
        if (line[0] == '\n' || line[0] == '\0') {
            continue;
        }


        uint16_t word = (uint16_t) (strtoul(line, NULL, 16));
        cpu->mem[addr++] = word;
    }

    fclose(in);
    cpu->pc = 0;
    return 0;

}

