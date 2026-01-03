#include "emulator.h"
#include "assembler.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void init_cpu(CPU* cpu) {
    memset(cpu, 0, sizeof(*cpu));
    cpu->running = 1;
    cpu->pc = 0;
}

int load_hex(const char* hex_path, CPU* cpu) {
    if (!hex_path || !cpu) {
        return 1;
    }

    FILE* in = fopen(hex_path, "r");
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

int cpu_step(CPU* cpu) {
    int pc = cpu->pc++;
    uint16_t ir = cpu->mem[pc];

    uint16_t op = (ir >> 12) & 0xF;
    uint16_t dr = (ir >> 9) & 0x7;
    uint16_t sr1 = (ir >> 6) & 0x7;
    uint16_t sr2 = ir & 0x7;

    switch (op) {
        case OP_ADD:
            cpu->regs[dr] = cpu->regs[sr1] + cpu->regs[sr2];
            break;
        case OP_SUB: 
            printf("Subtracting %d - %d into R%d\n", cpu->regs[sr1], cpu->regs[sr2], dr);
            cpu->regs[dr] = cpu->regs[sr1] - cpu->regs[sr2];
            break;
        case OP_OR: cpu->regs[dr] = cpu->regs[sr1] | cpu->regs[sr2]; break;
        case OP_AND: cpu->regs[dr] = cpu->regs[sr1] & cpu->regs[sr2]; break;
        case OP_NOT:
            cpu->regs[dr] = (uint16_t) ~cpu->regs[sr1];
            break;
        case OP_HALT: cpu->running = 0; return 0;
        default:
            fprintf(stderr, "Unknown Opcode %u at pc %u\n", op, (unsigned) (cpu->pc - 1));
            return 1;
    }
    return 0;
}

void cpu_run(CPU* cpu) {
    while (cpu->running) {
        if (cpu_step(cpu) != 0) {
            break;
        }
    }
}

 